/// @file MyGateOutput.h
/// @author Rafal Maselek <rafalmaselek@gmail.com>
/// @date 15.02.2018
/// @version 1.0
///
/// @section DESCRIPTION
/// User defined class that wraps automatically generated GateOutput class.
///
#ifndef MyGateOutput_h
#define MyGateOutput_h

#include <TObject.h>
#include <TLorentzVector.h>
#include "GateOutput.h"

class MyGateOutput : public GateOutput, public TObject
{
   public:
        long entries;
        long events;
        std::vector<std::vector<TLorentzVector> > emissionPoints;
        std::vector<std::vector<TLorentzVector> > fourMomenta;
        std::vector<std::vector<TLorentzVector> > hits;
        std::vector<std::vector<bool> > primaryPhotons;
        std::vector<std::vector<double> > edepVec;
        std::vector<std::vector<double> > edepSmearVec;
        std::vector<std::vector<double> > hitPhi;
        std::vector<std::vector<double> >hitTheta;
        std::vector<long> ids;

        MyGateOutput(){}
        virtual ~MyGateOutput(){}
        virtual void  Loop();
        ClassDef(MyGateOutput, 1)
        typedef TObject inherited;
};


#endif

