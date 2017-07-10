#ifndef INITIALCUTS_H
#define INITIALCUTS_H
#include <string>
#include "event.h"
//#include "TH1.h"
#include "TH2.h"
#include "TRandom3.h"


class InitialCuts
{
    public:
        InitialCuts(DecayType type=TWO, float R=500, float L=700, float p=1.0);
        InitialCuts(const InitialCuts&);
        InitialCuts& operator=(const InitialCuts& est);
        ~InitialCuts();

        // Getters and setters
        inline int GetAcceptedEvents() const {return fAcceptedEvents_;}
        inline int GetAcceptedGammas() const {return fAcceptedGammas_;}
        inline float GetRadius() const {return fR_;}
        inline void SetRadius(float R){fR_=R;}
        inline float GetLength() const {return fL_;}
        inline void SetLength(float L){fL_=L;}
        inline float GetDetectionProbability() const {return fDetectionProbability_;}
        inline void SetDetectionProbability(float p){if(p>1.0) fDetectionProbability_=1.0; else if(p<0.0) fDetectionProbability_=0.0; else fDetectionProbability_=p;}

        //silent mode switch on/off
        inline void EnableSilentMode(){fSilentMode_=true;}
        inline void DisableSilentMode(){fSilentMode_=false;}
        //adding cuts
        void AddCuts(Event& event);
        //drawing histograms
        void DrawHistograms(std::string prefix);
        void DrawCutsHistograms(std::string prefix);
        void DrawPassHistograms(std::string prefix);
        void DrawFailHistograms(std::string prefix);
    private:
        //if set to true, no output is generated to std::cout
        bool fSilentMode_; //false by default
        //type of the managed decay
        DecayType fDecayType_;
        std::string fTypeString_;
        // detector's parameters
        float fR_;  //radius in cm
        float fL_;  //length in cm
        float fDetectionProbability_; //probability that detector will detect gamma after being hit

        int fAcceptedEvents_;
        int fAcceptedGammas_;
        int fNumberOfEvents_;
        int fNumberOfGammas_;

        TRandom3* fRand_; //set seed
        // histograms with relative angles for events that passed cuts
        TH1F* fH_12_pass_;
        TH1F* fH_23_pass_;
        TH1F* fH_31_pass_;
        TH2F* fH_12_23_pass_;
        TH2F* fH_12_31_pass_;
        TH2F* fH_23_31_pass_;

        // histograms with relative angles for events that failed cuts
        TH1F* fH_12_fail_;
        TH1F* fH_23_fail_;
        TH1F* fH_31_fail_;
        TH2F* fH_12_23_fail_;
        TH2F* fH_12_31_fail_;
        TH2F* fH_23_31_fail_;

        //only for events that passed cuts
        TH1F* fH_en_pass_; //all gammas that passed cuts
        TH1F* fH_en_pass_event_; //gammas that passed cuts and could be used to reconstruct an event
        TH1F* fH_en_pass_low_; //gammas that passed cuts and could be used to reconstruct an event; low energy
        TH1F* fH_en_pass_mid_; //gammas that passed cuts and could be used to reconstruct an event; mid energy
        TH1F* fH_en_pass_high_; ////gammas that passed cuts and could be used to reconstruct an event; high energy
        TH1F* fH_p_pass_;
        TH1F* fH_phi_pass_;
        TH1F* fH_cosTheta_pass_;

        //only for events that did not pass cuts
        TH1F* fH_en_fail_;
        TH1F* fH_p_fail_;
        TH1F* fH_phi_fail_;
        TH1F* fH_cosTheta_fail_;

        //histogram for showing fraction of events that passed different cuts
        TH1F* fH_gamma_cuts_;
        TH1F* fH_event_cuts_;

        bool GeometricalAcceptance_(const TLorentzVector *source, const TLorentzVector* gamma);
        bool DetectionCut_();
        void FillValidEventHistograms_(const Event& event);
        void FillInvalidEventHistograms_(const Event& event);
        void FillDistributionHistograms_(const Event& event);

};

#endif // INITIALCUTS_H
