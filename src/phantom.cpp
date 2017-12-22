#include "phantom.h"
#include <TLorentzVector.h>

///
/// \brief Phantom::Phantom Full constructor.
/// \param type Type of phantom (shape).
/// \param a Dimension a.
/// \param b Dimension b.
/// \param c Dimension c.
/// \param isSmear True if detector-like smearing is enabled.
///
Phantom::Phantom(PhantomType type , double a, double b, double c, bool isSmear) :
fType_(type),
fA_(a),
fB_(b),
fC_(c),
fSmear_(isSmear),
fNaiveProb511_(0),
fNaiveProbprompt_(0)
{
    cs = nullptr;
}

///
/// \brief Phantom::Phantom Constructor for naive scattering -- dimensionless.
/// \param p511 Probability to scatter 511 keV photons inside the phantom.
/// \param pPrompt Probability to scatter prompt photons inside the phantom.
/// \param isSmear True if detector-like smearing is enabled.
///
Phantom::Phantom(double p511, double pPrompt, bool isSmear) :
fType_(Elipsoid),
fA_(0.0),
fB_(0.0),
fC_(0.0),
fSmear_(isSmear),
fNaiveProb511_(p511),
fNaiveProbprompt_(pPrompt)
{
    cs = nullptr;
}

Phantom::~Phantom()
{
    if(cs) delete cs;
}

///
/// \brief Phantom::Scatter Phantom simulation -- NOT IMPLEMENTED!
/// \param event
///
void Phantom::Scatter(Event* event)
{

}

///
/// \brief Phantom::NaiveScatter Naive model of in-phantom scattering, in which only the energy of photons is altered according to Klein-Nishina formula.
/// \param event Pointer to Event class object, for which in-phantom scattering is done.
///
void Phantom::NaiveScatter(Event* event)
{
    if(cs==nullptr)
    {
        //create ne ComptonScattering object to perform in-phantom scattering
        cs = new ComptonScattering(event->GetDecayType());
    }
    //loop over photons
    for(int ii=0; ii<event->GetNumberOfDecayProducts(); ii++)
    {
        int noOf511 = event->GetDecayType() == THREE ? 3 : 2; //two or three first photons are 511 keV photons
        double prob = ii < noOf511 ? fNaiveProb511_ : fNaiveProbprompt_;
        if(gRandom->Uniform(0.0, 1.0)<prob)
        {
            cs->Scatter(event, ii);
            TLorentzVector* v = event->GetFourMomentumOf(ii);
            double newE = 0.0;
            if(fSmear_)
                newE = event->GetEdepSmearOf(ii);
            else
                newE = event->GetEdepOf(ii);
            newE = newE > 0 ? newE : 0.0;
            TLorentzVector newVect(v->X()/v->E()*newE, v->Y()/v->E()*newE, v->Z()/v->E()*newE, newE);
            //altering the fourmomenta, but without change of their directions
            event->SetFourMomentumOf(ii, newVect);
            event->SetPrimaryPhoton(ii, false);
        }
        else
        {
            event->SetEdepSmearOf(ii, event->GetEdepSmearOf(ii));
        }

    }
}
