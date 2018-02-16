/// @file MyJPOSOutput.h
/// @author Rafal Maselek <rafalmaselek@gmail.com>
/// @date 15.02.2018
/// @version 1.0
///
/// @section DESCRIPTION
/// User defined class that wraps automatically generated JPOSOutput class.
///

#ifndef MyJPOSOutput_h
#define MyJPOSOutput_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <TLorentzVector.h>
#include "TObject.h"
#include "JPOSOutput.h"

class MyJPOSOutput : JPOSOutput 
{
    public:
    	int entries;

        std::vector<std::vector<TLorentzVector> > EmissionPoint; //x, y, z, t(irrelevant) [mm and s]
        std::vector<std::vector<TLorentzVector> > FourMomentum; //pX, pY, pZ, E [MeV/c and MeV]
        std::vector<std::vector<bool> > CutPassing; //indicates if gamma failed passing through cuts
        std::vector<bool> PassFlag; //if true, event can be reconstructed -- all necessary gammas passed through cuts
        std::vector<std::vector<bool> > PrimaryPhoton; //true is photon is primary (not scattered)
        std::vector<std::vector<TLorentzVector> > HitPoint; //x, y, z, t [mm and mikro s]
        std::vector<std::vector<double> > Edep; //deposited energy by gammas
        std::vector<std::vector<double> > EdepSmear; //deposited energy by gammas with experimental smearing
        std::vector<int> Id;

        inline MyJPOSOutput(TTree *tree=0) {}
        inline virtual ~MyJPOSOutput(){}
        virtual void     Loop();
};

#endif
