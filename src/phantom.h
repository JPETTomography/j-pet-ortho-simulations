#ifndef PHANTOM_H
#define PHANTOM_H
#include "event.h"
#include "comptonscattering.h"

enum PhantomType
{
    Cylinder = 0,
    Elipsoid = 1,
    Box = 2
};

class Phantom
{
    public:
        Phantom(PhantomType type = Box, double a=1, double b=1, double c=1, bool isSmear=false);
        Phantom(double p511, double pPrompt, bool isSmear); // NaiveConstructor
        ~Phantom();
        void Scatter(Event* event);
        void NaiveScatter(Event* event); //naive scattering, only energy of photons is altered
    private:
        //dimensions of the phantom in mm
        PhantomType fType_; //type of the phantom
        double fA_; //dimension A
        double fB_; //dimension B
        double fC_; //dimension C
        bool fSmear_; //Apply smearing
        double fNaiveProb511_; //probability to scatter inside a phantom for 511 keV photons
        double fNaiveProbprompt_; //probability to scatter inside a phantom for prompt photons
        ComptonScattering* cs;

};

#endif // PHANTOM_H
