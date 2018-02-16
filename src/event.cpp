/// @file event.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
#include <iostream>
#include "event.h"
#include "constants.h"
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
        fPrimaryPhoton_.push_back(true);
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
Event::Event(std::vector<TLorentzVector*>* emissionCoordinates, std::vector<TLorentzVector*>* fourMomentum, double weight, DecayType type) :
    fWeight_(weight),
    fDecayType_(type),
    fPassFlag_(true)
{
    fCounter_++;
    fId = fCounter_;
    int totalGammaNo = emissionCoordinates->size();
    for(int ii=0; ii<totalGammaNo; ii++)
    {
        if(emissionCoordinates->at(ii))
            fEmissionPoint_.push_back(*emissionCoordinates->at(ii));
        if(fourMomentum->at(ii))
        {
            fFourMomentum_.push_back(*fourMomentum->at(ii)*1000); //scale from GeV to MeV
            fCutPassing_.push_back(true);
            fPrimaryPhoton_.push_back(true);
            fEdep_.push_back(0.0);
            fEdepSmear_.push_back(0.0);
        }

    }
}

///
/// \brief Event::Event Developed constructor allowing to create a complete Event.
/// \param sourcePos Vector of TLorentzVector with entries corresponsing to X,Y,Z,T coordinates of emission point for a given particle.
/// \param pos Vector of TLorentzVector with entries corresponsing to X,Y,Z,T coordinates of hit point for a given particle.
/// \param momentum Vector of TLorentzVector with entries corresponsing to pX,pY,pZ,E coordinates of four-momentum for a given particle.
/// \param phi Vector of azimuthal angles corresponding to given hit points positions.
/// \param theta Vector of angular angles corresponding to given hit points positions.
/// \param cutPassing Vector of bool values corresponding, true if particle left a signal in the detector.
/// \param primary Vector of bool values corresponding, false if particle was scattered in a phantom.
/// \param edep Vector of deposited energy by particles.
/// \param edepSmear Vector of deposited energy by particles, smeared according to experimental formula.
/// \param Id Particle ids.
/// \param decayType Type of decay.
///
Event::Event(std::vector<TLorentzVector> &sourcePos, std::vector<TLorentzVector> &pos, std::vector<TLorentzVector> &momentum,\
    std::vector<double> &phi, std::vector<double> &theta, std::vector<bool> &cutPassing, std::vector<bool> &primary,\
    std::vector<double> &edep, std::vector<double> &edepSmear, long Id, int decayType)
{
    fCounter_++;
    fId = Id;
    fWeight_ = 1.0;
    fDecayType_ = (DecayType)decayType;
    fPassFlag_ = true;
    fEmissionPoint_.resize(sourcePos.size());
    std::copy(sourcePos.begin(), sourcePos.end(), fEmissionPoint_.begin());
    fHitPoint_.resize(pos.size());
    std::copy(pos.begin(), pos.end(), fHitPoint_.begin());
    fFourMomentum_.resize(momentum.size());
    std::copy(momentum.begin(), momentum.end(), fFourMomentum_.begin());
    fCutPassing_.resize(cutPassing.size());
    std::copy(cutPassing.begin(), cutPassing.end(), fCutPassing_.begin());
    fHitPhi_.resize(phi.size());
    std::copy(phi.begin(), phi.end(), fHitPhi_.begin());
    fHitTheta_.resize(theta.size());
    std::copy(theta.begin(), theta.end(), fHitTheta_.begin());
    fPrimaryPhoton_.resize(primary.size());
    std::copy(primary.begin(), primary.end(), fPrimaryPhoton_.begin());
    fEdep_.resize(edep.size());
    std::copy(edep.begin(), edep.end(), fEdep_.begin());
    fEdepSmear_.resize(edepSmear.size());
    std::copy(edepSmear.begin(), edepSmear.end(), fEdepSmear_.begin());
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
    fHitPhi_.resize(est.fHitPhi_.size());
    std::copy(est.fHitPhi_.begin(), est.fHitPhi_.end(), fHitPhi_.begin());
    fHitTheta_.resize(est.fHitTheta_.size());
    std::copy(est.fHitTheta_.begin(), est.fHitTheta_.end(), fHitTheta_.begin());
    fPrimaryPhoton_.resize(est.fPrimaryPhoton_.size());
    std::copy(est.fPrimaryPhoton_.begin(), est.fPrimaryPhoton_.end(), fPrimaryPhoton_.begin());
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
    fHitPhi_.resize(est.fHitPhi_.size());
    std::copy(est.fHitPhi_.begin(), est.fHitPhi_.end(), fHitPhi_.begin());
    fHitTheta_.resize(est.fHitTheta_.size());
    std::copy(est.fHitTheta_.begin(), est.fHitTheta_.end(), fHitTheta_.begin());
    fPrimaryPhoton_.resize(est.fPrimaryPhoton_.size());
    std::copy(est.fPrimaryPhoton_.begin(), est.fPrimaryPhoton_.end(), fPrimaryPhoton_.begin());
    return *this;
}

///
/// \brief Event::~Event Dummy destructor.
///
Event::~Event()
{

}

///
/// \brief Event::CalculateHitPoints Calculates hit points on the detector's surface
/// \param R Radius of the detector.
///
void Event::CalculateHitPoints(double R, double L)
{
    for(auto it = fFourMomentum_.begin(); it != fFourMomentum_.end(); ++it)
    {
        if(it->Pt() > TMath::Power(10, -10))
        {
            int iter = it-fFourMomentum_.begin();
            double x0 = fEmissionPoint_[iter].X();
            double y0 = fEmissionPoint_[iter].Y();
            double z0 = fEmissionPoint_[iter].Z();
            double px = it->X();
            double py = it->Y();
            double pt2 = px*px+py*py;
            double delta = 4*(x0*px+y0*py)*(x0*px+y0*py) - 4*(x0*x0+y0*y0-R*R)*pt2;
            double s = (-2*(x0*px+y0*py)+TMath::Sqrt(delta))/2/pt2;
            if(TMath::Abs(z0+it->Z()*s) > L/2.0)
            {
                //getting out of detector
                TLorentzVector hit(-2000, -2000, -2000, -2000);
                fHitPoint_.push_back(hit);
                fHitPhi_.push_back(-4);
                fHitTheta_.push_back(-4);
                continue;
            }
            TLorentzVector hit(x0+it->X()*s, y0+it->Y()*s, z0+it->Z()*s, it->T()*s*1000000/light_speed_SI);
            fHitPoint_.push_back(hit);
            fHitPhi_.push_back(hit.Phi());//(hit-fEmissionPoint_[it-fFourMomentum_.begin()]).Phi());
            fHitTheta_.push_back(hit.Theta());//(hit-fEmissionPoint_[it-fFourMomentum_.begin()]).Theta());
        }
        else
        {
             TLorentzVector hit(-2000, -2000, -2000, -2000);
             fHitPoint_.push_back(hit);
             fHitPhi_.push_back(-4);
             fHitTheta_.push_back(-4);

        }
    }
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
