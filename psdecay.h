/// @file psdecay.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 01.06.2017
///
#ifndef PSDECAY_H
#define PSDECAY_H

//#include "TObject.h"
#include "TLorentzVector.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TImage.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

///
/// \brief The PsDecay class Contains all information about simulation of a certain type of decay.
///
class PsDecay
{
    public:
        PsDecay(int noOfGammas=5, float R=50, float L=70);
        ~PsDecay();
        void AddEvent(Double_t weight, TLorentzVector* gamma1,  TLorentzVector* gamma2,  TLorentzVector* gamma3);
        void DrawHistograms(std::string prefix="basic_distr_");
        int GetAcceptedNo();
    private:
        // detector's parameters
        float fR_;  //radius in cm
        float fL_;  //length in cm
        double fMinCos_; //minimal value of cos theta
        // info about decay products
        int fNoOfDecayProducts_;
        std::vector<Double_t> fMasses_;

        // histograms with relative angles
        TH1F* fH_12_;
        TH2F* fH_12_23_;
        TH2F* fH_12_31_;
        TH2F* fH_23_31_;

        // histograms with distributions of basic quantities
        TH1F* fH_en_;
        TH1F* fH_p_;
        TH1F* fH_phi_;
        TH1F* fH_cosTheta_;
//        ClassDef(PsDecay, 1); // required by ROOT

        int fAcceptedNo_;
        bool AddCuts_(TLorentzVector* gamma);
        bool GeometricalAcceptance_(TLorentzVector* gamma);
};

#endif // PSDECAY_H
