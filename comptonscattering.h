#ifndef COMPTONSCATTERING_H
#define COMPTONSCATTERING_H
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "constants.h"
#include "TF2.h"

///
/// \brief The ComptonScattering class Class responsible for Compton scattering according to the Klein-Nishina formula.
///
class ComptonScattering
{
    public:
        ComptonScattering();
        void DrawPDF();
        void DrawElectronDist();
        void Scatter(double E); //perfors scattering
        TF1* fPDF;  //special root object

    private:
        static long double KleinNishina_(double* angle, double* energy);
        TH1F* fH_electron_E_;   //energy distribution for electrons
        TH1F* fH_electron_theta_;   //angle distribution for electrons
        TH2D* fH_PDF_;  // Klein-Nishina function plot, for testing purpose only

};

#endif // COMPTONSCATTERING_H
