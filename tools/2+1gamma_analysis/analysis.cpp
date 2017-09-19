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

#include <TF1.h>
#include "MyEventHits.h"

using namespace std;

///
/// \brief drawEdep Draws energy deposition histogram, can be used for both smeared and raw data.
/// \param text Text used as a picture title and file name.
/// \param hEdep Histogram containing data for all types of gamma.
/// \param hEdep511keV Histogram containing data for 511 keV gammas only.
/// \param hEdep1157keV Histogram containing data for 1157 keV gammas only.
///
void drawEdep(const string text, TH1F* hEdep, TH1F* hEdep511keV, TH1F* hEdep1157keV)
{

    TCanvas* cEdep = new TCanvas("cEdep", text.c_str(), 600, 400);
    cEdep->Divide(1,1);
    TLegend* legEdep = new TLegend(0.55, 0.4, 0.85, 0.7);
    cEdep->cd(1);

    hEdep->SetLineColor(kBlue);
    hEdep->SetLineWidth(2);
    legEdep -> AddEntry(hEdep, ("all, N="+to_string((int)hEdep->GetEntries())).c_str());
    hEdep->SetTitle(text.c_str());
    hEdep->Draw();

    if(hEdep511keV)
    {
        hEdep511keV->SetLineColor(kRed);
        hEdep511keV->SetLineWidth(2);
        hEdep511keV->Draw("same");
        legEdep -> AddEntry(hEdep511keV, ("511 keV, N="+to_string((int)hEdep511keV->GetEntries())).c_str());
    }
    if(hEdep1157keV)
    {
        hEdep1157keV->SetLineColor(kGreen+2);
        hEdep1157keV->SetLineWidth(2);
        hEdep1157keV->Draw("same");
        legEdep -> AddEntry(hEdep1157keV, ("1157 keV, N="+to_string((int)hEdep1157keV->GetEntries())).c_str());
    }
    if(hEdep511keV && hEdep1157keV)
        cout<<setprecision(2)<<"Ratio 2*(1157keV)/(511keV)="<<2*hEdep1157keV->GetEntries()/hEdep511keV->GetEntries()<<endl;

    legEdep->Draw();
    TImage *img = TImage::Create();
    img->FromPad(cEdep);
    img->WriteImage((text+".png").c_str());
    delete img;
    delete cEdep;
}

///
/// \brief drawEfficiency Draws efficiency histogram, can be used for both smeared and raw data.
/// \param text Text used as a picture title and file name.
/// \param hEdep Histogram containing data for all types of gamma.
/// \param hEdep511keV Histogram containing data for 511 keV gammas only.
/// \param hEdep1157keV Histogram containing data for 1157 keV gammas only.
///
void drawEfficiency(const string text, const TH1F* hEdep, const TH1F* hEdep511keV, const TH1F* hEdep1157keV)
{

    TCanvas* cEfficiency = new TCanvas("cEfficiency", "(1-) Efficiency", 600, 400);
    cEfficiency->cd(1);
    TLegend* legEff = new TLegend(0.60, 0.4, 0.90, 0.6);

    int NBins = 200;

    //creating efficiency histograms
    double EMax = hEdep->GetXaxis()->GetBinUpEdge(hEdep->GetXaxis()->GetLast());
    TH1F *hEff511keV = new TH1F("hEff511keV", text.c_str(), NBins, 0.0, EMax);
    hEff511keV->GetXaxis()->SetTitle("Treshold energy [MeV]");
    hEff511keV->GetYaxis()->SetTitle("Ratio");
    hEff511keV->SetLineColor(kBlue);
    hEff511keV->SetLineWidth(2);
    TH1F *hEff1157keV = new TH1F("hEff1157keV", "hEff1157keV" , NBins, 0.0, EMax);
    hEff1157keV->SetLineColor(kRed);
    hEff1157keV->SetLineWidth(2);
    //adding entries to the histogram
    legEff -> AddEntry(hEff511keV, "eff (511 keV)");
    legEff -> AddEntry(hEff1157keV, "1 - eff (1157 keV)");
    legEff -> SetTextSize(0.05);

    for(int ii=1; ii<NBins+1; ii++)
    {
        hEff511keV->SetBinContent(ii, hEdep511keV->Integral(0, ii));
        //below 1-eff histogram is filled
        hEff1157keV->SetBinContent(ii, hEdep1157keV->Integral()-hEdep1157keV->Integral(0, ii));
    }
    hEff511keV->Scale(1.0/hEdep511keV->Integral());
    hEff1157keV->Scale(1.0/hEdep1157keV->Integral());

    hEff511keV->Draw();
    hEff1157keV->Draw("same");

    legEff->Draw();
    TImage *img = TImage::Create();
    img->FromPad(cEfficiency);
    img->WriteImage((text+".png").c_str());
    delete img;
    delete legEff;
    delete hEff511keV;
    delete hEff1157keV;
    delete cEfficiency;
}

///
/// \brief drawPurity Draws purity histogram, can be used for both smeared and raw data.
/// \param text Text used as a picture title and file name.
/// \param hEdep Histogram containing data for all types of gamma.
/// \param hEdep511keV Histogram containing data for 511 keV gammas only.
/// \param hEdep1157keV Histogram containing data for 1157 keV gammas only.
///
void drawPurity(const string text, const TH1F* hEdep, const TH1F* hEdep511keV, const TH1F* hEdep1157keV)
{

    TCanvas* cPurity = new TCanvas("cPurity", "(1-) Purity", 600, 400);
    cPurity->cd(1);
    TLegend* legPur = new TLegend(0.50, 0.15, 0.9, 0.35);

    int NBins = 200;

    //creating purity histograms
    double EMax = hEdep->GetXaxis()->GetBinUpEdge(hEdep->GetXaxis()->GetLast());
    TH1F *hPur511keV = new TH1F("hPur511keV","hPur511keV" , NBins, 0.0, EMax);
    hPur511keV->GetXaxis()->SetTitle("Treshold energy [MeV]");
    hPur511keV->GetYaxis()->SetTitle("Ratio");
    hPur511keV->SetLineColor(kBlue);
    hPur511keV->SetLineWidth(2);
    TH1F *hPur1157keV = new TH1F("hPur1157keV", text.c_str(), NBins, 0.0, EMax);
    hPur1157keV->SetLineColor(kRed);
    hPur1157keV->SetLineWidth(2);
    //adding entries to the histogram
    legPur -> AddEntry(hPur511keV, "pur left (511 keV)");
    legPur -> AddEntry(hPur1157keV, "pur right (1157 keV)");
    legPur -> SetTextSize(0.05);

    for(int ii=1; ii<NBins+1; ii++)
    {
        if(hEdep->Integral(0, ii) == 0) // if integral in the denominator == 0 then set bin to 1
            hPur511keV->SetBinContent(ii, 1);
        else
            hPur511keV->SetBinContent(ii, hEdep511keV->Integral(0, ii) / (double)(hEdep->Integral(0, ii)));
        if(hEdep->Integral(ii, NBins) == 0)// if integral in the denominator == 0 then set bin to 1
            hPur1157keV->SetBinContent(ii, 1);
        else
            hPur1157keV->SetBinContent(ii, hEdep1157keV->Integral(ii, NBins) / (double)(hEdep->Integral(ii, NBins)));
    }
    hPur1157keV->Draw();
    hPur511keV->Draw("same");

    legPur->Draw();
    TImage *img = TImage::Create();
    img->FromPad(cPurity);
    img->WriteImage((text+".png").c_str());
    delete img;
    delete legPur;
    delete hPur511keV;
    delete hPur1157keV;
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
  // Drawing
  drawEdep("E_dep", eh.hRootEdep, eh.hRootEdep511keV, eh.hRootEdep1157keV);
  drawEdep("E_dep_with_smear", eh.hRootEdepSmear, eh.hRootEdepSmear511keV, eh.hRootEdepSmear1157keV);
  drawEfficiency("eff", eh.hRootEdep, eh.hRootEdep511keV, eh.hRootEdep1157keV);
  drawEfficiency("eff_with_smear", eh.hRootEdepSmear, eh.hRootEdepSmear511keV, eh.hRootEdepSmear1157keV);
  drawPurity("pur", eh.hRootEdep, eh.hRootEdep511keV, eh.hRootEdep1157keV);
  drawPurity("pur_with_smear", eh.hRootEdepSmear, eh.hRootEdepSmear511keV, eh.hRootEdepSmear1157keV);
  drawCutPassing(eh.hCutPassing);

  //  cout<<"GATE entries="<<h.hPos->GetEntries()<<" ROOT entriess="<<eh.hRootPos->GetEntries()<<endl;
  return 0;
}
