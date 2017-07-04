/// @file simulate.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 04.07.2017
/// @version 1.3
///
/// @section DESCRIPTION
/// Simple simulation of positronium decay to 2 or 3 gammas.
///
/// @section USAGE
/// To use, compile using Makefile, then run. Passing 2 or 3 as argument you specify the type of decay to be
/// generated. If another or no argument is specified, both scenarios will be generated.

#include "psdecay.h"

#include "TGenPhaseSpace.h"
#include <iomanip>
#include "parammanager.h"
#include <sys/stat.h>
#include <sstream>
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

///
/// \brief simulateDecay Runs Ps->2 or Ps->3 gamma decay.
/// \param Ps Four vector of positronium.
/// \\param noOfGammas How many gammas are produced in a Ps decay.
/// \param filePrefix
/// \param simSteps Number of events to be simulated, also the number of simulation steps.
/// \param sourceXYZ Three element array containg position of the source (x,y,z).
/// \param p Probability to interact with a detector.
/// \param R Radius of the detector.
/// \param L Length of the detector.
/// \return An instance of PsDecay class.
///
PsDecay* simulateDecay(TLorentzVector Ps, const int noOfGammas, std::string filePrefix, int simSteps=1000, double* sourceXYZ=nullptr, float p=1.0, float R=0.50, float L=0.70)
{
    //Descriptive part
    std::cout<<"[INFO] Simulating "<<noOfGammas<<"-gamma decays"<<std::endl;
    if (sourceXYZ)
    {
        std::cout<<"[INFO] Source coordinates: (";
        for(int ii=0; ii<3; ii++)
        {
            std::cout<<sourceXYZ[ii];
            if(ii != 2)
                std::cout<<", ";
            else
                std::cout<<")"<<std::endl;
        }
    }
    else
        throw("[ERROR] Source coordinates not found!");
    ////////////////////////////////////////////////////////////////////////
    double* masses = new double[noOfGammas]();

    //(Momentum, Energy units are Gev/C, GeV)
    TGenPhaseSpace event;
    event.SetDecay(Ps, noOfGammas, masses);
    DecayType type = static_cast<DecayType>(noOfGammas);
    PsDecay* decay = new PsDecay(type);
    InitialCuts cuts(type);
    ComptonScattering cs(noOfGammas);

    // Generacja eventow
    for (Int_t n=0; n<simSteps; n++)
    {
       double x = sourceXYZ[0];
       double y = sourceXYZ[1];
       double z = sourceXYZ[2];

       double weight = event.Generate();
       TLorentzVector* gamma1 = event.GetDecay(0);
       TLorentzVector* gamma2 = event.GetDecay(1);
       TLorentzVector* gamma3;
       if(noOfGammas==3)
           gamma3= event.GetDecay(2);
       else
           gamma3=nullptr;
       TLorentzVector* fourMomenta[3] = {gamma1, gamma2, gamma3};
//       std::cout<<"przed="<<fourMomenta[0]->E()<<" ";
       TLorentzVector* emission1 = new TLorentzVector(x,y,z,0.0);
       TLorentzVector* emission2 = new TLorentzVector(x,y,z,0.0);
       TLorentzVector* emission3 = new TLorentzVector(x,y,z,0.0);

       TLorentzVector* sourcePar[3] = {emission1, emission2, emission3};

       Event event(sourcePar, fourMomenta, weight, type);
       decay->AddEvent(event);
       cuts.AddCuts(event);

//       std::vector<bool> ifPassedCuts = decay->AddEvent(weight, gamma1, gamma2, gamma3);
//       if(ifPassedCuts[0]) cs.Scatter(gamma1->Energy()*1000);
//       if(ifPassedCuts[1]) cs.Scatter(gamma2->Energy()*1000);
//       if(ifPassedCuts[2]) cs.Scatter(gamma3->Energy()*1000);

    }
//    cs.DrawElectronDist(filePrefix); //Draw histograms with scattering angle and electron's energy distributions.
//    cs.DrawPDF(generalPrefix);
    delete[] masses;
    return decay;
}


///
/// \brief simulate Main function of the simulation.
/// \param simRun Index of the run.
/// \param noOfGammas How many decay products should be simulated. If different from 2 or 3 then both scenarios are simulated.
/// \param pManag ParamManager object containing parameters of the simulation and source coordinates.
///
void simulate(int simRun, ParamManager* pManag = nullptr)//int noOfGammas)
{
   // Settings
   TLorentzVector Ps;
   int simSteps = 0;
   int noOfGammas = 0;
   float p = 1.0;
   float R = 0.5;
   float L = 0.7;
   bool silentMode = false;
   double x,y,z,px,py,pz;
   std::string subDir;
   double sourcePos[3];
   //if ParamManager instance is provided, get number of simulation steps and source's x,y,z,px,py,pz.
   if(pManag)
   {
       simSteps = pManag->getSimEvents();
       p = pManag->getP();
       noOfGammas = pManag->getNoOfGammas();
       R = pManag->getR();
       L = pManag->getL();
       silentMode = pManag->isSilentMode();

       std::vector<double> sourceParams = (pManag->getDataAt(simRun));
       x=(sourceParams)[0];
       y=(sourceParams)[1];
       z=(sourceParams)[2];
       for(int ii=0; ii<3; ii++)
           sourcePos[ii]=sourceParams[ii];
       px=(sourceParams)[3];
       py=(sourceParams)[4];
       pz=(sourceParams)[5];
       Ps = TLorentzVector(px, py, pz, 1.022/1000);
       subDir = to_string(x)+std::string("_")+to_string(y)+std::string("_")+to_string(z)+std::string("_")\
               +to_string(px)+std::string("_")+to_string(py)+std::string("_")+to_string(pz)+std::string("/");
   }
   else
   {
     std::cout<<"[ERROR] Unable to load params! Terminating the application!"<<std::endl;
     exit(1);
   }

   // PErforming the simulation
   PsDecay* decayTo2;
   PsDecay* decayTo3;

   // creating new directories see https://linux.die.net/man/3/mkdir
   mkdir((comptonPrefix+subDir).c_str(), ACCESSPERMS);
   chmod((comptonPrefix+subDir).c_str(), ACCESSPERMS);
   mkdir((decaysPrefix+subDir).c_str(), ACCESSPERMS);
   chmod((decaysPrefix+subDir).c_str(), ACCESSPERMS);

   //Performing simulations based on the provided number of gammas
   if(noOfGammas==2)
   {
       decayTo2 = simulateDecay(Ps, 2, (comptonPrefix+subDir).c_str(), simSteps, sourcePos, p, R, L);
       if(silentMode) decayTo2->EnableSilentMode();
       else decayTo2->DisableSilentMode();
       decayTo3 = nullptr;
   }
   else if(noOfGammas==3)
   {
//       std::cout<<"::::::::::::Simulating 3-gamma decays::::::::::::"<<std::endl;
       decayTo3 = simulateDecay(Ps, 3, (comptonPrefix+subDir).c_str(), simSteps, sourcePos, p, R, L);
       if(silentMode) decayTo3->EnableSilentMode();
       else decayTo3->DisableSilentMode();
       decayTo2 = nullptr;
   }
   else
   {
       std::cout<<"::::::::::::Simulating both 2-gamma and 3-gammas decays::::::::::::"<<std::endl;
       decayTo2 = simulateDecay(Ps, 2, (comptonPrefix+subDir).c_str(), simSteps, sourcePos, p, R, L);
       decayTo3 = simulateDecay(Ps, 3, (comptonPrefix+subDir).c_str(), simSteps, sourcePos, p, R, L);
       if(silentMode) decayTo2->EnableSilentMode();
       else decayTo2->DisableSilentMode();
       if(silentMode) decayTo3->EnableSilentMode();
       else decayTo3->DisableSilentMode();
   }

   // Getting the results
   if(decayTo2)
   {
        std::cout<<"[INFO] Saving results for 2-gamma decays"<<std::endl;
        decayTo2->DrawHistograms((decaysPrefix+subDir).c_str());
//        std::cout << std::fixed;
//        std::cout << std::setprecision(2);
//        std::cout<<"[INFO] Fraction of accepted 2-gamma decays: "<<decayTo2->GetAcceptedEvents()<<"/"<<simSteps<<"="<<\
//                   decayTo2->GetAcceptedEvents()/(float)simSteps*100<<"%"<<std::endl;
//        std::cout<<std::endl;
   }
   if(decayTo3)
   {
       std::cout<<"[INFO] Saving results for 3-gamma decays"<<std::endl;
       decayTo3->DrawHistograms((decaysPrefix+subDir).c_str());
//       std::cout << std::fixed;
//       std::cout << std::setprecision(2);
//       std::cout<<"[INFO] Fraction of accepted 3-gamma decays: "<<decayTo3->GetAcceptedEvents()<<"/"<<simSteps<<"="<<\
//                  decayTo3->GetAcceptedEvents()/(float)simSteps*100<<"%"<<std::endl;
//       std::cout<<std::endl;
   }

   delete decayTo2;
   delete decayTo3;

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
  ParamManager* par_man;

  bool pars_imported = false;
  par_man = new ParamManager();
  if(argc == 3 && std::string(argv[2]) == "-i") //parsing command line arguments
  {
        //importing source parameters from external file
          par_man->ImportParams(argv[3]);
          pars_imported=true;
  }

  if(!pars_imported)
  {
      std::cout<<"[WARNING] Input file not provided! Trying to load simulation_parameters.par!"<<std::endl;
      par_man->ImportParams();
  }
  //creating directories for storing the results
  mkdir(generalPrefix.c_str(), ACCESSPERMS);
  chmod(generalPrefix.c_str(), ACCESSPERMS);
  mkdir(comptonPrefix.c_str(), ACCESSPERMS);
  chmod(comptonPrefix.c_str(), ACCESSPERMS);
  mkdir(decaysPrefix.c_str(), ACCESSPERMS);
  chmod(decaysPrefix.c_str(), ACCESSPERMS);

  //loop with simulation runs
  for(int ii=0; ii< (par_man->getSimRuns()); ii++)
  {
      std::cout<<":::::::::::: START OF RUN NO: "<<ii<<" ::::::::::::"<<std::endl;
      simulate(ii, par_man);
      std::cout<<":::::::::::: END OF RUN NO:  "<<ii<<" ::::::::::::"<<"\n"<<std::endl;
  }
  std::cout<<"\n:::::::::::: END OF PROGRAM. ::::::::::::\n"<<std::endl;
  return 0;
}
# endif
