#include "event.h"
#include <iostream>

Event::Event(TLorentzVector** emissionCoordinates, TLorentzVector** fourMomentum, double weights, DecayType type) :
    fWeight_(weights),
    fDecayType_(type),
    fPassFlag_(true)
{
    int totalGammaNo = type ==TWO ? 2 : 3;
    fCutPassing_ = new bool[totalGammaNo]{true};
    fPhi_ = new double[totalGammaNo];
    fTheta_ = new double[totalGammaNo];
    for(int ii=0; ii<totalGammaNo; ii++)
    {
        if(emissionCoordinates[ii])
            fEmissionPoint_.push_back(*emissionCoordinates[ii]);
        if(fourMomentum[ii])
        {
            fFourMomentum_.push_back(*fourMomentum[ii]*1000); //scale from GeV to MeV
            fPhi_[ii] = fourMomentum[ii]->Phi();
            fTheta_[ii] = fourMomentum[ii]->Theta();
        }

    }
}

Event::Event(const Event& est)
{
    fDecayType_ = est.fDecayType_;
    int totalGammaNo = fDecayType_ ==TWO ? 2 : 3;
    fCutPassing_ = new bool[totalGammaNo]{true};
    fPhi_ = new double[totalGammaNo];
    fTheta_ = new double[totalGammaNo];

    fEmissionPoint_.resize(est.fEmissionPoint_.size());
    std::copy(est.fEmissionPoint_.begin(), est.fEmissionPoint_.end(), fEmissionPoint_.begin());
    fFourMomentum_.resize(est.fFourMomentum_.size());
    std::copy(est.fFourMomentum_.begin(), est.fFourMomentum_.end(), fFourMomentum_.begin());
    for(int ii=0; ii<totalGammaNo; ii++)
    {
        fCutPassing_[ii] = est.fCutPassing_[ii];
        fPhi_[ii] = est.fPhi_[ii];
        fTheta_[ii] = est.fTheta_[ii];
    }
}

Event& Event::operator=(const Event& est)
{
    fDecayType_ = est.fDecayType_;
    int totalGammaNo = fDecayType_ ==TWO ? 2 : 3;
    fCutPassing_ = new bool[totalGammaNo]{true};
    fPhi_ = new double[totalGammaNo];
    fTheta_ = new double[totalGammaNo];

    fEmissionPoint_.resize(est.fEmissionPoint_.size());
    std::copy(est.fEmissionPoint_.begin(), est.fEmissionPoint_.end(), fEmissionPoint_.begin());
    fFourMomentum_.resize(est.fFourMomentum_.size());
    std::copy(est.fFourMomentum_.begin(), est.fFourMomentum_.end(), fFourMomentum_.begin());
    for(int ii=0; ii<totalGammaNo; ii++)
    {
        fCutPassing_[ii] = est.fCutPassing_[ii];
        fPhi_[ii] = est.fPhi_[ii];
        fTheta_[ii] = est.fTheta_[ii];
    }
    return *this;
}

Event::~Event()
{
    if(fCutPassing_) delete[] fCutPassing_;
    if(fPhi_) delete[] fPhi_;
    if(fTheta_) delete[] fTheta_;
}

void Event::DeducePassFlag()
{
    int validGammaNo = fDecayType_ == THREE ? 3 : 2;
    for(int ii=0; ii<validGammaNo; ii++)
        fPassFlag_ &= fCutPassing_[ii];
}
