/// @file simulate.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 3.10.2017
/// @version 1.6
///
/// @section DESCRIPTION
/// Simple simulation of 1/2/3 gammas or 2+1 or 2+N.
///
/// @section USAGE
/// To use, compile using Makefile, then simply run. See README.md for more details.

#include <unistd.h>
#include <sys/stat.h>
#include <sstream>
#include <ctime>
#include "TGenPhaseSpace.h"
#include "TFile.h"
#include "TROOT.h"
#include "TList.h"
#include "event.h"
#include "parammanager.h"
#include "psdecay.h"
#include "initialcuts.h"
#include "particlegenerator.h"
#include "phantom.h"

// Paths to folders containing results.
static std::string generalPrefix("results/");

///
/// \brief Small function to convert double numbers into strings with pretty appearence
///
template <typename T>
std::string toStringPretty(const T a_value)
{
    std::ostringstream out;
    out << a_value;
    return out.str();
}

///
/// \brief simulateDecay A function that performs run for many decays with one parameter set.
/// \param Ps Fourmomentum of the source [GeV]
/// \param source Fourvector with the position of the source, fourth coordinate represents radius of the source ball [mm].
/// \param pManag ParamManager reference containing parameters of the simulation.
/// \param type TWO, THREE or TWOandONE.
/// \param filePrefix Prefix for all files.
/// \param tree Instance of TTree to save results from this run.
///
void simulateDecay(TLorentzVector Ps, const TLorentzVector& source, const ParamManager& pManag, const DecayType type, const std::string filePrefix = "", TTree* tree = nullptr)
{
    std::string type_string;
    int noOfGammas = 0;
    type_string = recognizeType(type, noOfGammas);
    ////////////////////////////////////////////////////////////////////////
    double* masses = new double[noOfGammas]();
    //(Momentum, Energy units are Gev/C, GeV)
    TGenPhaseSpace phaseSpaceGen;
    phaseSpaceGen.SetDecay(Ps, noOfGammas, masses);
    // creating necessary objects
    Event* eventDecay = nullptr;//new Event;
    PsDecay decay(type);
    Phantom phantom(pManag.GetPhantomNaive511Prob(), pManag.GetPhantomNaivePromptProb(), pManag.GetPhantomSmear());
    InitialCuts cuts(type, pManag.GetR(), pManag.GetL(), pManag.GetEff());
    ComptonScattering cs(type, pManag.GetSmearLowLimit(), pManag.GetSmearHighLimit());
    //setting SilentMode if necessary
    if(pManag.IsSilentMode())
    {
        //Descriptive part
        decay.EnableSilentMode();
        cuts.EnableSilentMode();
        cs.EnableSilentMode();
    }
    else
    {
        std::cout<<"[INFO] Simulating "<<type_string<<"-gamma decays"<<std::endl;
        std::cout<<"[INFO] Source coordinates: ("<<source.X()<<", "<<source.Y()<<", "<<source.Z()<<") r="<<source.T()<<" [mm]"<<std::endl;
        std::cout<<"[INFO] Generation start!"<<std::endl;
    }

    //***   EVENT LOOP  ***
    for (Int_t n=0; n<pManag.GetSimEvents(); n++)
    {
       //generation of an Event
       eventDecay = generateEvent(phaseSpaceGen, source, pManag, type);
       //Filling histograms, event analysis
       try
       {
           //Getting initial distributions
           decay.AddEvent(eventDecay);
           //Aplying Compton scattering in phantom
           if(pManag.GetPhantomUse())
                phantom.NaiveScatter(eventDecay);
           //Applying cuts
           cuts.AddCuts(eventDecay);
           //Performing the Compton Scattering
           cs.Scatter(eventDecay);
           //we select what kind of events will be saved to the tree and save them
       }
       catch(std::string e)
       {
           std::cout<<e;
           exit(-1);
       }
       //writing to tree
       if(tree!=nullptr && ((pManag.GetEventTypeToSave()==PASS && eventDecay->GetPassFlag()) || (pManag.GetEventTypeToSave()==FAIL && !(eventDecay->GetPassFlag())) || (pManag.GetEventTypeToSave()==ALL)))
       {
           if(n==0)
           {
               if(!pManag.IsSilentMode())
                   std::cout<<"[INFO] Creating a new branch for storing events.\n"<<std::endl;
               tree->Branch("event_split", "Event", &eventDecay, 32000, 99);
           }
           tree->Fill();
       }
       delete eventDecay;
    }
    //***   END OF EVENT LOOP   ***

    //Drawing results
    decay.DrawHistograms(filePrefix, pManag.GetOutputType());
    cuts.DrawHistograms(filePrefix, pManag.GetOutputType());
    cs.DrawComptonHistograms(filePrefix, pManag.GetOutputType()); //Draw histograms with scattering angle and electron's energy distributions.
    delete[] masses;
}

///
/// \brief simulate Function that manages the current run and invokes simulateDecay function.
/// \param simRun Number of current run.
/// \param pManag ParamManager reference with all necessary parameters.
/// \param treeFile Pointer to TFile object in which all data may be stored.
/// \param outputFileAndDirName Name that will be used as output folder name (in PNG mode) and/or output file prefix (in TREE mode).
/// \return Pointer to the TTree object.
///
TTree* simulate(const int simRun, ParamManager& pManag, TFile* treeFile, std::string outputFileAndDirName="")
{

   // Settings
   TLorentzVector Ps;
   TLorentzVector sourcePos; //x,y,z,r !
   int noOfGammas = 0;
   std::string subDir;
   TTree* tree = nullptr;
   TDirectory* runDir = nullptr;
   TDirectory* histDir = nullptr;
   //reading source parameters
   noOfGammas = pManag.GetNoOfGammas();
   std::vector<double> sourceParams = pManag.GetDataAt(simRun);
   double x = sourceParams[0];
   double y = sourceParams[1];
   double z = sourceParams[2];
   double px = sourceParams[3];
   double py = sourceParams[4];
   double pz = sourceParams[5];
   double r = TMath::Abs(sourceParams[6]);

   //checking if source position is correct
   if((TMath::Abs(x)+r)*(TMath::Abs(x)+r)+(TMath::Abs(y)+r)*(TMath::Abs(y)+r) >= pManag.GetR()*pManag.GetR() || (TMath::Abs(z)+r)>=pManag.GetL())
   {
       std::cerr<<"[ERROR] Source outside the barrel! Terminating current run!"<<std::endl;
       return nullptr;
   }

   //setting the parameters of the source and subdirectory name
   Ps = TLorentzVector(px/1000000.0, py/1000000.0, pz/1000000.0, 1.022/1000); //scaling back to GeV
   sourcePos = TLorentzVector(x,y,z,r);
   subDir = toStringPretty(x)+std::string("_")+toStringPretty(y)+std::string("_")+toStringPretty(z)+std::string("_")\
           +toStringPretty(px)+std::string("_")+toStringPretty(py)+std::string("_")+toStringPretty(pz)+std::string("/");

   //setting the right output
   if(pManag.GetOutputType()==BOTH || pManag.GetOutputType()==PNG)
   {
        // creating new directories; see https://linux.die.net/man/3/mkdir
       mkdir((generalPrefix+outputFileAndDirName+subDir).c_str(), ACCESSPERMS);
       chmod((generalPrefix+outputFileAndDirName+subDir).c_str(), ACCESSPERMS);
   }
   if(pManag.GetOutputType()==BOTH || pManag.GetOutputType()==TREE)
   {
       tree = new TTree("tree", "Tree with events and histograms");
       runDir = treeFile->mkdir(subDir.c_str());
       runDir->cd();
       histDir = runDir->mkdir("Histograms");
       histDir->cd();
   }

   //Performing simulations based on the provided number of gammas
   if(noOfGammas==1)
   {
       std::cout<<"::::::::::::Simulating 1-gamma generation::::::::::::"<<std::endl;
       simulateDecay(Ps, sourcePos, pManag, ONE, generalPrefix+outputFileAndDirName+subDir, tree);
   }
   else if(noOfGammas==2)
   {
       std::cout<<"::::::::::::Simulating 2-gamma decays::::::::::::"<<std::endl;
       simulateDecay(Ps, sourcePos, pManag, TWO, generalPrefix+outputFileAndDirName+subDir, tree);
   }
   else if(noOfGammas==3)
   {
       std::cout<<"::::::::::::Simulating 3-gamma decays::::::::::::"<<std::endl;
       simulateDecay(Ps, sourcePos, pManag, THREE, generalPrefix+outputFileAndDirName+subDir, tree);
   }
   else if(noOfGammas==4)
   {
        std::cout<<"::::::::::::Simulating 2+1-gamma decays::::::::::::"<<std::endl;
        simulateDecay(Ps, sourcePos, pManag, TWOandONE, generalPrefix+outputFileAndDirName+subDir, tree);
   }
   else if(noOfGammas==5)
   {
        std::cout<<"::::::::::::Simulating 2+N-gamma decays::::::::::::"<<std::endl;
        simulateDecay(Ps, sourcePos, pManag, TWOandN, generalPrefix+outputFileAndDirName+subDir, tree);
   }
   else
   {
       std::cout<<"::::::::::::Simulating both 2-gamma and 3-gammas decays::::::::::::"<<std::endl;
       simulateDecay(Ps, sourcePos, pManag, TWO, generalPrefix+outputFileAndDirName+subDir, tree);
       simulateDecay(Ps, sourcePos, pManag, THREE, generalPrefix+outputFileAndDirName+subDir, tree);
   }
   if(pManag.GetOutputType()==BOTH || pManag.GetOutputType()==TREE)
       runDir->cd();
   return tree;
}

///
/// \brief main Main function of the program.
/// \param argc Number of provided arguments.
/// \param argv Array of arguments.
/// \return 0
///
int main(int argc, char* argv[])
{
  PrintConstants(); //prints physics constants values implemented in code
  ParamManager par_man;
  bool pars_imported = false;
//  default name of the output file is current date and time
//  by now it is disabled, but with in future may be useful
/*
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
  std::string outputFileAndDirName = oss.str();
  */
  std::string outputFileAndDirName = "result";
  //parsing command line arguments
  for(int nn=1; nn<argc; nn++)
  {
      if(argc > nn+1)
      {
          if(std::string(argv[nn]) == "-i")
          {
              //importing source parameters from external file
              par_man.ImportParams(argv[nn+1]);
              pars_imported=true;
              nn +=1;
          }
          else if(std::string(argv[nn]) == "-n") //parsing command line arguments
          {
              //setting the name for the output folder/file
              outputFileAndDirName = std::string(argv[nn+1]);
              nn +=1;
          }
          else if(std::string(argv[nn]) == "-d") //parsing command line arguments
          {
              //importing 2&N data from an external file
              par_man.Import2nNdata(argv[nn+1]);
              nn +=1;
          }
      }
  }

  if(!pars_imported)
  {
      std::cout<<"[WARNING] Input file not provided! Trying to load simpar.par!"<<std::endl;
      par_man.ImportParams();
  }
  if(par_man.GetNoOfGammas()==5 && !par_man.Is2nNDataImported())
  {
      std::cout<<"[WARNING] Data file for 2&N decays not provided! Trying to load 2nN_data.dat!"<<std::endl;
      par_man.Import2nNdata();
      par_man.Print2nNdata();
  }
  //creating directories for storing the results
  mkdir(generalPrefix.c_str(), ACCESSPERMS);
  chmod(generalPrefix.c_str(), ACCESSPERMS);
  mkdir((generalPrefix+outputFileAndDirName).c_str(), ACCESSPERMS);
  chmod((generalPrefix+outputFileAndDirName).c_str(), ACCESSPERMS);

  TFile *treeFile = nullptr;
  TTree *tree = nullptr;
  if(par_man.GetOutputType() != PNG) //if necessary, create a file to store a tree
  {
    treeFile = new TFile((generalPrefix+outputFileAndDirName+"/"+outputFileAndDirName+".root").c_str(), "recreate");
    treeFile->cd();
  }

  //setting the seed for global pseudo-random number generator
  gRandom = new TRandom3(par_man.GetSeed());
  //loop with simulation runs
  for(int ii=0; ii< (par_man.GetSimRuns()); ii++)
  {
      std::cout<<":::::::::::: START OF RUN NO: "<<ii+1<<" ::::::::::::"<<std::endl;
      tree = simulate(ii, par_man, treeFile, outputFileAndDirName+"/");
      if(tree)
      {
          tree->Write();
          delete tree;
      }
      std::cout<<":::::::::::: END OF RUN NO:  "<<ii+1<<" ::::::::::::"<<"\n"<<std::endl;
  }
  if(treeFile)
  {
      treeFile->Write();
      treeFile->Close();
      delete treeFile;
  }
  std::cout<<"\n:::::::::::: END OF PROGRAM. ::::::::::::\n"<<std::endl;
  return 0;
}
