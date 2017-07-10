/// @file simulate.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 04.07.2017
/// @version 1.5
///
/// @section DESCRIPTION
/// Simple simulation of positronium decay to 2 or 3 gammas.
///
/// @section USAGE
/// To use, compile using Makefile, then run.

#include <iomanip>
#include <sys/stat.h>
#include <sstream>
#include "TGenPhaseSpace.h"
#include "parammanager.h"
#include "psdecay.h"
#include "initialcuts.h"


// Paths to folders containing results.
static std::string generalPrefix("results/");
static std::string comptonPrefix = generalPrefix + std::string("compton/");
static std::string decaysPrefix = generalPrefix + std::string("decays/");

///
/// \brief Template used to cast any value to string. Mainly used to cast double to string, preserving scientific notation.
///
template <typename T>
std::string to_string(const T a_value)
{
    std::ostringstream out;
    out << a_value;
    return out.str();
}


void simulateDecay(TLorentzVector Ps, const TLorentzVector& source, const ParamManager& pManag, const DecayType type, const std::string filePrefix = "")
{
    std::string type_string;
    int noOfGammas = 0;
    TRandom3 rand(0);
    if(type==TWO)
    {
        type_string = "2";
        noOfGammas = 2;
    }
    else if(type==THREE)
    {
        type_string = "3";
        noOfGammas = 3;
    }
    else if(type==TWOandONE)
    {
        type_string = "2&1";
        noOfGammas = 2;
    }

    //Descriptive part
    std::cout<<"[INFO] Simulating "<<type_string<<"-gamma decays"<<std::endl;
    std::cout<<"[INFO] Source coordinates: ("<<source.X()<<", "<<source.Y()<<", "<<source.Z()<<") r="<<source.T()<<" [mm]"<<std::endl;
    ////////////////////////////////////////////////////////////////////////
    double* masses = new double[noOfGammas]();
    //(Momentum, Energy units are Gev/C, GeV)
    TGenPhaseSpace event;
    event.SetDecay(Ps, noOfGammas, masses);
    // creating necessary objects
    PsDecay decay(type);
    InitialCuts cuts(type, pManag.GetR(), pManag.GetL(), pManag.GetP());
    ComptonScattering cs(type);
    if(pManag.IsSilentMode())
    {
        decay.EnableSilentMode();
        cuts.EnableSilentMode();
        cs.EnableSilentMode();
    }
    else
        std::cout<<"[INFO] Generation start!"<<std::endl;
    //***   EVENT LOOP  ***
    for (Int_t n=0; n<pManag.GetSimEvents(); n++)
    {
       //Generation of a decay
       double weight = event.Generate();
       TLorentzVector* gamma1 = event.GetDecay(0);
       TLorentzVector* gamma2 = event.GetDecay(1);
       TLorentzVector* gamma3;
       //Generating emission point inside a ball
       TLorentzVector* emission1;
       if(source.T() != 0)
            emission1= new TLorentzVector(source.X()+rand.Uniform(-1.0,1.0)*source.T(), source.Y()+rand.Uniform(-1.0,1.0)*source.T(),\
                                                      source.Z()+rand.Uniform(-1.0,1.0)*source.T(), 0.0);
       else
           emission1 = new TLorentzVector(source.X(), source.Y(), source.Z(), 0.0);
       TLorentzVector* emission2 = new TLorentzVector(*emission1);
       TLorentzVector* emission3;
       if(type == THREE)
       {
           gamma3=event.GetDecay(2);
           emission3 = new TLorentzVector(*emission1);
       }
       else if(type == TWO)
       {
           gamma3 = nullptr;
           emission3 = nullptr;

       }
       else if(type == TWOandONE)
       {
           if(rand.Uniform() < pManag.GetPSc())
           {

               double theta = TMath::ACos(rand.Uniform(-1.0, 1.0));
               double phi = rand.Uniform(0.0, 2*TMath::Pi());
               double P = pManag.GetESc()/1000000.0; //GeV
               gamma3 = new TLorentzVector(P*TMath::Sin(theta)*TMath::Cos(phi), P*TMath::Sin(theta)*TMath::Sin(phi), P*TMath::Cos(theta), P);
               emission3 = new TLorentzVector(*emission1);
           }
           else
           {
               gamma3 = nullptr;
               emission3=nullptr;
           }
       }
       //Packing everything to EVENT object
       TLorentzVector* fourMomenta[3] = {gamma1, gamma2, gamma3};
       TLorentzVector* sourcePar[3] = {emission1, emission2, emission3};
       Event eventDecay(sourcePar, fourMomenta, weight, type);
       //Getting initial distributions
       decay.AddEvent(eventDecay);
//       //Applying cuts
       cuts.AddCuts(eventDecay);
//       //Performing the Compton Scattering
       cs.Scatter(eventDecay, 0.0);

       delete emission1;
       delete emission2;
       if(emission3) delete emission3;
    }
    //***   END OF EVENT LOOP   ***
    //Drawing results
    decay.DrawHistograms(filePrefix);
    cuts.DrawHistograms(filePrefix);
    cs.DrawComptonHistograms(filePrefix); //Draw histograms with scattering angle and electron's energy distributions.
    delete[] masses;
}

void simulate(const int simRun, ParamManager& pManag)
{
   // Settings
   TLorentzVector Ps;
   TLorentzVector sourcePos; //x,y,z,r !
   int noOfGammas = 0;
   double x,y,z,px,py,pz,r;
   std::string subDir;

   noOfGammas = pManag.GetNoOfGammas();
   std::vector<double> sourceParams = (pManag.GetDataAt(simRun));
   x=(sourceParams)[0];
   y=(sourceParams)[1];
   z=(sourceParams)[2];
   px=(sourceParams)[3];
   py=(sourceParams)[4];
   pz=(sourceParams)[5];
   r=TMath::Abs((sourceParams)[6]);

   //checking if source position is correct
   if((TMath::Abs(x)+r)*(TMath::Abs(x)+r)+(TMath::Abs(y)+r)*(TMath::Abs(y)+r) >= pManag.GetR()*pManag.GetR() || (TMath::Abs(z)+r)>=pManag.GetL())
   {
       std::cerr<<"[ERROR] Source outside the barrel! Terminating current run!"<<std::endl;
       return;
   }

   Ps = TLorentzVector(px/1000000.0, py/1000000.0, pz/1000000.0, 1.022/1000); //scaling to GeV
   sourcePos = TLorentzVector(x,y,z,r);
   subDir = to_string(x)+std::string("_")+to_string(y)+std::string("_")+to_string(z)+std::string("_")\
           +to_string(px)+std::string("_")+to_string(py)+std::string("_")+to_string(pz)+std::string("/");

   // creating new directories see https://linux.die.net/man/3/mkdir
   mkdir((generalPrefix+subDir).c_str(), ACCESSPERMS);
   chmod((generalPrefix+subDir).c_str(), ACCESSPERMS);
//   mkdir((decaysPrefix+subDir).c_str(), ACCESSPERMS);
//   chmod((decaysPrefix+subDir).c_str(), ACCESSPERMS);

   //Performing simulations based on the provided number of gammas
    if(noOfGammas==1)
    {
        std::cout<<"::::::::::::Simulating 2+1-gamma decays::::::::::::"<<std::endl;
        simulateDecay(Ps, sourcePos, pManag, TWOandONE, generalPrefix+subDir);
    }
   else if(noOfGammas==2)
   {
       std::cout<<"::::::::::::Simulating 2-gamma decays::::::::::::"<<std::endl;
       simulateDecay(Ps, sourcePos, pManag, TWO, generalPrefix+subDir);
   }
   else if(noOfGammas==3)
   {
       std::cout<<"::::::::::::Simulating 3-gamma decays::::::::::::"<<std::endl;
       simulateDecay(Ps, sourcePos, pManag, THREE, generalPrefix+subDir);
   }
   else
   {
       std::cout<<"::::::::::::Simulating both 2-gamma and 3-gammas decays::::::::::::"<<std::endl;
       simulateDecay(Ps, sourcePos, pManag, TWO, generalPrefix+subDir);
       simulateDecay(Ps, sourcePos, pManag, THREE, generalPrefix+subDir);
   }
}

# ifndef __CINT__
///
/// \brief main C++ wrapper of simulate function.
/// \param argc Number of provided arguments.
/// \param argv Array of arguments.
/// \return 0
///
int main(int argc, char* argv[])
{
  PrintConstants();
  ParamManager par_man;
  bool pars_imported = false;
  if(argc == 3 && std::string(argv[2]) == "-i") //parsing command line arguments
  {
        //importing source parameters from external file
          par_man.ImportParams(argv[3]);
          pars_imported=true;
  }

  if(!pars_imported)
  {
      std::cout<<"[WARNING] Input file not provided! Trying to load simpar.par!"<<std::endl;
      par_man.ImportParams();
  }
  //creating directories for storing the results
  mkdir(generalPrefix.c_str(), ACCESSPERMS);
  chmod(generalPrefix.c_str(), ACCESSPERMS);
//  mkdir(comptonPrefix.c_str(), ACCESSPERMS);
//  chmod(comptonPrefix.c_str(), ACCESSPERMS);
//  mkdir(decaysPrefix.c_str(), ACCESSPERMS);
//  chmod(decaysPrefix.c_str(), ACCESSPERMS);

  //loop with simulation runs
  for(int ii=0; ii< (par_man.GetSimRuns()); ii++)
  {
      std::cout<<":::::::::::: START OF RUN NO: "<<ii+1<<" ::::::::::::"<<std::endl;
      simulate(ii, par_man);
      std::cout<<":::::::::::: END OF RUN NO:  "<<ii+1<<" ::::::::::::"<<"\n"<<std::endl;
  }
  std::cout<<"\n:::::::::::: END OF PROGRAM. ::::::::::::\n"<<std::endl;
  return 0;
}
# endif
