#include "event.h"
#include <iostream>

Event::Event(TLorentzVector** emissionCoordinates, TLorentzVector** fourMomentum, double weights, DecayType type) : fWeight_(weights), fDecayType_(type)
{
    fCutPassing_ = new bool[3]{true};
    for(int ii=0; ii<3; ii++)
    {
        if(emissionCoordinates[ii])
            fEmissionPoint_.push_back(*emissionCoordinates[ii]);
        if(fourMomentum[ii])
            fFourMomentum_.push_back(*fourMomentum[ii]*1000); //scale from GeV to MeV
    }
}

Event::Event(const Event& est)
{
    fDecayType_ = est.fDecayType_;
    fEmissionPoint_.resize(est.fEmissionPoint_.size());
    std::copy(est.fEmissionPoint_.begin(), est.fEmissionPoint_.end(), fEmissionPoint_.begin());
    fFourMomentum_.resize(est.fFourMomentum_.size());
    std::copy(est.fFourMomentum_.begin(), est.fFourMomentum_.end(), fFourMomentum_.begin());
    for(int ii=0; ii<3; ii++)
    {
        fCutPassing_[ii] = est.fCutPassing_[ii];
    }
}

Event& Event::operator=(const Event& est)
{
    fDecayType_ = est.fDecayType_;
    fEmissionPoint_.resize(est.fEmissionPoint_.size());
    std::copy(est.fEmissionPoint_.begin(), est.fEmissionPoint_.end(), fEmissionPoint_.begin());
    fFourMomentum_.resize(est.fFourMomentum_.size());
    std::copy(est.fFourMomentum_.begin(), est.fFourMomentum_.end(), fFourMomentum_.begin());
    for(int ii=0; ii<3; ii++)
    {
        fCutPassing_[ii] = est.fCutPassing_[ii];
    }
    return *this;
}

Event::~Event()
{
    if(fCutPassing_) delete[] fCutPassing_;
}
