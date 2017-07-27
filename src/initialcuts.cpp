/// @file initialcuts.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
#include <iostream>
#include <sstream>
#include <iomanip>
#include "TCanvas.h"
#include "TImage.h"
#include "TLegend.h"
#include "TText.h"
#include "initialcuts.h"

unsigned InitialCuts::objectID_ = 1;

///
/// \brief InitialCuts::InitialCuts The only constructor used.
/// \param type Type of the decay, can be: TWO, THREE or TWOandONE.
/// \param R Radius of the detector.
/// \param L Length of the detector.
/// \param p Probability to interacti with scintillator.
///
InitialCuts::InitialCuts(DecayType type, float R, float L, float p) :
    fDecayType_(type),
    fR_(R),
    fL_(L),
    fDetectionProbability_(p),
    fAcceptedEvents_(0),
    fAcceptedGammas_(0),
    fNumberOfEvents_(0),
    fNumberOfGammas_(0)
{
    fRand_ = new TRandom3(0);
    //Creating histograms for angle distributions
    if(type==THREE)
    {
        fH_en_pass_ = new TH1F((std::string("fH_en_pass_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_p_pass_ = new TH1F((std::string("fH_p_pass_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_p_", 52, 0.0, 0.6);
        fH_en_fail_ = new TH1F((std::string("fH_en_fail_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_p_fail_ = new TH1F((std::string("fH_p_fail_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_p_", 52, 0.0, 0.6);

        fTypeString_="3";
        fH_12_pass_=nullptr;
        fH_12_fail_=nullptr;
        fH_23_pass_=nullptr;
        fH_23_fail_=nullptr;
        fH_31_pass_=nullptr;
        fH_31_fail_=nullptr;

        // histograms for events tha passed cuts
        fH_12_23_pass_ = new TH2F((std::string("fH_12_23_pass_")+std::to_string(objectID_)).c_str(),"fH_12_23_pass", 50,0, 3.15, 50,0,3.15);
        fH_12_23_pass_ -> SetTitle("Polar angle distr, 12 vs 23");
        fH_12_23_pass_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_23_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_23_pass_ -> GetYaxis()->SetTitle("#theta_{23} [rad]");
        fH_12_23_pass_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_31_pass_ = new TH2F(("fH_12_31_pass"+std::to_string(objectID_)).c_str(),"fH_12_31_pass", 50,0, 3.15, 50,0,3.15);
        fH_12_31_pass_ -> SetTitle("Polar angle distr, 12 vs 31");
        fH_12_31_pass_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_31_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_31_pass_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_12_31_pass_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_23_31_pass_ = new TH2F(("fH_23_31_pass"+std::to_string(objectID_)).c_str(),"fH_23_31_pass", 50,0, 3.15, 50,0,3.15);
        fH_23_31_pass_ -> SetTitle("Polar angle distr, 23 vs 31");
        fH_23_31_pass_ -> GetXaxis()->SetTitle("#theta_{23} [rad]");
        fH_23_31_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_31_pass_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_23_31_pass_ -> GetYaxis()->SetTitleOffset(1.4);

        //histograms for events that failed cuts
        fH_12_23_fail_ = new TH2F(("fH_12_23_fail"+std::to_string(objectID_)).c_str(),"fH_12_23_fail", 50,0, 3.15, 50,0,3.15);
        fH_12_23_fail_ -> SetTitle("Polar angle distr, 12 vs 23");
        fH_12_23_fail_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_23_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_23_fail_ -> GetYaxis()->SetTitle("#theta_{23} [rad]");
        fH_12_23_fail_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_31_fail_ = new TH2F(("fH_12_31_fail"+std::to_string(objectID_)).c_str(),"fH_12_31_fail", 50,0, 3.15, 50,0,3.15);
        fH_12_31_fail_ -> SetTitle("Polar angle distr, 12 vs 31");
        fH_12_31_fail_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_31_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_31_fail_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_12_31_fail_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_23_31_fail_ = new TH2F(("fH_23_31_fail"+std::to_string(objectID_)).c_str(),"fH_23_31_fail", 50,0, 3.15, 50,0,3.15);
        fH_23_31_fail_ -> SetTitle("Polar angle distr, 23 vs 31");
        fH_23_31_fail_ -> GetXaxis()->SetTitle("#theta_{23} [rad]");
        fH_23_31_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_31_fail_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_23_31_fail_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_en_pass_low_ = new TH1F((std::string("fH_en_pass_LOW")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_en_pass_high_ = new TH1F((std::string("fH_en_pass_HIGH")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_en_pass_event_ = new TH1F((std::string("fH_en_pass_event")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_en_pass_mid_ = new TH1F((std::string("fH_en_pass_MID")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_en_pass_mid_ -> SetTitle("Energy distribution, mid energy gammas");
        fH_en_pass_mid_ -> GetXaxis()->SetTitle("E [MeV]");
        fH_en_pass_mid_ -> GetXaxis()->SetTitleOffset(1.6);
        fH_en_pass_mid_ -> GetYaxis()->SetTitle("dN/dE");
        fH_en_pass_mid_ -> GetYaxis()->SetTitleOffset(2.);
    }
    else if(type==TWO)
    {
        fH_en_pass_ = new TH1F((std::string("fH_en_pass_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_p_pass_ = new TH1F((std::string("fH_p_pass_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_p_", 52, 0.0, 0.6);
        fH_en_fail_ = new TH1F((std::string("fH_en_fail_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_p_fail_ = new TH1F((std::string("fH_p_fail_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_p_", 52, 0.0, 0.6);

        fTypeString_ = "2";
        fH_12_23_pass_ = nullptr;
        fH_12_31_pass_ = nullptr;
        fH_23_31_pass_ = nullptr;
        fH_23_pass_ = nullptr;
        fH_31_pass_ = nullptr;
        fH_12_23_fail_ = nullptr;
        fH_12_31_fail_ = nullptr;
        fH_23_31_fail_ = nullptr;
        fH_23_fail_ = nullptr;
        fH_31_fail_=nullptr;


        fH_en_pass_mid_ = nullptr;
        fH_en_pass_low_ = new TH1F((std::string("fH_en_pass_LOW")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 19, 0.510, 0.512);
        fH_en_pass_low_->GetXaxis()->SetNdivisions(5, false);
        fH_en_pass_high_ = new TH1F((std::string("fH_en_pass_HIGH")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 19, 0.510, 0.512);
        fH_en_pass_high_->GetXaxis()->SetNdivisions(5, false);
        fH_en_pass_event_ = new TH1F((std::string("fH_en_pass_event")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 19, 0.510, 0.512);
        fH_en_pass_event_->GetXaxis()->SetNdivisions(5, false);

        //histogram for events that passed cuts
        fH_12_pass_ = new TH1F(("fH_12_pass"+std::to_string(objectID_)).c_str(), "fH_12_pass", 20, 3.13, 3.15);
        fH_12_pass_->SetFillColor(kBlue);
        fH_12_pass_ -> SetTitle("Distribution of polar angle between 2 gammas");
        fH_12_pass_ -> GetXaxis()->SetNdivisions(5, false);
        fH_12_pass_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_pass_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_pass_ -> GetYaxis()->SetTitleOffset(1.4);
        // histogram for events that did not passed cuts
        fH_12_fail_ = new TH1F(("fH_12_fail"+std::to_string(objectID_)).c_str(), "fH_12_fail", 20, 3.13, 3.15);
        fH_12_fail_->SetFillColor(kBlue);
        fH_12_fail_ -> SetTitle("Distribution of polar angle between 2 gammas");
        fH_12_fail_ -> GetXaxis()->SetNdivisions(5, false);
        fH_12_fail_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_fail_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_fail_ -> GetYaxis()->SetTitleOffset(1.4);
    }
    else if(type==TWOandONE)
    {
        fH_en_pass_ = new TH1F((std::string("fH_en_pass_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.3, 1.3);
        fH_p_pass_ = new TH1F((std::string("fH_p_pass_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_p_", 52, 0.3, 1.3);
        fH_en_fail_ = new TH1F((std::string("fH_en_fail_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.3, 1.3);
        fH_p_fail_ = new TH1F((std::string("fH_p_fail_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_p_", 52, 0.3, 1.3);

        fTypeString_ = "2&1";
        fH_12_23_pass_ = nullptr;
        fH_12_31_pass_ = nullptr;
        fH_23_31_pass_ = nullptr;
        fH_12_23_fail_ = nullptr;
        fH_12_31_fail_ = nullptr;
        fH_23_31_fail_ = nullptr;
        fH_en_pass_mid_ = nullptr;

        fH_en_pass_low_ = new TH1F((std::string("fH_en_pass_LOW")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 19, 0.510, 0.512);
        fH_en_pass_low_->GetXaxis()->SetNdivisions(5, false);
        fH_en_pass_high_ = new TH1F((std::string("fH_en_pass_HIGH")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 19, 0.510, 0.512);
        fH_en_pass_high_->GetXaxis()->SetNdivisions(5, false);
        fH_en_pass_event_ = new TH1F((std::string("fH_en_pass_event")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_en_", 19, 0.510, 0.512);
        fH_en_pass_event_->GetXaxis()->SetNdivisions(5, false);
        //histograms for events that passed cuts
        fH_12_pass_ = new TH1F(("fH_12_pass"+std::to_string(objectID_)).c_str(), "fH_12_pass", 19, 3.13, 3.15);
        fH_12_pass_->SetFillColor(kBlue);
        fH_12_pass_ -> SetTitle("Polar angle distribution between gamma1 and gamma2");
        fH_12_pass_ -> GetXaxis()->SetNdivisions(5, false);
        fH_12_pass_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_pass_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_pass_ -> GetYaxis()->SetTitleOffset(1.4);
        fH_23_pass_ = new TH1F(("fH_23_pass"+std::to_string(objectID_)).c_str(), "fH_23_pass", 50, 0.0, 3.15);
        fH_23_pass_->SetFillColor(kBlue);
        fH_23_pass_ -> SetTitle("Polar angle distribution between gamma2 and gamma3");
        fH_23_pass_ -> GetXaxis()->SetNdivisions(5, false);
        fH_23_pass_ -> GetXaxis()->SetTitle("#theta_{23} [rad]");
        fH_23_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_pass_ -> GetYaxis()->SetTitle("dN/d#theta_{23}");
        fH_23_pass_ -> GetYaxis()->SetTitleOffset(1.4);
        fH_31_pass_ = new TH1F(("fH_31_pass"+std::to_string(objectID_)).c_str(), "fH_31_pass", 50, 0.0, 3.15);
        fH_31_pass_->SetFillColor(kBlue);
        fH_31_pass_ -> SetTitle("Polar angle distribution between gamma1 and gamma3");
        fH_31_pass_ -> GetXaxis()->SetNdivisions(5, false);
        fH_31_pass_ -> GetXaxis()->SetTitle("#theta_{31} [rad]");
        fH_31_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_31_pass_ -> GetYaxis()->SetTitle("dN/d#theta_{31}");
        fH_31_pass_ -> GetYaxis()->SetTitleOffset(1.4);
        // histogram for events that did not passed cuts
        fH_12_fail_ = new TH1F(("fH_12_fail"+std::to_string(objectID_)).c_str(), "fH_12_fail", 19, 3.13, 3.15);
        fH_12_fail_->SetFillColor(kBlue);
        fH_12_fail_ -> SetTitle("Polar angle distribution between gamma1 and gamma2");
        fH_12_fail_ -> GetXaxis()->SetNdivisions(5, false);
        fH_12_fail_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_fail_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_fail_ -> GetYaxis()->SetTitleOffset(1.4);
        fH_23_fail_ = new TH1F(("fH_23_fail"+std::to_string(objectID_)).c_str(), "fH_23_fail", 50, 0.0, 3.15);
        fH_23_fail_->SetFillColor(kBlue);
        fH_23_fail_ -> SetTitle("Polar angle distribution between gamma2 and gamma3");
        fH_23_fail_ -> GetXaxis()->SetNdivisions(5, false);
        fH_23_fail_ -> GetXaxis()->SetTitle("#theta_{23} [rad]");
        fH_23_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_fail_ -> GetYaxis()->SetTitle("dN/d#theta_{23}");
        fH_23_fail_ -> GetYaxis()->SetTitleOffset(1.4);
        fH_31_fail_ = new TH1F(("fH_31_fail"+std::to_string(objectID_)).c_str(), "fH_31_fail", 50, 0.0, 3.15);
        fH_31_fail_->SetFillColor(kBlue);
        fH_31_fail_ -> SetTitle("Polar angle distribution between gamma1 and gamma3");
        fH_31_fail_ -> GetXaxis()->SetNdivisions(5, false);
        fH_31_fail_ -> GetXaxis()->SetTitle("#theta_{31} [rad]");
        fH_31_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_31_fail_ -> GetYaxis()->SetTitle("dN/d#theta_{31}");
        fH_31_fail_ -> GetYaxis()->SetTitleOffset(1.4);
    }
    else
    {
        throw("Invalid no of decay products!");
    }
    //histograms for particles that passed through cuts
    fH_en_pass_->SetFillColor(kBlue);
    fH_en_pass_ -> SetTitle("Energy distribution");
    fH_en_pass_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_pass_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_pass_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_pass_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_p_pass_->SetFillColor(kBlue);
    fH_p_pass_ -> SetTitle("Momentum distribution");
    fH_p_pass_ -> GetXaxis()->SetTitle("p [MeV/c]");
    fH_p_pass_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_p_pass_ -> GetYaxis()->SetTitle("dN/dp");
    fH_p_pass_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_phi_pass_ = new TH1F((std::string("fH_phi_pass_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_phi_", 52, -3.2, 3.2);
    fH_phi_pass_->SetFillColor(kBlue);
    fH_phi_pass_ -> SetTitle("Azimuthal angle distribution");
    fH_phi_pass_ -> GetXaxis()->SetTitle("#phi [rad]");
    fH_phi_pass_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_phi_pass_ -> GetYaxis()->SetTitle("dN/d #phi");
    fH_phi_pass_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_cosTheta_pass_ = new TH1F((std::string("fH_cosTheta_pass_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_cosTheta_", 52, -1.01, 1.01);
    fH_cosTheta_pass_->SetFillColor(kBlue);
    fH_cosTheta_pass_ -> SetTitle("Cosine of polar angle distribution");
    fH_cosTheta_pass_ -> GetXaxis()->SetTitle("cos(#theta)");
    fH_cosTheta_pass_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_cosTheta_pass_ -> GetYaxis()->SetTitle("dN/d cos(#theta)");
    fH_cosTheta_pass_ -> GetYaxis()->SetTitleOffset(1.8);

    //histograms for particles that did not pass through cuts
    fH_en_fail_->SetFillColor(kBlue);
    fH_en_fail_ -> SetTitle("Energy distribution");
    fH_en_fail_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_fail_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_fail_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_fail_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_p_fail_->SetFillColor(kBlue);
    fH_p_fail_ -> SetTitle("Momentum distribution");
    fH_p_fail_ -> GetXaxis()->SetTitle("p [MeV/c]");
    fH_p_fail_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_p_fail_ -> GetYaxis()->SetTitle("dN/dp");
    fH_p_fail_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_phi_fail_ = new TH1F((std::string("fH_phi_fail_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_phi_", 52, -3.2, 3.2);
    fH_phi_fail_->SetFillColor(kBlue);
    fH_phi_fail_ -> SetTitle("Azimuthal angle distribution");
    fH_phi_fail_ -> GetXaxis()->SetTitle("#phi [rad]");
    fH_phi_fail_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_phi_fail_ -> GetYaxis()->SetTitle("dN/d #phi");
    fH_phi_fail_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_cosTheta_fail_ = new TH1F((std::string("fH_cosTheta_fail_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(), "fH_cosTheta_", 52, -1.01, 1.01);
    fH_cosTheta_fail_->SetFillColor(kBlue);
    fH_cosTheta_fail_ -> SetTitle("Cosine of polar angle distribution");
    fH_cosTheta_fail_ -> GetXaxis()->SetTitle("cos(#theta)");
    fH_cosTheta_fail_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_cosTheta_fail_ -> GetYaxis()->SetTitle("dN/d cos(#theta)");
    fH_cosTheta_fail_ -> GetYaxis()->SetTitleOffset(1.8);

    //histograms for gammas that passed through cuts, differentation for low, mid and high energy gammas
    fH_en_pass_event_ -> SetTitle("Energy distribution");
    fH_en_pass_event_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_pass_event_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_pass_event_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_pass_event_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_en_pass_low_ -> SetTitle("Energy distribution, low energy gammas");
    fH_en_pass_low_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_pass_low_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_pass_low_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_pass_low_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_en_pass_high_ -> SetTitle("Energy distribution, high energy gammas");
    fH_en_pass_high_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_pass_high_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_pass_high_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_pass_high_ -> GetYaxis()->SetTitleOffset(1.8);

    //histograms to monitor cuts passing
    fH_event_cuts_ = new TH1F((std::string("fH_event_cuts_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(),\
            "fH_event_cuts_", 3, 0.0, 3.0);
    fH_event_cuts_->SetTitle("Passing cuts by events");
    fH_event_cuts_->GetYaxis()->SetTitle("% passed");
    fH_event_cuts_->GetYaxis()->SetTitleOffset(1.4);
    fH_event_cuts_->GetXaxis()->SetLabelSize(0);
    fH_event_cuts_->GetXaxis()->SetTickLength(0);
    fH_gamma_cuts_ = new TH1F((std::string("fH_gamma_cuts_")+std::to_string(fDecayType_)+std::to_string(objectID_)).c_str(),\
            "fH_gamma_cuts_", 3, 0.0, 3.0);
    fH_gamma_cuts_->SetTitle("Passing cuts by gammas");
    fH_gamma_cuts_->GetYaxis()->SetTitle("% passed");
    fH_gamma_cuts_->GetYaxis()->SetTitleOffset(1.4);
    fH_gamma_cuts_->GetXaxis()->SetLabelSize(0);
    fH_gamma_cuts_->GetXaxis()->SetTickLength(0);

    objectID_++;
}

///
/// \brief InitialCuts::InitialCuts Copy constructor.
/// \param est Instance of InitialCuts class.
///
InitialCuts::InitialCuts(const InitialCuts& est)
{
    fSilentMode_=est.fSilentMode_;
    fR_ = est.fR_;  //radius in m
    fL_ = est.fL_;  //length in m
    fDetectionProbability_ = est.fDetectionProbability_;
    fDecayType_ = est.fDecayType_;
    fTypeString_ = est.fTypeString_;
    fRand_ = new TRandom3(0);

    fH_12_pass_ = nullptr;
    fH_12_fail_ = nullptr;
    fH_23_pass_ = nullptr;
    fH_31_pass_ = nullptr;
    fH_12_23_pass_ = nullptr;
    fH_12_31_pass_ = nullptr;
    fH_23_31_pass_ = nullptr;
    fH_12_23_fail_ = nullptr;
    fH_12_31_fail_ = nullptr;
    fH_23_31_fail_ = nullptr;
    fH_31_fail_ = nullptr;
    fH_23_fail_ = nullptr;
    fH_en_pass_mid_ = nullptr;

    if(fDecayType_==TWO)
    {
        fH_12_pass_ = new TH1F(*est.fH_12_pass_);
        fH_12_fail_ = new TH1F(*est.fH_12_fail_);
    }
    else if(fDecayType_==THREE)
    {
        fH_12_23_pass_ = new TH2F(*est.fH_12_23_pass_);
        fH_12_31_pass_ = new TH2F(*est.fH_12_31_pass_);
        fH_23_31_pass_ = new TH2F(*est.fH_23_31_pass_);
        fH_12_23_fail_ = new TH2F(*est.fH_12_23_fail_);
        fH_12_31_fail_ = new TH2F(*est.fH_12_31_fail_);
        fH_23_31_fail_ = new TH2F(*est.fH_23_31_fail_);
        fH_en_pass_mid_ = new TH1F(*est.fH_en_pass_mid_);
    }
    else if(fDecayType_==TWOandONE)
    {
        fH_12_pass_ = new TH1F(*est.fH_12_pass_);
        fH_12_fail_ = new TH1F(*est.fH_12_fail_);
        fH_23_pass_ = new TH1F(*est.fH_23_pass_);
        fH_23_fail_ = new TH1F(*est.fH_23_fail_);
        fH_31_pass_ = new TH1F(*est.fH_31_pass_);
        fH_31_fail_ = new TH1F(*est.fH_31_fail_);
    }

    fH_en_pass_ = new TH1F(*est.fH_en_pass_);
    fH_p_pass_ = new TH1F(*est.fH_p_pass_);
    fH_phi_pass_= new TH1F(*est.fH_phi_pass_);
    fH_cosTheta_pass_ = new TH1F(*est.fH_cosTheta_pass_);
    fH_en_fail_ = new TH1F(*est.fH_en_fail_);
    fH_p_fail_ = new TH1F(*est.fH_p_fail_);
    fH_phi_fail_= new TH1F(*est.fH_phi_fail_);
    fH_cosTheta_fail_ = new TH1F(*est.fH_cosTheta_fail_);
    fH_en_pass_low_ = new TH1F(*est.fH_en_pass_low_);
    fH_en_pass_high_ = new TH1F(*est.fH_en_pass_high_);
    fH_en_pass_event_ = new TH1F(*est.fH_en_pass_event_);

    fH_event_cuts_ = new TH1F(*est.fH_event_cuts_);
    fH_gamma_cuts_ = new TH1F(*est.fH_gamma_cuts_);
    fNumberOfEvents_ = est.fNumberOfEvents_;
    fNumberOfGammas_ = est.fNumberOfGammas_;
    fAcceptedEvents_ = est.fAcceptedEvents_;
    fAcceptedGammas_ = est.fAcceptedGammas_;
}

///
/// \brief InitialCuts::operator = Assignement operator.
/// \param est Instance of InitialCuts to be copied from.
/// \return Ref to new InitialCuts, identical to est.
///
InitialCuts& InitialCuts::operator=(const InitialCuts& est)
{
    fSilentMode_=est.fSilentMode_;
    fR_ = est.fR_;  //radius in m
    fL_ = est.fL_;  //length in m
    fDetectionProbability_ = est.fDetectionProbability_;
    fDecayType_ = est.fDecayType_;
    fTypeString_ = est.fTypeString_;

    fRand_ = new TRandom3(0);

    fH_12_pass_ = nullptr;
    fH_12_fail_ = nullptr;
    fH_23_pass_ = nullptr;
    fH_31_pass_ = nullptr;
    fH_12_23_pass_ = nullptr;
    fH_12_31_pass_ = nullptr;
    fH_23_31_pass_ = nullptr;
    fH_12_23_fail_ = nullptr;
    fH_12_31_fail_ = nullptr;
    fH_23_31_fail_ = nullptr;
    fH_31_fail_ = nullptr;
    fH_23_fail_ = nullptr;
    fH_en_pass_mid_ = nullptr;

    if(fDecayType_==TWO)
    {
        fH_12_pass_ = new TH1F(*est.fH_12_pass_);
        fH_12_fail_ = new TH1F(*est.fH_12_fail_);
    }
    else if(fDecayType_==THREE)
    {
        fH_12_23_pass_ = new TH2F(*est.fH_12_23_pass_);
        fH_12_31_pass_ = new TH2F(*est.fH_12_31_pass_);
        fH_23_31_pass_ = new TH2F(*est.fH_23_31_pass_);
        fH_12_23_fail_ = new TH2F(*est.fH_12_23_fail_);
        fH_12_31_fail_ = new TH2F(*est.fH_12_31_fail_);
        fH_23_31_fail_ = new TH2F(*est.fH_23_31_fail_);
        fH_en_pass_mid_ = new TH1F(*est.fH_en_pass_mid_);
    }
    else if(fDecayType_==TWOandONE)
    {
        fH_12_pass_ = new TH1F(*est.fH_12_pass_);
        fH_12_fail_ = new TH1F(*est.fH_12_fail_);
        fH_23_pass_ = new TH1F(*est.fH_23_pass_);
        fH_23_fail_ = new TH1F(*est.fH_23_fail_);
        fH_31_pass_ = new TH1F(*est.fH_31_pass_);
        fH_31_fail_ = new TH1F(*est.fH_31_fail_);
    }

    fH_en_pass_ = new TH1F(*est.fH_en_pass_);
    fH_p_pass_ = new TH1F(*est.fH_p_pass_);
    fH_phi_pass_= new TH1F(*est.fH_phi_pass_);
    fH_cosTheta_pass_ = new TH1F(*est.fH_cosTheta_pass_);
    fH_en_fail_ = new TH1F(*est.fH_en_fail_);
    fH_p_fail_ = new TH1F(*est.fH_p_fail_);
    fH_phi_fail_= new TH1F(*est.fH_phi_fail_);
    fH_cosTheta_fail_ = new TH1F(*est.fH_cosTheta_fail_);
    fH_en_pass_low_ = new TH1F(*est.fH_en_pass_low_);
    fH_en_pass_high_ = new TH1F(*est.fH_en_pass_high_);
    fH_en_pass_event_ = new TH1F(*est.fH_en_pass_event_);

    fH_event_cuts_ = new TH1F(*est.fH_event_cuts_);
    fH_gamma_cuts_ = new TH1F(*est.fH_gamma_cuts_);
    fNumberOfEvents_ = est.fNumberOfEvents_;
    fNumberOfGammas_ = est.fNumberOfGammas_;
    fAcceptedEvents_ = est.fAcceptedEvents_;
    fAcceptedGammas_ = est.fAcceptedGammas_;
    return *this;
}

///
/// \brief InitialCuts::~InitialCuts Releases memory (mostly allocated for histograms).
///
InitialCuts::~InitialCuts()
{
    if(fH_12_23_pass_) delete fH_12_23_pass_;
    if(fH_12_31_pass_) delete fH_12_31_pass_;
    if(fH_23_31_pass_) delete fH_23_31_pass_;
    if(fH_12_pass_) delete fH_12_pass_;
    if(fH_23_pass_) delete fH_23_pass_;
    if(fH_31_pass_) delete fH_31_pass_;
    if(fH_12_23_fail_) delete fH_12_23_fail_;
    if(fH_12_31_fail_) delete fH_12_31_fail_;
    if(fH_23_31_fail_) delete fH_23_31_fail_;
    if(fH_12_fail_) delete fH_12_fail_;
    if(fH_23_fail_) delete fH_23_fail_;
    if(fH_31_fail_) delete fH_31_fail_;
    if(fH_en_pass_) delete fH_en_pass_;
    if(fH_p_pass_) delete fH_p_pass_;
    if(fH_phi_pass_) delete fH_phi_pass_;
    if(fH_cosTheta_pass_) delete fH_cosTheta_pass_;
    if(fH_en_fail_) delete fH_en_fail_;
    if(fH_p_fail_) delete fH_p_fail_;
    if(fH_phi_fail_) delete fH_phi_fail_;
    if(fH_cosTheta_fail_) delete fH_cosTheta_fail_;
    if(fH_en_pass_low_) delete fH_en_pass_low_;
    if(fH_en_pass_mid_) delete fH_en_pass_mid_;
    if(fH_en_pass_high_) delete fH_en_pass_high_;
    if(fH_en_pass_event_) delete fH_en_pass_event_;
    if(fH_event_cuts_) delete fH_event_cuts_;
    if(fH_gamma_cuts_) delete fH_gamma_cuts_;
    if(fRand_) delete fRand_;
}

///
/// \brief InitialCuts::AddCuts Checks if an event and particular gammas passed through cuts. Sets flags in Event instance. Fills histograms.
/// \param event Pointer to an Event object representing a single decay.
///
void InitialCuts::AddCuts(Event* event)
{
    fNumberOfEvents_++;
    bool geo_event_pass = true;
    bool inter_event_pass = true;
    fH_event_cuts_->Fill(0); //events at the beginning
    for(int ii=0; ii<3; ii++)
    {
        fH_gamma_cuts_->Fill(0); //gammas at the beginning
        if(event->GetEmissionPointOf(ii)!=nullptr && event->GetFourMomentumOf(ii)!=nullptr)
        {
            fNumberOfGammas_++;
            bool geo_pass = GeometricalAcceptance_(event->GetEmissionPointOf(ii), event->GetFourMomentumOf(ii));
            bool inter_pass = geo_pass ? DetectionCut_() : false;
            event->SetCutPassing(ii, geo_pass && inter_pass);
            if(!(event->GetDecayType()==TWOandONE && ii==2))
            {
                geo_event_pass &= geo_pass;
                inter_event_pass &= inter_pass;
            }
            if(geo_pass && inter_pass) //gamma passed all cuts
            {
                fAcceptedGammas_++;
                fH_en_pass_->Fill(event->GetFourMomentumOf(ii)->Energy());
                fH_p_pass_->Fill(event->GetFourMomentumOf(ii)->P());
                fH_phi_pass_->Fill(event->GetFourMomentumOf(ii)->Phi());
                fH_cosTheta_pass_->Fill(event->GetFourMomentumOf(ii)->CosTheta());
            }
            else //gamma failed at least one of the cuts
            {
                fH_en_fail_->Fill(event->GetFourMomentumOf(ii)->Energy());
                fH_p_fail_->Fill(event->GetFourMomentumOf(ii)->P());
                fH_phi_fail_->Fill(event->GetFourMomentumOf(ii)->Phi());
                fH_cosTheta_fail_->Fill(event->GetFourMomentumOf(ii)->CosTheta());
            }
        }
    }
    if(geo_event_pass)
        fH_event_cuts_->Fill(1);
    if(inter_event_pass)
        fH_event_cuts_->Fill(2);
    if(geo_event_pass && inter_event_pass)
        FillValidEventHistograms_(event);
    else
        FillInvalidEventHistograms_(event);
    FillDistributionHistograms_(event);

    //Let event deduce its flag!
    event->DeducePassFlag();
}

///
/// \brief InitialCuts::GeometricalAcceptance_ Checks if gamma hit the detector.
/// \param source Coordinates of the emmision point.
/// \param gamma Photon's fourmomentum.
/// \return True if gamma hit the detector, false otherwise.
///
bool InitialCuts::GeometricalAcceptance_(const TLorentzVector* source, const TLorentzVector* gamma)
{
    int side = (gamma->Pz()>0) ? 1 : -1;
    double s = (side*fL_/2.0 - source->Z())/gamma->Pz();
    double x = source->X() + s*(gamma->Px());
    double y = source->Y() +s*(gamma->Py());

    if(x*x + y*y > fR_*fR_)
    {
        fH_gamma_cuts_->Fill(1);
        return true;
    }
    else return false;
}

///
/// \brief InitialCuts::DetectionCut_ Checks if gamma interacted with the detector.
/// \return True if gamma interacted with the detector, false otherwise.
///
bool InitialCuts::DetectionCut_()
{
    bool pass = false;
    if(fDetectionProbability_==1.0)
        pass = true;
    else
    {
        float p = fRand_->Uniform(0.0, 1.0);
        pass = p <= fDetectionProbability_;
    }
    if(pass)
        fH_gamma_cuts_->Fill(2);
    return pass;
}

///
/// \brief InitialCuts::FillValidEventHistograms_ Fills histograms only for EVENTS that passed cuts and can be correctly reconstructed.
/// \param event Pointer to Event object.
///
void InitialCuts::FillValidEventHistograms_(const Event* event)
{
    fAcceptedEvents_++;
    bool thirdGammaSc = false;
    int minIndex=0; //indices of min and max energy out of 2 or 3 gammas
    int maxIndex=0;
    for(int ii=0; ii<3; ii++)
    {
        if(event->GetFourMomentumOf(ii) != nullptr)
        {
            if(ii==2 && fDecayType_==TWOandONE)
            {
                thirdGammaSc = true;
                break;
            }
            fH_en_pass_event_->Fill(event->GetFourMomentumOf(ii)->Energy());
            minIndex = event->GetFourMomentumOf(ii)->E() < event->GetFourMomentumOf(minIndex)->E() ? ii : minIndex;
            maxIndex = event->GetFourMomentumOf(ii)->E() > event->GetFourMomentumOf(maxIndex)->E() ? ii : maxIndex;
        }
    }
    fH_en_pass_low_->Fill(event->GetFourMomentumOf(minIndex)->Energy());
    fH_en_pass_high_->Fill(event->GetFourMomentumOf(maxIndex)->Energy());
    if(fDecayType_==THREE)
    {
        //If there are 3 gammas, draw also middle value.
        int midIndex = minIndex==maxIndex ? minIndex : 3-minIndex-maxIndex;
        fH_en_pass_mid_->Fill(event->GetFourMomentumOf(midIndex)->Energy());
        fH_12_23_pass_->Fill(event->GetFourMomentumOf(0)->Angle(event->GetFourMomentumOf(1)->Vect()), \
                             event->GetFourMomentumOf(1)->Angle(event->GetFourMomentumOf(2)->Vect()), event->GetWeight());
        fH_12_31_pass_->Fill(event->GetFourMomentumOf(0)->Angle(event->GetFourMomentumOf(1)->Vect()), \
                             event->GetFourMomentumOf(2)->Angle(event->GetFourMomentumOf(0)->Vect()), event->GetWeight());
        fH_23_31_pass_->Fill(event->GetFourMomentumOf(1)->Angle(event->GetFourMomentumOf(2)->Vect()), \
                             event->GetFourMomentumOf(2)->Angle(event->GetFourMomentumOf(0)->Vect()), event->GetWeight());
    }
    else if(fDecayType_ == TWO)
        fH_12_pass_->Fill(event->GetFourMomentumOf(0)->Angle((event->GetFourMomentumOf(1))->Vect()), event->GetWeight());
    else if(fDecayType_ == TWOandONE)
    {
        fH_12_pass_->Fill(event->GetFourMomentumOf(0)->Angle((event->GetFourMomentumOf(1))->Vect()), event->GetWeight());
        if(thirdGammaSc)
        {
            fH_23_pass_->Fill(event->GetFourMomentumOf(1)->Angle((event->GetFourMomentumOf(2))->Vect()), event->GetWeight());
            fH_31_pass_->Fill(event->GetFourMomentumOf(2)->Angle((event->GetFourMomentumOf(0))->Vect()), event->GetWeight());
        }
    }
    else
    {
         throw("Invalid no of decay products in InitialCuts!");
    }
}

///
/// \brief InitialCuts::FillInvalidEventHistograms_ Fills histograms for EVENTS that did not pass cuts and cannot be reconstructed.
/// \param event Pointer to Event object.
///
void InitialCuts::FillInvalidEventHistograms_(const Event* event)
{

    if(fDecayType_==THREE)
    {
        fH_12_23_fail_->Fill(event->GetFourMomentumOf(0)->Angle(event->GetFourMomentumOf(1)->Vect()), \
                             event->GetFourMomentumOf(1)->Angle(event->GetFourMomentumOf(2)->Vect()), event->GetWeight());
        fH_12_31_fail_->Fill(event->GetFourMomentumOf(0)->Angle(event->GetFourMomentumOf(1)->Vect()), \
                             event->GetFourMomentumOf(2)->Angle(event->GetFourMomentumOf(0)->Vect()), event->GetWeight());
        fH_23_31_fail_->Fill(event->GetFourMomentumOf(1)->Angle(event->GetFourMomentumOf(2)->Vect()), \
                             event->GetFourMomentumOf(2)->Angle(event->GetFourMomentumOf(0)->Vect()), event->GetWeight());
    }
    else if(fDecayType_ == TWO)
        fH_12_fail_->Fill(event->GetFourMomentumOf(0)->Angle((event->GetFourMomentumOf(1))->Vect()), event->GetWeight());
    else if(fDecayType_ == TWOandONE)
    {
        fH_12_fail_->Fill(event->GetFourMomentumOf(0)->Angle((event->GetFourMomentumOf(1))->Vect()), event->GetWeight());
        if(event->GetFourMomentumOf(2) != nullptr)
        {
                fH_23_fail_->Fill(event->GetFourMomentumOf(1)->Angle((event->GetFourMomentumOf(2))->Vect()), event->GetWeight());
                fH_31_fail_->Fill(event->GetFourMomentumOf(2)->Angle((event->GetFourMomentumOf(0))->Vect()), event->GetWeight());
        }
    }
    else
    {
         throw("Invalid no of decay products in InitialCuts!");
    }
}

///
/// \brief InitialCuts::FillDistributionHistograms_ Fill histograms with distributions of E, p, phi and cos(theta).
/// \param event Pointer to Event object.
///
void InitialCuts::FillDistributionHistograms_(const Event* event)
{
    for(int ii=0; ii<3; ii++)
    {
        if(event->GetFourMomentumOf(ii)!=nullptr)
        {
            if(event->GetCutPassingOf(ii))
            {
                fH_en_pass_->Fill(event->GetFourMomentumOf(ii)->Energy());
                fH_p_pass_->Fill(event->GetFourMomentumOf(ii)->P());
                fH_phi_pass_->Fill(event->GetFourMomentumOf(ii)->Phi());
                fH_cosTheta_pass_->Fill(event->GetFourMomentumOf(ii)->CosTheta());
            }
            else
            {
                fH_en_fail_->Fill(event->GetFourMomentumOf(ii)->Energy());
                fH_p_fail_->Fill(event->GetFourMomentumOf(ii)->P());
                fH_phi_fail_->Fill(event->GetFourMomentumOf(ii)->Phi());
                fH_cosTheta_fail_->Fill(event->GetFourMomentumOf(ii)->CosTheta());
            }
        }
    }
}

///
/// \brief InitialCuts::DrawHistograms One function to rule... draw them all!
/// \param prefix Prefix of histograms file names.
/// \param output Type of output to be saved. Can be TREE, PNG or BOTH.
///
void InitialCuts::DrawHistograms(std::string prefix, OutputOptions output)
{
    DrawCutsHistograms(prefix, output);
    DrawPassHistograms(prefix, output);
    DrawFailHistograms(prefix, output);
}

///
/// \brief InitialCuts::DrawCutsHistograms Draws histograms indicating what percent of gammas/events passed through certain cuts.
/// \param prefix Prefix of histograms file names.
/// \param output Type of output to be saved. Can be TREE, PNG or BOTH.
///
void InitialCuts::DrawCutsHistograms(std::string prefix, OutputOptions output)
{
    std::string outFile;
    if(!fSilentMode_) std::cout<<"[INFO] Drawing histograms for cuts passing."<<std::endl;
    TCanvas* cuts = new TCanvas((fTypeString_+"-gammas_cuts_passed").c_str(),\
                                (std::string("Fraction of events/gammas that passed cuts, ")+fTypeString_+std::string("-gamma")).c_str(),\
                                800,400);
    cuts->Divide(2,1);
    //drawing text on histograms
    TText* labelBefore = new TText();
    labelBefore-> SetNDC();
    labelBefore->SetTextAngle(45);
    TText* labelGeo = new TText();
    labelGeo -> SetNDC();
    labelGeo->SetTextAngle(45);
    TText* labelP = new TText();
    labelP -> SetNDC();
    labelP->SetTextAngle(45);
    TText* labelPercent = new TText();
    labelPercent->SetNDC();
    labelPercent->SetTextColor(kRed+2);

    //string stream for writing percentage values on the plots
    std::stringstream ss;
    ss<<std::setprecision(2);

    //drawing histograms
    //for gammas
    cuts->cd(1);
    fH_gamma_cuts_->Scale(100.0/fNumberOfGammas_);
    fH_gamma_cuts_->GetYaxis()->SetRangeUser(0.0, 101.0);
    fH_gamma_cuts_->SetStats(kFALSE);
    fH_gamma_cuts_->Draw();
    labelBefore -> DrawText(0.15, 0.55, "before cuts");
    labelGeo -> DrawText(0.4, 0.55, "geom. accept.");
    labelP -> DrawText(0.65, 0.55, "interaction prob.");
    ss<<fAcceptedGammas_/(double)fNumberOfGammas_*100.0;
    labelPercent->DrawText(0.75, 0.2, (ss.str()+std::string("%")).c_str());
    ss.str(std::string());
    //for events
    cuts->cd(2);
    fH_event_cuts_->Scale(100.0/fNumberOfEvents_);
    fH_event_cuts_->GetYaxis()->SetRangeUser(0.0, 101.0);
    fH_event_cuts_->SetStats(kFALSE);
    fH_event_cuts_->Draw();
    labelBefore -> DrawText(0.15, 0.55, "before cuts");
    labelGeo -> DrawText(0.4, 0.55, "geom. accept.");
    labelP -> DrawText(0.65, 0.55, "interaction prob.");
    ss<<fAcceptedEvents_/(double)fNumberOfEvents_*100.0;
    labelPercent->DrawText(0.75, 0.2, (ss.str()+std::string("%")).c_str());

    if(!fSilentMode_) std::cout<<"[INFO] Saving histograms for cuts passing."<<std::endl;
    if(output==BOTH || output==PNG)
    {
        TImage *img_cut = TImage::Create();
        img_cut->FromPad(cuts);
        outFile=prefix+fTypeString_+"-gammas_cuts.png";
        img_cut->WriteImage(outFile.c_str());
        delete img_cut;
    }
    if(output==BOTH || output==TREE)
    {
        cuts->Write();
    }
    delete cuts;
    delete labelBefore;
    delete labelGeo;
    delete labelP;
    delete labelPercent;
}

///
/// \brief InitialCuts::DrawPassHistograms Draws histograms for gammas and events that passed cuts.
/// \param prefix Prefix of histograms file names.
/// \param output Type of output to be saved. Can be TREE, PNG or BOTH.

///
void InitialCuts::DrawPassHistograms(std::string prefix, OutputOptions output)
{
    std::string outFile1;
    std::string outFile2;
    if(!fSilentMode_) std::cout<<"[INFO] Drawing histograms for gammas that passed cuts."<<std::endl;
    TCanvas* angles_pass;

    TCanvas* dist_pass = new TCanvas((fTypeString_+"-gammas_dist_pass").c_str(),\
                                     ("Basic distributions for passed "\
                                     +fTypeString_+std::string("-gamma events")).c_str(), 900, 800);
    //differentation of low, mid and high energy gammas
    TCanvas *diff_en;
    std::string outFileDiffEn;

    dist_pass->Divide(2, 2);
    dist_pass->cd(1);
    fH_en_pass_->Draw();
    dist_pass->cd(2);
    fH_p_pass_->Draw();
    dist_pass->cd(3);
    fH_phi_pass_->Draw();
    dist_pass->cd(4);
    fH_cosTheta_pass_->Draw();
    dist_pass->Update();

    //setting fill colors for histograms that differentiate gammas with low, mid and high energies
    fH_en_pass_event_->SetFillColor(kBlack);
    fH_en_pass_low_->SetFillColor(kBlue);
    fH_en_pass_high_->SetFillColor(kRed);
    TLegend* legend = new TLegend(0.1, 0.5, 0.4, 0.9);
    legend->AddEntry(fH_en_pass_event_, "all gammas", "f");
    legend->AddEntry(fH_en_pass_low_, "lowest energy", "f");
    legend->AddEntry(fH_en_pass_high_, "highest energy", "f");

    //angle distribution depends on the number of decay products
    if(fDecayType_ == THREE)
    {
        angles_pass = new TCanvas((fTypeString_+"-gammas_angles_pass").c_str(),\
                                    ("Angle ditribution for all generated "\
                                    +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);
        angles_pass->Divide(3,1);
        angles_pass->cd(1);
        fH_12_23_pass_->Draw("colz");
        angles_pass->cd(2);
        fH_12_31_pass_->Draw("colz");
        angles_pass->cd(3);
        fH_23_31_pass_->Draw("colz");
        outFile2 = prefix+fTypeString_+"-gammas_angles_pass_.png";
    }
    else if(fDecayType_ == TWO)
    {
        angles_pass = new TCanvas((fTypeString_+"-gammas_angles_pass").c_str(),\
                                  ("Angle ditribution for all generated "\
                                  +fTypeString_+std::string("-gamma events")).c_str(), 500, 400);
        fH_12_pass_->Draw();
        outFile2 = prefix+fTypeString_+"-gammas_angles_pass_.png";
    }
    else if(fDecayType_ == TWOandONE)
    {
        angles_pass = new TCanvas((fTypeString_+"-gammas_angles_pass").c_str(),\
                                    ("Angle ditribution for all generated "\
                                    +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);
        angles_pass->Divide(3,1);
        angles_pass->cd(1);
        fH_12_pass_->Draw();
        angles_pass->cd(2);
        fH_23_pass_->Draw();
        angles_pass->cd(3);
        fH_31_pass_->Draw();
        outFile2 = prefix+fTypeString_+"-gammas_angles_pass_.png";
    }
    //temporary pointers
    TH1F* objLow = nullptr;
    TH1F* objMid = nullptr;
    TH1F* objHigh = nullptr;
    if(fDecayType_==THREE)
    {
        //Drawing energy distribution of gammas from valid events
        diff_en= new TCanvas((fTypeString_+"-gammas_diff_energies").c_str(),\
                                      ("Distribution of energy for incident photons from "\
                                       +fTypeString_+std::string("-gamma events")).c_str(), 1000, 900);
        fH_en_pass_mid_->SetFillColor(kGreen);
        legend->AddEntry(fH_en_pass_mid_, "medium energy", "f");
        diff_en->Divide(2,2);
        diff_en->cd(2);
        objLow = (TH1F*)fH_en_pass_low_->DrawClone(); //need to draw clones to make it look pretty
        diff_en->cd(3);
        objMid = (TH1F*)fH_en_pass_mid_->DrawClone();
        diff_en->cd(4);
        objHigh = (TH1F*)fH_en_pass_high_->DrawClone();
        diff_en->Update();
        diff_en->cd(1);
        fH_en_pass_low_->SetStats(0);
        fH_en_pass_mid_->SetStats(0);
        fH_en_pass_high_->SetStats(0);
        fH_en_pass_event_->Draw();
        fH_en_pass_low_->Draw("same");
        fH_en_pass_mid_->Draw("same");
        fH_en_pass_high_->Draw("same");
        diff_en->Update();
        legend->Draw();
        outFileDiffEn = prefix+fTypeString_+"-gammas_diff_energies_.png";
    }
    else
    {

        //Drawing energy distribution of gammas from valid events
        diff_en= new TCanvas((fTypeString_+"-gammas_diff_energies").c_str(),\
                                      ("Distribution of energy for incident photons from "\
                                       +fTypeString_+std::string("-gamma events")).c_str(), 1200, 450);
        diff_en->Divide(3,1);
        diff_en->cd(2);
        objLow = (TH1F*)fH_en_pass_low_->DrawClone();
        diff_en->cd(3);
        objHigh = (TH1F*)fH_en_pass_high_->DrawClone();
        diff_en->cd(1);
        fH_en_pass_event_->Draw();
        diff_en->Update();
        fH_en_pass_low_->SetStats(0);
        fH_en_pass_low_->Draw("same");
        fH_en_pass_high_->SetStats(0);
        fH_en_pass_high_->Draw("same");
        legend->Draw();
        diff_en->Update();
        outFileDiffEn = prefix+fTypeString_+"-gammas_diff_energies_.png";
    }

    if(!fSilentMode_) std::cout<<"[INFO] Saving histograms for passed events."<<std::endl;
    if(output==BOTH || output==PNG)
    {
        TImage *img = TImage::Create();
        img->FromPad(dist_pass);
        outFile1=prefix+fTypeString_+"-gammas_dist_pass_.png";
        img->WriteImage(outFile1.c_str());
        angles_pass->Update();
        TImage *img2 = TImage::Create();
        img2->FromPad(angles_pass);
        img2->WriteImage(outFile2.c_str());
        diff_en->Update();
        TImage *img3 = TImage::Create();
        img3->FromPad(diff_en);
        img3->WriteImage(outFileDiffEn.c_str());
        delete img;
        delete img2;
        delete img3;
    }
    if(output==BOTH ||output==TREE)
    {
        dist_pass->Write();
        angles_pass->Write();
        diff_en->Write();
    }
    delete dist_pass;
    delete angles_pass;
    delete diff_en;
    delete legend;
    if(objLow) delete objLow;
    if(objMid) delete objMid;
    if(objHigh) delete objHigh;
}

///
/// \brief InitialCuts::DrawFailHistograms Draws histograms for gammas and events that did not pass cuts.
/// \param prefix Prefix of histograms file names.
/// \param output Type of output to be saved. Can be TREE, PNG or BOTH.

///
void InitialCuts::DrawFailHistograms(std::string prefix, OutputOptions output)
{
        std::string outFile1;
        std::string outFile2;
        if(!fSilentMode_) std::cout<<"[INFO] Drawing histograms for events that did not pass cuts."<<std::endl;
        TCanvas* angles_fail;
        TCanvas* dist_fail= new TCanvas((fTypeString_+"-gammas_dist_fail").c_str(), ("Basic distributions for passed "\
                                         +fTypeString_+std::string("-gamma events")).c_str(), 900, 800);
        dist_fail->Divide(2, 2);
        dist_fail->cd(1);
        fH_en_fail_->Draw();
        dist_fail->cd(2);
        fH_p_fail_->Draw();
        dist_fail->cd(3);
        fH_phi_fail_->Draw();
        dist_fail->cd(4);
        fH_cosTheta_fail_->Draw();
        dist_fail->Update();

        //angle distribution depends on the number of decay products
        if(fDecayType_ == THREE)
        {
            angles_fail = new TCanvas((fTypeString_+"-gammas_angles_fail").c_str(), \
                                      ("Angle ditribution for all generated "\
                                      +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);

            angles_fail->Divide(3,1);
            angles_fail->cd(1);
            fH_12_23_fail_->Draw("colz");
            angles_fail->cd(2);
            fH_12_31_fail_->Draw("colz");
            angles_fail->cd(3);
            fH_23_31_fail_->Draw("colz");
            outFile2 = prefix+fTypeString_+std::string("-gammas_angles_fail.png");

        }
        else if(fDecayType_ == TWO)
        {
            angles_fail = new TCanvas((fTypeString_+"-gammas_angles_fail").c_str(), \
                                      ("Angle ditribution for all generated "\
                                      +fTypeString_+std::string("-gamma events")).c_str(), 500, 400);

            fH_12_fail_->Draw();
            outFile2 = prefix+fTypeString_+std::string("-gammas_angles_fail.png");
        }
        else if(fDecayType_ == TWOandONE)
        {
            angles_fail = new TCanvas((fTypeString_+"-gammas_angles_fail").c_str(), \
                                      ("Angle ditribution for all generated "\
                                      +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);

            angles_fail->Divide(3,1);
            angles_fail->cd(1);
            fH_12_fail_->Draw();
            angles_fail->cd(2);
            fH_23_fail_->Draw();
            angles_fail->cd(3);
            fH_31_fail_->Draw();
            outFile2 = prefix+fTypeString_+std::string("-gammas_angles_fail.png");
        }
        angles_fail->Update();
        if(!fSilentMode_) std::cout<<"[INFO] Saving histograms for failed events."<<std::endl;
        if(output==BOTH || output==PNG)
        {
            TImage *img = TImage::Create();
            img->FromPad(dist_fail);
            outFile1=prefix+fTypeString_+std::string("-gammas_dist_fail.png");
            img->WriteImage(outFile1.c_str());
            TImage *img2 = TImage::Create();
            img2->FromPad(angles_fail);
            img2->WriteImage(outFile2.c_str());
            delete img;
            delete img2;
        }
        if(output==BOTH || output==TREE)
        {
            dist_fail->Write();
            angles_fail->Write();
        }
        delete dist_fail;
        delete angles_fail;
}
