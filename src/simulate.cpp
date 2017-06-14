/// @file simulate.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 14.06.2017
/// @version 1.2
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
/// \param simSteps Number of events to be simulated, also the number of simulation steps.
/// \param sourceXYZ Three element array containg position of the source (x,y,z).
/// \return An instance of PsDecay class.
///
PsDecay* simulateDecay(TLorentzVector Ps, const int noOfGammas, std::string filePrefix, int simSteps=1000, double* sourceXYZ=nullptr)
{
    double* masses = new double[noOfGammas]();
    PsDecay* decay = new PsDecay(noOfGammas, sourceXYZ);

    //(Momentum, Energy units are Gev/C, GeV)
    TGenPhaseSpace event;
    event.SetDecay(Ps, noOfGammas, masses);

    ComptonScattering cs(noOfGammas);
    // Generacja eventow
    for (Int_t n=0; n<simSteps; n++)
    {
       double weight = event.Generate();

       TLorentzVector *gamma1 = event.GetDecay(0);
       TLorentzVector *gamma2 = event.GetDecay(1);
       TLorentzVector *gamma3;
       if(noOfGammas==3)
           gamma3= event.GetDecay(2);
       else
           gamma3=nullptr;

       std::vector<bool> ifPassedCuts = decay->AddEvent(weight, gamma1, gamma2, gamma3);
       if(ifPassedCuts[0]) cs.Scatter(gamma1->Energy()*1000);
       if(ifPassedCuts[1]) cs.Scatter(gamma2->Energy()*1000);
       if(ifPassedCuts[2]) cs.Scatter(gamma3->Energy()*1000);

    }
    cs.DrawElectronDist(filePrefix); //Draw histograms with scattering angle and electron's energy distributions.
    delete[] masses;
    return decay;
}


///
/// \brief simulate Main function of the simulation.
/// \param simRun Index of the run.
/// \param noOfGammas How many decay products should be simulated. If different from 2 or 3 then both scenarios are simulated.
/// \param pManag ParamManager object containing parameters of the simulation and source coordinates.
///
void simulate(int simRun, int noOfGammas = 0, ParamManager* pManag = nullptr)//int noOfGammas)
{
   // Settings
   TLorentzVector Ps;
   int simSteps = 0;
   double x,y,z,px,py,pz;
   std::string subDir;
   double sourcePos[3];
   //if ParamManager instance is provided, get number of simulation steps and source's x,y,z,px,py,pz.
   if(pManag)
   {
       simSteps = pManag->getSimSteps();
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
       std::cout<<"[INFO] Simulating 2-gamma decays"<<std::endl;
       decayTo2 = simulateDecay(Ps, 2, (comptonPrefix+subDir).c_str(), simSteps, sourcePos);
       decayTo3 = nullptr;
   }
   else if(noOfGammas==3)
   {
       std::cout<<"[INFO] Simulating 3-gamma decays"<<std::endl;       
       decayTo3 = simulateDecay(Ps, 3, (comptonPrefix+subDir).c_str(), simSteps, sourcePos);
       decayTo2 = nullptr;
   }
   else
   {
       std::cout<<"[INFO] Simulating both 2-gamma and 3-gammas decays"<<std::endl;
       decayTo2 = simulateDecay(Ps, 2, (comptonPrefix+subDir).c_str(), simSteps, sourcePos);
       decayTo3 = simulateDecay(Ps, 3, (comptonPrefix+subDir).c_str(), simSteps, sourcePos);
   }

   // Getting the results
   if(decayTo2)
   {
        decayTo2->DrawHistograms((decaysPrefix+subDir).c_str());
        std::cout << std::fixed;
        std::cout << std::setprecision(2);
        std::cout<<"[INFO] Fraction of accepted 2-gamma decays: "<<decayTo2->GetAcceptedNo()<<"/"<<simSteps<<"="<<\
                   decayTo2->GetAcceptedNo()/(float)simSteps*100<<"%"<<std::endl;
        std::cout<<std::endl;
   }
   if(decayTo3)
   {
       decayTo3->DrawHistograms((decaysPrefix+subDir).c_str());
       std::cout << std::fixed;
       std::cout << std::setprecision(2);
       std::cout<<"[INFO] Fraction of accepted 3-gamma decays: "<<decayTo3->GetAcceptedNo()<<"/"<<simSteps<<"="<<\
                  decayTo3->GetAcceptedNo()/(float)simSteps*100<<"%"<<std::endl;
       std::cout<<std::endl;
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
  int noOfGammasToSimulate = 0;
  if(argc>1)
    noOfGammasToSimulate = atoi(argv[1]);

  par_man = new ParamManager();
  //importing source parameters from external file
  if(argc == 3)
  {
     par_man->ImportParams(argv[2]);
  }
  else
  {
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
      simulate(ii, noOfGammasToSimulate, par_man);
      std::cout<<"[INFO] End of run no:  "<<ii<<"\n"<<std::endl;
  }
  std::cout<<"\n[INFO] END OF PROGRAM."<<std::endl;
  return 0;
}
# endif
