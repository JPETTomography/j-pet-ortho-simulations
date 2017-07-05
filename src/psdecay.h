/// @file psdecay.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 01.06.2017
///
#ifndef PSDECAY_H
#define PSDECAY_H
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include "TH2.h"
#include "TCanvas.h"
#include "TImage.h"
#include "event.h"
#include "comptonscattering.h"
///
/// \brief The PsDecay class Contains histograms with gammas' characteristics and inforamtion about passed cuts..
///
class PsDecay
{
    public:
        PsDecay(DecayType type=TWO);
        PsDecay(const PsDecay&);
        PsDecay& operator=(const PsDecay& est);
        ~PsDecay();
        void AddEvent(const Event& event);
        void DrawHistograms(std::string prefix="RM");

        //silent mode switch on/off
        inline void EnableSilentMode(){fSilentMode_=true;}
        inline void DisableSilentMode(){fSilentMode_=false;}
    private:
        //if set to true, no output is generated to std::cout
        bool fSilentMode_; //false by default

        DecayType fDecayType_;
        std::string fTypeString_;

        // histograms with relative angles for all events generated
        TH1F* fH_12_;
        TH2F* fH_12_23_;
        TH2F* fH_12_31_;
        TH2F* fH_23_31_;

        // histograms with distributions of basic quantities for all events generated
        TH1F* fH_en_;
        TH1F* fH_p_;
        TH1F* fH_phi_;
        TH1F* fH_cosTheta_;

};

#endif // PSDECAY_H
