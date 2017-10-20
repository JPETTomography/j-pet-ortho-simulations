#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <TCanvas.h>
#include <TImage.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <TTree.h>
#include <TFile.h>

#include <TF1.h>
#include "MyEventHits.h"

using namespace std;

///
/// \brief drawEdep Draws energy deposition histogram, can be used for both smeared and raw data.
/// \param text Text used as a picture title and file name.
/// \param hEdep Histogram containing data for all types of gamma.
/// \param hEdep511keV Histogram containing data for 511 keV gammas only.
/// \param hEdepPrompt Histogram containing data for Prompt keV gammas only.
/// \param additionalHist Provide this only for o-Ps three gamma decay.
///
void drawEdep(const string text, TH1F* hEdep, TH1F* hEdep511keV, TH1F* hEdepPrompt, TH1F* additionalHist = nullptr)
{

    TCanvas* cEdep = new TCanvas("cEdep", text.c_str(), 600, 400);
    cEdep->Divide(1,1);
    TLegend* legEdep = new TLegend(0.55, 0.4, 0.85, 0.6);
    cEdep->cd(1);

    hEdep->SetLineColor(kBlue);
    hEdep->SetLineWidth(2);
//    legEdep -> AddEntry(hEdep, ("all, N="+to_string((int)hEdep->GetEntries())).c_str());
    hEdep->SetTitle(text.c_str());
    hEdep->GetXaxis()->SetTitle("Edep [MeV]");
//    double scaleFact = 1.0/hEdep->Integral();
//    hEdep->Scale(scaleFact);
    hEdep->GetXaxis()->SetRangeUser(0.06, 2.4);
//    hEdep->Draw();

    if(hEdep511keV)
    {
        hEdep511keV->SetLineColor(kRed);
        hEdep511keV->SetLineWidth(2);
//        hEdep511keV->Scale(hEdepPrompt->GetEntries()/hEdep511keV->GetEntries());
        hEdep511keV->GetXaxis()->SetRangeUser(0.06, 2.4);
        hEdep511keV->Draw("same");
        if(additionalHist)
            legEdep -> AddEntry(hEdep511keV, ("gamma1, N="+to_string((int)hEdep511keV->GetEntries())).c_str());
        else
            legEdep -> AddEntry(hEdep511keV, ("511 keV"));//, N="+to_string((int)hEdep511keV->GetEntries())).c_str());
    }
    if(hEdepPrompt)
    {
        hEdepPrompt->SetLineColor(kGreen+2);
        hEdepPrompt->SetLineWidth(2);
        hEdepPrompt->GetXaxis()->SetRangeUser(0.06, 2.4);
//        hEdepPrompt->Scale(scaleFact);
        hEdepPrompt->Draw("same");
        if(additionalHist)
            legEdep -> AddEntry(hEdepPrompt, ("gamma2, N="+to_string((int)hEdepPrompt->GetEntries())).c_str());
        else
            legEdep -> AddEntry(hEdepPrompt, ("Prompt"));//, N="+to_string((int)hEdepPrompt->GetEntries())).c_str());
    }
    if(additionalHist)
    {
        additionalHist->SetLineColor(kViolet);
        additionalHist->SetLineWidth(2);
        additionalHist->Draw("same");
        legEdep->AddEntry(additionalHist, ("gamma3, N="+to_string((int)additionalHist->GetEntries())).c_str());
    }
    if(hEdep511keV && hEdepPrompt && !additionalHist)
        cout<<setprecision(3)<<"Ratio 2*(Prompt)/(511keV)="<<2*hEdepPrompt->GetEntries()/hEdep511keV->GetEntries()<<endl;

    legEdep->Draw();
    TImage *img = TImage::Create();
    img->FromPad(cEdep);
    img->WriteImage((text+".png").c_str());
    cEdep->Write();
    delete img;
    delete cEdep;
}

void drawEdepSum(const string text, TH1F* h1, TH1F* h2=nullptr)
{
    TCanvas* canvas = new TCanvas("edepSum", "Sum of Edep in each step", 600, 400);
    canvas->cd(1);
    TLegend* legEdepSum = nullptr;
    double scale = h1->GetEntries();
    if(h2)
    {
        h2->SetLineColor(kRed);
        h2->Draw();
        legEdepSum = new TLegend(0.55, 0.6, 0.85, 0.8);
        legEdepSum -> AddEntry(h2, "e+e-");
    }
    h1->SetTitle("Sum of Edep in each step");
    h1->GetXaxis()->SetTitle("E [MeV]");
    h1->GetYaxis()->SetTitle("N");
    if(h2)
    {
        h1->Draw("same");
        legEdepSum->AddEntry(h1, "prompt");
        legEdepSum->Draw();
    }
    else
        h1->Draw();
    TImage *img = TImage::Create();
    img->FromPad(canvas);
    img->WriteImage((text+".png").c_str());
    canvas->Write();
    delete img;
    delete canvas;
    if(legEdepSum) delete legEdepSum;
}

void drawFermiBall(const string text, TH3F* h1, TH2F* h2 = nullptr)
{
    TCanvas* canvas;
    if(h2)
    {
        canvas = new TCanvas("FermiBall", "Edep distribution", 600, 800);
        canvas->Divide(1,2);
    }
    else
    {
         canvas = new TCanvas("FermiBall", "Edep distribution", 600, 400);
    }
    canvas->cd(1);
    if(!h2)
    {
        h1->SetTitle("Edep distribution");
        h1->GetXaxis()->SetTitle("E1 [MeV]");
        h1->GetYaxis()->SetTitle("E2 [MeV]");
        h1->GetZaxis()->SetTitle("E3 [MeV]");
        gPad->SetTheta(45);
        gPad->SetPhi(210);
        h1->Draw("iso");
//        std::cout<<"correlation factor 12 ="<<h1->GetCorrelationFactor(1,2)<<std::endl;
//        std::cout<<"correlation factor 23 ="<<h1->GetCorrelationFactor(2,3)<<std::endl;
//        std::cout<<"correlation factor 13 ="<<h1->GetCorrelationFactor(1,3)<<std::endl;
    }
    else
    {
        h2->SetTitle("Edep distribution");
        h2->GetXaxis()->SetTitle("E1 [MeV]");
        h2->GetYaxis()->SetTitle("E2 [MeV]");
        h2->Draw("colz");
        std::cout<<"correlation factor 12 ="<<h2->GetCorrelationFactor(1,2)<<std::endl;
        canvas->cd(2);
        h2->ProjectionX()->Draw();
        TH1D* hist = h2->ProjectionY();
        hist->SetLineColor(kRed);
        hist->Draw("same");

    }
    TImage *img = TImage::Create();
    img->FromPad(canvas);
    img->WriteImage((text+".png").c_str());
    canvas->Write();
    delete img;
    delete canvas;
}

///
/// \brief drawEfficiency Draws efficiency histogram, can be used for both smeared and raw data.
/// \param text Text used as a picture title and file name.
/// \param hEdep Histogram containing data for all types of gamma.
/// \param hEdep511keV Histogram containing data for 511 keV gammas only.
/// \param hEdepPrompt Histogram containing data for Prompt keV gammas only.
///
void drawEfficiency(const string text, const TH1F* hEdep, const TH1F* hEdep511keV, const TH1F* hEdepPrompt)
{

    TCanvas* cEfficiency = new TCanvas("cEfficiency", "(1-) Efficiency", 600, 400);
    cEfficiency->cd(1);
    TLegend* legEff = new TLegend(0.55, 0.4, 0.90, 0.6);

    int NBins = hEdep->GetXaxis()->GetNbins();

    //creating efficiency histograms
    double EMax = hEdep->GetXaxis()->GetBinUpEdge(hEdep->GetXaxis()->GetLast());
    TH1F *hEff511keV = new TH1F("hEff511keV", text.c_str(), NBins, 0.0, EMax);
    hEff511keV->GetXaxis()->SetTitle("Treshold energy [MeV]");
    hEff511keV->GetYaxis()->SetTitle("Ratio");
    hEff511keV->SetLineColor(kBlue);
    hEff511keV->SetLineWidth(2);
    TH1F *hEffPrompt = new TH1F("hEffPrompt", "hEffPrompt" , NBins, 0.0, EMax);
    hEffPrompt->SetLineColor(kRed);
    hEffPrompt->SetLineWidth(2);
    //adding entries to the histogram
    legEff -> AddEntry(hEff511keV, "eff (511 keV)");
    legEff -> AddEntry(hEffPrompt, "1 - eff (Prompt keV)");
    legEff -> SetTextSize(0.05);

    for(int ii=1; ii<=NBins; ii++)
    {
        hEff511keV->SetBinContent(ii, hEdep511keV->Integral(1, ii));
        //below 1-eff histogram is filled
        hEffPrompt->SetBinContent(ii, hEdepPrompt->Integral(1, NBins)-hEdepPrompt->Integral(1, ii));
    }
    hEff511keV->Scale(1.0/hEdep511keV->Integral(1, NBins));
    hEff511keV->Draw();
    if(hEdepPrompt)
    {
        hEffPrompt->Scale(1.0/hEdepPrompt->Integral(1, NBins));
        hEffPrompt->Draw("same");
    }

    legEff->Draw();
    TImage *img = TImage::Create();
    img->FromPad(cEfficiency);
    img->WriteImage((text+".png").c_str());
    cEfficiency->Write();
    delete img;
    delete legEff;
    delete hEff511keV;
    delete hEffPrompt;
    delete cEfficiency;
}

///
/// \brief drawPurity Draws purity histogram, can be used for both smeared and raw data.
/// \param text Text used as a picture title and file name.
/// \param hEdep Histogram containing data for all types of gamma.
/// \param hEdep511keV Histogram containing data for 511 keV gammas only.
/// \param hEdepPrompt Histogram containing data for Prompt keV gammas only.
///
void drawPurity(const string text, const TH1F* hEdep, const TH1F* hEdep511keV, const TH1F* hEdepPrompt)
{

    TCanvas* cPurity = new TCanvas("cPurity", "(1-) Purity", 600, 400);
    cPurity->cd(1);
    TLegend* legPur = new TLegend(0.50, 0.15, 0.9, 0.35);

    int NBins = hEdep->GetXaxis()->GetNbins();
    //creating purity histograms
    double EMax = hEdep->GetXaxis()->GetBinUpEdge(hEdep->GetXaxis()->GetLast());
    TH1F *hPur511keV = new TH1F("hPur511keV","hPur511keV" , NBins, 0.0, EMax);
    hPur511keV->GetXaxis()->SetTitle("Treshold energy [MeV]");
    hPur511keV->GetYaxis()->SetTitle("Ratio");
    hPur511keV->SetLineColor(kBlue);
    hPur511keV->SetLineWidth(2);
    TH1F *hPurPrompt = new TH1F("hPurPrompt", text.c_str(), NBins, 0.0, EMax);
    hPurPrompt->SetLineColor(kRed);
    hPurPrompt->SetLineWidth(2);
    //adding entries to the histogram
    legPur -> AddEntry(hPur511keV, "pur left (511 keV)");
    legPur -> AddEntry(hPurPrompt, "pur right (Prompt keV)");
    legPur -> SetTextSize(0.05);

    for(int ii=1; ii<=NBins; ii++)
    {
        if(hEdep->Integral(1, ii) == 0) // if integral in the denominator == 0 then set bin to 1
            hPur511keV->SetBinContent(ii, 1);
        else
            hPur511keV->SetBinContent(ii, hEdep511keV->Integral(1, ii) / (double)(hEdep->Integral(1, ii)));
        if(hEdep->Integral(ii, NBins) == 0)// if integral in the denominator == 0 then set bin to 1
            hPurPrompt->SetBinContent(ii, 1);
        else
            hPurPrompt->SetBinContent(ii, hEdepPrompt->Integral(ii, NBins) / (double)(hEdep->Integral(ii, NBins)));
    }
    hPurPrompt->Draw();
    hPur511keV->Draw("same");

    legPur->Draw();
    TImage *img = TImage::Create();
    img->FromPad(cPurity);
    img->WriteImage((text+".png").c_str());
    cPurity -> Write();
    delete img;
    delete legPur;
    delete hPur511keV;
    delete hPurPrompt;
    delete cPurity;
}

///
/// \brief drawCutPassing Draws normalized fCutPassing_ histogram. For debugging purpose.
/// \param hCutPassing Histogram to be plotted.
///
void drawCutPassing(TH1F* hCutPassing)
{
    TCanvas* c = new TCanvas("c", "c", 600, 400);
    c->cd(1);
    hCutPassing->Scale(1.0/hCutPassing->GetBinContent(2));
    hCutPassing->Draw("hist");
    TImage *img = TImage::Create();
    img->FromPad(c);
    img->WriteImage("cut_pass.png");
    delete img;
    delete c;
}


void calculatePurityEfficiency(const TH1F* hEdep, const TH1F* hEdep511keV, const TH1F* hEdepPrompt,  double Etreshold = 0.4)
{
    int n =hEdep->GetXaxis()->GetNbins();
    int n511 =hEdep511keV->GetXaxis()->GetNbins();
    int nPrompt =hEdepPrompt->GetXaxis()->GetNbins();
    int index = 0;
    int index511 = 0;
    int indexPrompt = 0;
    for(int ii=0; ii<n; ii++)
    {
        if(hEdep->GetBinLowEdge(ii+1)+TMath::Power(10, -8)>Etreshold)
        {
            std::cout<<hEdep->GetBinLowEdge(ii)<<" < " <<Etreshold<<" < "<<hEdep->GetBinLowEdge(ii+1)<<std::endl;
            index = ii;
            std::cout<<ii<<std::endl;
            break;
        }
    }
    if(n==n511 && n==nPrompt)
    {
        index511=index;
        indexPrompt=index;
    }
    else
    {
        std::cout<<"511 gammas:"<<std::endl;
        for(int ii=0; ii<n511; ii++)
        {
            if(hEdep511keV->GetBinLowEdge(ii+1)>=Etreshold)
            {
                std::cout<<hEdep511keV->GetBinLowEdge(ii)<<" < " <<Etreshold<<" < "<<hEdep511keV->GetBinLowEdge(ii+1)<<std::endl;
                index511 = ii;
                std::cout<<ii<<std::endl;
                break;
            }
        }
        std::cout<<"Prompt gammas:"<<std::endl;
        for(int ii=0; ii<nPrompt; ii++)
        {
            if(hEdepPrompt->GetBinLowEdge(ii+1)>=Etreshold)
            {
                std::cout<<hEdepPrompt->GetBinLowEdge(ii)<<" < " <<Etreshold<<" < "<<hEdepPrompt->GetBinLowEdge(ii+1)<<std::endl;
                indexPrompt = ii;
                std::cout<<ii<<std::endl;
                break;
            }
        }

    }
    std::cout<<"Results:"<<std::endl;
    double eff511 = hEdep511keV->Integral(1, index511) / hEdep511keV->Integral(1, n511);
//    std::cout<<hEdep511keV->Integral(1, hEdep511keV->GetXaxis()->GetNbins())<<" "<<hEdep511keV->Integral();
    double purity511 = hEdep511keV->Integral(1, index511) / (hEdep->Integral(1, index));
    double purityPrompt = hEdepPrompt->Integral(indexPrompt, nPrompt) / (hEdep->Integral(index, n));
    double effPrompt =  hEdepPrompt->Integral(indexPrompt, nPrompt) / hEdepPrompt->Integral(1, nPrompt);
    std::cout<<"511keV eff="<<eff511*100<<" %"<<std::endl;
    std::cout<<"511keV purity="<<purity511*100<<" %"<<std::endl;
    std::cout<<"Prompt eff="<<effPrompt*100<<" %"<<std::endl;
    std::cout<<"Prompt purity="<<purityPrompt*100<<" %"<<std::endl;

}

///
/// \brief main Main function of the program, used to call drawing functions.
/// \param argc
/// \param argv
/// \return
///
int main (int argc, char* argv[])
{
  MyEventHits eh;
  eh.Loop();
  TFile* file = new TFile("hist.root", "recreate");
  file -> cd();

  double Etreshold = 0.330; //MeV
  for(int ii=0; ii <=20; ii++)
  {
      std::cout<<"NO SMEARING"<<std::endl;
      calculatePurityEfficiency(eh.hRootEdep, eh.hRootEdep511keV, eh.hRootEdepPrompt, Etreshold+ii*0.005);
      std::cout<<"WITH SMEARING"<<std::endl;
      calculatePurityEfficiency(eh.hRootEdepSmear, eh.hRootEdepSmear511keV, eh.hRootEdepSmearPrompt, Etreshold+ii*0.005);
      std::cout<<std::endl;
  }
  // Drawing
  drawEdep("E_dep", eh.hRootEdep, eh.hRootEdep511keV, eh.hRootEdepPrompt);
  drawEdep("E_dep_with_smear", eh.hRootEdepSmear, eh.hRootEdepSmear511keV, eh.hRootEdepSmearPrompt);
  drawEfficiency("eff", eh.hRootEdep, eh.hRootEdep511keV, eh.hRootEdepPrompt);
  drawEfficiency("eff_with_smear", eh.hRootEdepSmear, eh.hRootEdepSmear511keV, eh.hRootEdepSmearPrompt);
  drawPurity("pur", eh.hRootEdep, eh.hRootEdep511keV, eh.hRootEdepPrompt);
  drawPurity("pur_with_smear", eh.hRootEdepSmear, eh.hRootEdepSmear511keV, eh.hRootEdepSmearPrompt);
  drawCutPassing(eh.hCutPassing);
  drawEdepSum("edepSum", eh.hRootEdepSum, eh.hRootEdepSum511kev);
  drawEdepSum("edepSumSmear", eh.hRootEdepSumSmear, eh.hRootEdepSumSmear511kev);
  drawFermiBall("edepBall",  eh.hFermiBall);//, eh.hFermiCircle);
  drawFermiBall("edepBallSmear", eh.hFermiBall);//, eh.hFermiCircleSmear);

  //Saving basic hostograms
//  eh.hRootEdep -> Write();
//  eh.hCutPassing -> Write();
//  eh.hFermiBall->Write();
//  eh.hFermiBallSmear->Write();
//  eh.hFermiCircle->Write();
//  eh.hFermiCircleSmear->Write();
//  eh.hRootEdep511keV->Write();
//  eh.hRootEdep511keVOne->Write();
//  eh.hRootEdep511keVTwo->Write();
//  eh.hRootEdepPrompt->Write();
//  eh.hRootEdepSmearPrompt->Write();
//  eh.hRootEdepSmear->Write();
//  eh.hRootEdepSmear511keV->Write();
//  eh.hRootEdepSmear511keVOne->Write();
//  eh.hRootEdepSmear511keVTwo->Write();
//  eh.hRootEdepSum->Write();
//  eh.hRootEdepSum511kev->Write();
//  eh.hRootEdepSumSmear511kev->Write();

//  drawEdep("E_dep", eh.hRootEdep, eh.hRootEdep511keVOne, eh.hRootEdep511keVTwo, eh.hRootEdepPrompt);
//  drawEdep("E_dep_with_smear", eh.hRootEdepSmear, eh.hRootEdepSmear511keVOne, eh.hRootEdepSmear511keVTwo,eh.hRootEdepSmearPrompt);
//  drawEfficiency("eff", eh.hRootEdep, eh.hRootEdep511keVOne, eh.hRootEdep511keVTwo, eh.hRootEdepPrompt);
//  drawEfficiency("eff_with_smear", eh.hRootEdepSmear, eh.hRootEdepSmear511keVOne, eh.hRootEdepSmear511keVTwo,eh.hRootEdepSmearPrompt);
//  drawPurity("pur", eh.hRootEdep, eh.hRootEdep511keVOne, eh.hRootEdep511keVTwo, eh.hRootEdepPrompt);
//  drawPurity("pur_with_smear", eh.hRootEdepSmear, eh.hRootEdepSmear511keVOne, eh.hRootEdepSmear511keVTwo,eh.hRootEdepSmearPrompt);
//  drawCutPassing(eh.hCutPassing);

  //saving to .root file
  file->Write();
  file->Close();
  delete file;
  return 0;
}
