#ifndef EVENT_H
#define EVENT_H
#include "TLorentzVector.h"
#include <vector>

enum DecayType
{
    WRONG = 0,
    TWO = 2,
    TWOandONE = 1,
    THREE = 3
};

class Event
{
    public:
        Event(TLorentzVector** emissionCoordinates, TLorentzVector** fourMomentum, double weight, DecayType type);
        Event(const Event& est);
        Event& operator=(const Event &est);
        ~Event();
        inline TLorentzVector* GetEmissionPointOf(const unsigned index) const
            {return index<fEmissionPoint_.size() ? const_cast<TLorentzVector*>(&fEmissionPoint_[index]) : nullptr;}
        inline TLorentzVector* GetFourMomentumOf(const unsigned index) const
            {return index<fFourMomentum_.size() ? const_cast<TLorentzVector*>(&fFourMomentum_[index]) : nullptr;}
        inline bool GetCutPassingOf(const unsigned index) const {return fCutPassing_[index];}
        inline void SetCutPassing(int ii, bool val) {fCutPassing_[ii]=val;}
        inline double GetWeight() const {return fWeight_;}
        inline DecayType GetDecayType() const {return fDecayType_;}
        inline bool GetPassFlag() const {return fPassFlag_;}
        inline double GetPhiOf(const unsigned index) const {return fPhi_[index];}
        inline double GetThetaOf(const unsigned index) const {return fTheta_[index];}
        void DeducePassFlag();

    private:
        std::vector<TLorentzVector> fEmissionPoint_; //x, y, z, t [m and s]
        std::vector<TLorentzVector> fFourMomentum_; //pX, pY, pZ, E [MeV/c and MeV]
        bool* fCutPassing_; //indicates if gamma failed passing through cuts
        double fWeight_;
        DecayType fDecayType_;
        bool fPassFlag_;
        double *fPhi_;
        double *fTheta_;
};

#endif // EVENT_H
