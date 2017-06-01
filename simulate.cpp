#include "psdecay.h"
//#include <iostream>
#include "TGenPhaseSpace.h"

static int noOfGammasToSimulate = 0;

PsDecay* simulateDecayTo3(TLorentzVector Ps, Int_t simSteps=1000000)
{
//    if (!gROOT->GetClass("TGenPhaseSpace")) gSystem.Load("libPhysics");
    PsDecay* decayTo3 = new PsDecay(3);
    //(Momentum, Energy units are Gev/C, GeV)
    Double_t masses[3] = {0.0, 0.0, 0.0};
    TGenPhaseSpace event;
    event.SetDecay(Ps, 3, masses);

    // Generacja eventow
    for (Int_t n=0;n<simSteps;n++)
    {
       Double_t weight = event.Generate();

       TLorentzVector *fgamma1 = event.GetDecay(0);
       TLorentzVector *fgamma2 = event.GetDecay(1);
       TLorentzVector *fgamma3 = event.GetDecay(2);
       decayTo3->AddEvent(weight, fgamma1, fgamma2, fgamma3);
    }
    return decayTo3;
}


PsDecay* simulateDecayTo2(TLorentzVector Ps, Int_t simSteps=1000000)
{
//    if (!gROOT->GetClass("TGenPhaseSpace")) gSystem.Load("libPhysics");
    PsDecay* decayTo2 = new PsDecay(2);
    //(Momentum, Energy units are Gev/C, GeV)
    Double_t masses[2] = {0.0, 0.0};
    TGenPhaseSpace event;
    event.SetDecay(Ps, 2, masses);

    // Generacja eventow
    for (Int_t n=0;n<simSteps;n++)
    {
       Double_t weight = event.Generate();

       TLorentzVector *fgamma1 = event.GetDecay(0);
       TLorentzVector *fgamma2 = event.GetDecay(1);
//       TLorentzVector *fgamma3 = event.GetDecay(2);
       decayTo2->AddEvent(weight, fgamma1, fgamma2, nullptr);
    }
    return decayTo2;
}




void simulate()//int noOfGammas)
{
   int noOfGammas = noOfGammasToSimulate;
   TLorentzVector Ps(0.0, 0.0, 0.0001, 1.022); //zrodlo
   Int_t simSteps = 1000000;
   PsDecay* decayTo2;
   PsDecay* decayTo3;
   if(noOfGammas==2)
   {
       std::cout<<"[INFO] Simulating 2-gamma decays"<<std::endl;
       decayTo2 = simulateDecayTo2(Ps, simSteps);
       decayTo3 = nullptr;
   }
   else if(noOfGammas==3)
   {
       std::cout<<"[INFO] Simulating 3-gamma decays"<<std::endl;
       decayTo3 = simulateDecayTo3(Ps, simSteps);
       decayTo2 = nullptr;
   }
   else
   {
       std::cout<<"[INFO] Simulating both 2-gamma and 3-gammas decays"<<std::endl;
       decayTo2 = simulateDecayTo2(Ps, simSteps);
       decayTo3 = simulateDecayTo3(Ps, simSteps);
   }

   if(decayTo2)
   {
        decayTo2->DrawHistograms();
   }
   if(decayTo3)
   {
       decayTo3->DrawHistograms();
   }

}

# ifndef __CINT__
int main(int argc, char* argv[])
{
//  std::cout<<"main"<<std::endl;
  if(argc>1)
    noOfGammasToSimulate = atoi(argv[1]);
  simulate();
  return 0;
}
# endif
