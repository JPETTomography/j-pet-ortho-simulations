/// @file cuts_tests.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 02.10.2017
/// @version 1.3
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
          pManag->SetR(437.3);
          pManag->SetL(500);
          pManag->SetEff(0.17);

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
/// \attention Max error allowed for the probability to hit the detector is set to 1.0% for circa 30k events. Scales slowly for larger number of events, main reason: floating point numbers arithmetics.
///
TEST_F (cutsTestFixture, GeometricalAcceptanceTestCenter)
{
    int simSteps = 30007;
    double R = pManag->GetR();
    double L = pManag->GetL();
    double eff = 1;
    std::vector<TLorentzVector*> sourcePar;
    std::vector<TLorentzVector*> fourMomenta;
    Event* eventDecay;
    double weight = 0;

    InitialCuts* cuts3 = new InitialCuts(THREE, R, L, eff);
    cuts3->EnableSilentMode();

    //r is the distance between the source and the edge of detecting area
    double r = TMath::Sqrt(R*R+0.25*L*L);
    //one can calculate that the expression below is the probability to git the detector when source is in the center
    double pForGammaToPass = eff*L/2/r;
    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);
    for(int ii=0; ii<3; ii++)
    {
        sourcePar.push_back(new TLorentzVector(0.0, 0.0, 0.0, 0.0));
        fourMomenta.push_back(nullptr);
    }

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       weight = event->Generate();
       for(int ii=0; ii<3; ii++)
       {
           fourMomenta[ii]=event->GetDecay(ii);
       }

       eventDecay = new Event(&sourcePar, &fourMomenta, weight, type);
       //Applying cuts
       cuts3->AddCuts(eventDecay);
       delete eventDecay;

    }
    // how many gammas hit the detector
    double reallyPassed3 = ((double)cuts3->GetAcceptedGammas())/(double)simSteps/3.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed3, 0.01); //ASSERTION HERE with maximum error of 1.0%
    delete cuts3;

//    //// 2 GAMMA DECAYS
    InitialCuts* cuts2 = new InitialCuts(TWO, R, L, eff);
    cuts2->EnableSilentMode();
    event->SetDecay(Ps, 2, masses2);
//    // Generating 2-gamma events
    fourMomenta[2]=nullptr;
    for (int n=0; n<simSteps; n++)
    {
       weight = event->Generate();
       fourMomenta[0] = event->GetDecay(0);
       fourMomenta[1] = event->GetDecay(1);
       eventDecay = new Event(&sourcePar, &fourMomenta, weight, type);
       //Applying cuts
       cuts2->AddCuts(eventDecay);
       delete eventDecay;
    }
//     how many gammas hit the detector
    double reallyPassed2 = ((double)cuts2->GetAcceptedGammas())/(double)simSteps/2.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed2, 0.01); //ASSERTION HERE with maximum error of 1.0%
    delete cuts2;
}

///
/// \brief TEST_F (decayTestFixture, GeometricalAcceptanceTestZAxis) Tests if the geometrical acceptance is correctly calculated for both 2- and 3-gamma decays, when source is somewhere on the main symmetry axis of the barrel.
/// \attention Max error allowed for the probability to hit the detector is set to 1.0% for circa 30k events. Scales slowly for larger number of events, main reason: floating point numbers arithmetics.
///
TEST_F (cutsTestFixture, GeometricalAcceptanceTestZAxis)
{
    int simSteps = 30007;
    double R = pManag->GetR();
    double L = pManag->GetL();
    double eff = 1;
    double weight = 0;
    std::vector<TLorentzVector*> fourMomenta;
    std::vector<TLorentzVector*> sourcePar;
    for(int ii=0; ii<3; ii++)
    {
        fourMomenta.push_back(nullptr);
        sourcePar.push_back(new TLorentzVector(0.0, 0.0, L/2.0, 0.0));
    }

    //r is the distance between the source and the edge of detecting area
    //1 indicates the left side of the detector, 2 the right side
    double r1 = TMath::Sqrt((L/2.0-TMath::Abs(sourcePar[0]->Z())) * (L/2.0-TMath::Abs(sourcePar[0]->Z()))+ R*R);
    double r2 = TMath::Sqrt((L/2.0+TMath::Abs(sourcePar[0]->Z())) * (L/2.0+TMath::Abs(sourcePar[0]->Z())) + R*R);
    //S is the area of a spherical cup dictated by the solid angle
    double S1 = (2*TMath::Pi()*r1*(r1-(L/2.0)+TMath::Abs(sourcePar[0]->Z())));
    double S2 = (2*TMath::Pi()*r2*(r2-(L/2.0)-TMath::Abs(sourcePar[0]->Z())));
    //pForGammaToPass = 1 - S1/(4*PI*r1^2) - S2/(4*PI*r2^2)
    double pForGammaToPass = (1.0 - S1/4.0/TMath::Pi()/r1/r1 -  S2/4.0/TMath::Pi()/r2/r2);

    Event* eventDecay;

    InitialCuts* cuts3 = new InitialCuts(THREE, R, L, eff);
    cuts3->EnableSilentMode();

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       weight = event->Generate();
       fourMomenta[0] = event->GetDecay(0);
       fourMomenta[1] = event->GetDecay(1);
       fourMomenta[2] = event->GetDecay(2);
       eventDecay = new Event(&sourcePar, &fourMomenta, weight, type);
       //Applying cuts
       cuts3->AddCuts(eventDecay);
       delete eventDecay;
    }

    // how many gammas hit the detector
    double reallyPassed3 = ((double)cuts3->GetAcceptedGammas())/(double)simSteps/3.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed3, 0.01); //ASSERTION HERE with maximum error of 1.1%
    delete cuts3;
//    //// 2 GAMMA DECAYS
    InitialCuts* cuts2 = new InitialCuts(TWO, R, L, eff);
    cuts2->EnableSilentMode();
    event->SetDecay(Ps, 2, masses2);
    // Generating 2-gamma events
    fourMomenta[2]=nullptr;
    for (int n=0; n<simSteps; n++)
    {
       weight = event->Generate();
       fourMomenta[0] = event->GetDecay(0);
       fourMomenta[1] = event->GetDecay(1);
       eventDecay = new Event(&sourcePar, &fourMomenta, weight, type);
//       //Applying cuts
       cuts2->AddCuts(eventDecay);
       delete eventDecay;
    }
//    // how many gammas hit the detector
    double reallyPassed2 = ((double)cuts2->GetAcceptedGammas())/(double)simSteps/2.0;
    EXPECT_NEAR(pForGammaToPass, reallyPassed2, 0.01); //ASSERTION HERE with maximum error of 1.0%
    delete cuts2;
}

/////
///// \brief TEST_F(decayTestFixture, InteractionProbability3Gammas) Tests if the number of accepted 3-gamma events scales correctly with the probability of interacting with a detector (like p^3). p is random from (0.1; 0.3).
///// \attention Maximum relative error is set to 2.5% for 150k. Depends mainly on statistics.
/////
TEST_F(cutsTestFixture, InteractionProbability3Gammas)
{
    int simSteps = 150003;
    TRandom3 rand(0);
    double R = pManag->GetR();
    double L = pManag->GetL();
    float eff = rand.Uniform(0.1, 0.3);

    TLorentzVector* emission1= new TLorentzVector(rand.Uniform(0.0, R/2), rand.Uniform(0.0, R/2), rand.Uniform(-L/2, L/2), 0.0);
    std::vector<TLorentzVector*> fourMomenta;
    std::vector<TLorentzVector*> sourcePar;
    for(int ii=0; ii<3; ii++)
    {
        sourcePar.push_back(new TLorentzVector(*emission1));
        fourMomenta.push_back(nullptr);
    }

    Event* eventDecay;
    InitialCuts* cuts3_p1 = new InitialCuts(THREE, R, L, eff);
    cuts3_p1->SetDetectionProbability(1.0);
    cuts3_p1->EnableSilentMode();
    InitialCuts* cuts3 = new InitialCuts(THREE, R, L, eff);
    cuts3->SetDetectionProbability(eff);
    cuts3->EnableSilentMode();

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       fourMomenta[0] = event->GetDecay(0);
       fourMomenta[1] = event->GetDecay(1);
       fourMomenta[2] = event->GetDecay(2);
       eventDecay = new Event(&sourcePar, &fourMomenta, weight, type);
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
    double error = ((int)(TMath::Abs(interactionP-interactionPEq1*eff*eff*eff)/interactionPEq1*10000)/100.0);
    ASSERT_LE(error, 2.5); //ASSERTION HERE
    if(emission1) delete emission1;
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
    float eff = rand.Uniform(0.1, 0.3);

    std::vector<TLorentzVector*> fourMomenta;
    std::vector<TLorentzVector*> sourcePar;
    TLorentzVector* emission1= new TLorentzVector(rand.Uniform(0.0, R/2), rand.Uniform(0.0, R/2), rand.Uniform(-L/2, L/2), 0.0);
    for(int ii=0; ii<2; ii++)
    {
        sourcePar.push_back(new TLorentzVector(*emission1));
        fourMomenta.push_back(nullptr);
    }

    Event* eventDecay;
    InitialCuts* cuts2_p1 = new InitialCuts(TWO, R, L, eff);
    cuts2_p1->SetDetectionProbability(1.0);
    cuts2_p1->EnableSilentMode();
    InitialCuts* cuts2 = new InitialCuts(TWO, R, L, eff);
    cuts2->SetDetectionProbability(eff);
    cuts2->EnableSilentMode();

    //// 2 GAMMA DECAYS
    event->SetDecay(Ps, 2, masses2);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       fourMomenta[0] = event->GetDecay(0);
       fourMomenta[1] = event->GetDecay(1);
       eventDecay = new Event(&sourcePar, &fourMomenta, weight, type);
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
    double error = ((int)(TMath::Abs(interactionP-interactionPEq1*eff*eff)/interactionPEq1*10000)/100.0);
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
    double eff = 1.0;
    std::vector<TLorentzVector*> fourMomenta;
    std::vector<TLorentzVector*> sourcePar;
    for(int ii=0; ii<3; ii++)
    {
        fourMomenta.push_back(nullptr);
        sourcePar.push_back(new TLorentzVector(0.0, 0.0, 0.0, 0.0));
    }

    Event* eventDecay;
    InitialCuts* cuts3 = new InitialCuts(THREE, R, L, eff);
    cuts3->SetDetectionProbability(eff);
    cuts3->EnableSilentMode();

    //// 3 GAMMA DECAYS
    event->SetDecay(Ps, 3, masses3);

    // Generating 3-gamma events
    for (int n=0; n<simSteps; n++)
    {
       double weight = event->Generate();
       fourMomenta[0] = event->GetDecay(0);
       fourMomenta[1] = event->GetDecay(1);
       fourMomenta[2] = event->GetDecay(2);
       eventDecay = new Event(&sourcePar, &fourMomenta, weight, type);
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
