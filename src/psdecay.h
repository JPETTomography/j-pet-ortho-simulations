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
        PsDecay(int noOfGammas, double* sourcePos,  float p=1.0, float R=0.50, float L=0.70);
        PsDecay(const PsDecay&);
        PsDecay& operator=(const PsDecay& est);
        ~PsDecay();
        std::vector<bool> AddEvent(Double_t weight, TLorentzVector* gamma1,  TLorentzVector* gamma2,  TLorentzVector* gamma3);
        void DrawHistograms(std::string prefix="RM", bool all=true, bool pass=true, bool fail=true, \
                            bool compare=true, bool cuts=true);

        // Getters and setters
        int GetAcceptedEvents(){return fAcceptedEvents_;}        
        int GetAcceptedGammas(){return fAcceptedGammas_;}
        float GetRadius(){return fR_;}
        void SetRadius(float R){fR_=R;}
        float GetLength(){return fL_;}
        void SetLength(float L){fL_=L;}
        double* GetSourcePos(){return fSourcePos_;}
        void SetSourcePos(double* pos){for(int ii=0; ii<3; ii++) fSourcePos_[ii]=pos[ii];}
        float GetDetectionProbability(){return fDetectionProbability_;}
        void SetDetectionProbability(float p){if(p>1.0) fDetectionProbability_=1.0; else if(p<0.0) fDetectionProbability_=0.0; else fDetectionProbability_=p;}

        //silent mode switch on/off
        void EnableSilentMode(){fSilentMode_=true;}
        void DisableSilentMode(){fSilentMode_=false;}
        //cuts
        bool GeometricalAcceptance(TLorentzVector* gamma);
        bool DetectionCut();
    private:
        //if set to true, no output is generated to std::cout
        bool fSilentMode_; //false by default
        //source's parameters
        double fSourcePos_[3];
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

        int fAcceptedEvents_;
        int fAcceptedGammas_;
        int fNumberOfEvents_;
        int fNumberOfGammas_;

        float fDetectionProbability_; //probability that detector will detect gamma after being hit
        bool AddCuts_(TLorentzVector* gamma);

};

#endif // PSDECAY_H
