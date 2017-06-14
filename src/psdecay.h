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

#include "comptonscattering.h"
///
/// \brief The PsDecay class Contains histograms with gammas' characteristics and inforamtion about passed cuts..
///
class PsDecay
{
    public:
        PsDecay(int noOfGammas, double* sourcePos, float R=0.50, float L=0.70);
        PsDecay(const PsDecay&);
        PsDecay& operator=(const PsDecay& est);
        ~PsDecay();
        std::vector<bool> AddEvent(Double_t weight, TLorentzVector* gamma1,  TLorentzVector* gamma2,  TLorentzVector* gamma3);
        void DrawHistograms(std::string prefix="RM", bool all=true, bool pass=true, bool fail=true, \
                            bool compare=true, bool cuts=true);
        int GetAcceptedNo();
    private:
        //source's parameters
        double sourcePos_[3];

        // detector's parameters
        float fR_;  //radius in cm
        float fL_;  //length in cm
        // info about decay products
        int fNoOfDecayProducts_;
        std::vector<Double_t> fMasses_;

        // histograms with relative angles for all events generated
        TH1F* fH_12_;
        TH2F* fH_12_23_;
        TH2F* fH_12_31_;
        TH2F* fH_23_31_;

        // histograms with relative angles for events that passed cuts
        TH1F* fH_12_pass_;
        TH2F* fH_12_23_pass_;
        TH2F* fH_12_31_pass_;
        TH2F* fH_23_31_pass_;

        // histograms with relative angles for events that failed cuts
        TH1F* fH_12_fail_;
        TH2F* fH_12_23_fail_;
        TH2F* fH_12_31_fail_;
        TH2F* fH_23_31_fail_;

        // histograms with distributions of basic quantities for all events generated
        TH1F* fH_en_;
        TH1F* fH_p_;
        TH1F* fH_phi_;
        TH1F* fH_cosTheta_;
        //only for events that passed cuts
        TH1F* fH_en_pass_;
        TH1F* fH_p_pass_;
        TH1F* fH_phi_pass_;
        TH1F* fH_cosTheta_pass_;
        //only for events that did not pass cuts
        TH1F* fH_en_fail_;
        TH1F* fH_p_fail_;
        TH1F* fH_phi_fail_;
        TH1F* fH_cosTheta_fail_;
//        ClassDef(PsDecay, 1); // required by ROOT

        //histogram for showing fraction of events that passed different cuts
        TH1F* fH_event_cuts_;
        TH1F* fH_gamma_cuts_;

        int fAcceptedNo_;
        int fNumberOfEvents_;
        int fNumberOfGammas_;

        bool AddCuts_(TLorentzVector* gamma);
        bool GeometricalAcceptance_(TLorentzVector* gamma);
};

#endif // PSDECAY_H
