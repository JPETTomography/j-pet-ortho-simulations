/// @file event.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
#ifndef EVENT_H
#define EVENT_H
#include "TLorentzVector.h"
#include "TObject.h"
#include "TTree.h"
#include <vector>

///
/// \brief The DecayType enum Specifies the type of decay in which the event was produced.
///
enum DecayType
{
    WRONG = 0,
    TWO = 2,
    TWOandONE = 1,
    THREE = 3
};

///
/// \brief The Event class Class holding information about products of one decay.
///
class Event : public TObject
{
    public:
        Event();
        Event(TLorentzVector** emissionCoordinates, TLorentzVector** fourMomentum, double weight, DecayType type);
        Event(const Event& est);
        Event& operator=(const Event &est);
        virtual ~Event();
        //setters and getters
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
        //set fPassFlag_ by checking values in fCutPassing_
        void DeducePassFlag();
        //number of event
        long fId;
        //ROOT stuff
        ClassDef(Event, 8)

    private:
        static long fCounter_; //static variable incremented with every call of a constructor (but not copy constructor)
        std::vector<TLorentzVector> fEmissionPoint_; //x, y, z, t(irrelevant) [mm and s]
        std::vector<TLorentzVector> fFourMomentum_; //pX, pY, pZ, E [MeV/c and MeV]
        std::vector<bool> fCutPassing_; //indicates if gamma failed passing through cuts
        double fWeight_; //weight of the event
        DecayType fDecayType_; //type of the event
        bool fPassFlag_; //if true, event can be reconstructed -- all necessary gammas passed through cuts
        std::vector<double> fPhi_; //values of azimuthal angle for gammas
        std::vector<double> fTheta_;//values of polar angle for gammas
        typedef TObject inherited;


};
#endif
