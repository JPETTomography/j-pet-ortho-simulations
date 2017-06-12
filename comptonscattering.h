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
        void DrawPDF(std::string filePrefix="");
        void DrawElectronDist(std::string filePrefix="");
        void Scatter(double E); //perfors scattering
        TF1* fPDF;  //special root object

    private:
        int fNoOfGammas_;
//        std::string fFilePrefix_;
        static long double KleinNishina_(double* angle, double* energy);
        TH1F* fH_electron_E_;   //energy distribution for electrons
        TH1F* fH_electron_theta_;   //angle distribution for electrons
        TH2D* fH_PDF_;  // Klein-Nishina function plot, for testing purpose only

};

#endif // COMPTONSCATTERING_H
