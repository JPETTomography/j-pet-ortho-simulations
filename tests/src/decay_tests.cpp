/// @file decay_tests.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 22.06.2017
/// @version 1.1
///
/// @section DESCRIPTION
/// The following tests test the functionality of PsDecay class.
/// Some of the test can fail due to statistical reasons.
/// They shouldn't fail more often than 1 per 100 test runs.

#include "gtest/gtest.h"
#include "psdecay.h"
#include "TGenPhaseSpace.h"
#include "TRandom3.h"
#include <sys/stat.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS //CXX11 support hacks
#include "boost/filesystem.hpp"
#undef BOOST_NO_CXX11_SCOPED_ENUMS

///
/// \brief The decayTestFixture class Sets the necessary fields for tests in this test case.
///
class decayTestFixture: public ::testing::Test
{
    public:
        TGenPhaseSpace* event; //event generator
        double* sourcePos; //position of the source
        PsDecay* decay3;    //objects for 2- and 3- gamma decays
        PsDecay* decay2;
        double masses2[2];
        double masses3[3];
        TLorentzVector Ps;  //four-momentum vector of the source
       decayTestFixture( )
       {
           // initialization code here
           event = new TGenPhaseSpace();
           sourcePos = new double[3]();
           Ps = TLorentzVector(0.000000001, 0.000000001, 0.000000001, 1.022/1000);
           for(int ii=0; ii<3; ii++)
           {
                sourcePos[ii]=0;
                masses3[ii]=0.0;
                if(ii<2)
                    masses2[ii]=0.0;
           }

           decay3 = new PsDecay(3,  sourcePos,  1.0, 0.50, 0.70);
           decay2 = new PsDecay(2,  sourcePos,  1.0, 0.50, 0.70);

       }

       void SetUp()
       {
           // code here will execute just before the test ensues
           decay2->EnableSilentMode();
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
           if(event) delete event;
       }

};

///
/// \brief TEST_F (decayTestFixture, GeometricalAcceptanceTestCenter) Tests if the geometrical acceptance is correctly calculated for both 2- and 3-gamma decays, when source is in the center of the barrel.
/// \attention Max error allowed for the probability to hit the detector is set to 1.1% for circa 30k events. Scales slowly for larger number of events, main reason: floating point numbers arithmetics.
///
TEST_F (decayTestFixture, GeometricalAcceptanceTestCenter)
{
    int simSteps = 30007;
    double R = decay3->GetRadius();
    double L = decay3->GetLength();

    //r is the distance between the source and the edge of detecting area
    double r = TMath::Sqrt(R*R+0.25*L*L); //distance from the center to the edge of detecting region
    //S is the area of a spherical cup dictated by the solid angle
    double S = (2*TMath::Pi()*r*(r-(L/2.0)));
    double pForGammaToPass = (1.0 - 2.0*S/4.0/TMath::Pi()/r/r);

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
    // how many gammas hit the detector
    double reallyPassed3 = ((double)decay3->GetAcceptedGammas())/(double)simSteps/3.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed3, 0.011); //ASSERTION HERE with maximum error of 1.1%

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
    // how many gammas hit the detector
    double reallyPassed2 = ((double)decay2->GetAcceptedGammas())/(double)simSteps/2.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed2, 0.011); //ASSERTION HERE with maximum error of 1.1%
}

///
/// \brief TEST_F (decayTestFixture, GeometricalAcceptanceTestZAxis) Tests if the geometrical acceptance is correctly calculated for both 2- and 3-gamma decays, when source is somewhere on the main symmetry axis of the barrel.
/// \attention Max error allowed for the probability to hit the detector is set to 1.1% for circa 30k events. Scales slowly for larger number of events, main reason: floating point numbers arithmetics.
///
TEST_F (decayTestFixture, GeometricalAcceptanceTestZAxis)
{
    int simSteps = 30001;
    double pos[3]={0.0, 0.0, decay3->GetLength()/(-2.0)};
    double R = decay3->GetRadius();
    double L = decay3->GetLength();
    //r is the distance between the source and the edge of detecting area
    //1 indicates the left side of the detector, 2 the right side
    double r1 = TMath::Sqrt((L/2.0-TMath::Abs(pos[2])) * (L/2.0-TMath::Abs(pos[2]))+ R*R);
    double r2 = TMath::Sqrt((L/2.0+TMath::Abs(pos[2])) * (L/2.0+TMath::Abs(pos[2])) + R*R);
    //S is the area of a spherical cup dictated by the solid angle
    double S1 = (2*TMath::Pi()*r1*(r1-(L/2.0)+TMath::Abs(pos[2])));
    double S2 = (2*TMath::Pi()*r2*(r2-(L/2.0)-TMath::Abs(pos[2])));
    //pForGammaToPass = 1 - S1/(4*PI*r1^2) - S2/(4*PI*r2^2)
    double pForGammaToPass = (1.0 - S1/4.0/TMath::Pi()/r1/r1 -  S2/4.0/TMath::Pi()/r2/r2);

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
    //empirical probability to pass the geometrical acceptance cut
    double reallyPassed3 = ((double)decay3->GetAcceptedGammas())/((double)simSteps)/3.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed3, 0.011); //ASSERTION HERE with maximum error 1.1%

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

    //empirical probability to pass the geometrical acceptance cut
    double reallyPassed2 = ((double)decay2->GetAcceptedGammas())/((double)simSteps)/2.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed2, 0.011); //ASSERTION HERE with maximum error 1.1%
}

///
/// \brief TEST_F(decayTestFixture, InteractionProbability3Gammas) Tests if the number of accepted 3-gamma events scales correctly with the probability of interacting with a detector (like p^3). p is random from (0.1; 0.3).
/// \attention Maximum relative error is set to 2% for 150k. If set to 2% then 1 or 2 out of 100 will fail. Depends mainly on statistics.
///
TEST_F(decayTestFixture, InteractionProbability3Gammas)
{
    int simSteps = 150003;
    TRandom3 rand(0);
    double R = decay3->GetRadius();
    double L = decay3->GetLength();
    double pos[3]={rand.Uniform(0.0, R/2), rand.Uniform(0.0, R/2), rand.Uniform(-L/2, L/2)};

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);
    decay3->SetSourcePos(pos);
    PsDecay* decay3_copy = new PsDecay(*decay3);

    decay3->SetDetectionProbability(1.0);
    // creating another PsDecay object with random p
    float p = rand.Uniform(0.1, 0.3);
    decay3_copy->SetDetectionProbability(p);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = event->GetDecay(2);
       decay3->AddEvent(weight, gamma1, gamma2, gamma3);
       decay3_copy->AddEvent(weight, gamma1, gamma2, gamma3);
    }
    //number of accepted events when interaction probability is set to 1.0
    float interactionPEq1 = decay3->GetAcceptedEvents();
    //number of accepted events when interaction probability is set to p
    float interactionP = decay3_copy->GetAcceptedEvents();
    delete decay3_copy;
    //relative error in %
    double error = ((int)(TMath::Abs(interactionP-interactionPEq1*p*p*p)/interactionPEq1*10000)/100.0);
    ASSERT_LE(error, 2.5); //ASSERTION HERE
}

///
/// \brief TEST_F(decayTestFixture, InteractionProbability2Gammas) Tests if the number of accepted 2-gamma events scales correctly with the probability of interacting with a detector (like p^3). p is random from (0.1; 0.3).
/// \attention Maximum relative error is set to 2% for 150k. Lower value than for 3-gammas is caused by better statistics (more 2-gamma events pass the geometrical acceptance cut).
///
TEST_F(decayTestFixture, InteractionProbability2Gammas)
{
    int simSteps = 150003;
    TRandom3 rand(0);
    double R = decay2->GetRadius();
    double L = decay2->GetLength();
    double pos[3]={rand.Uniform(0.0, R/2), rand.Uniform(0.0, R/2), rand.Uniform(-L/2, L/2)};
    //// 2 GAMMA DECAYS
    event->SetDecay(Ps, 2, masses2);
    decay2->SetSourcePos(pos);
    PsDecay* decay2_copy = new PsDecay(*decay2);

    decay2->SetDetectionProbability(1.0);
    float p = rand.Uniform(0.1, 0.3);
    decay2_copy->SetDetectionProbability(p);

    // Generating 2-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();

       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = nullptr;
       decay2->AddEvent(weight, gamma1, gamma2, gamma3);
       decay2_copy->AddEvent(weight, gamma1, gamma2, gamma3);
    }
    //number of accepted events when interaction probability is set to 1.0
    float interactionPEq1 = decay2->GetAcceptedEvents();
    //number of accepted events when interaction probability is set to p
    float interactionP = decay2_copy->GetAcceptedEvents();
    delete decay2_copy;
    //relative error in %
    double error = ((int)(TMath::Abs(interactionP-interactionPEq1*p*p)/interactionPEq1*10000)/100.0);
    ASSERT_LE(error, 2); //ASSERTION HERE
}

///
/// \brief TEST_F(decayTestFixture, DiskAccess) Tests the possibility to create/remove directories and saving histograms.
///
TEST_F(decayTestFixture, DiskAccess)
{
    int simSteps = 100;
    double pos[3]={0.0, 0.0, 0.0};
    event->SetDecay(Ps, 2, masses2);
    decay2->SetSourcePos(pos);
    // Generating 2-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();

       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = nullptr;
       decay2->AddEvent(weight, gamma1, gamma2, gamma3);
    }
    // creating subdirectories and changing their access rights
    mkdir("test_tmp", ACCESSPERMS);
    chmod("test_tmp", ACCESSPERMS);
    mkdir("test_tmp/results", ACCESSPERMS);
    chmod("test_tmp/results", ACCESSPERMS);

    decay2->DrawHistograms("test_tmp/results/");
    boost::filesystem::path p ("test_tmp/results");
    int noOfFiles = 0;
    try
    {
        if (boost::filesystem::exists(p)) //check if the path is correct
        {
          if (boost::filesystem::is_directory(p)) //check if it's a folder
          {
            for (boost::filesystem::directory_entry& x : boost::filesystem::directory_iterator(p))
              {
                noOfFiles++; //count elements in a folder
              }
          }
          else
            ADD_FAILURE();
        }
        else
          ADD_FAILURE();
      }
    catch (const boost::filesystem::filesystem_error& ex)
    {
        FAIL(); //if any exception is thrown, then fail
    }

    EXPECT_EQ(7, noOfFiles); //check if the number of histograms is correct
    boost::filesystem::remove_all("test_tmp");
    ASSERT_FALSE(boost::filesystem::exists("test_tmp")); //check if the folder was removed
}
