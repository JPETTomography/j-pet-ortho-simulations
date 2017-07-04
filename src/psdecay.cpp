/// @file psdecay.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 01.06.2017

#include <typeinfo>
#include <cstdio>
#include <iomanip>
#include "TLegend.h"
#include "TText.h"
#include "psdecay.h"


PsDecay::PsDecay(DecayType type) :
      fSilentMode_(false),
      fDecayType_(type)
{
    //Creating histograms for angle distributions
    if(fDecayType_ == THREE || fDecayType_ == TWOandONE)
    {
        fH_12_=nullptr;

        //histograms for all events generated
        fH_12_23_ = new TH2F("fH_12_23_","fH_12_23_all", 50,0, 3.15, 50,0,3.15);
        fH_12_23_ -> SetTitle("Polar angle distr, 12 vs 23");
        fH_12_23_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_23_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_23_ -> GetYaxis()->SetTitle("#theta_{23} [rad]");
        fH_12_23_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_31_ = new TH2F("fH_12_31_","fH_12_31_all", 50,0, 3.15, 50, 0, 3.15);
        fH_12_31_ -> SetTitle("Polar angle distr, 12 vs 31");
        fH_12_31_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_31_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_31_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_12_31_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_23_31_ = new TH2F("fH_23_31_","fH_23_31_all", 50,0, 3.15, 50,0,3.15);
        fH_23_31_ -> SetTitle("Polar angle distr, 23 vs 31");
        fH_23_31_ -> GetXaxis()->SetTitle("#theta_{23} [rad]");
        fH_23_31_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_31_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_23_31_ -> GetYaxis()->SetTitleOffset(1.4);
        if(fDecayType_==THREE)
            fTypeString_="3";
        else
            fTypeString_="2&1";
    }
    else if(fDecayType_ == TWO)
    {
        fH_12_23_ = nullptr;
        fH_12_31_ = nullptr;
        fH_23_31_ = nullptr;

        //histogram for all events generated
        fH_12_ = new TH1F("fH_12_all", "fH_12_all", 20, 3.13, 3.15);
        fH_12_ -> SetTitle("Distribution of polar angle between 2 gammas");
        fH_12_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_ -> GetYaxis()->SetTitleOffset(1.4);
        fTypeString_="2";
    }
    else
    {
        throw("Invalid no of decay products!");
    }

    // histograms common for all particles
    fH_en_ = new TH1F((std::string("fH_en_")+fTypeString_).c_str(), "fH_en_", 100, 0.0, 0.6);
    fH_en_ -> SetTitle("Energy distribution");
    fH_en_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_ -> GetYaxis()->SetTitleOffset(2.);
    fH_p_ = new TH1F((std::string("fH_p_")+fTypeString_).c_str(), "fH_p_", 100, 0.0, 0.6);
    fH_p_ -> SetTitle("Momentum distribution");
    fH_p_ -> GetXaxis()->SetTitle("p [MeV/c]");
    fH_p_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_p_ -> GetYaxis()->SetTitle("dN/dp");
    fH_p_ -> GetYaxis()->SetTitleOffset(2.);
    fH_phi_ = new TH1F((std::string("fH_phi_")+fTypeString_).c_str(), "fH_phi_", 100, -3.2, 3.2);
    fH_phi_ -> SetTitle("Azimuthal angle distribution");
    fH_phi_ -> GetXaxis()->SetTitle("#phi [rad]");
    fH_phi_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_phi_ -> GetYaxis()->SetTitle("dN/d #phi");
    fH_phi_ -> GetYaxis()->SetTitleOffset(2.);
    fH_cosTheta_ = new TH1F((std::string("fH_cosTheta_")+fTypeString_).c_str(), "fH_cosTheta_", 100, -1.01, 1.01);
    fH_cosTheta_ -> SetTitle("Cosine of polar angle distribution");
    fH_cosTheta_ -> GetXaxis()->SetTitle("cos(#theta)");
    fH_cosTheta_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_cosTheta_ -> GetYaxis()->SetTitle("dN/d cos(#theta)");
    fH_cosTheta_ -> GetYaxis()->SetTitleOffset(2.);

}

///
/// \brief PsDecay::PsDecay Copy constructor.
/// \param est Instance of PsDecay to be copied.
///
PsDecay::PsDecay(const PsDecay& est)
{
    fSilentMode_=est.fSilentMode_;
    fDecayType_=est.fDecayType_;
    if(fDecayType_==TWO)
    {
        fH_12_ = new TH1F(*est.fH_12_);
        fH_12_23_ = nullptr;
        fH_12_31_ = nullptr;
        fH_23_31_ = nullptr;
    }
    else
    {
        fH_12_ = nullptr;
        fH_12_23_ = new TH2F(*est.fH_12_23_);
        fH_12_31_ = new TH2F(*est.fH_12_31_);
        fH_23_31_ = new TH2F(*est.fH_23_31_);

    }
    fH_en_ = new TH1F(*est.fH_en_);
    fH_p_ = new TH1F(*est.fH_p_);
    fH_phi_= new TH1F(*est.fH_phi_);
    fH_cosTheta_ = new TH1F(*est.fH_cosTheta_);

}

///
/// \brief PsDecay::operator = Assignement operator.
/// \param est Instance of PsDecay to be copied.
/// \return Reference to instance of PsDecay with values of member fields copied.
///
PsDecay& PsDecay::operator=(const PsDecay& est)
{
    fSilentMode_=est.fSilentMode_;
    fDecayType_=est.fDecayType_;
    if(fDecayType_==TWO)
    {
        fH_12_ = new TH1F(*est.fH_12_);
        fH_12_23_ = nullptr;
        fH_12_31_ = nullptr;
        fH_23_31_ = nullptr;
    }
    else
    {
        fH_12_ = nullptr;
        fH_12_23_ = new TH2F(*est.fH_12_23_);
        fH_12_31_ = new TH2F(*est.fH_12_31_);
        fH_23_31_ = new TH2F(*est.fH_23_31_);
    }
    fH_en_ = new TH1F(*est.fH_en_);
    fH_p_ = new TH1F(*est.fH_p_);
    fH_phi_= new TH1F(*est.fH_phi_);
    fH_cosTheta_ = new TH1F(*est.fH_cosTheta_);
    return *this;
}

///
/// \brief PsDecay::~PsDecay Destructor, deletes all histograms.
///
PsDecay::~PsDecay()
{
    if(fH_12_23_) delete fH_12_23_;
    if(fH_12_31_) delete fH_12_31_;
    if(fH_23_31_) delete fH_23_31_;
    if(fH_12_) delete fH_12_;
    if(fH_en_) delete fH_en_;
    if(fH_p_) delete fH_p_;
    if(fH_phi_) delete fH_phi_;
    if(fH_cosTheta_) delete fH_cosTheta_;
}


void PsDecay::AddEvent(Event& event)
{
    for(int ii=0; ii<3; ii++)
    {
        if(event.GetFourMomentumAt(ii)!=nullptr)
        {
            //filling histograms for all gammas
            fH_en_->Fill(event.GetFourMomentumAt(ii)->Energy());
            fH_p_->Fill(event.GetFourMomentumAt(ii)->P());
            fH_phi_->Fill(event.GetFourMomentumAt(ii)->Phi());
            fH_cosTheta_->Fill(event.GetFourMomentumAt(ii)->CosTheta());
        }
    }
    if(fDecayType_==TWO)
    {
        fH_12_->Fill(event.GetFourMomentumAt(0)->Angle((event.GetFourMomentumAt(1))->Vect()), event.GetWeight());
    }
    else
    {
        fH_12_23_->Fill(event.GetFourMomentumAt(0)->Angle(event.GetFourMomentumAt(1)->Vect()), \
                        event.GetFourMomentumAt(1)->Angle(event.GetFourMomentumAt(2)->Vect()), event.GetWeight());
        fH_12_31_->Fill(event.GetFourMomentumAt(0)->Angle(event.GetFourMomentumAt(1)->Vect()),\
                        event.GetFourMomentumAt(2)->Angle(event.GetFourMomentumAt(0)->Vect()), event.GetWeight());
        fH_23_31_->Fill(event.GetFourMomentumAt(1)->Angle(event.GetFourMomentumAt(2)->Vect()),\
                        event.GetFourMomentumAt(2)->Angle(event.GetFourMomentumAt(0)->Vect()), event.GetWeight());
    }
}


void PsDecay::DrawHistograms(std::string prefix)
{
    std::string outFile;

    if(!fSilentMode_) std::cout<<"[INFO] Drawing histograms for all generated events."<<std::endl;
    TCanvas* angles_all;

    TCanvas* dist_all = new TCanvas((std::string("dist_all")+fTypeString_).c_str(), \
                                    (std::string("Basic distributions for all generated ")\
                                    +fTypeString_+std::string("-gamma events")).c_str(), 800, 800);
    dist_all->Divide(2, 2);
    dist_all->cd(1);
    fH_en_->Draw();
    dist_all->cd(2);
    fH_p_->Draw();
    dist_all->cd(3);
    fH_phi_->Draw();
    dist_all->cd(4);
    fH_cosTheta_->Draw();
    dist_all->Update();

    if(!fSilentMode_) std::cout<<"[INFO] Saving histograms for all events"<<std::endl;
    TImage *img = TImage::Create();
    img->FromPad(dist_all);
    outFile=prefix+std::string("dist_all_")+fTypeString_+std::string("gammas.png");
    img->WriteImage(outFile.c_str());

    //angle distribution depends on the number of decay products
    if(fDecayType_==THREE || fDecayType_==TWOandONE)
    {
        angles_all = new TCanvas((std::string("angles_all")+fTypeString_).c_str(), \
                                                  (std::string("Angle ditribution for all generated ")\
                                                  +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);
        angles_all->Divide(3,1);
        angles_all->cd(1);
        outFile = prefix+std::string("angles_all_3gammas.png");
        fH_12_23_->Draw("colz");
        angles_all->cd(2);
        fH_12_31_->Draw("colz");
        angles_all->cd(3);
        fH_23_31_->Draw("colz");
    }
    else
    {
        angles_all = new TCanvas((std::string("angles_all")+fTypeString_).c_str(), \
                                               (std::string("Angle ditribution for all generated ")\
                                               +fTypeString_+std::string("-gamma events")).c_str(), 500, 400);
        fH_12_->Draw();
        outFile = prefix+std::string("angles_all_2gammas.png");
    }
    angles_all->Update();
    TImage *img2 = TImage::Create();
    img2->FromPad(angles_all);
    img2->WriteImage(outFile.c_str());

    delete dist_all;
    delete angles_all;
}
