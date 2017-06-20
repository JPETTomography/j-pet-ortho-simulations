#include "gtest/gtest.h"
#include "psdecay.h"
#include "TGenPhaseSpace.h"
//#include "TMath"

class decayTestFixture: public ::testing::Test
{
    public:
        TGenPhaseSpace* event;
        double* sourcePos;
        PsDecay* decay3;
        PsDecay* decay2;

       decayTestFixture( )
       {
           // initialization code here
           event = new TGenPhaseSpace();
           sourcePos = new double[3]();
           for(int ii=0; ii<3; ii++)
           {
                sourcePos[ii]=0;
           }

           decay3 = new PsDecay(3,  sourcePos,  1.0, 0.50, 0.70);
           decay2 = new PsDecay(2,  sourcePos,  1.0, 0.50, 0.70);

       }

       void SetUp()
       {
           // code here will execute just before the test ensues
       }

       void TearDown( )
       {
           // code here will be called just after the test completes
           // ok to through exceptions from here if need be
       }

       ~decayTestFixture( )
       {
           // cleanup any pending stuff, but no exceptions allowed
           if(decay2) delete decay2;
           if(decay3) delete decay3;
           if(sourcePos) delete[] sourcePos;
       }

       // put in any custom data members that you need
};

TEST_F (decayTestFixture, GeometricalAcceptanceTestCenter)
{

    double masses3[3] = {0.0, 0.0, 0.0};
    double masses2[2] = {0.0, 0.0};
    TLorentzVector Ps(0.000000001, 0.000000001, 0.000000001, 1.022/1000);
    int simSteps = 100000;

    double r = TMath::Sqrt(decay3->GetRadius()*decay3->GetRadius()+0.25*decay3->GetLength()*decay3->GetLength()); //distance from the center to the edge of detecting region
    double S = (2*TMath::Pi()*r*(r-(decay3->GetLength()/2.0)));
    float pForGammaToPass = ((int)(100*(1.0 - 2*S/4.0/TMath::Pi()/r/r))) / 100.0;

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);
    decay3->SetDetectionProbability(1.0);
    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();

       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = event->GetDecay(2);
       decay3->AddEvent(weight, gamma1, gamma2, gamma3);
    }

    float reallyPassed3 = ((int)(100*((float)decay3->GetAcceptedGammas())/simSteps/3.0)) / 100.0;
    ASSERT_FLOAT_EQ(pForGammaToPass, reallyPassed3);

    //// 2 GAMMA DECAYS
    event->SetDecay(Ps, 2, masses2);
    decay2->SetDetectionProbability(1.0);
    // Generating 2-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();

       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = nullptr;
       decay2->AddEvent(weight, gamma1, gamma2, gamma3);
    }

    float reallyPassed2 = ((int)(100*((float)decay2->GetAcceptedGammas())/simSteps/2.0)) / 100.0;
    ASSERT_FLOAT_EQ(pForGammaToPass, reallyPassed2);
}

TEST_F (decayTestFixture, GeometricalAcceptanceTestZAxis)
{

    double masses3[3] = {0.0, 0.0, 0.0};
    double masses2[2] = {0.0, 0.0};
    TLorentzVector Ps(0.000000001, 0.000000001, 0.000000001, 1.022/1000);
    int simSteps = 1000000;

    double pos[3]={0.0, 0.0, decay3->GetLength()/(-4.0)};
    //1 indicates the left side of the detector, 2 the right side
    double r1 = TMath::Sqrt((decay3->GetLength()/2.0-TMath::Abs(pos[2])) * (decay3->GetLength()/2.0-TMath::Abs(pos[2]))\
            + ((decay3->GetRadius()) * (decay3->GetRadius())));
    double r2 = TMath::Sqrt((decay3->GetLength()/2.0+TMath::Abs(pos[2])) * (decay3->GetLength()/2.0+TMath::Abs(pos[2]))\
            + ((decay3->GetRadius()) * (decay3->GetRadius())));
    double S1 = (2*TMath::Pi()*r1*(r1-(decay3->GetLength()/2.0)));
    double S2 = (2*TMath::Pi()*r2*(r2-(decay3->GetLength()/2.0)));

    std::cout<<S1/4.0/TMath::Pi()/r1/r1<<"  "<<S2/4.0/TMath::Pi()/r2/r2<<std::endl;
    float pForGammaToPass = ((int)(100*(1.0 - S1/4.0/TMath::Pi()/r1/r1 -  S2/4.0/TMath::Pi()/r2/r2))) / 100.0;

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);
    decay3->SetSourcePos(pos);
    decay3->SetDetectionProbability(1.0);
    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();

       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = event->GetDecay(2);
       decay3->AddEvent(weight, gamma1, gamma2, gamma3);
    }

    float reallyPassed3 = ((int)(100*((float)decay3->GetAcceptedGammas())/simSteps/3.0)) / 100.0;
    ASSERT_FLOAT_EQ(pForGammaToPass, reallyPassed3);

    //// 2 GAMMA DECAYS
    event->SetDecay(Ps, 2, masses2);
    decay2->SetSourcePos(pos);
    decay2->SetDetectionProbability(1.0);
    // Generating 2-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();

       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = nullptr;
       decay2->AddEvent(weight, gamma1, gamma2, gamma3);
    }

    float reallyPassed2 = ((int)(100*((float)decay2->GetAcceptedGammas())/simSteps/2.0)) / 100.0;
    ASSERT_FLOAT_EQ(pForGammaToPass, reallyPassed2);
}


