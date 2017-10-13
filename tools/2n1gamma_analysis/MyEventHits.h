#ifndef MYEVENTHITS_H
#define MYEVENTHITS_H

#include "EventHits.h"
#include <TH3.h>
#include <TH2.h>
#include <TGraph2D.h>
//#include "Constants.h"

//#include <TLorentzVector.h>

class MyEventHits : public EventHits
{
public:
    TH1F* hRootEdep;
    TH1F* hRootEdep511keV;
    TH1F* hRootEdep511keVOne;
    TH1F* hRootEdep511keVTwo;
    TH1F* hRootEdepPrompt;
    TH1F* hRootEdepSmear;
    TH1F* hRootEdepSmear511keV;
    TH1F* hRootEdepSmear511keVOne;
    TH1F* hRootEdepSmear511keVTwo;
    TH1F* hRootEdepSmearPrompt;
    TH1F* hCutPassing;
    TH1F* hRootEdepSum;
    TH1F* hRootEdepSum511kev;
    TH1F* hRootEdepSumSmear;
    TH1F* hRootEdepSumSmear511kev;
    TH2F* hFermiCircle;
    TH2F* hFermiCircleSmear;
    TH3F* hFermiBall;
    TH3F* hFermiBallSmear;
    MyEventHits();
    virtual ~MyEventHits();
    virtual void Loop();
};

#endif // MYEVENTHITS_H
