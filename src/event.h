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
        inline TLorentzVector* GetEmissionPointAt(const unsigned index) const
            {return index<fEmissionPoint_.size() ? const_cast<TLorentzVector*>(&fEmissionPoint_[index]) : nullptr;}
        inline TLorentzVector* GetFourMomentumAt(const unsigned index) const
            {return index<fFourMomentum_.size() ? const_cast<TLorentzVector*>(&fFourMomentum_[index]) : nullptr;}
        inline bool GetCutPassingAt(const unsigned index) const {return fCutPassing_[index];}
        inline void SetCutPassing(int ii, bool val) {fCutPassing_[ii]=val;}
        inline double GetWeight() const {return fWeight_;}
        inline DecayType GetDecayType() const {return fDecayType_;}

    private:
        std::vector<TLorentzVector> fEmissionPoint_; //x, y, z, t [m and s]
        std::vector<TLorentzVector> fFourMomentum_; //pX, pY, pZ, E [GeV/c and MeV]
        bool* fCutPassing_; //indicates if gamma failed passing through cuts
        double fWeight_;
        DecayType fDecayType_;
};

#endif // EVENT_H
