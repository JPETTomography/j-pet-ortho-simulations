/// @file cuts_tests.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 24.07.2017
/// @version 1.2
///
/// @section DESCRIPTION
/// The following tests test the functionality of InitialCuts class.
/// Some of the test can fail due to statistical reasons.
/// They shouldn't fail more often than 1 per 100 test runs.

#include "gtest/gtest.h"
#include "../../src/initialcuts.h"
#include "../../src/event.h"
#include "TGenPhaseSpace.h"
#include "TRandom3.h"
#include <sys/stat.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS //CXX11 support hacks
#include "boost/filesystem.hpp"
#undef BOOST_NO_CXX11_SCOPED_ENUMS

///
/// \brief The cutsTestFixture class Sets the necessary fields for tests in this test case.
///
class cutsTestFixture: public ::testing::Test
{
    //TODO: fix the linker problem
    public:
       TGenPhaseSpace* event; //event generator
       double* sourcePos; //position of the source
       ParamManager* pManag;    //objects for 2- and 3- gamma decays
       double masses2[2];
       double masses3[3];
       DecayType type;
       TLorentzVector Ps;  //four-momentum vector of the source

       cutsTestFixture( )
       {
          // initialization code here
          type = THREE;
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
          pManag = new ParamManager();
          pManag->SetR(500);
          pManag->SetL(700);
          pManag->SetP(1.0);

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

       ~cutsTestFixture( )
       {
           // cleanup any pending stuff, but no exceptions allowed
           if(pManag) delete pManag;
           if(sourcePos) delete[] sourcePos;
           if(event) delete event;
       }

};

///
/// \brief TEST_F (decayTestFixture, GeometricalAcceptanceTestCenter) Tests if the geometrical acceptance is correctly calculated for both 2- and 3-gamma decays, when source is in the center of the barrel.
/// \attention Max error allowed for the probability to hit the detector is set to 1.1% for circa 30k events. Scales slowly for larger number of events, main reason: floating point numbers arithmetics.
///
TEST_F (cutsTestFixture, GeometricalAcceptanceTestCenter)
{
    int simSteps = 30007;
    double R = pManag->GetR();
    double L = pManag->GetL();
    double p = pManag->GetP();
    TLorentzVector* emission1= new TLorentzVector(0.0, 0.0, 0.0, 0.0);
    TLorentzVector* emission2 = new TLorentzVector(*emission1); //making copies
    TLorentzVector* emission3 = new TLorentzVector(*emission1);
    TLorentzVector* sourcePar[3] = {emission1, emission2, emission3};
    TLorentzVector* fourMomenta[3];

    Event* eventDecay;

    InitialCuts* cuts3 = new InitialCuts(THREE, R, L, p);
    cuts3->EnableSilentMode();

    //r is the distance between the source and the edge of detecting area
    double r = TMath::Sqrt(R*R+0.25*L*L); //distance from the center to the edge of detecting region
    //S is the area of a spherical cup dictated by the solid angle
    double S = (2*TMath::Pi()*r*(r-(L/2.0)));
    double pForGammaToPass = (1.0 - 2.0*S/4.0/TMath::Pi()/r/r);

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = event->GetDecay(2);
       fourMomenta[0] = gamma1;
       fourMomenta[1] = gamma2;
       fourMomenta[2] = gamma3;
       eventDecay = new Event(sourcePar, fourMomenta, weight, type);
       //Applying cuts
       cuts3->AddCuts(eventDecay);
       delete eventDecay;
    }

    // how many gammas hit the detector
    double reallyPassed3 = ((double)cuts3->GetAcceptedGammas())/(double)simSteps/3.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed3, 0.011); //ASSERTION HERE with maximum error of 1.1%
    delete cuts3;
    //// 2 GAMMA DECAYS
    InitialCuts* cuts2 = new InitialCuts(TWO, R, L, p);
    cuts2->EnableSilentMode();
    event->SetDecay(Ps, 2, masses2);
    // Generating 2-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = nullptr;
       fourMomenta[0] = gamma1;
       fourMomenta[1] = gamma2;
       fourMomenta[2] = gamma3;
       eventDecay = new Event(sourcePar, fourMomenta, weight, type);
       //Applying cuts
       cuts2->AddCuts(eventDecay);
       delete eventDecay;
    }
    // how many gammas hit the detector
    double reallyPassed2 = ((double)cuts2->GetAcceptedGammas())/(double)simSteps/2.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed2, 0.011); //ASSERTION HERE with maximum error of 1.1%
    delete cuts2;
    delete emission1;
    delete emission2;
    delete emission3;
}

///
/// \brief TEST_F (decayTestFixture, GeometricalAcceptanceTestZAxis) Tests if the geometrical acceptance is correctly calculated for both 2- and 3-gamma decays, when source is somewhere on the main symmetry axis of the barrel.
/// \attention Max error allowed for the probability to hit the detector is set to 1.1% for circa 30k events. Scales slowly for larger number of events, main reason: floating point numbers arithmetics.
///
TEST_F (cutsTestFixture, GeometricalAcceptanceTestZAxis)
{
    int simSteps = 30007;
    double R = pManag->GetR();
    double L = pManag->GetL();
    double p = pManag->GetP();

    TLorentzVector* emission1= new TLorentzVector(0.0, 0.0, L/2.0, 0.0);
    TLorentzVector* emission2 = new TLorentzVector(*emission1); //making copies
    TLorentzVector* emission3 = new TLorentzVector(*emission1);
    TLorentzVector* sourcePar[3] = {emission1, emission2, emission3};
    TLorentzVector* fourMomenta[3];

    //r is the distance between the source and the edge of detecting area
    //1 indicates the left side of the detector, 2 the right side
    double r1 = TMath::Sqrt((L/2.0-TMath::Abs(emission1->Z())) * (L/2.0-TMath::Abs(emission1->Z()))+ R*R);
    double r2 = TMath::Sqrt((L/2.0+TMath::Abs(emission1->Z())) * (L/2.0+TMath::Abs(emission1->Z())) + R*R);
    //S is the area of a spherical cup dictated by the solid angle
    double S1 = (2*TMath::Pi()*r1*(r1-(L/2.0)+TMath::Abs(emission1->Z())));
    double S2 = (2*TMath::Pi()*r2*(r2-(L/2.0)-TMath::Abs(emission1->Z())));
    //pForGammaToPass = 1 - S1/(4*PI*r1^2) - S2/(4*PI*r2^2)
    double pForGammaToPass = (1.0 - S1/4.0/TMath::Pi()/r1/r1 -  S2/4.0/TMath::Pi()/r2/r2);

    Event* eventDecay;

    InitialCuts* cuts3 = new InitialCuts(THREE, R, L, p);
    cuts3->EnableSilentMode();

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = event->GetDecay(2);
       fourMomenta[0] = gamma1;
       fourMomenta[1] = gamma2;
       fourMomenta[2] = gamma3;
       eventDecay = new Event(sourcePar, fourMomenta, weight, type);
       //Applying cuts
       cuts3->AddCuts(eventDecay);
       delete eventDecay;
    }

    // how many gammas hit the detector
    double reallyPassed3 = ((double)cuts3->GetAcceptedGammas())/(double)simSteps/3.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed3, 0.011); //ASSERTION HERE with maximum error of 1.1%
    delete cuts3;
    //// 2 GAMMA DECAYS
    InitialCuts* cuts2 = new InitialCuts(TWO, R, L, p);
    cuts2->EnableSilentMode();
    event->SetDecay(Ps, 2, masses2);
    // Generating 2-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = nullptr;
       fourMomenta[0] = gamma1;
       fourMomenta[1] = gamma2;
       fourMomenta[2] = gamma3;
       eventDecay = new Event(sourcePar, fourMomenta, weight, type);
       //Applying cuts
       cuts2->AddCuts(eventDecay);
       delete eventDecay;
    }
    // how many gammas hit the detector
    double reallyPassed2 = ((double)cuts2->GetAcceptedGammas())/(double)simSteps/2.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed2, 0.011); //ASSERTION HERE with maximum error of 1.1%
    delete cuts2;
    delete emission1;
    delete emission2;
    delete emission3;
}

/////
///// \brief TEST_F(decayTestFixture, InteractionProbability3Gammas) Tests if the number of accepted 3-gamma events scales correctly with the probability of interacting with a detector (like p^3). p is random from (0.1; 0.3).
///// \attention Maximum relative error is set to 2% for 150k. If set to 2% then 1 or 2 out of 100 will fail. Depends mainly on statistics.
/////
TEST_F(cutsTestFixture, InteractionProbability3Gammas)
{
    int simSteps = 150003;
    TRandom3 rand(0);
    double R = pManag->GetR();
    double L = pManag->GetL();
    float p = rand.Uniform(0.1, 0.3);

    TLorentzVector* emission1= new TLorentzVector(rand.Uniform(0.0, R/2), rand.Uniform(0.0, R/2), rand.Uniform(-L/2, L/2), 0.0);
    TLorentzVector* emission2 = new TLorentzVector(*emission1); //making copies
    TLorentzVector* emission3 = new TLorentzVector(*emission1);
    TLorentzVector* sourcePar[3] = {emission1, emission2, emission3};
    TLorentzVector* fourMomenta[3];

    Event* eventDecay;
    InitialCuts* cuts3_p1 = new InitialCuts(THREE, R, L, p);
    cuts3_p1->SetDetectionProbability(1.0);
    cuts3_p1->EnableSilentMode();
    InitialCuts* cuts3 = new InitialCuts(THREE, R, L, p);
    cuts3->SetDetectionProbability(p);
    cuts3->EnableSilentMode();

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = event->GetDecay(2);
       fourMomenta[0] = gamma1;
       fourMomenta[1] = gamma2;
       fourMomenta[2] = gamma3;
       eventDecay = new Event(sourcePar, fourMomenta, weight, type);
       //Applying cuts
       cuts3->AddCuts(eventDecay);
       cuts3_p1->AddCuts(eventDecay);
       delete eventDecay;
    }
    //number of accepted events when interaction probability is set to 1.0
    float interactionPEq1 = cuts3_p1->GetAcceptedEvents();
    //number of accepted events when interaction probability is set to p
    float interactionP = cuts3->GetAcceptedEvents();
    delete cuts3;
    delete cuts3_p1;

    //relative error in %
    double error = ((int)(TMath::Abs(interactionP-interactionPEq1*p*p*p)/interactionPEq1*10000)/100.0);
    ASSERT_LE(error, 2.5); //ASSERTION HERE
}

/////
///// \brief TEST_F(decayTestFixture, InteractionProbability2Gammas) Tests if the number of accepted 2-gamma events scales correctly with the probability of interacting with a detector (like p^3). p is random from (0.1; 0.3).
///// \attention Maximum relative error is set to 2% for 150k. Lower value than for 3-gammas is caused by better statistics (more 2-gamma events pass the geometrical acceptance cut).
/////
TEST_F(cutsTestFixture, InteractionProbability2Gammas)
{
    int simSteps = 150003;
    TRandom3 rand(0);
    double R = pManag->GetR();
    double L = pManag->GetL();
    float p = rand.Uniform(0.1, 0.3);

    TLorentzVector* emission1= new TLorentzVector(rand.Uniform(0.0, R/2), rand.Uniform(0.0, R/2), rand.Uniform(-L/2, L/2), 0.0);
    TLorentzVector* emission2 = new TLorentzVector(*emission1); //making copies
    TLorentzVector* emission3 = nullptr;
    TLorentzVector* sourcePar[3] = {emission1, emission2, emission3};
    TLorentzVector* fourMomenta[3];

    Event* eventDecay;
    InitialCuts* cuts2_p1 = new InitialCuts(TWO, R, L, p);
    cuts2_p1->SetDetectionProbability(1.0);
    cuts2_p1->EnableSilentMode();
    InitialCuts* cuts2 = new InitialCuts(TWO, R, L, p);
    cuts2->SetDetectionProbability(p);
    cuts2->EnableSilentMode();

    //// 2 GAMMA DECAYS
    event->SetDecay(Ps, 2, masses2);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = nullptr;
       fourMomenta[0] = gamma1;
       fourMomenta[1] = gamma2;
       fourMomenta[2] = gamma3;
       eventDecay = new Event(sourcePar, fourMomenta, weight, type);
       //Applying cuts
       cuts2->AddCuts(eventDecay);
       cuts2_p1->AddCuts(eventDecay);
       delete eventDecay;
    }
    //number of accepted events when interaction probability is set to 1.0
    float interactionPEq1 = cuts2_p1->GetAcceptedEvents();
    //number of accepted events when interaction probability is set to p
    float interactionP = cuts2->GetAcceptedEvents();
    delete cuts2;
    delete cuts2_p1;

    //relative error in %
    double error = ((int)(TMath::Abs(interactionP-interactionPEq1*p*p)/interactionPEq1*10000)/100.0);
    ASSERT_LE(error, 2); //ASSERTION HERE
}

/////
///// \brief TEST_F(decayTestFixture, DiskAccess) Tests the possibility to create/remove directories and saving histograms.
/////
TEST_F(cutsTestFixture, DiskAccessAndInitialCutsNoOfHistograms)
{
    int simSteps = 100;
    double R = pManag->GetR();
    double L = pManag->GetL();
    double p = 1.0;
    TLorentzVector* emission1= new TLorentzVector(0.0, 0.0, 0.0, 0.0);
    TLorentzVector* emission2 = new TLorentzVector(*emission1); //making copies
    TLorentzVector* emission3 = new TLorentzVector(*emission1);
    TLorentzVector* sourcePar[3] = {emission1, emission2, emission3};
    TLorentzVector* fourMomenta[3];

    Event* eventDecay;
    InitialCuts* cuts3 = new InitialCuts(THREE, R, L, p);
    cuts3->SetDetectionProbability(p);
    cuts3->EnableSilentMode();

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       TLorentzVector *gamma1 = event->GetDecay(0);
       TLorentzVector *gamma2 = event->GetDecay(1);
       TLorentzVector *gamma3 = event->GetDecay(2);
       fourMomenta[0] = gamma1;
       fourMomenta[1] = gamma2;
       fourMomenta[2] = gamma3;
       eventDecay = new Event(sourcePar, fourMomenta, weight, type);
       //Applying cuts
       cuts3->AddCuts(eventDecay);
       delete eventDecay;
    }

    // creating subdirectories and changing their access rights
    mkdir("test_tmp", ACCESSPERMS);
    chmod("test_tmp", ACCESSPERMS);

    cuts3->DrawHistograms("test_tmp/");
    boost::filesystem::path path ("test_tmp/");
    int noOfFiles = 0;
    try
    {
        if (boost::filesystem::exists(path)) //check if the path is correct
        {
          if (boost::filesystem::is_directory(path)) //check if it's a folder
          {
            for (boost::filesystem::directory_entry& x : boost::filesystem::directory_iterator(path))
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

    EXPECT_EQ(6, noOfFiles); //check if the number of histograms is correct
    boost::filesystem::remove_all("test_tmp");
    ASSERT_FALSE(boost::filesystem::exists("test_tmp")); //check if the folder was removed*/
}
