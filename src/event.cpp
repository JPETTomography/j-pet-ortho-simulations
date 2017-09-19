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
//        fPhi_.push_back(0.0);
//        fTheta_.push_back(0.0);
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
//            fPhi_.push_back(fourMomentum[ii]->Phi());
//            fTheta_.push_back(fourMomentum[ii]->Theta());
            fCutPassing_.push_back(true);
            fEdep_.push_back(0.0);
            fEdepSmear_.push_back(0.0);
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
    fHitPhi_.resize(est.fHitPhi_.size());
    std::copy(est.fHitPhi_.begin(), est.fHitPhi_.end(), fHitPhi_.begin());
    fHitTheta_.resize(est.fHitTheta_.size());
    std::copy(est.fHitTheta_.begin(), est.fHitTheta_.end(), fHitTheta_.begin());
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
    return *this;
}

///
/// \brief Event::~Event Dummy destructor.
///
Event::~Event()
{

}

void Event::CalculateHitPoints(double R)
{
    double L = 500;
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
