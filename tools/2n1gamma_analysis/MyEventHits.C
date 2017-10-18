#define MyEventHits_cxx
#include "MyEventHits.h"
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
MyEventHits::MyEventHits()
{
    hRootEdep = new TH1F("hRootEdep", "hRootEdep", 240, 0.0, 1.2);
    hRootEdep511keV = new TH1F("hRootEdep511keV", "hRootEdep", 240, 0.0, 1.2);
    hRootEdep511keVOne = new TH1F("hRootEdep511keVOne", "hRootEdep", 240, 0.0, 1.2);
    hRootEdep511keVTwo = new TH1F("hRootEdep511keVTwo", "hRootEdep", 240, 0.0, 1.2);
    hRootEdepPrompt = new TH1F("hRootEdepPrompt", "hRootEdep", 240, 0.0, 1.2);
    hRootEdepSmear = new TH1F("hRootEdepSmear", "hRootEdepPre", 240, 0.0, 1.2);;
    hRootEdepSmear511keV = new TH1F("hRootEdepSmear511keV", "hRootEdepPre", 240, 0.0, 1.2);
    hRootEdepSmear511keVOne = new TH1F("hRootEdepSmear511keVOne", "hRootEdepPre", 240, 0.0, 1.2);
    hRootEdepSmear511keVTwo = new TH1F("hRootEdepSmear511keVTwo", "hRootEdepPre", 240, 0.0, 1.2);
    hRootEdepSmearPrompt = new TH1F("hRootEdepSmearPrompt", "hRootEdepPre", 240, 0.0, 1.2);
    hCutPassing = new TH1F("hCutPassing", "hCutPassing", 5, 0, 5);
    hRootEdepSum511kev = new TH1F("hRootEdepSum511kev", "hRootEdepSum511kev", 240, 0.0, 1.2);
    hRootEdepSum = new TH1F("hRootEdepSum", "hRootEdepSum", 240, 0.0, 1.2);
    hRootEdepSumSmear = new TH1F("hRootEdepSumSmear", "hRootEdepSumSmear", 240, 0.0, 1.2);
    hRootEdepSumSmear511kev = new TH1F("hRootEdepSumSmear511kev", "hRootEdepSumSmear511kev", 240, 0.0, 1.2);
    hFermiBall = new TH3F("hFermiBall", "hFermiBall", 200, 0.0, 0.4, 200, 0.0, 0.4, 200, 0.0, 0.4);
    hFermiBallSmear = new TH3F("hFermiBallSmear", "hFermiBallSmear", 200, 0.0, 0.4, 200, 0.0, 0.4, 200, 0.0, 0.4);
    hFermiCircle = new TH2F("hFermiCircle", "hFermiCircle", 200, 0.0, 0.4, 200, 0.0, 0.4);
    hFermiCircleSmear = new TH2F("hFermiCircleSmear", "hFermiCircleSmear", 200, 0.0, 0.4, 200, 0.0, 0.4);
}

MyEventHits::~MyEventHits()
{
    if(hRootEdep) delete hRootEdep;
    if(hRootEdep511keV) delete hRootEdep511keV;
    if(hRootEdepPrompt) delete hRootEdepPrompt;
    if(hRootEdepSmear) delete hRootEdepSmear;
    if(hRootEdepSmear511keV) delete hRootEdepSmear511keV;
    if(hRootEdepSmearPrompt) delete hRootEdepSmearPrompt;
    if(hCutPassing) delete hCutPassing;
    if(hRootEdep511keVOne) delete hRootEdep511keVOne;
    if(hRootEdep511keVTwo) delete hRootEdep511keVTwo;
    if(hRootEdepSmear511keVOne) delete hRootEdepSmear511keVOne;
    if(hRootEdepSmear511keVTwo) delete hRootEdepSmear511keVTwo;
    if(hRootEdepSum) delete hRootEdepSum;
    if(hRootEdepSumSmear) delete hRootEdepSumSmear;
    if(hFermiBall) delete hFermiBall;
    if(hFermiBallSmear) delete hFermiBallSmear;
    if(hFermiCircle) delete hFermiCircle;
    if(hFermiCircleSmear) delete hFermiCircleSmear;
    if(hRootEdepSum511kev) delete hRootEdepSum511kev;
    if(hRootEdepSumSmear511kev) delete hRootEdepSumSmear511kev;
}

void MyEventHits::Loop()
{
//   In a ROOT session, you can do:
//      root> .L EventHits.C
//      root> EventHits t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   double RemoveSmearBellowE = 0.000; //MeV

   for (Long64_t jentry=0; jentry<nentries;jentry++)
   {
       GetEntry(jentry);
       //POINT - SOURCE
       TLorentzVector v1(fHitPoint_[0].X() - fEmissionPoint_[0].X(), fHitPoint_[0].Y() - fEmissionPoint_[0].Y(), fHitPoint_[0].Z() - fEmissionPoint_[0].Z(), 0.0);
       TLorentzVector v2(fHitPoint_[1].X() - fEmissionPoint_[1].X(), fHitPoint_[1].Y() - fEmissionPoint_[1].Y(), fHitPoint_[1].Z() - fEmissionPoint_[1].Z(), 0.0);
       //POINT (hit point on the cylinder surface)
       TLorentzVector v10(fHitPoint_[0].X(), fHitPoint_[0].Y(), fHitPoint_[0].Z(), 0.0);
       TLorentzVector v20(fHitPoint_[1].X(), fHitPoint_[1].Y(), fHitPoint_[1].Z(), 0.0);

       //Lower treshhold on energy (not blurred!) in MeV
       double COMPTON_E_TH_0 = 0.010;
       double edepSum = 0.0;
       double edepSum511 = 0.0;
       double edepSumSmear511 = 0.0;
       double edepSumSmear = 0.0;
       if(fCutPassing_[0] && fEdep_[0]>COMPTON_E_TH_0)
       {
           hRootEdep->Fill(fEdep_[0]);
           edepSum += fEdep_[0];
           edepSum511 += fEdep_[0];
           hRootEdep511keV->Fill(fEdep_[0]);
           hRootEdep511keVOne->Fill(fEdep_[0]);
           if(fEdep_[0] > RemoveSmearBellowE)
           {
               edepSumSmear += fEdepSmear_[0];
               edepSumSmear511 += fEdepSmear_[0];
               hRootEdepSmear->Fill(fEdepSmear_[0]);
               hRootEdepSmear511keV->Fill(fEdepSmear_[0]);
               hRootEdepSmear511keVOne->Fill(fEdepSmear_[0]);
           }
           else
           {
               edepSumSmear += fEdep_[0];
               edepSumSmear511 += fEdep_[0];
               hRootEdepSmear->Fill(fEdep_[0]);
               hRootEdepSmear511keV->Fill(fEdep_[0]);
               hRootEdepSmear511keVOne->Fill(fEdep_[0]);
           }
           hCutPassing->Fill(1);
       }
       if(fCutPassing_[1] && fEdep_[1]>COMPTON_E_TH_0)
       {
           hRootEdep->Fill(fEdep_[1]);
           edepSum += fEdep_[1];
           edepSum511 += fEdep_[1];
           hRootEdep511keV->Fill(fEdep_[1]);
           hRootEdep511keVTwo->Fill(fEdep_[1]);
           if(fEdep_[1]>RemoveSmearBellowE)
           {
                edepSumSmear += fEdepSmear_[1];
                edepSumSmear511 += fEdepSmear_[1];
                hRootEdepSmear->Fill(fEdepSmear_[1]);
                hRootEdepSmear511keV->Fill(fEdepSmear_[1]);
                hRootEdepSmear511keVTwo->Fill(fEdepSmear_[1]);
           }
           else
           {
               edepSumSmear += fEdep_[1];
               edepSumSmear511 += fEdep_[1];
               hRootEdepSmear->Fill(fEdep_[1]);
               hRootEdepSmear511keV->Fill(fEdep_[1]);
               hRootEdepSmear511keVTwo->Fill(fEdep_[1]);
           }
           hCutPassing->Fill(2);         
       }

       if((fCutPassing_[0] && fEdep_[0]>COMPTON_E_TH_0) ||  (fCutPassing_[1] && fEdep_[1]>COMPTON_E_TH_0))
       {
           hFermiCircle->Fill(fEdep_[0], fEdep_[1]);
           hFermiCircleSmear->Fill(fEdepSmear_[0], fEdepSmear_[1]);
       }
       if(fCutPassing_.size() > 2 && fCutPassing_[2] && fEdep_[2]>COMPTON_E_TH_0)
       {
           TLorentzVector v3(fHitPoint_[2].X() - fEmissionPoint_[2].X(), fHitPoint_[2].Y() - fEmissionPoint_[2].Y(), fHitPoint_[2].Z() - fEmissionPoint_[2].Z(), 0.0);
           TLorentzVector v30(fHitPoint_[2].X(), fHitPoint_[2].Y(), fHitPoint_[2].Z(), 0.0);
           hRootEdep->Fill(fEdep_[2]);
           edepSum += fEdep_[2];
           hRootEdepPrompt->Fill(fEdep_[2]);
           if(fEdep_[2]>RemoveSmearBellowE)
           {
               edepSumSmear += fEdepSmear_[2];
               hRootEdepSmear->Fill(fEdepSmear_[2]);
               hRootEdepSmearPrompt->Fill(fEdepSmear_[2]);
           }
           else
           {
               edepSumSmear += fEdep_[2];
               hRootEdepSmear->Fill(fEdep_[2]);
               hRootEdepSmearPrompt->Fill(fEdep_[2]);
           }
           hCutPassing->Fill(3);

       }
       if((fCutPassing_[0] && fEdep_[0]>COMPTON_E_TH_0) ||  (fCutPassing_[1] && fEdep_[1]>COMPTON_E_TH_0) || (fCutPassing_[2] && fEdep_[2]>COMPTON_E_TH_0))
       {
           hFermiBall->Fill(fEdep_[0], fEdep_[1], fEdep_[2]);
           hFermiBallSmear->Fill(fEdepSmear_[0], fEdepSmear_[1], fEdepSmear_[2]);
       }
       if(edepSum>0)
            hRootEdepSum -> Fill(edepSum);
       if(edepSum511>0)
           hRootEdepSum511kev -> Fill(edepSum511);
       if(edepSumSmear>0)
            hRootEdepSumSmear -> Fill(edepSumSmear);
       if(edepSumSmear511>0)
           hRootEdepSumSmear511kev -> Fill(edepSumSmear511);
   }
}
