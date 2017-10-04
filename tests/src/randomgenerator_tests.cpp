/// @file randomgenerator_tests.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 04.10.2017
/// @version 1.0
///
/// @section DESCRIPTION
/// The following tests check if random seed settings cause the expected behavior.
#include "gtest/gtest.h"
#include "../../src/particlegenerator.h"
#include "../../src/psdecay.h"
#include "../../src/parammanager.h"
#include "../../src/comptonscattering.h"
#include "../../src/initialcuts.h"
#include <fstream>
#include <TLorentzVector.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS //CXX11 support hacks
#include "boost/filesystem.hpp"
#undef BOOST_NO_CXX11_SCOPED_ENUMS

//
/// \brief The TwoAndNTestFixture class Sets the necessary fields for tests in this test case.
///
class RandomGeneratorTestFixture: public ::testing::Test
{
    //TODO: fix the linker problem
    public:
       TGenPhaseSpace event; //event generator
       TLorentzVector sourcePos; //position of the source
       ParamManager pManag;    //objects for 2- and 3- gamma decays
       DecayType type;
       TLorentzVector Ps;  //four-momentum vector of the source
       double masses2[2];
       PsDecay* decay;
       TH1F* hist;
       RandomGeneratorTestFixture( )
       {
          // initialization code here
          type = TWOandONE;
          decay = new PsDecay(type);
          Ps = TLorentzVector(0.000000001, 0.000000001, 0.000000001, 1.022/1000);
          for(int ii=0; ii<4; ii++)
          {
              sourcePos[ii] = 0;
               if(ii<2)
                   masses2[ii]=0.0;
          }
          pManag.SetR(437.3);
          pManag.SetL(500);
          pManag.SetEff(1);
          pManag.SetP(0.98);
          pManag.SetE(1157);
          pManag.EnableSilentMode();
          pManag.SetSeed(123456789);
          event.SetDecay(Ps, 2, masses2);

       }
       ///
       /// \brief SetUp Creates a dummy file with 2&N data. File is removed by destructor.
       ///
       void SetUp()
       {

       }

       void TearDown( )
       {
           // code here will be called just after the test completes
           // ok to through exceptions from here if need be
       }

       ~RandomGeneratorTestFixture( )
       {
           // cleanup any pending stuff, but no exceptions allowed
       }

};

///
/// \brief TEST_F This test checks if results obtained with random seed are really random.
///
TEST_F(RandomGeneratorTestFixture, RandomResults)
{
    Event* eventDecay = nullptr;
    ComptonScattering cs1(type, 0.0, 2.0);
    InitialCuts cuts1(type, pManag.GetR(),pManag.GetL(), pManag.GetEff());
    ComptonScattering cs2(type, 0.0, 2.0);
    InitialCuts cuts2(type, pManag.GetR(),pManag.GetL(), pManag.GetEff());
    gRandom = new TRandom3(0);
    for (int n=0; n<100; n++)
    {
        try
        {
            eventDecay = generateEvent(event, sourcePos, pManag, type);
            cuts1.AddCuts(eventDecay);
            cs1.Scatter(eventDecay);
        }
        catch(std::string ex)
        {
            FAIL();
        }
    }
    gRandom = new TRandom3(0);
    for (int n=0; n<100; n++)
    {
        try
        {
            eventDecay = generateEvent(event, sourcePos, pManag, type);
            cuts2.AddCuts(eventDecay);
            cs2.Scatter(eventDecay);
        }
        catch(std::string ex)
        {
            FAIL();
        }
    }
    ASSERT_FALSE(cs1==cs2);
}

///
/// \brief TEST_F This test checks if results obtained with the same random seed are the identical.
///
TEST_F(RandomGeneratorTestFixture, RepeatableResults)
{
    Event* eventDecay = nullptr;
    ComptonScattering cs1(type, 0.0, 2.0);
    InitialCuts cuts1(type, pManag.GetR(),pManag.GetL(), pManag.GetEff());
    ComptonScattering cs2(type, 0.0, 2.0);
    InitialCuts cuts2(type, pManag.GetR(),pManag.GetL(), pManag.GetEff());
    gRandom = new TRandom3(pManag.GetSeed());
    for (int n=0; n<100; n++)
    {
        try
        {
            eventDecay = generateEvent(event, sourcePos, pManag, type);
            cuts1.AddCuts(eventDecay);
            cs1.Scatter(eventDecay);
        }
        catch(std::string ex)
        {
            FAIL();
        }
    }
    gRandom = new TRandom3(pManag.GetSeed());
    for (int n=0; n<100; n++)
    {
        try
        {
            eventDecay = generateEvent(event, sourcePos, pManag, type);
            cuts2.AddCuts(eventDecay);
            cs2.Scatter(eventDecay);
        }
        catch(std::string ex)
        {
            FAIL();
        }
    }
    ASSERT_TRUE(cs1==cs2);
}
