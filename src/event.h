#ifndef EVENT_H
#define EVENT_H
#include "TLorentzVector.h"
#include "TObject.h"
#include "TTree.h"
#include <vector>

enum DecayType
{
    WRONG = 0,
    TWO = 2,
    TWOandONE = 1,
    THREE = 3
};

class Event : public TObject
{
    public:

        Event();
        Event(TLorentzVector** emissionCoordinates, TLorentzVector** fourMomentum, double weight, DecayType type);
        Event(const Event& est);
        Event& operator=(const Event &est);
        virtual ~Event();
        inline TLorentzVector* GetEmissionPointOf(const unsigned index) const
            {return index<fEmissionPoint_.size() ? const_cast<TLorentzVector*>(&fEmissionPoint_[index]) : NULL;}
        inline TLorentzVector* GetFourMomentumOf(const unsigned index) const
            {return index<fFourMomentum_.size() ? const_cast<TLorentzVector*>(&fFourMomentum_[index]) : NULL;}
        inline bool GetCutPassingOf(const unsigned index) const {return fCutPassing_[index];}
        inline void SetCutPassing(int ii, bool val) {fCutPassing_[ii]=val;}
        inline double GetWeight() const {return fWeight_;}
        inline DecayType GetDecayType() const {return fDecayType_;}
        inline bool GetPassFlag() const {return fPassFlag_;}
        inline double GetPhiOf(const unsigned index) const {return fPhi_[index];}
        inline double GetThetaOf(const unsigned index) const {return fTheta_[index];}
        void DeducePassFlag();
        ClassDef(Event, 8)

    private:
        std::vector<TLorentzVector> fEmissionPoint_; //x, y, z, t [m and s]
        std::vector<TLorentzVector> fFourMomentum_; //pX, pY, pZ, E [MeV/c and MeV]
        std::vector<bool> fCutPassing_; //indicates if gamma failed passing through cuts
        double fWeight_;
        DecayType fDecayType_;
        bool fPassFlag_;
        std::vector<double> fPhi_;
        std::vector<double> fTheta_;
        typedef TObject inherited;


};
//#ifdef __MAKECINT__
//#pragma link C++ class Event;
//#endif
#endif // EVENT_H
