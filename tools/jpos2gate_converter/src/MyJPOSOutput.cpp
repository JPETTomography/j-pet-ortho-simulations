/// @file MyJPOSOutput.cpp
/// @author Rafal Maselek <rafalmaselek@gmail.com>
/// @date 15.02.2018
/// @version 1.0
///
/// @section DESCRIPTION
/// User defined class that wraps automatically generated JPOSOutput class.
///
///
#define MyJPOSOutput_cxx
#include "MyJPOSOutput.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
//#include <math.h>

// Function that loads data from the input file and fills fields of MyJPOSOutput class.
void MyJPOSOutput::Loop()
{
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();
    entries = nentries;

    Long64_t nbytes = 0, nb = 0;
    std::cout<<"[LOADING DATA]"<<std::endl;
    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        if( (double)jentry/nentries * 100 - (int)((double)jentry/nentries * 100) < 100.0/nentries)
            std::cout<<"["<<jentry*100/nentries<<"\% LOADED]"<<std::endl;

        EmissionPoint.push_back(fEmissionPoint_);
        FourMomentum.push_back(fFourMomentum_);
        CutPassing.push_back(fCutPassing_);
        PassFlag.push_back(fPassFlag_);
        PrimaryPhoton.push_back(fPrimaryPhoton_);
        HitPoint.push_back(fHitPoint_);
        Edep.push_back(fEdep_);
        EdepSmear.push_back(fEdepSmear_);
        Id.push_back(fId);
   }
   std::cout<<"[DATA LOADED]"<<std::endl;
}
