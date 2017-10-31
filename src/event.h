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
        inline TLorentzVector* GetHitPointOf(const unsigned index) const
            {return index<fHitPoint_.size() ? const_cast<TLorentzVector*>(&fHitPoint_[index]) : NULL;}
        inline bool GetCutPassingOf(const unsigned index) const
            {return index<fCutPassing_.size() ? fCutPassing_[index] : false;}
        inline void SetCutPassing(unsigned ii, bool val)
            {if(ii<fCutPassing_.size()) fCutPassing_[ii]=val;}
        inline double GetWeight() const {return fWeight_;}
        inline DecayType GetDecayType() const {return fDecayType_;}
        inline bool GetPassFlag() const {return fPassFlag_;}
        inline double GetHitPhiOf(const unsigned index) const {return fHitPhi_[index];}
        inline double GetHitThetaOf(const unsigned index) const {return fHitTheta_[index];}
        inline double GetEdepOf(const unsigned index) const {return fEdep_[index];}
        inline double GetEdepSmearOf(const unsigned index) const {return fEdep_[index];}
        inline void SetEdepOf(int ii, double val) {fEdep_[ii]=val;}
        inline void SetEdepSmearOf(int ii, double val) {fEdepSmear_[ii]=val;}

        //set fPassFlag_ by checking values in fCutPassing_
        void DeducePassFlag();
        //calculates hit point of gammas on a detectors surface and fills fHitTheta_ and fHitPhi_ histograms
        // angles are calculated in reference to the center of the reference system's center !!!
        void CalculateHitPoints(double R);
        //number of event
        long fId;
        //ROOT stuff
        ClassDef(Event, 13)

    private:
        static long fCounter_; //static variable incremented with every call of a constructor (but not copy constructor)
        std::vector<TLorentzVector> fEmissionPoint_; //x, y, z, t(irrelevant) [mm and s]
        std::vector<TLorentzVector> fFourMomentum_; //pX, pY, pZ, E [MeV/c and MeV]
        std::vector<bool> fCutPassing_; //indicates if gamma failed passing through cuts
        double fWeight_; //weight of the event
        DecayType fDecayType_; //type of the event
        bool fPassFlag_; //if true, event can be reconstructed -- all necessary gammas passed through cuts
        //Estimated values for hit points, ALWAYS CHECK CUTS PASSING BEFORE USING !!! (fails included)
        std::vector<double> fHitPhi_; //values of azimuthal angle for hit points
        std::vector<double> fHitTheta_;//values of polar angle for hit points
        std::vector<TLorentzVector> fHitPoint_; //x, y, z, t [mm and mikro s]
        std::vector<double> fEdep_;
        std::vector<double> fEdepSmear_;
        typedef TObject inherited;


};
#endif
