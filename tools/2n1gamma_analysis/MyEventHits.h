#ifndef MYEVENTHITS_H
#define MYEVENTHITS_H

#include "EventHits.h"
#include <TH3.h>
//#include "Constants.h"

//#include <TLorentzVector.h>

class MyEventHits : public EventHits
{
public:
    TH1F* hRootEdep;
    TH1F* hRootEdep511keV;
    TH1F* hRootEdepPrompt;
    TH1F* hRootEdepSmear;
    TH1F* hRootEdepSmear511keV;
    TH1F* hRootEdepSmearPrompt;
    TH1F* hCutPassing;
    MyEventHits();
    virtual ~MyEventHits();
    virtual void Loop();
};

#endif // MYEVENTHITS_H
