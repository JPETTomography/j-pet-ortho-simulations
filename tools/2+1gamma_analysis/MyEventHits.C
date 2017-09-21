#define MyEventHits_cxx
#include "MyEventHits.h"
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
MyEventHits::MyEventHits()
{
    hRootEdep = new TH1F("hRootEdep", "hRootEdep", 200, 0.0, 1.2);
    hRootEdep511keV = new TH1F("hRootEdep511keV", "hRootEdep", 200, 0.0, 1.2);
    hRootEdepPrompt = new TH1F("hRootEdepPrompt", "hRootEdep", 200, 0.0, 1.2);
    hRootEdepSmear = new TH1F("hRootEdepSmear", "hRootEdepPre", 200, 0.0, 1.2);
    hRootEdepSmear511keV = new TH1F("hRootEdepSmear511keV", "hRootEdepPre", 200, 0.0, 1.2);
    hRootEdepSmearPrompt = new TH1F("hRootEdepSmearPrompt", "hRootEdepPre", 200, 0.0, 1.2);
    hCutPassing = new TH1F("hCutPassing", "hCutPassing", 5, 0, 5);
}

MyEventHits::~MyEventHits()
{
    delete hRootEdep;
    delete hRootEdep511keV;
    delete hRootEdepPrompt;
    delete hRootEdepSmear;
    delete hRootEdepSmear511keV;
    delete hRootEdepSmearPrompt;
    delete hCutPassing;
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

       if(fCutPassing_[0] && fEdep_[0]>COMPTON_E_TH_0)
       {
           hRootEdep->Fill(fEdep_[0]);
           hRootEdep511keV->Fill(fEdep_[0]);
           hRootEdepSmear->Fill(fEdepSmear_[0]);
           hRootEdepSmear511keV->Fill(fEdepSmear_[0]);
           hCutPassing->Fill(1);
       }
       if(fCutPassing_[1] && fEdep_[1]>COMPTON_E_TH_0)
       {
           hRootEdep->Fill(fEdep_[1]);
           hRootEdep511keV->Fill(fEdep_[1]);
           hRootEdepSmear->Fill(fEdepSmear_[1]);
           hRootEdepSmear511keV->Fill(fEdepSmear_[1]);
           hCutPassing->Fill(2);
       }
       if(fCutPassing_.size() > 2 && fCutPassing_[2] && fEdep_[2]>COMPTON_E_TH_0)
       {
           TLorentzVector v3(fHitPoint_[2].X() - fEmissionPoint_[2].X(), fHitPoint_[2].Y() - fEmissionPoint_[2].Y(), fHitPoint_[2].Z() - fEmissionPoint_[2].Z(), 0.0);
           TLorentzVector v30(fHitPoint_[2].X(), fHitPoint_[2].Y(), fHitPoint_[2].Z(), 0.0);
           hRootEdep->Fill(fEdep_[2]);
           hRootEdepPrompt->Fill(fEdep_[2]);
           hRootEdepSmear->Fill(fEdepSmear_[2]);
           hRootEdepSmearPrompt->Fill(fEdepSmear_[2]);
           hCutPassing->Fill(3);
       }

   }
}
