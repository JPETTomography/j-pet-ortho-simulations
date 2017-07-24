/// @file event.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
#include <iostream>
#include "event.h"
//ROOT stuff
ClassImp(Event)

long Event::fCounter_ = 0;

///
/// \brief Event::Event Basic constructor. Should not be used!
///
Event::Event()
{
    fWeight_=0;
    fDecayType_=TWO;
    fPassFlag_=false;
    fCounter_++;
    fId = fCounter_;
    for(int ii=0; ii<2; ii++)
    {
        fFourMomentum_.push_back(TLorentzVector(0.0, 0.0, 1.022, 1.022)); //scale from GeV to MeV
        fCutPassing_.push_back(false);
        fPhi_.push_back(0.0);
        fTheta_.push_back(0.0);
    }
    std::cerr<<"[WARNING] Default constructor used for Event class!"<<std::endl;
}

///
/// \brief Event::Event The main constructor.
/// \param emissionCoordinates Coordinates of the emission point in mm.
/// \param fourMomentum Fourmomenta of gammas in GeV.
/// \param weights Weight of the event.
/// \param type Type of the event.
///
Event::Event(TLorentzVector** emissionCoordinates, TLorentzVector** fourMomentum, double weight, DecayType type) :
    fWeight_(weight),
    fDecayType_(type),
    fPassFlag_(true)
{
    fCounter_++;
    fId = fCounter_;
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

///
/// \brief Event::Event Copy constructor.
/// \param est An instance of Event class to be copied.
///
Event::Event(const Event& est) : TObject(est)
{
    fId = est.fId;
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

///
/// \brief Event::operator = Assignement operator.
/// \param est An instance of Event class to be copied.
/// \return Reference to an object identical with the given one.
///
Event& Event::operator=(const Event& est)
{
    fId = est.fId;
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

///
/// \brief Event::~Event Dummy destructor.
///
Event::~Event()
{

}

///
/// \brief Event::DeducePassFlag Checks if relevant gammas passed through cuts and sets event passing flag.
///
void Event::DeducePassFlag()
{
    int validGammaNo = fDecayType_ == THREE ? 3 : 2;
    for(int ii=0; ii<validGammaNo; ii++)
        fPassFlag_ &= fCutPassing_[ii];
}