//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct 19 11:12:52 2017 by ROOT version 6.08/06
// from TTree tree/Tree with events and histograms
// found on file: o14/o14_new.root
//////////////////////////////////////////////////////////

#ifndef EventHits_h
#define EventHits_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <vector>
#include <TLorentzVector.h>
// Header file for the classes stored in the TTree if any.
#include "TObject.h"

class EventHits {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   const Int_t kMaxfEmissionPoint = 1;
   const Int_t kMaxfFourMomentum = 1;
   const Int_t kMaxfCutPassing = 1;
   const Int_t kMaxfWeight = 1;
   const Int_t kMaxfDecayType = 1;
   const Int_t kMaxfPassFlag = 1;
   const Int_t kMaxfHitPhi = 1;
   const Int_t kMaxfHitTheta = 1;
   const Int_t kMaxfHitPoint = 1;
   const Int_t kMaxfEdep = 1;
   const Int_t kMaxfEdepSmear = 1;

   // Declaration of leaf types
 //Event           *event_split;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   Long_t          fId;
   std::vector<TLorentzVector> fEmissionPoint_;
   std::vector<TLorentzVector> fFourMomentum_;
   std::vector<bool>    fCutPassing_;
   Double_t        fWeight_;
   Int_t           fDecayType_;
   Bool_t          fPassFlag_;
   std::vector<double>  fHitPhi_;
   std::vector<double>  fHitTheta_;
   std::vector<TLorentzVector> fHitPoint_;
   std::vector<double>  fEdep_;
   std::vector<double>  fEdepSmear_;

   // List of branches
   TBranch        *b_event_split_fUniqueID;   //!
   TBranch        *b_event_split_fBits;   //!
   TBranch        *b_event_split_fId;   //!
   TBranch        *b_event_split_fEmissionPoint_;   //!
   TBranch        *b_event_split_fFourMomentum_;   //!
   TBranch        *b_event_split_fCutPassing_;   //!
   TBranch        *b_event_split_fWeight_;   //!
   TBranch        *b_event_split_fDecayType_;   //!
   TBranch        *b_event_split_fPassFlag_;   //!
   TBranch        *b_event_split_fHitPhi_;   //!
   TBranch        *b_event_split_fHitTheta_;   //!
   TBranch        *b_event_split_fHitPoint_;   //!
   TBranch        *b_event_split_fEdep_;   //!
   TBranch        *b_event_split_fEdepSmear_;   //!

   EventHits(TTree *tree=0);
   virtual ~EventHits();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EventHits_cxx
EventHits::EventHits(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("o14/o14_new.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("o14/o14_new.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("o14/o14_new.root:/0_0_0_0_0_0");
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

EventHits::~EventHits()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t EventHits::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EventHits::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void EventHits::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_event_split_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_event_split_fBits);
   fChain->SetBranchAddress("fId", &fId, &b_event_split_fId);
   fChain->SetBranchAddress("fEmissionPoint_", &fEmissionPoint_, &b_event_split_fEmissionPoint_);
   fChain->SetBranchAddress("fFourMomentum_", &fFourMomentum_, &b_event_split_fFourMomentum_);
   fChain->SetBranchAddress("fCutPassing_", &fCutPassing_, &b_event_split_fCutPassing_);
   fChain->SetBranchAddress("fWeight_", &fWeight_, &b_event_split_fWeight_);
   fChain->SetBranchAddress("fDecayType_", &fDecayType_, &b_event_split_fDecayType_);
   fChain->SetBranchAddress("fPassFlag_", &fPassFlag_, &b_event_split_fPassFlag_);
   fChain->SetBranchAddress("fHitPhi_", &fHitPhi_, &b_event_split_fHitPhi_);
   fChain->SetBranchAddress("fHitTheta_", &fHitTheta_, &b_event_split_fHitTheta_);
   fChain->SetBranchAddress("fHitPoint_", &fHitPoint_, &b_event_split_fHitPoint_);
   fChain->SetBranchAddress("fEdep_", &fEdep_, &b_event_split_fEdep_);
   fChain->SetBranchAddress("fEdepSmear_", &fEdepSmear_, &b_event_split_fEdepSmear_);
   Notify();
}

Bool_t EventHits::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EventHits::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EventHits::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EventHits_cxx
