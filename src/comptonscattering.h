/// @file comptonscattering.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
#ifndef COMPTONSCATTERING_H
#define COMPTONSCATTERING_H
#include <string>
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TF2.h"
#include "TRandom3.h"
#include "constants.h"
#include "event.h"
#include "parammanager.h"

///
/// \brief The ComptonScattering class Class responsible for Compton scattering according to the Klein-Nishina formula.
///
class ComptonScattering
{
    public:
        ComptonScattering(DecayType type, float low=0.0, float high=2.0);
        ComptonScattering(const ComptonScattering& est);
        ComptonScattering& operator=(const ComptonScattering& est);
        bool operator==(const ComptonScattering &cs) const;
        ~ComptonScattering();
        void DrawPDF(std::string filePrefix="", double crossSectionE=0.511);
        void DrawComptonHistograms(std::string filePrefix, OutputOptions output=PNG);
        void Scatter(Event* event, int index=-1) const; //perfors scattering
        inline void EnableSilentMode() {fSilentMode_=true;}
        inline void DisableSilentMode() {fSilentMode_=false;}
        inline float GetSmearLowLimit() const {return fSmearLowLimit_;}
        inline float GetSmearHighLimit() const {return fSmearLowLimit_;}
        inline void SetSmearLowLimit(float limit) {fSmearLowLimit_=limit;}
        inline void SetSmearHighLimit(float limit) {fSmearHighLimit_=limit;}
        TF1* fPDF;  //root function wrapper, Klein-Nishina formula
        TF1* fPDF_Theta;  //root function wrapper, dN/d theta

    private:
        bool fSilentMode_; //if true then less output is generated
        DecayType fDecayType_;
        std::string fTypeString_;
        TH1F* fH_electron_E_;   //energy distribution for electrons
        TH1F* fH_electron_E_blur_;   //energy distribution for electrons blurred by detector effects
        TH1F* fH_photon_E_depos_; //distribution of energy deposited by incident photons
        TH1F* fH_photon_theta_;   //angle distribution for scattered photons
        TH2D* fH_PDF_;  // Klein-Nishina function plot, for testing purpose only
        TH1D* fH_PDF_cross; //Klein-Nishina function for specified value of incident's photon energy
        TH2D* fH_PDF_Theta_; //Klein-Nishina based theta PDF function
        TH1D* fH_PDF_Theta_cross; //Klein-Nishina based theta PDF function for specified value of incident's photon energy
        float fSmearLowLimit_; //lower limit for phenomenologicly derived smearing effect
        float fSmearHighLimit_; //higher limit for phenomenologicly derived smearing effect
        static long double KleinNishina_(double* angle, double* energy); //Klein-Nishina function
        static long double KleinNishinaTheta_(double* angle, double* energy); //Klein-Nishina based theta PDF
        double sigmaE(double E, double coeff=0.044) const; //calculate std dev for the smearing effevt

        static unsigned objectID_;

};

#endif // COMPTONSCATTERING_H
