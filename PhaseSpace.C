void                    PhaseSpace()
{

   if (!gROOT->GetClass("TGenPhaseSpace")) gSystem.Load("libPhysics");

    TLorentzVector Ps(0.0, 0.0, 0.0001, 1.022); //zrodlo
   //(Momentum, Energy units are Gev/C, GeV)
    Double_t masses[3] = {0.0, 0.0, 0.0};

   TGenPhaseSpace event;
   event.SetDecay(Ps, 3, masses);

   // Tworzenie histogramow
   TH2F *h_12_23 = new TH2F("h_12_23","h_12_23", 50,0, 3.15, 50,0,3.15);
   h_12_23 -> SetTitle("Rozklad katow 12 vs 23");
   h_12_23 -> GetXaxis()->SetTitle("\theta_{12}");
   h_12_23 -> GetYaxis()->SetTitle("\theta_{23}");
   TH2F *h_12_31 = new TH2F("h_12_31","h_12_31", 50,0, 3.15, 50,0,3.15);
   h_12_31 -> SetTitle("Rozklad katow 12 vs 31");
   h_12_31 -> GetXaxis()->SetTitle("\theta_{12}");
   h_12_31 -> GetYaxis()->SetTitle("\theta_{31}");
   TH2F *h_23_31 = new TH2F("h_23_31","h_23_31", 50,0, 3.15, 50,0,3.15);
   h_23_31 -> SetTitle("Rozklad katow 23 vs 31");
   h_23_31 -> GetXaxis()->SetTitle("\theta_{23}");
   h_23_31 -> GetYaxis()->SetTitle("\theta_{31}");
   TH1F *h_en = new TH1F("h_en", "h_en", 100, 0.0, 1.03);
   h_en -> SetTitle("Rozklad energii");
   h_en -> GetXaxis()->SetTitle("Energy [GeV]");
   h_en -> GetYaxis()->SetTitle("dN/dE");
   TH1F *h_p = new TH1F("h_p", "h_p", 100, 0.0, 1);
   h_p -> SetTitle("Rozklad pedu");
   h_p -> GetXaxis()->SetTitle("Momentum [GeV/c]");
   h_p -> GetYaxis()->SetTitle("dN/dp");
   TH1F *h_phi = new TH1F("h_phi", "h_phi", 100, -3.2, 3.2);
   h_phi -> SetTitle("Rozklad kata azymutalnego");
   h_phi -> GetXaxis()->SetTitle("\phi");
   h_phi -> GetYaxis()->SetTitle("dN/d \phi");

   // Generacja eventow
   for (Int_t n=0;n<1000000;n++)
   {
      Double_t weight = event.Generate();

      TLorentzVector *fgamma1 = event.GetDecay(0);
      TLorentzVector *fgamma2 = event.GetDecay(1);
      TLorentzVector *fgamma3 = event.GetDecay(2);
//      TLorentzVector *fgammas = &(*fgamma1+*fgamma2+*fgamma3);

      h_12_23->Fill(fgamma1->Angle(fgamma2->Vect()), fgamma2->Angle(fgamma3->Vect()), weight);
      h_12_31->Fill(fgamma1->Angle(fgamma2->Vect()), fgamma3->Angle(fgamma1->Vect()), weight);
      h_23_31->Fill(fgamma2->Angle(fgamma3->Vect()), fgamma3->Angle(fgamma1->Vect()), weight);
      h_en->Fill(fgamma1->Energy());
      h_p->Fill(fgamma1->P());
      h_phi->Fill(fgamma1->Phi());

   }

   // Rysowanie
   TCanvas *canvas = new TCanvas("canvas", "canvas", 1200, 800);
   canvas -> Divide(3,2);
   canvas->cd(1);
   h_12_23->Draw("colz");
   canvas->cd(2);
   h_12_31->Draw("colz");
   canvas->cd(3);
   h_23_31->Draw("colz");
   canvas->cd(4);
   h_en->Draw();
   canvas->cd(5);
   h_p->Draw();
   canvas->cd(6);
   h_phi->Draw();
}
