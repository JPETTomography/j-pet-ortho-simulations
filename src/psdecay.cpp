/// @file psdecay.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017

#include <typeinfo>
#include <cstdio>
#include <algorithm>
#include <iomanip>
#include "TLegend.h"
#include "TText.h"
#include "psdecay.h"

unsigned PsDecay::objectID_;


///
/// \brief PsDecay::PsDecay The only used constructor.
/// \param type Type of decay. Can be TWO, THREE or TWOandONE.
///
PsDecay::PsDecay(DecayType type) :
      fSilentMode_(false),
      fDecayType_(type)
{
    fH_12_=nullptr;
    fH_23_=nullptr;
    fH_31_=nullptr;
    fH_23_ = nullptr;
    fH_31_ = nullptr;
    fH_12_23_ = nullptr;
    fH_12_31_ = nullptr;
    fH_23_31_ = nullptr;
    fH_min_mid_ = nullptr;
    fH_min_max_ = nullptr;
    fH_mid_max_ = nullptr;
    //Creating histograms for angle distributions
    if(fDecayType_ == THREE)
    {
        fTypeString_="3";
        //general purpose histograms are created here to ensure right limits
        fH_en_ = new TH1F((std::string("fH_en_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_p_ = new TH1F((std::string("fH_p_")+fTypeString_).c_str(), "fH_p_", 52, 0.0, 0.6);
        //histograms for all events generated
        fH_12_23_ = new TH2F((std::string("fH_12_23_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(),"fH_12_23_all", 50,0, 3.15, 50,0,3.15);
        fH_12_23_ -> SetTitle("Polar angle distr, 12 vs 23");
        fH_12_23_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_23_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_23_ -> GetYaxis()->SetTitle("#theta_{23} [rad]");
        fH_12_23_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_31_ = new TH2F((std::string("fH_12_31_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(),"fH_12_31_all", 50,0, 3.15, 50, 0, 3.15);
        fH_12_31_ -> SetTitle("Polar angle distr, 12 vs 31");
        fH_12_31_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_31_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_31_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_12_31_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_23_31_ = new TH2F((std::string("fH_23_31_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(),"fH_23_31_all", 50,0, 3.15, 50,0,3.15);
        fH_23_31_ -> SetTitle("Polar angle distr, 23 vs 31");
        fH_23_31_ -> GetXaxis()->SetTitle("#theta_{23} [rad]");
        fH_23_31_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_31_ -> GetYaxis()->SetTitle("#theta_{31} [rad]");
        fH_23_31_ -> GetYaxis()->SetTitleOffset(1.4);

        //histograms for all events generated with ordered angles
        fH_min_mid_ = new TH2F((std::string("fH_min_mid_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(),"fH_min_mid_all", 50,0, 3.15, 50,0,3.15);
        fH_min_mid_ -> SetTitle("Polar angle distr, min vs med");
        fH_min_mid_ -> GetXaxis()->SetTitle("#theta_{min} [rad]");
        fH_min_mid_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_min_mid_ -> GetYaxis()->SetTitle("#theta_{med} [rad]");
        fH_min_mid_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_min_max_ = new TH2F((std::string("fH_min_max_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(),"fH_min_max_all", 50,0, 3.15, 50, 0, 3.15);
        fH_min_max_ -> SetTitle("Polar angle distr, min vs max");
        fH_min_max_ -> GetXaxis()->SetTitle("#theta_{min} [rad]");
        fH_min_max_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_min_max_ -> GetYaxis()->SetTitle("#theta_{max} [rad]");
        fH_min_max_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_mid_max_ = new TH2F((std::string("fH_mid_max_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(),"fH_mid_max_all", 50,0, 3.15, 50,0,3.15);
        fH_mid_max_ -> SetTitle("Polar angle distr, mid vs max");
        fH_mid_max_ -> GetXaxis()->SetTitle("#theta_{mid} [rad]");
        fH_mid_max_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_mid_max_ -> GetYaxis()->SetTitle("#theta_{max} [rad]");
        fH_mid_max_ -> GetYaxis()->SetTitleOffset(1.4);

    }
    else if(fDecayType_ == TWO)
    {
        fTypeString_="2";
        //general purpose histograms are created here to ensure right limits
        fH_en_ = new TH1F((std::string("fH_en_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 0.6);
        fH_p_ = new TH1F((std::string("fH_p_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_p_", 52, 0.0, 0.6);
        //histogram for all events generated
        fH_12_ = new TH1F((std::string("fH_12_all")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_12_all", 19, 3.13, 3.15);
        fH_12_->SetFillColor(kBlue);
        fH_12_ -> SetTitle("Distribution of polar angle between 2 gammas");
        fH_12_ -> GetXaxis()->SetNdivisions(5, false);
        fH_12_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_ -> GetYaxis()->SetTitleOffset(1.4);

    }
    else if(fDecayType_ == TWOandONE)
    {
        fTypeString_="2&1";
        //general purpose histograms are created here to ensure right limits
        fH_en_ = new TH1F((std::string("fH_en_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.3, 1.3);
        fH_p_ = new TH1F((std::string("fH_p_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_p_", 52, 0.3, 1.3);
        //histograms for all events generated
        fH_12_ = new TH1F((std::string("fH_12_all")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_12_all", 19, 3.13, 3.15);
        fH_12_->SetFillColor(kBlue);
        fH_12_ -> SetTitle("Polar angle distribution between gamma1 and gamma2");
        fH_12_ -> GetXaxis()->SetNdivisions(5, false);
        fH_12_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_ -> GetYaxis()->SetTitleOffset(1.4);
        fH_23_ = new TH1F((std::string("fH_23_all")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_23_all", 50, 0, 3.15);
        fH_23_->SetFillColor(kBlue);
        fH_23_ -> SetTitle("Polar angle distribution between gamma2 and gamma3");
        fH_23_ -> GetXaxis()->SetNdivisions(5, false);
        fH_23_ -> GetXaxis()->SetTitle("#theta_{23} [rad]");
        fH_23_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_ -> GetYaxis()->SetTitle("dN/d#theta_{23}");
        fH_23_ -> GetYaxis()->SetTitleOffset(1.4);
        fH_31_ = new TH1F((std::string("fH_31_all")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_31_all", 50, 0, 3.15);
        fH_31_->SetFillColor(kBlue);
        fH_31_ -> SetTitle("Polar angle distribution between gamma1 and gamma3");
        fH_31_ -> GetXaxis()->SetNdivisions(5, false);
        fH_31_ -> GetXaxis()->SetTitle("#theta_{31} [rad]");
        fH_31_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_31_ -> GetYaxis()->SetTitle("dN/d#theta_{31}");
        fH_31_ -> GetYaxis()->SetTitleOffset(1.4);

    }
    else if(fDecayType_ == TWOandN)
    {
        fTypeString_="2&N";
        //general purpose histograms are created here to ensure right limits
        fH_en_ = new TH1F((std::string("fH_en_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_en_", 104, 0.0, 4.0);
        fH_p_ = new TH1F((std::string("fH_p_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_p_", 104, 0.0, 4.0);
        //histogram for all events generated
        fH_12_ = new TH1F("fH_12_all", "fH_12_all", 19, 3.10, 3.2);
        fH_12_->SetFillColor(kBlue);
        fH_12_ -> SetTitle("Distribution of polar angle between 2 gammas");
        fH_12_ -> GetXaxis()->SetNdivisions(5, false);
        fH_12_ -> GetXaxis()->SetTitle("#theta_{12} [rad]");
        fH_12_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_ -> GetYaxis()->SetTitle("dN/d#theta_{12}");
        fH_12_ -> GetYaxis()->SetTitleOffset(1.4);

    }
    else if(fDecayType_ == ONE)
    {
        fTypeString_="1";
        //general purpose histograms are created here to ensure right limits
        fH_en_ = new TH1F((std::string("fH_en_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_en_", 52, 0.0, 2.0);
        fH_p_ = new TH1F((std::string("fH_p_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_p_", 52, 0.0, 2.0);
    }
    else
    {
        throw(std::string("Invalid no of decay products!"));
    }

    // histograms common for all decay types
    fH_en_->SetFillColor(kBlue);
    fH_en_ -> SetTitle("Energy distribution");
    fH_en_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_en_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_p_->SetFillColor(kBlue);
    fH_p_ -> SetTitle("Momentum distribution");
    fH_p_ -> GetXaxis()->SetTitle("p [MeV/c]");
    fH_p_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_p_ -> GetYaxis()->SetTitle("dN/dp");
    fH_p_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_phi_ = new TH1F((std::string("fH_phi_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_phi_", 52, -3.2, 3.2);
    fH_phi_->SetFillColor(kBlue);
    fH_phi_ -> SetTitle("Azimuthal angle distribution");
    fH_phi_ -> GetXaxis()->SetTitle("#phi [rad]");
    fH_phi_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_phi_ -> GetYaxis()->SetTitle("dN/d #phi");
    fH_phi_ -> GetYaxis()->SetTitleOffset(1.8);
    fH_cosTheta_ = new TH1F((std::string("fH_cosTheta_")+fTypeString_+"_"+std::to_string(objectID_)).c_str(), "fH_cosTheta_", 52, -1.01, 1.01);
    fH_cosTheta_->SetFillColor(kBlue);
    fH_cosTheta_ -> SetTitle("Cosine of polar angle distribution");
    fH_cosTheta_ -> GetXaxis()->SetTitle("cos(#theta)");
    fH_cosTheta_ -> GetXaxis()->SetTitleOffset(1.6);
    fH_cosTheta_ -> GetYaxis()->SetTitle("dN/d cos(#theta)");
    fH_cosTheta_ -> GetYaxis()->SetTitleOffset(1.8);

    objectID_++;
}

///
/// \brief PsDecay::PsDecay Copy constructor.
/// \param est Instance of PsDecay to be copied.
///
PsDecay::PsDecay(const PsDecay& est)
{
    fSilentMode_=est.fSilentMode_;
    fDecayType_=est.fDecayType_;
    fTypeString_ = est.fTypeString_;

    fH_12_23_ = nullptr;
    fH_12_31_ = nullptr;
    fH_23_31_ = nullptr;
    fH_min_mid_ = nullptr;
    fH_min_max_ = nullptr;
    fH_mid_max_ = nullptr;
    fH_12_ = nullptr;
    fH_23_ = nullptr;
    fH_31_ = nullptr;

    if(fDecayType_==TWO)
    {
        fH_12_ = new TH1F(*est.fH_12_);
    }
    else if(fDecayType_==THREE)
    {
        fH_12_23_ = new TH2F(*est.fH_12_23_);
        fH_12_31_ = new TH2F(*est.fH_12_31_);
        fH_23_31_ = new TH2F(*est.fH_23_31_);
        fH_min_mid_ = new TH2F(*est.fH_min_mid_);
        fH_min_max_ = new TH2F(*est.fH_min_max_);
        fH_mid_max_ = new TH2F(*est.fH_mid_max_);

    }
    else if(fDecayType_==TWOandONE)
    {
        fH_12_ = new TH1F(*est.fH_12_);
        fH_23_ = new TH1F(*est.fH_23_);
        fH_31_ = new TH1F(*est.fH_31_);
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
    fTypeString_ = est.fTypeString_;

    fH_12_23_ = nullptr;
    fH_12_31_ = nullptr;
    fH_23_31_ = nullptr;
    fH_min_mid_ = nullptr;
    fH_min_max_ = nullptr;
    fH_mid_max_ = nullptr;
    fH_12_ = nullptr;
    fH_23_ = nullptr;
    fH_31_ = nullptr;

    if(fDecayType_==TWO)
    {
        fH_12_ = new TH1F(*est.fH_12_);
    }
    else if(fDecayType_==THREE)
    {
        fH_12_23_ = new TH2F(*est.fH_12_23_);
        fH_12_31_ = new TH2F(*est.fH_12_31_);
        fH_23_31_ = new TH2F(*est.fH_23_31_);
        fH_min_mid_ = new TH2F(*est.fH_min_mid_);
        fH_min_max_ = new TH2F(*est.fH_min_max_);
        fH_mid_max_ = new TH2F(*est.fH_mid_max_);

    }
    else if(fDecayType_==TWOandONE)
    {
        fH_12_ = new TH1F(*est.fH_12_);
        fH_23_ = new TH1F(*est.fH_23_);
        fH_31_ = new TH1F(*est.fH_31_);
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
    if(fH_min_mid_) delete fH_min_mid_;
    if(fH_min_max_) delete fH_min_max_;
    if(fH_mid_max_) delete fH_mid_max_;
    if(fH_12_) delete fH_12_;
    if(fH_23_) delete fH_23_;
    if(fH_31_) delete fH_31_;
    if(fH_en_) delete fH_en_;
    if(fH_p_) delete fH_p_;
    if(fH_phi_) delete fH_phi_;
    if(fH_cosTheta_) delete fH_cosTheta_;
}

///
/// \brief PsDecay::AddEvent Takes information out of Event class instance and fills the histograms.
/// \param event Pointer to Event object.
///
void PsDecay::AddEvent(const Event* event) const
{
    bool thirdGammaExists = false; //check if 3rd pointer !=nullptr
    for(int ii=0; ii<event->GetNumberOfDecayProducts(); ii++)
    {
        if(event->GetFourMomentumOf(ii)!=nullptr)
        {
            //filling histograms for all gammas
            if(ii==2) {thirdGammaExists = true;}
            fH_en_->Fill(event->GetFourMomentumOf(ii)->Energy());
            fH_p_->Fill(event->GetFourMomentumOf(ii)->P());
            fH_phi_->Fill(event->GetFourMomentumOf(ii)->Phi());
            fH_cosTheta_->Fill(event->GetFourMomentumOf(ii)->CosTheta());
        }
    }
    if(fDecayType_==TWO || fDecayType_==TWOandONE || fDecayType_==TWOandN)
    {
        fH_12_->Fill(event->GetFourMomentumOf(0)->Angle((event->GetFourMomentumOf(1))->Vect()), event->GetWeight());
    }

    if((fDecayType_==TWOandONE) & thirdGammaExists)
    {
        fH_23_->Fill(event->GetFourMomentumOf(1)->Angle((event->GetFourMomentumOf(2))->Vect()), event->GetWeight());
        fH_31_->Fill(event->GetFourMomentumOf(2)->Angle((event->GetFourMomentumOf(0))->Vect()), event->GetWeight());
    }
    else if(fDecayType_==THREE)
    {
        double theta12 = event->GetFourMomentumOf(0)->Angle(event->GetFourMomentumOf(1)->Vect());
        double theta23 = event->GetFourMomentumOf(1)->Angle(event->GetFourMomentumOf(2)->Vect());
        double theta31 = event->GetFourMomentumOf(2)->Angle(event->GetFourMomentumOf(0)->Vect());
        fH_12_23_->Fill(theta12, theta23, event->GetWeight());
        fH_12_31_->Fill(theta12, theta31, event->GetWeight());
        fH_23_31_->Fill(theta23, theta31, event->GetWeight());
        //sorting the angles
        double thetas[3] = {theta12, theta23, theta31};
        unsigned indMin = 0;
        unsigned indMid = 0;
        unsigned indMax = 0;
        for(int ii=0; ii<3; ii++)
        {
            indMin = thetas[ii] < thetas[indMin] ? ii : indMin;
            indMax = thetas[ii] > thetas[indMin] ? ii : indMax;
        }
        indMid = indMax==indMin ? indMax : 3-indMax-indMin;
        fH_min_max_->Fill(thetas[indMin], thetas[indMax], event->GetWeight());
        fH_min_mid_->Fill(thetas[indMin], thetas[indMid], event->GetWeight());
        fH_mid_max_->Fill(thetas[indMid], thetas[indMax], event->GetWeight());
   }
}

///
/// \brief PsDecay::DrawHistograms Draws all member field histograms applicable to selected type of decay.
/// \param prefix Prefix for histogram file names.
/// \param output Specifies whether draw histograms as PNG files, or to ROOT file, or both.
///
void PsDecay::DrawHistograms(std::string prefix, OutputOptions output)
{
    std::string outFile1;
    std::string outFile2;
    std::string outFile3;

    if(!fSilentMode_) std::cout<<"[INFO] Drawing histograms for all generated events."<<std::endl;
    TCanvas* angles_all = nullptr;
    TCanvas* angles_sorted_all = nullptr;
    TCanvas* dist_all = new TCanvas((fTypeString_+"-gammas_dist_all").c_str(), \
                                    (std::string("Basic distributions for all generated ")\
                                    +fTypeString_+std::string("-gamma events")).c_str(), 900, 800);
    //drawing histograms for distributions
    dist_all->Divide(2, 2);
    dist_all->cd(1);
    fH_en_->Draw("h");
    dist_all->cd(2);
    fH_p_->Draw();
    dist_all->cd(3);
    fH_phi_->Draw();
    dist_all->cd(4);
    fH_cosTheta_->Draw();
    dist_all->Update();
    outFile1=prefix+fTypeString_+std::string("-gammas_dist_all_.png");

    //angle distribution appearance depends on the number of decay products
    if(fDecayType_==THREE)
    {
        angles_all = new TCanvas((fTypeString_+"-gammas_angles_all").c_str(), \
                                                  (std::string("Angle ditribution for all generated ")\
                                                  +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);
        angles_all->Divide(3,1);
        angles_all->cd(1);
        fH_12_23_->Draw("colz");
        angles_all->cd(2);
        fH_12_31_->Draw("colz");
        angles_all->cd(3);
        fH_23_31_->Draw("colz");
        outFile2 = prefix+std::string("3-gammas_angles_all.png");
        //writing histograms with sorted angles
        angles_sorted_all = new TCanvas((fTypeString_+"-gammas_angles_sorted_all").c_str(), \
                                                  (std::string("Sorted angle ditribution for all generated ")\
                                                  +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);
        angles_sorted_all->Divide(3,1);
        angles_sorted_all->cd(1);
        fH_min_mid_->Draw("colz");
        angles_sorted_all->cd(2);
        fH_mid_max_->Draw("colz");
        angles_sorted_all->cd(3);
        fH_min_max_->Draw("colz");
        outFile3 = prefix+fTypeString_+std::string("-gammas_angles_sorted_all.png");
    }
    else if(fDecayType_==TWO || fDecayType_==TWOandN)
    {
        angles_all = new TCanvas((fTypeString_+"-gammas_angles_all").c_str(), \
                                               (std::string("Angle ditribution for all generated ")\
                                               +fTypeString_+std::string("-gamma events")).c_str(), 500, 400);
        fH_12_->Draw();
        outFile2 = prefix+fTypeString_+std::string("-gammas_angles_all.png");
    }
    else if(fDecayType_==TWOandONE)
    {
        angles_all = new TCanvas((fTypeString_+"-gammas_angles_all").c_str(), \
                                                  (std::string("Angle ditribution for all generated ")\
                                                  +fTypeString_+std::string("-gamma events")).c_str(), 1200, 400);
        angles_all->Divide(3,1);
        angles_all->cd(1);
        fH_12_->Draw();
        angles_all->cd(2);
        fH_23_->Draw();
        angles_all->cd(3);
        fH_31_->Draw();
        outFile2 = prefix+fTypeString_+std::string("-gammas_angles_all.png");
    }

    //writing depends on the 'output' prameter
    if(!fSilentMode_) std::cout<<"[INFO] Saving histograms for all events."<<std::endl;
    if(output==BOTH || output==PNG)
    {
        TImage *img = TImage::Create();
        img->FromPad(dist_all);
        img->WriteImage(outFile1.c_str());
        delete img;
        if(fDecayType_!=ONE)
        {
            angles_all->Update();
            TImage *img2 = TImage::Create();
            img2->FromPad(angles_all);
            img2->WriteImage(outFile2.c_str());
            delete img2;
        }
        if(fDecayType_==THREE)
        {
            TImage *img3 = TImage::Create();
            img3->FromPad(angles_sorted_all);
            img3->WriteImage(outFile3.c_str());
            delete img3;
        }
    }
    if(output==BOTH || output==TREE)
    {
        dist_all->Write();
        if(fDecayType_!=ONE)
        {
            angles_all->Update();
            angles_all->Write();
        }
        if(fDecayType_==THREE)
            angles_sorted_all->Write();
    }

    if(angles_sorted_all) delete angles_sorted_all;
    if(dist_all) delete dist_all;
    if(angles_all) delete angles_all;
}
