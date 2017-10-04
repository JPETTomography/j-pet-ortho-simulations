/// @file psdecay.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
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
#include "TTree.h"
#include "event.h"
#include "comptonscattering.h"
#include "parammanager.h"

class TwoAndNTestFixture;
///
/// \brief The PsDecay class Contains histograms with gammas' characteristics at the beginning of simulation.
///
class PsDecay
{
    public:
        PsDecay(DecayType type=TWO);
        PsDecay(const PsDecay&);
        PsDecay& operator=(const PsDecay& est);
        ~PsDecay();
        void AddEvent(const Event* event) const;
        void DrawHistograms(std::string prefix="RM", OutputOptions output=PNG);

        //silent mode switch on/off
        inline void EnableSilentMode(){fSilentMode_=true;}
        inline void DisableSilentMode(){fSilentMode_=false;}
    private:
        //if set to true, no output is generated to std::cout
        bool fSilentMode_; //false by default

        DecayType fDecayType_;
        std::string fTypeString_;

        // histograms with relative angles for all events generated
        TH1F* fH_12_; //used when TWO or TWOandONE
        TH1F* fH_23_; //used only when TWOandONE
        TH1F* fH_31_; //used only when TWOandONE
        TH2F* fH_12_23_; //used only when THREE
        TH2F* fH_12_31_; //used only when TWOandONE
        TH2F* fH_23_31_; //used only when TWOandONE
        TH2F* fH_min_mid_; //used only when TWOandONE
        TH2F* fH_min_max_; //used only when TWOandONE
        TH2F* fH_mid_max_; //used only when TWOandONE

        // histograms with distributions of basic quantities for all events generated
        TH1F* fH_en_;
        TH1F* fH_p_;
        TH1F* fH_phi_;
        TH1F* fH_cosTheta_;

        friend class TwoAndNTestFixture; // for testing

        static unsigned objectID_;

};

#endif // PSDECAY_H
