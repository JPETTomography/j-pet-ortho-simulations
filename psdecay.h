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


class PsDecay
{
    public:
        PsDecay(int noOfGammas);
        ~PsDecay();
        void AddEvent(Double_t weight, TLorentzVector* gamma1,  TLorentzVector* gamma2,  TLorentzVector* gamma3);
        void DrawHistograms(std::string prefix="basic_distr_");
    private:
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
};

#endif // PSDECAY_H
