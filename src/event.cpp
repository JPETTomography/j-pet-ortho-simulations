#include "event.h"
#include <iostream>
ClassImp(Event)

Event::Event()
{
    fWeight_=0;
    fDecayType_=TWO;
    fPassFlag_=false;
    for(int ii=0; ii<2; ii++)
    {
        fFourMomentum_.push_back(TLorentzVector(0.0, 0.0, 1.022, 1.022)); //scale from GeV to MeV
        fCutPassing_.push_back(false);
        fPhi_.push_back(0.0);
        fTheta_.push_back(0.0);
    }
}

Event::Event(TLorentzVector** emissionCoordinates, TLorentzVector** fourMomentum, double weights, DecayType type) :
    fWeight_(weights),
    fDecayType_(type),
    fPassFlag_(true)
{
    int totalGammaNo = type ==TWO ? 2 : 3;
    for(int ii=0; ii<totalGammaNo; ii++)
    {
        if(emissionCoordinates[ii])
            fEmissionPoint_.push_back(*emissionCoordinates[ii]);
        if(fourMomentum[ii])
        {
            fFourMomentum_.push_back(*fourMomentum[ii]*1000); //scale from GeV to MeV
            fPhi_.push_back(fourMomentum[ii]->Phi());
            fTheta_.push_back(fourMomentum[ii]->Theta());
            fCutPassing_.push_back(true);
        }

    }
}

Event::Event(const Event& est) : TObject(est)
{
    fDecayType_ = est.fDecayType_;
    fEmissionPoint_.resize(est.fEmissionPoint_.size());
    std::copy(est.fEmissionPoint_.begin(), est.fEmissionPoint_.end(), fEmissionPoint_.begin());
    fFourMomentum_.resize(est.fFourMomentum_.size());
    std::copy(est.fFourMomentum_.begin(), est.fFourMomentum_.end(), fFourMomentum_.begin());
    fCutPassing_.resize(est.fCutPassing_.size());
    std::copy(est.fCutPassing_.begin(), est.fCutPassing_.end(), fCutPassing_.begin());
    fPhi_.resize(est.fPhi_.size());
    std::copy(est.fPhi_.begin(), est.fPhi_.end(), fPhi_.begin());
    fTheta_.resize(est.fTheta_.size());
    std::copy(est.fTheta_.begin(), est.fTheta_.end(), fTheta_.begin());
}

Event& Event::operator=(const Event& est)
{
    fDecayType_ = est.fDecayType_;
    fEmissionPoint_.resize(est.fEmissionPoint_.size());
    std::copy(est.fEmissionPoint_.begin(), est.fEmissionPoint_.end(), fEmissionPoint_.begin());
    fFourMomentum_.resize(est.fFourMomentum_.size());
    std::copy(est.fFourMomentum_.begin(), est.fFourMomentum_.end(), fFourMomentum_.begin());
    fCutPassing_.resize(est.fCutPassing_.size());
    std::copy(est.fCutPassing_.begin(), est.fCutPassing_.end(), fCutPassing_.begin());
    fPhi_.resize(est.fPhi_.size());
    std::copy(est.fPhi_.begin(), est.fPhi_.end(), fPhi_.begin());
    fTheta_.resize(est.fTheta_.size());
    std::copy(est.fTheta_.begin(), est.fTheta_.end(), fTheta_.begin());
    return *this;
}

Event::~Event()
{

}

void Event::DeducePassFlag()
{
    int validGammaNo = fDecayType_ == THREE ? 3 : 2;
    for(int ii=0; ii<validGammaNo; ii++)
        fPassFlag_ &= fCutPassing_[ii];
}
