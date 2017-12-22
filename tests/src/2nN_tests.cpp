/// @file 2nN_tests.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 03.10.2017
/// @version 1.0
///
/// @section DESCRIPTION
/// The following tests test the possibility to generate 2+N gamma decays (beta decay to
/// an excited atomic state, deexcitation is followed with the emission of N photons.
#include "gtest/gtest.h"
#include "../../src/particlegenerator.h"
#include "../../src/parammanager.h"
#include "../../src/psdecay.h"
#include <fstream>
#include <TLorentzVector.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS //CXX11 support hacks
#include "boost/filesystem.hpp"
#undef BOOST_NO_CXX11_SCOPED_ENUMS
//
/// \brief The TwoAndNTestFixture class Sets the necessary fields for tests in this test case.
///
class TwoAndNTestFixture: public ::testing::Test
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
       TwoAndNTestFixture( )
       {
          // initialization code here
          type = TWOandN;
          decay = new PsDecay(type);
          hist = decay->fH_en_;
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
          pManag.EnableSilentMode();
          event.SetDecay(Ps, 2, masses2);

       }
       ///
       /// \brief SetUp Creates a dummy file with 2&N data. File is removed by destructor.
       ///
       void SetUp()
       {
           // code here will execute just before the test ensues
           std::ofstream myfile ("tmp.dat");
           if (myfile.is_open())
           {
               myfile << "#This is a commented line.\n";
               myfile << "#This is another line.\n";
               myfile << "0.40 250 750 1250 1750\n";
               myfile << "0.10 0";
               myfile.close();
           }
           else std::cout << "Unable to open file!";
       }

       void TearDown( )
       {
           // code here will be called just after the test completes
           // ok to through exceptions from here if need be
       }
       ///
       /// \brief SetDataManually Used to load some dummy data into ParamManager instance.
       ///
       void SetDataManually()
       {
           pManag.fDecayBranchProbability_.push_back(0.8);
           pManag.fDecayBranchProbability_.push_back(0.2);
           std::vector<double> en1;
           std::vector<double> en2;
           en2.push_back(0.0);
           for(int ii=0; ii<4; ii++)
                en1.push_back(250.0+ii*500.0);
          pManag.fGammaEnergy_.push_back(en1);
          pManag.fGammaEnergy_.push_back(en2);
       }

       ~TwoAndNTestFixture( )
       {
           // cleanup any pending stuff, but no exceptions allowed
           if(decay) delete decay;
           boost::filesystem::remove_all("tmp.dat");
       }

};

///
/// \brief TEST_F Checks if data is correctly loaded from file and renormalized.
///
TEST_F(TwoAndNTestFixture, ReadingParametersFromFile)
{
    pManag.Import2nNdata("tmp.dat");
    ASSERT_FLOAT_EQ(0.8, pManag.GetDecayBranchProbabilityAt(0));
    ASSERT_FLOAT_EQ(0.2, pManag.GetDecayBranchProbabilityAt(1));
    ASSERT_EQ(4, pManag.GetBranchSize(0));
    for(int ii=0; ii<4; ii++)
        ASSERT_EQ((ii*500)+250, pManag.GetGammaEnergyAt(0, ii));
}

///
/// \brief TEST_F Tests the ratio between gammas from anihilation and from deexcitations.
///
TEST_F(TwoAndNTestFixture, ParticleGeneration)
{
    TwoAndNTestFixture::SetDataManually();
    Event* eventDecay = nullptr;
    for (int n=0; n<10e3; n++)
    {
        try
        {
            eventDecay = generateEvent(event, sourcePos, pManag, TWOandN);
            decay->AddEvent(eventDecay);
        }
        catch(std::string ex)
        {
            FAIL();
        }
    }
    double max = hist->GetBinContent(hist->GetMaximumBin());
    double all = hist->Integral();
    ASSERT_NEAR(20.0/52.0, max/all, 10e-3);
}
