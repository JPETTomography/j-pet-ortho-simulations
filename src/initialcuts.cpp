#include <iostream>
#include <sstream>
#include <iomanip>
#include "TCanvas.h"
#include "TImage.h"
#include "TLegend.h"
#include "TText.h"
#include "initialcuts.h"

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
    if(type==THREE || type==TWOandONE)
    {

        fH_12_pass_=nullptr;
        fH_12_fail_=nullptr;

        fH_en_pass_low_ = new TH1F((std::string("fH_en_pass_LOW")+std::to_string(fDecayType_)).c_str(), "fH_en_", 100, 0.0, 0.6);
        fH_en_pass_high_ = new TH1F((std::string("fH_en_pass_HIGH")+std::to_string(fDecayType_)).c_str(), "fH_en_", 100, 0.0, 0.6);
        fH_en_pass_event_ = new TH1F((std::string("fH_en_pass_event")+std::to_string(fDecayType_)).c_str(), "fH_en_", 100, 0.0, 0.6);

        // histograms for events tha passed cuts
        fH_12_23_pass_ = new TH2F("fH_12_23_pass","fH_12_23_pass", 50,0, 3.15, 50,0,3.15);
        fH_12_23_pass_ -> SetTitle("Polar angle distr, 12 vs 23");
        fH_12_23_pass_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_23_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_23_pass_ -> GetYaxis()->SetTitle("#theta_{23} [rad]");
        fH_12_23_pass_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_31_pass_ = new TH2F("fH_12_31_pass","fH_12_31_pass", 50,0, 3.15, 50,0,3.15);
        fH_12_31_pass_ -> SetTitle("Polar angle distr, 12 vs 31");
        fH_12_31_pass_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_31_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_31_pass_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_12_31_pass_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_23_31_pass_ = new TH2F("fH_23_31_pass","fH_23_31_pass", 50,0, 3.15, 50,0,3.15);
        fH_23_31_pass_ -> SetTitle("Polar angle distr, 23 vs 31");
        fH_23_31_pass_ -> GetXaxis()->SetTitle("#theta_{23} [rad]");
        fH_23_31_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_31_pass_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_23_31_pass_ -> GetYaxis()->SetTitleOffset(1.4);

        //histograms for events that failed cuts
        fH_12_23_fail_ = new TH2F("fH_12_23_fail","fH_12_23_fail", 50,0, 3.15, 50,0,3.15);
        fH_12_23_fail_ -> SetTitle("Polar angle distr, 12 vs 23");
        fH_12_23_fail_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_23_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_23_fail_ -> GetYaxis()->SetTitle("#theta_{23} [rad]");
        fH_12_23_fail_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_31_fail_ = new TH2F("fH_12_31_fail","fH_12_31_fail", 50,0, 3.15, 50,0,3.15);
        fH_12_31_fail_ -> SetTitle("Polar angle distr, 12 vs 31");
        fH_12_31_fail_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_31_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_31_fail_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_12_31_fail_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_23_31_fail_ = new TH2F("fH_23_31_fail","fH_23_31_fail", 50,0, 3.15, 50,0,3.15);
        fH_23_31_fail_ -> SetTitle("Polar angle distr, 23 vs 31");
        fH_23_31_fail_ -> GetXaxis()->SetTitle("#theta_{23} [rad]");
        fH_23_31_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_31_fail_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_23_31_fail_ -> GetYaxis()->SetTitleOffset(1.4);

        if(type==THREE)
        {
            fH_en_pass_mid_ = new TH1F((std::string("fH_en_pass_MID")+std::to_string(fDecayType_)).c_str(), "fH_en_", 100, 0.0, 0.6);
            fH_en_pass_mid_ -> SetTitle("Energy distribution, mid energy gammas");
            fH_en_pass_mid_ -> GetXaxis()->SetTitle("E [MeV]");
            fH_en_pass_mid_ -> GetXaxis()->SetTitleOffset(1.6);
            fH_en_pass_mid_ -> GetYaxis()->SetTitle("dN/dE");
            fH_en_pass_mid_ -> GetYaxis()->SetTitleOffset(2.);
            fTypeString_="3";
        }
        else
        {
            fH_en_pass_mid_ = nullptr;
            fTypeString_="2&1";
        }
    }
    else if(type==TWO)
    {
        fTypeString_ = "2";
        fH_12_23_pass_ = nullptr;
        fH_12_31_pass_ = nullptr;
        fH_23_31_pass_ = nullptr;
        fH_12_23_fail_ = nullptr;
        fH_12_31_fail_ = nullptr;
        fH_23_31_fail_ = nullptr;

        fH_en_pass_mid_ = nullptr;
        fH_en_pass_low_ = new TH1F((std::string("fH_en_pass_LOW")+std::to_string(fDecayType_)).c_str(), "fH_en_", 20, 0.510, 0.512);
        fH_en_pass_low_->GetXaxis()->SetNdivisions(7, false);
        fH_en_pass_high_ = new TH1F((std::string("fH_en_pass_HIGH")+std::to_string(fDecayType_)).c_str(), "fH_en_", 20, 0.510, 0.512);
        fH_en_pass_high_->GetXaxis()->SetNdivisions(7, false);
        fH_en_pass_event_ = new TH1F((std::string("fH_en_pass_event")+std::to_string(fDecayType_)).c_str(), "fH_en_", 20, 0.510, 0.512);
        fH_en_pass_event_->GetXaxis()->SetNdivisions(7, false);

        //histogram for events that passed cuts
        fH_12_pass_ = new TH1F("fH_12_pass", "fH_12_pass", 20, 3.13, 3.15);
        fH_12_pass_ -> SetTitle("Distribution of polar angle between 2 gammas");
        fH_12_pass_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_pass_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_pass_ -> GetYaxis()->SetTitleOffset(1.4);
        // histogram for events that did not passed cuts
        fH_12_fail_ = new TH1F("fH_12_fail", "fH_12_fail", 20, 3.13, 3.15);;
        fH_12_fail_ -> SetTitle("Distribution of polar angle between 2 gammas");
        fH_12_fail_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_fail_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_fail_ -> GetYaxis()->SetTitleOffset(1.4);
    }
    else
    {
        throw("Invalid no of decay products!");
    }
    //histograms for particles that passed through cuts
    fH_en_pass_ = new TH1F((std::string("fH_en_pass_")+std::to_string(fDecayType_)).c_str(), "fH_en_", 100, 0.0, 0.6);
    fH_en_pass_ -> SetTitle("Energy distribution");
    fH_en_pass_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_pass_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_pass_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_pass_ -> GetYaxis()->SetTitleOffset(2.);
    fH_p_pass_ = new TH1F((std::string("fH_p_pass_")+std::to_string(fDecayType_)).c_str(), "fH_p_", 100, 0.0, 0.6);
    fH_p_pass_ -> SetTitle("Momentum distribution");
    fH_p_pass_ -> GetXaxis()->SetTitle("p [MeV/c]");
    fH_p_pass_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_p_pass_ -> GetYaxis()->SetTitle("dN/dp");
    fH_p_pass_ -> GetYaxis()->SetTitleOffset(2.);
    fH_phi_pass_ = new TH1F((std::string("fH_phi_pass_")+std::to_string(fDecayType_)).c_str(), "fH_phi_", 100, -3.2, 3.2);
    fH_phi_pass_ -> SetTitle("Azimuthal angle distribution");
    fH_phi_pass_ -> GetXaxis()->SetTitle("#phi [rad]");
    fH_phi_pass_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_phi_pass_ -> GetYaxis()->SetTitle("dN/d #phi");
    fH_phi_pass_ -> GetYaxis()->SetTitleOffset(2.);
    fH_cosTheta_pass_ = new TH1F((std::string("fH_cosTheta_pass_")+std::to_string(fDecayType_)).c_str(), "fH_cosTheta_", 100, -1.01, 1.01);
    fH_cosTheta_pass_ -> SetTitle("Cosine of polar angle distribution");
    fH_cosTheta_pass_ -> GetXaxis()->SetTitle("cos(#theta)");
    fH_cosTheta_pass_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_cosTheta_pass_ -> GetYaxis()->SetTitle("dN/d cos(#theta)");
    fH_cosTheta_pass_ -> GetYaxis()->SetTitleOffset(2.);

    //histograms for particles that did not pass through cuts
    fH_en_fail_ = new TH1F((std::string("fH_en_fail_")+std::to_string(fDecayType_)).c_str(), "fH_en_", 100, 0.0, 0.6);
    fH_en_fail_ -> SetTitle("Energy distribution");
    fH_en_fail_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_fail_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_fail_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_fail_ -> GetYaxis()->SetTitleOffset(2.);
    fH_p_fail_ = new TH1F((std::string("fH_p_fail_")+std::to_string(fDecayType_)).c_str(), "fH_p_", 100, 0.0, 0.6);
    fH_p_fail_ -> SetTitle("Momentum distribution");
    fH_p_fail_ -> GetXaxis()->SetTitle("p [MeV/c]");
    fH_p_fail_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_p_fail_ -> GetYaxis()->SetTitle("dN/dp");
    fH_p_fail_ -> GetYaxis()->SetTitleOffset(2.);
    fH_phi_fail_ = new TH1F((std::string("fH_phi_fail_")+std::to_string(fDecayType_)).c_str(), "fH_phi_", 100, -3.2, 3.2);
    fH_phi_fail_ -> SetTitle("Azimuthal angle distribution");
    fH_phi_fail_ -> GetXaxis()->SetTitle("#phi [rad]");
    fH_phi_fail_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_phi_fail_ -> GetYaxis()->SetTitle("dN/d #phi");
    fH_phi_fail_ -> GetYaxis()->SetTitleOffset(2.);
    fH_cosTheta_fail_ = new TH1F((std::string("fH_cosTheta_fail_")+std::to_string(fDecayType_)).c_str(), "fH_cosTheta_", 100, -1.01, 1.01);
    fH_cosTheta_fail_ -> SetTitle("Cosine of polar angle distribution");
    fH_cosTheta_fail_ -> GetXaxis()->SetTitle("cos(#theta)");
    fH_cosTheta_fail_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_cosTheta_fail_ -> GetYaxis()->SetTitle("dN/d cos(#theta)");
    fH_cosTheta_fail_ -> GetYaxis()->SetTitleOffset(2.);

    //histograms for gammas that passed through cuts, differentation for low, mid and high energy gammas
    fH_en_pass_event_ -> SetTitle("Energy distribution");
    fH_en_pass_event_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_pass_event_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_pass_event_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_pass_event_ -> GetYaxis()->SetTitleOffset(2.);
    fH_en_pass_low_ -> SetTitle("Energy distribution, low energy gammas");
    fH_en_pass_low_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_pass_low_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_pass_low_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_pass_low_ -> GetYaxis()->SetTitleOffset(2.);
    fH_en_pass_high_ -> SetTitle("Energy distribution, high energy gammas");
    fH_en_pass_high_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_pass_high_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_pass_high_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_pass_high_ -> GetYaxis()->SetTitleOffset(2.);

    //histograms to monitor cuts passing
    fH_event_cuts_ = new TH1F((std::string("fH_event_cuts_")+std::to_string(fDecayType_)).c_str(),\
            "fH_event_cuts_", 3, 0.0, 3.0);
    fH_event_cuts_->SetTitle("Passing cuts by events");
    fH_event_cuts_->GetYaxis()->SetTitle("% passed");
    fH_event_cuts_->GetYaxis()->SetTitleOffset(1.4);
    fH_event_cuts_->GetXaxis()->SetLabelSize(0);
    fH_event_cuts_->GetXaxis()->SetTickLength(0);
    fH_gamma_cuts_ = new TH1F((std::string("fH_gamma_cuts_")+std::to_string(fDecayType_)).c_str(),\
            "fH_gamma_cuts_", 3, 0.0, 3.0);
    fH_gamma_cuts_->SetTitle("Passing cuts by gammas");
    fH_gamma_cuts_->GetYaxis()->SetTitle("% passed");
    fH_gamma_cuts_->GetYaxis()->SetTitleOffset(1.4);
    fH_gamma_cuts_->GetXaxis()->SetLabelSize(0);
    fH_gamma_cuts_->GetXaxis()->SetTickLength(0);

}

InitialCuts::InitialCuts(const InitialCuts& est)
{
    fSilentMode_=est.fSilentMode_;
    fR_ = est.fR_;  //radius in m
    fL_ = est.fL_;  //length in m
    fDetectionProbability_ = est.fDetectionProbability_;
    fDecayType_ = est.fDecayType_;
    fTypeString_ = est.fTypeString_;
    fRand_ = new TRandom3(0);

    if(fDecayType_==TWO)
    {
        fH_12_pass_ = new TH1F(*est.fH_12_pass_);
        fH_12_fail_ = new TH1F(*est.fH_12_fail_);
        fH_12_23_pass_ = nullptr;
        fH_12_31_pass_ = nullptr;
        fH_23_31_pass_ = nullptr;
        fH_12_23_fail_ = nullptr;
        fH_12_31_fail_ = nullptr;
        fH_23_31_fail_ = nullptr;
        fH_en_pass_mid_ = nullptr;
    }
    else
    {
        fH_12_pass_ = nullptr;
        fH_12_fail_ = nullptr;
        fH_12_23_pass_ = new TH2F(*est.fH_12_23_pass_);
        fH_12_31_pass_ = new TH2F(*est.fH_12_31_pass_);
        fH_23_31_pass_ = new TH2F(*est.fH_23_31_pass_);
        fH_12_23_fail_ = new TH2F(*est.fH_12_23_fail_);
        fH_12_31_fail_ = new TH2F(*est.fH_12_31_fail_);
        fH_23_31_fail_ = new TH2F(*est.fH_23_31_fail_);
        fH_en_pass_mid_ = new TH1F(*est.fH_en_pass_mid_);

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

InitialCuts& InitialCuts::operator=(const InitialCuts& est)
{
    fSilentMode_=est.fSilentMode_;
    fR_ = est.fR_;  //radius in m
    fL_ = est.fL_;  //length in m
    fDetectionProbability_ = est.fDetectionProbability_;
    fDecayType_ = est.fDecayType_;
    fTypeString_ = est.fTypeString_;

    fRand_ = new TRandom3(0);

    if(fDecayType_==TWO)
    {
        fH_12_pass_ = new TH1F(*est.fH_12_pass_);
        fH_12_fail_ = new TH1F(*est.fH_12_fail_);
        fH_12_23_pass_ = nullptr;
        fH_12_31_pass_ = nullptr;
        fH_23_31_pass_ = nullptr;
        fH_12_23_fail_ = nullptr;
        fH_12_31_fail_ = nullptr;
        fH_23_31_fail_ = nullptr;
        fH_en_pass_mid_ = nullptr;
    }
    else
    {
        fH_12_pass_ = nullptr;
        fH_12_fail_ = nullptr;
        fH_12_23_pass_ = new TH2F(*est.fH_12_23_pass_);
        fH_12_31_pass_ = new TH2F(*est.fH_12_31_pass_);
        fH_23_31_pass_ = new TH2F(*est.fH_23_31_pass_);
        fH_12_23_fail_ = new TH2F(*est.fH_12_23_fail_);
        fH_12_31_fail_ = new TH2F(*est.fH_12_31_fail_);
        fH_23_31_fail_ = new TH2F(*est.fH_23_31_fail_);
        fH_en_pass_mid_ = new TH1F(*est.fH_en_pass_mid_);

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

InitialCuts::~InitialCuts()
{
    if(fH_12_23_pass_) delete fH_12_23_pass_;
    if(fH_12_31_pass_) delete fH_12_31_pass_;
    if(fH_23_31_pass_) delete fH_23_31_pass_;
    if(fH_12_pass_) delete fH_12_pass_;
    if(fH_12_23_fail_) delete fH_12_23_fail_;
    if(fH_12_31_fail_) delete fH_12_31_fail_;
    if(fH_23_31_fail_) delete fH_23_31_fail_;
    if(fH_12_fail_) delete fH_12_fail_;
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

void InitialCuts::AddCuts(Event& event)
{
    fNumberOfEvents_++;
    bool geo_event_pass = true;
    bool inter_event_pass = true;
    fH_event_cuts_->Fill(0); //events at the beginning
    for(int ii=0; ii<3; ii++)
    {
        fH_gamma_cuts_->Fill(0); //gammas at the beginning
        if(event.GetEmissionPointOf(ii)!=nullptr && event.GetFourMomentumOf(ii)!=nullptr)
        {
            fNumberOfGammas_++;
            bool geo_pass = GeometricalAcceptance_(event.GetEmissionPointOf(ii), event.GetFourMomentumOf(ii));
            bool inter_pass = geo_pass ? DetectionCut_() : false;
            event.SetCutPassing(ii, geo_pass && inter_pass);
            geo_event_pass &= geo_pass;
            inter_event_pass &= inter_pass;
            if(geo_pass && inter_pass)
            {
                fAcceptedGammas_++;
                fH_en_pass_->Fill(event.GetFourMomentumOf(ii)->Energy());
                fH_p_pass_->Fill(event.GetFourMomentumOf(ii)->P());
                fH_phi_pass_->Fill(event.GetFourMomentumOf(ii)->Phi());
                fH_cosTheta_pass_->Fill(event.GetFourMomentumOf(ii)->CosTheta());
            }
            else
            {
                fH_en_fail_->Fill(event.GetFourMomentumOf(ii)->Energy());
                fH_p_fail_->Fill(event.GetFourMomentumOf(ii)->P());
                fH_phi_fail_->Fill(event.GetFourMomentumOf(ii)->Phi());
                fH_cosTheta_fail_->Fill(event.GetFourMomentumOf(ii)->CosTheta());
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
    event.DeducePassFlag();
}

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

void InitialCuts::FillValidEventHistograms_(const Event& event)
{
    fAcceptedEvents_++;
    int minIndex=0;
    int maxIndex=0;
    for(int ii=0; ii<3; ii++)
    {
        if(event.GetFourMomentumOf(ii) != nullptr)
        {
            fH_en_pass_event_->Fill(event.GetFourMomentumOf(ii)->Energy());
            minIndex = event.GetFourMomentumOf(ii)->E() < event.GetFourMomentumOf(minIndex)->E() ? ii : minIndex;
            maxIndex = event.GetFourMomentumOf(ii)->E() > event.GetFourMomentumOf(maxIndex)->E() ? ii : maxIndex;
        }
    }
    fH_en_pass_low_->Fill(event.GetFourMomentumOf(minIndex)->Energy());
    fH_en_pass_high_->Fill(event.GetFourMomentumOf(maxIndex)->Energy());
    if(fDecayType_==THREE || fDecayType_==TWOandONE)
    {
         if(fDecayType_==THREE)
         {
             int midIndex = minIndex==maxIndex ? minIndex : 3-minIndex-maxIndex;
             fH_en_pass_mid_->Fill(event.GetFourMomentumOf(midIndex)->Energy());
         }
        fH_12_23_pass_->Fill(event.GetFourMomentumOf(0)->Angle(event.GetFourMomentumOf(1)->Vect()), \
                             event.GetFourMomentumOf(1)->Angle(event.GetFourMomentumOf(2)->Vect()), event.GetWeight());
        fH_12_31_pass_->Fill(event.GetFourMomentumOf(0)->Angle(event.GetFourMomentumOf(1)->Vect()), \
                             event.GetFourMomentumOf(2)->Angle(event.GetFourMomentumOf(0)->Vect()), event.GetWeight());
        fH_23_31_pass_->Fill(event.GetFourMomentumOf(1)->Angle(event.GetFourMomentumOf(2)->Vect()), \
                             event.GetFourMomentumOf(2)->Angle(event.GetFourMomentumOf(0)->Vect()), event.GetWeight());
    }
    else if(fDecayType_ == TWO)
        fH_12_pass_->Fill(event.GetFourMomentumOf(0)->Angle((event.GetFourMomentumOf(1))->Vect()), event.GetWeight());
    else
    {
         throw("Invalid no of decay products in InitialCuts!");
    }
}

void InitialCuts::FillInvalidEventHistograms_(const Event& event)
{
    if(fDecayType_==THREE || fDecayType_==TWOandONE)
    {
        fH_12_23_fail_->Fill(event.GetFourMomentumOf(0)->Angle(event.GetFourMomentumOf(1)->Vect()), \
                             event.GetFourMomentumOf(1)->Angle(event.GetFourMomentumOf(2)->Vect()), event.GetWeight());
        fH_12_31_fail_->Fill(event.GetFourMomentumOf(0)->Angle(event.GetFourMomentumOf(1)->Vect()), \
                             event.GetFourMomentumOf(2)->Angle(event.GetFourMomentumOf(0)->Vect()), event.GetWeight());
        fH_23_31_fail_->Fill(event.GetFourMomentumOf(1)->Angle(event.GetFourMomentumOf(2)->Vect()), \
                             event.GetFourMomentumOf(2)->Angle(event.GetFourMomentumOf(0)->Vect()), event.GetWeight());
    }
    else if(fDecayType_ == TWO)
        fH_12_fail_->Fill(event.GetFourMomentumOf(0)->Angle((event.GetFourMomentumOf(1))->Vect()), event.GetWeight());
    else
    {
         throw("Invalid no of decay products in InitialCuts!");
    }
}


void InitialCuts::FillDistributionHistograms_(const Event& event)
{
    for(int ii=0; ii<3; ii++)
    {
        if(event.GetFourMomentumOf(ii)!=nullptr)
        {
            if(event.GetCutPassingOf(ii))
            {
                fH_en_pass_->Fill(event.GetFourMomentumOf(ii)->Energy());
                fH_p_pass_->Fill(event.GetFourMomentumOf(ii)->P());
                fH_phi_pass_->Fill(event.GetFourMomentumOf(ii)->Phi());
                fH_cosTheta_pass_->Fill(event.GetFourMomentumOf(ii)->CosTheta());
            }
            else
            {
                fH_en_fail_->Fill(event.GetFourMomentumOf(ii)->Energy());
                fH_p_fail_->Fill(event.GetFourMomentumOf(ii)->P());
                fH_phi_fail_->Fill(event.GetFourMomentumOf(ii)->Phi());
                fH_cosTheta_fail_->Fill(event.GetFourMomentumOf(ii)->CosTheta());
            }
        }
    }
}

void InitialCuts::DrawHistograms(std::string prefix)
{
    DrawCutsHistograms(prefix);
    DrawPassHistograms(prefix);
    DrawFailHistograms(prefix);
}

void InitialCuts::DrawCutsHistograms(std::string prefix)
{
    std::string outFile;
    if(!fSilentMode_) std::cout<<"[INFO] Drawing histograms for cuts passing."<<std::endl;
    TCanvas* cuts = new TCanvas((std::string("cuts_passed")+fTypeString_).c_str(),\
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
    TImage *img_cut = TImage::Create();
    img_cut->FromPad(cuts);
    outFile=prefix+std::string("cuts_")+fTypeString_+std::string("gammas.png");
    img_cut->WriteImage(outFile.c_str());

    delete cuts;
    delete labelBefore;
    delete labelGeo;
    delete labelP;
    delete labelPercent;
    delete img_cut;
}


void InitialCuts::DrawPassHistograms(std::string prefix)
{
    std::string outFile;
    if(!fSilentMode_) std::cout<<"[INFO] Drawing histograms for events that passed cuts."<<std::endl;
    TCanvas* angles_pass;

    TCanvas* dist_pass = new TCanvas((std::string("dist_pass")+fTypeString_).c_str(),\
                                     ("Basic distributions for passed "\
                                     +fTypeString_+std::string("-gamma events")).c_str(), 800, 800);
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

    if(!fSilentMode_) std::cout<<"[INFO] Saving histograms for passed events"<<std::endl;
    TImage *img = TImage::Create();
    img->FromPad(dist_pass);
    outFile=prefix+std::string("dist_pass_")+fTypeString_+std::string("_gammas.png");
    img->WriteImage(outFile.c_str());

    //setting fill colors for histograms that differentiate gammas with low, mid and high energies
    fH_en_pass_event_->SetFillColor(kBlack);
    fH_en_pass_low_->SetFillColor(kRed);
    fH_en_pass_high_->SetFillColor(kBlue);
    TLegend* legend = new TLegend(0.1, 0.5, 0.4, 0.9);
    legend->AddEntry(fH_en_pass_event_, "all gammas", "f");
    legend->AddEntry(fH_en_pass_low_, "lowest energy", "f");
    legend->AddEntry(fH_en_pass_high_, "highest energy", "f");

    //angle distribution depends on the number of decay products
    if(fDecayType_ == THREE || fDecayType_ == TWOandONE)
    {
        angles_pass = new TCanvas((std::string("angles_pass")+fTypeString_).c_str(),\
                                    ("Angle ditribution for all generated "\
                                    +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);
        angles_pass->Divide(3,1);
        angles_pass->cd(1);
        outFile = prefix+std::string("angles_pass_3gammas.png");
        fH_12_23_pass_->Draw("colz");
        angles_pass->cd(2);
        fH_12_31_pass_->Draw("colz");
        angles_pass->cd(3);
        fH_23_31_pass_->Draw("colz");
    }
    else
    {
        angles_pass = new TCanvas((std::string("angles_pass")+fTypeString_).c_str(),\
                                  ("Angle ditribution for all generated "\
                                  +fTypeString_+std::string("-gamma events")).c_str(), 500, 400);
        fH_12_pass_->Draw();
        outFile = prefix+std::string("angles_pass_2gammas.png");
    }

    if(fDecayType_==THREE)
    {
        //Drawing energy distribution of gammas from valid events
        diff_en= new TCanvas((std::string("diff_energies")+fTypeString_).c_str(),\
                                      ("Distribution of energy for incident photons from "\
                                       +fTypeString_+std::string("-gamma events")).c_str(), 1000, 900);
        diff_en->Divide(2,2);
        diff_en->cd(1);
        fH_en_pass_mid_->SetFillColor(kGreen);
        legend->AddEntry(fH_en_pass_mid_, "medium energy", "f");
        fH_en_pass_event_->Draw();
        fH_en_pass_low_->Draw("same");
        fH_en_pass_mid_->Draw("same");
        fH_en_pass_high_->Draw("same");
        legend->Draw();
        diff_en->cd(2);
        fH_en_pass_low_->Draw();
        diff_en->cd(3);
        fH_en_pass_mid_->Draw();
        diff_en->cd(4);
        fH_en_pass_high_->Draw();
        outFileDiffEn = prefix+std::string("diff_energies_")+fTypeString_+std::string("gammas.png");
    }
    else
    {

        //Drawing energy distribution of gammas from valid events
        diff_en= new TCanvas((std::string("diff_energies")+fTypeString_).c_str(),\
                                      ("Distribution of energy for incident photons from "\
                                       +fTypeString_+std::string("-gamma events")).c_str(), 1200, 450);
        diff_en->Divide(3,1);
        diff_en->cd(1);
        fH_en_pass_event_->Draw();
        fH_en_pass_low_->Draw("same");
        fH_en_pass_high_->Draw("same");
        legend->Draw();
        diff_en->Update();
        diff_en->cd(2);
        fH_en_pass_low_->Draw();
        diff_en->cd(3);
        fH_en_pass_high_->Draw();
        outFileDiffEn = prefix+std::string("diff_energies_")+fTypeString_+std::string("gammas.png");
    }

    angles_pass->Update();
    TImage *img2 = TImage::Create();
    img2->FromPad(angles_pass);
    img2->WriteImage(outFile.c_str());

    diff_en->Update();
    TImage *img3 = TImage::Create();
    img3->FromPad(diff_en);
    img3->WriteImage(outFileDiffEn.c_str());

    delete dist_pass;
    delete angles_pass;
    delete diff_en;
    delete legend;
    delete img;
    delete img2;
    delete img3;
}

void InitialCuts::DrawFailHistograms(std::string prefix)
{
        std::string outFile;
        if(!fSilentMode_) std::cout<<"[INFO] Drawing histograms for events that did not pass cuts."<<std::endl;
        TCanvas* angles_fail;
        TCanvas* dist_fail= new TCanvas((std::string("dist_fail")+fTypeString_).c_str(), ("Basic distributions for passed "\
                                         +fTypeString_+std::string("-gamma events")).c_str(), 800, 800);
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

        if(!fSilentMode_) std::cout<<"[INFO] Saving histograms for failed events"<<std::endl;
        TImage *img = TImage::Create();
        img->FromPad(dist_fail);
        outFile=prefix+std::string("dist_fail_")+fTypeString_+std::string("_gammas.png");
        img->WriteImage(outFile.c_str());

        //angle distribution depends on the number of decay products
        if(fDecayType_ == THREE || fDecayType_ == TWOandONE)
        {
            angles_fail = new TCanvas((std::string("angles_fail")+fTypeString_).c_str(), \
                                      ("Angle ditribution for all generated "\
                                      +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);

            angles_fail->Divide(3,1);
            angles_fail->cd(1);
            outFile = prefix+std::string("angles_fail_")+fTypeString_+std::string("gammas.png");
            fH_12_23_fail_->Draw("colz");
            angles_fail->cd(2);
            fH_12_31_fail_->Draw("colz");
            angles_fail->cd(3);
            fH_23_31_fail_->Draw("colz");
        }
        else
        {
            angles_fail = new TCanvas((std::string("angles_fail")+fTypeString_).c_str(), \
                                      ("Angle ditribution for all generated "\
                                      +fTypeString_+std::string("-gamma events")).c_str(), 500, 400);

            fH_12_fail_->Draw();
            outFile = prefix+std::string("angles_fail_")+fTypeString_+std::string("gammas.png");
        }
        angles_fail->Update();
        TImage *img2 = TImage::Create();
        img2->FromPad(angles_fail);
        img2->WriteImage(outFile.c_str());

        delete dist_fail;
        delete angles_fail;
        delete img;
        delete img2;
}
