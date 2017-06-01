#include "psdecay.h"
//#ifndef __CINT__
//ClassImp(PsDecay);
//#endif
#include <typeinfo>
PsDecay::PsDecay(int noOfGammas)
{
    if(noOfGammas==3)
    {
        fH_12_23_ = new TH2F("fH_12_23_","fH_12_23_", 50,0, 3.15, 50,0,3.15);
        fH_12_23_ -> SetTitle("Rozklad katow 12 vs 23");
        fH_12_23_ -> GetXaxis()->SetTitle("\theta_{12}");
        fH_12_23_ -> GetYaxis()->SetTitle("\theta_{23}");
        fH_12_31_ = new TH2F("fH_12_31_","fH_12_31_", 50,0, 3.15, 50,0,3.15);
        fH_12_31_ -> SetTitle("Rozklad katow 12 vs 31");
        fH_12_31_ -> GetXaxis()->SetTitle("\theta_{12}");
        fH_12_31_ -> GetYaxis()->SetTitle("\theta_{31}");
        fH_23_31_ = new TH2F("fH_23_31_","fH_23_31_", 50,0, 3.15, 50,0,3.15);
        fH_23_31_ -> SetTitle("Rozklad katow 23 vs 31");
        fH_23_31_ -> GetXaxis()->SetTitle("\theta_{23}");
        fH_23_31_ -> GetYaxis()->SetTitle("\theta_{31}");
        fH_12_=nullptr;
    }
    else if(noOfGammas==2)
    {
        fH_12_ = new TH1F("fH_12_", "fH_12_", 50, 0, 3.15);
    }
    else
    {
        throw("Invalin no of decay products!");
    }

    fH_en_ = new TH1F("fH_en_", "fH_en_", 100, 0.0, 0.6);
    fH_en_ -> SetTitle("Rozklad energii");
    fH_en_ -> GetXaxis()->SetTitle("Energy [GeV]");
    fH_en_ -> GetYaxis()->SetTitle("dN/dE");
    fH_p_ = new TH1F("fH_p_", "fH_p_", 100, 0.0, 0.6);
    fH_p_ -> SetTitle("Rozklad pedu");
    fH_p_ -> GetXaxis()->SetTitle("Momentum [GeV/c]");
    fH_p_ -> GetYaxis()->SetTitle("dN/dp");
    fH_phi_ = new TH1F("fH_phi_", "fH_phi_", 100, -3.2, 3.2);
    fH_phi_ -> SetTitle("Rozklad kata azymutalnego");
    fH_phi_ -> GetXaxis()->SetTitle("\phi");
    fH_phi_ -> GetYaxis()->SetTitle("dN/d \phi");
    fH_cosTheta_ = new TH1F("fH_cosTheta_", "fH_cosTheta_", 100, -1.01, 1.01);
    fH_cosTheta_ -> SetTitle("Rozklad kosinusa kata ?");
    fH_cosTheta_ -> GetXaxis()->SetTitle("\theta");
    fH_cosTheta_ -> GetYaxis()->SetTitle("dN/d cos \theta");

    fNoOfDecayProducts_=noOfGammas;
    for(int ii=0; ii<noOfGammas; ii++)
        fMasses_.push_back(0.0);
}


PsDecay::~PsDecay()
{
    if(fH_12_23_)
    {
        delete fH_12_23_;
    }
    if(fH_12_31_) delete fH_12_31_;
    if(fH_23_31_) delete fH_23_31_;
    if(fH_12_) delete fH_12_;
    if(fH_en_) delete fH_en_;
    if(fH_p_) delete fH_p_;
    if(fH_phi_) delete fH_phi_;
    if(fH_cosTheta_) delete fH_cosTheta_;
}


void PsDecay::AddEvent(Double_t weight, TLorentzVector* gamma1,  TLorentzVector* gamma2,  TLorentzVector* gamma3)
{
        if(fNoOfDecayProducts_==2)
        {
            fH_12_->Fill(gamma1->Angle((gamma2)->Vect()), weight);
        }
        else if(fNoOfDecayProducts_==3)
        {
            fH_12_23_->Fill(gamma1->Angle(gamma2->Vect()), gamma2->Angle(gamma3->Vect()), weight);
            fH_12_31_->Fill(gamma1->Angle(gamma2->Vect()), gamma3->Angle(gamma1->Vect()), weight);
            fH_23_31_->Fill(gamma2->Angle(gamma3->Vect()), gamma3->Angle(gamma1->Vect()), weight);
        }
        // CZY TU POWINIENEM UZYWAC WAG???
        fH_en_->Fill(gamma1->Energy());
        fH_p_->Fill(gamma1->P());
        fH_phi_->Fill(gamma1->Phi());
        fH_cosTheta_->Fill(TMath::Cos(gamma1->Theta()));

}

void PsDecay::DrawHistograms(std::string prefix)
{
    TCanvas* c = new TCanvas("c", "Simulation results", 1200, 800);
    c->Divide(fNoOfDecayProducts_+1, 2);
    c->cd(1);
//    std::cout<<"ile gamm="<<fNoOfDecayProducts_<<std::endl;
    std::string outFile;
    if(fNoOfDecayProducts_==3)
    {
        outFile = prefix+std::string("3.png");
        fH_12_23_->Draw("colz");
        c->cd(2);
        fH_12_31_->Draw("colz");
        c->cd(3);
        fH_23_31_->Draw("colz");
    }
    else
    {
        fH_12_->Draw();
        outFile = prefix+std::string("2.png");
    }
    int pad = 1+fNoOfDecayProducts_;
    c->cd(pad);
    fH_en_->Draw();
    c->cd(pad+1);
    fH_p_->Draw();
    c->cd(pad+2);
    fH_phi_->Draw();
    c->cd(pad+3);
    fH_cosTheta_->Draw();
    c->Update();

    std::cout<<"[INFO] Saving canvas to file"<<std::endl;
    TImage *img = TImage::Create();
    img->FromPad(c);
    img->WriteImage(outFile.c_str());
}

