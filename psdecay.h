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
/// \brief The PsDecay class Contains all information about simulation of a certain type of decay.
///
class PsDecay
{
    public:
        PsDecay(int noOfGammas=5, float R=50, float L=70);
        ~PsDecay();
        void AddEvent(Double_t weight, TLorentzVector* gamma1,  TLorentzVector* gamma2,  TLorentzVector* gamma3);
        void DrawHistograms(std::string prefix="RM", bool all=true, bool pass=true, bool fail=true, \
                            bool compare=true, bool cuts=true);
        int GetAcceptedNo();
    private:
        // detector's parameters
        float fR_;  //radius in cm
        float fL_;  //length in cm
        double fMinCos_; //minimal value of cos theta
        // info about decay products
        int fNoOfDecayProducts_;
        std::vector<Double_t> fMasses_;
        // Compton scattering
        ComptonScattering* cs_;

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

        int fNumberOfEvents_;
        int fNumberOfGammas_;
        int fAcceptedNo_;
        bool AddCuts_(TLorentzVector* gamma);
        bool GeometricalAcceptance_(TLorentzVector* gamma);
};

#endif // PSDECAY_H
