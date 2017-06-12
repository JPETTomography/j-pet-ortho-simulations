/// @file simulate.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 01.06.2017
/// @version 1.1
///
/// @section DESCRIPTION
/// Simple simulation of positronium decay to 2 or 3 gammas.
///
/// @section USAGE
/// To use, compile using Makefile, then run. Passing 2 or 3 as argument you specify the type of decay to be
/// generated. If another or no argument is specified, both scenarios will be generated.

#include "psdecay.h"
//#include "constants.h"
//#include <iostream>
#include "TGenPhaseSpace.h"
#include <iomanip>
#include "parammanager.h"
#include <sys/stat.h>
#include <sstream>

static std::string generalPrefix("results/");
static std::string comptonPrefix = generalPrefix + std::string("compton/");
static std::string decaysPrefix = generalPrefix + std::string("decays/");



template <typename T>
std::string to_string(const T a_value)
{
    std::ostringstream out;
    out << a_value;
    return out.str();
}



///
/// \brief simulateDecay Runs Ps->2 or Ps->3 gamma decay.
/// \param Ps   Four vector of positronium.
/// \\param noOfGammas How many gammas are produced in a Ps decay.
/// \param simSteps Number of events to be simulated, also the number of simulation steps.
/// \return An instance of PsDecay class.
///
PsDecay* simulateDecay(TLorentzVector Ps, const int noOfGammas, std::string filePrefix, int simSteps=1000)
{
    double* masses = new double[noOfGammas]();
    PsDecay* decay = new PsDecay(noOfGammas);

    //(Momentum, Energy units are Gev/C, GeV)
    TGenPhaseSpace event;
    event.SetDecay(Ps, noOfGammas, masses);

    ComptonScattering cs(noOfGammas);
    // Generacja eventow
    for (Int_t n=0;n<simSteps;n++)
    {
       double weight = event.Generate();

       TLorentzVector *gamma1 = event.GetDecay(0);
       TLorentzVector *gamma2 = event.GetDecay(1);
       TLorentzVector *gamma3;
       if(noOfGammas==3)
              gamma3= event.GetDecay(1);
       else
           gamma3=nullptr;

       std::vector<bool> ifPassedCuts = decay->AddEvent(weight, gamma1, gamma2, gamma3);
       if(ifPassedCuts[0]) cs.Scatter(gamma1->Energy()*1000);
       if(ifPassedCuts[1]) cs.Scatter(gamma2->Energy()*1000);
       if(ifPassedCuts[2]) cs.Scatter(gamma3->Energy()*1000);

    }
    cs.DrawElectronDist(filePrefix);
    delete[] masses;
    return decay;
}

///
/// \brief simulate Main function of simulation.
///
void simulate(int simRun, int noOfGammas = 0, ParamManager* pManag = nullptr)//int noOfGammas)
{
   // Settings
   TLorentzVector Ps;
   int simSteps = 0;
   double x,y,z,px,py,pz;
   std::string subDir;
   if(pManag)
   {
       simSteps = pManag->getSimSteps();
       std::vector<double> sourceParams = (pManag->getDataAt(simRun));
       x=(sourceParams)[0];
       y=(sourceParams)[1];
       z=(sourceParams)[2];
       px=(sourceParams)[3];
       py=(sourceParams)[4];
       pz=(sourceParams)[5];
       Ps = TLorentzVector(px, py, pz, 1.022/1000); //zrodlo [GeV/c, GeV]
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

   // creating a new directory see https://linux.die.net/man/3/mkdir
   mkdir((comptonPrefix+subDir).c_str(), ACCESSPERMS);
   chmod((comptonPrefix+subDir).c_str(), ACCESSPERMS);
   mkdir((decaysPrefix+subDir).c_str(), ACCESSPERMS);
   chmod((decaysPrefix+subDir).c_str(), ACCESSPERMS);


   if(noOfGammas==2)
   {
       std::cout<<"[INFO] Simulating 2-gamma decays"<<std::endl;
       decayTo2 = simulateDecay(Ps, 2, (comptonPrefix+subDir).c_str() ,simSteps);
       decayTo3 = nullptr;
   }
   else if(noOfGammas==3)
   {
       std::cout<<"[INFO] Simulating 3-gamma decays"<<std::endl;       
       decayTo3 = simulateDecay(Ps, 3, (comptonPrefix+subDir).c_str(), simSteps);
       decayTo2 = nullptr;
   }
   else
   {
       std::cout<<"[INFO] Simulating both 2-gamma and 3-gammas decays"<<std::endl;
       decayTo2 = simulateDecay(Ps, 2, (comptonPrefix+subDir).c_str(), simSteps);
       decayTo3 = simulateDecay(Ps, 3, (comptonPrefix+subDir).c_str(), simSteps);
   }

   // Getting the results
   if(decayTo2)
   {
        decayTo2->DrawHistograms((decaysPrefix+subDir).c_str());
        std::cout << std::fixed;
        std::cout << std::setprecision(2);
        std::cout<<"[INFO] fraction of accepted 2-gamma decays: "<<decayTo2->GetAcceptedNo()<<"/"<<simSteps<<"="<<\
                   decayTo2->GetAcceptedNo()/(float)simSteps*100<<"%"<<std::endl;
        std::cout<<std::endl;
//        delete decayTo2;
   }
   if(decayTo3)
   {
       decayTo3->DrawHistograms((decaysPrefix+subDir).c_str());
       std::cout << std::fixed;
       std::cout << std::setprecision(2);
       std::cout<<"[INFO] fraction of accepted 3-gamma decays: "<<decayTo3->GetAcceptedNo()<<"/"<<simSteps<<"="<<\
                  decayTo3->GetAcceptedNo()/(float)simSteps*100<<"%"<<std::endl;
       std::cout<<std::endl;

//       delete decayTo2;

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
  ParamManager* par_man;

  int noOfGammasToSimulate = 0;
  if(argc>1)
    noOfGammasToSimulate = atoi(argv[1]);

  par_man = new ParamManager();
  if(argc == 3)
  {
     par_man->ImportParams(argv[2]);
  }
  else
  {
      par_man->ImportParams();
  }

  mkdir(generalPrefix.c_str(), ACCESSPERMS);
  chmod(generalPrefix.c_str(), ACCESSPERMS);
  mkdir(comptonPrefix.c_str(), ACCESSPERMS);
  chmod(comptonPrefix.c_str(), ACCESSPERMS);
  mkdir(decaysPrefix.c_str(), ACCESSPERMS);
  chmod(decaysPrefix.c_str(), ACCESSPERMS);


  for(int ii = (par_man->getSimRuns())-1; ii>-1; ii--)
    simulate(ii, noOfGammasToSimulate, par_man);

  return 0;
}
# endif
