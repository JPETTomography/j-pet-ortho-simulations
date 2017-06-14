#ifndef COMPTONSCATTERING_H
#define COMPTONSCATTERING_H
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "constants.h"
#include "TF2.h"
#include <string>

///
/// \brief The ComptonScattering class Class responsible for Compton scattering according to the Klein-Nishina formula.
///
class ComptonScattering
{
    public:
        ComptonScattering(int noOfGammas=0);
        ComptonScattering(const ComptonScattering& est);
        ComptonScattering& operator=(const ComptonScattering& est);
        ~ComptonScattering();
        void DrawPDF(std::string filePrefix="");
        void DrawElectronDist(std::string filePrefix="");
        void Scatter(double E); //perfors scattering
        TF1* fPDF;  //root function wrapper

    private:
        int fNoOfGammas_;
        TH1F* fH_electron_E_;   //energy distribution for electrons
        TH1F* fH_photon_theta_;   //angle distribution for scattered photons
        TH2D* fH_PDF_;  // Klein-Nishina function plot, for testing purpose only
        static long double KleinNishina_(double* angle, double* energy);
};

#endif // COMPTONSCATTERING_H
