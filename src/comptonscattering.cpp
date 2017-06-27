#include "comptonscattering.h"
#include "TImage.h"
#include "TRandom3.h"
#include "TCanvas.h"

///
/// \brief ComptonScattering::ComptonScattering Basic constructor.
/// \param prefix File prefix.
/// \param noOfGammas No of gammas (only for file naming).
///
ComptonScattering::ComptonScattering(int noOfGammas) : fNoOfGammas_(noOfGammas)
{
    fH_electron_E_ = new TH1F("fH_electron_E_", "fH_electron_E_", 100, 0.0, 0.511);
    fH_electron_E_->SetTitle("Electrons' energy distribution");
    fH_electron_E_->GetXaxis()->SetTitle("E [MeV]");
    fH_electron_E_->GetYaxis()->SetTitle("dN/dE");
    fH_electron_E_->GetYaxis()->SetTitleOffset(1.8);
    
    if(noOfGammas==3) fH_photon_E_depos_ = new TH1F("fH_photon_E_depos_", "fH_photon_E_depos_", 100, 0.0, 0.511);
    else fH_photon_E_depos_ = new TH1F("fH_photon_E_depos_", "fH_photon_E_depos_", 20, 0.510, 0.512);
    fH_photon_E_depos_->SetTitle("Incident photon energy deposition");
    fH_photon_E_depos_->GetXaxis()->SetTitle("E [MeV]");
    fH_photon_E_depos_->GetYaxis()->SetTitle("dN/dE");
    fH_photon_E_depos_->GetYaxis()->SetTitleOffset(1.8);
    
    fH_photon_theta_ = new TH1F("fH_photon_theta_", "fH_photon_theta_", 100, 0.0, TMath::Pi());
    fH_photon_theta_->SetTitle("Scattering angle distribution");
    fH_photon_theta_->GetXaxis()->SetTitle("#theta");
    fH_photon_theta_->GetYaxis()->SetTitle("dN/d#theta");
    fH_photon_theta_->GetYaxis()->SetTitleOffset(1.8);

    fH_PDF_ = new TH2D("fH_PDF_", "fH_PDF_", 1000, 0.0, 1.022, 1000, 0.0, TMath::Pi());
    fH_PDF_->SetTitle("Klein-Nishima function");
    fH_PDF_->GetXaxis()->SetTitle("E [MeV]");
    fH_PDF_->GetYaxis()->SetTitle("#theta'");
    fH_PDF_->SetStats(kFALSE);
    fH_PDF_cross = new TH1D("fH_PDF_cross_", "fH_PDF_cross", 1000, 0.0, TMath::Pi());
    fH_PDF_cross->GetYaxis()->SetTitle("d N/ d #Omega");
    fH_PDF_cross->GetXaxis()->SetTitle("#theta'");
    fH_PDF_cross->SetStats(kFALSE);

    fH_PDF_Theta_ = new TH2D("fH_PDF_Theta_", "fH_PDF_Theta_", 1000, 0.0, 1.022, 1000, 0.0, TMath::Pi());
    fH_PDF_Theta_->SetTitle("Klein-Nishima function * 2*#pi*sin(#theta)");
    fH_PDF_Theta_->GetXaxis()->SetTitle("E [MeV]");
    fH_PDF_Theta_->GetYaxis()->SetTitle("#theta'");
    fH_PDF_Theta_->SetStats(kFALSE);
    fH_PDF_Theta_cross = new TH1D("fH_PDF_Theta_cross_", "fH_PDF_Theta_cross_", 1000, 0.0, TMath::Pi());
    fH_PDF_Theta_cross->GetYaxis()->SetTitle("d #N/ d #theta");
    fH_PDF_Theta_cross->GetXaxis()->SetTitle("#theta'");
    fH_PDF_Theta_cross->SetStats(kFALSE);
    //creating function wrapper around KleinNishina_ function
    fPDF = new TF1("KleinNishima_", KleinNishina_, 0.0 , TMath::Pi(), 1);
    fPDF_Theta = new TF1("KleinNishimaTheta_", KleinNishinaTheta_, 0.0 , TMath::Pi(), 1);
}

///
/// \brief ComptonScattering::ComptonScattering Copy constructor.
/// \param est ComptonScattering instance to be copied.
///
ComptonScattering::ComptonScattering(const ComptonScattering &est)
{
    fNoOfGammas_=est.fNoOfGammas_;
    fPDF = new TF1(*est.fPDF);  //special root object
    fPDF_Theta = new TF1(*est.fPDF_Theta);
    fH_photon_E_depos_=new TH1F(*est.fH_photon_E_depos_); //distribution of energy deposited by incident photons
    fH_electron_E_ = new TH1F(*est.fH_electron_E_);   //energy distribution for electrons
    fH_photon_theta_ = new TH1F(*est.fH_photon_theta_);   //angle distribution for electrons
    fH_PDF_ = new TH2D(*est.fH_PDF_);  // Klein-Nishina function plot, for testing purpose only
    fH_PDF_cross = new TH1D(*est.fH_PDF_cross);
    fH_PDF_Theta_ = new TH2D(*est.fH_PDF_Theta_);
    fH_PDF_Theta_cross = new TH1D(*est.fH_PDF_Theta_cross);
}

///
/// \brief ComptonScattering::operator = Copies histograms and field values.
/// \param est ComptonScattering instance to be copied.
/// \return Reference to ComptonScattering instance with copied fields.
///
ComptonScattering& ComptonScattering::operator=(const ComptonScattering &est)
{
    fNoOfGammas_=est.fNoOfGammas_;
    fPDF = new TF1(*est.fPDF);  //special root object
    fPDF_Theta = new TF1(*est.fPDF_Theta);
    fH_photon_E_depos_=new TH1F(*est.fH_photon_E_depos_); //distribution of energy deposited by incident photons
    fH_electron_E_ = new TH1F(*est.fH_electron_E_);   //energy distribution for electrons
    fH_photon_theta_ = new TH1F(*est.fH_photon_theta_);   //angle distribution for electrons
    fH_PDF_ = new TH2D(*est.fH_PDF_);  // Klein-Nishina function plot, for testing purpose only
    fH_PDF_cross = new TH1D(*est.fH_PDF_cross);
    fH_PDF_Theta_ = new TH2D(*est.fH_PDF_Theta_);
    fH_PDF_Theta_cross = new TH1D(*est.fH_PDF_Theta_cross);
    return *this;
}

///
/// \brief ComptonScattering::~ComptonScattering Destructor, releases memory after histograms.
///
ComptonScattering::~ComptonScattering()
{
    if(fH_electron_E_) delete fH_electron_E_;
    if(fH_photon_theta_) delete fH_photon_theta_;
    if(fH_photon_E_depos_) delete fH_photon_E_depos_;
    if(fH_PDF_) delete fH_PDF_;
    if(fH_PDF_cross) delete fH_PDF_cross;
    if(fPDF) delete fPDF;
    if(fH_PDF_Theta_) delete fH_PDF_Theta_;
    if(fH_PDF_Theta_cross) delete fH_PDF_Theta_cross;
    if(fPDF_Theta) delete fPDF_Theta;
}

///
/// \brief ComptonScattering::DrawPDF Draws Klein-Nishina function and saves to a file.
/// \param filePrefix Prefix of the output file, may contain path.
///
void ComptonScattering::DrawPDF(std::string filePrefix, double crossSectionE)
{
    std::cout<<"\n[INFO] Drawing Klein-Nishima function."<<std::endl;
    int range = 1000;
    //Two loops create a grid, where the value of function is calculated.
    double crossE[1] = {crossSectionE};
    for(int xx=0; xx<range; xx++)
    {
        double E[1] = {xx/((float)range)*1.022};
        for(int yy=0; yy<range; yy++)
        {
            double theta[1] = {yy/((float)range)*TMath::Pi()};
            long double valPDF = fPDF->EvalPar(theta, E);
            long double valPDFTheta = fPDF_Theta->EvalPar(theta, E);
            fH_PDF_->SetBinContent(xx, yy, valPDF);
            fH_PDF_Theta_->SetBinContent(xx, yy, valPDFTheta);
            long double valCross = fPDF->EvalPar(theta, crossE);
            fH_PDF_cross->SetBinContent(yy, valCross);
            long double valCrossTheta = fPDF_Theta->EvalPar(theta, crossE);
            fH_PDF_Theta_cross->SetBinContent(yy, valCrossTheta);
        }

    }
    //drawing and saving to file
    TCanvas* c = new TCanvas("c", "c", 1300, 380);
    c->Divide(2);
    c->cd(1);
    fH_PDF_->Draw("colz");
    c->cd(2);
    fH_PDF_cross->SetTitle((std::string("Klein-Nishima function for ")+std::to_string(crossSectionE)+std::string(" [MeV]")).c_str());
    fH_PDF_cross->Draw();
    TImage *img = TImage::Create();
    img->FromPad(c);
    img->WriteImage((filePrefix+std::string("Klein-Nishina.png")).c_str());
    //drawing dN/d theta
    TCanvas* c2 = new TCanvas("c2", "c2", 1300, 380);
    c2->Divide(2);
    c2->cd(1);
    fH_PDF_Theta_->Draw("colz");
    c2->cd(2);
    fH_PDF_Theta_cross->SetTitle((std::string("Scattering angle distribution for ")+std::to_string(crossSectionE)+std::string(" [MeV]")).c_str());
    fH_PDF_Theta_cross->Draw();
    TImage *img2 = TImage::Create();
    img2->FromPad(c2);
    img2->WriteImage((filePrefix+std::string("Klein-Nishina_Theta.png")).c_str());

    std::cout<<"[INFO] Klein-Nishina function saved to file.\n"<<std::endl;
    delete c;
    delete c2;
}

///
/// \brief ComptonScattering::DrawElectronDist Draws and saves to file energy of Compton electrons and angle distributions for scattered photons.
/// \param filePrefix Prefix of the output file, may contain path.
///
void ComptonScattering::DrawElectronDist(std::string filePrefix)
{
    std::cout<<"\n[INFO] Drawing histograms for Compton electrons and scattered photons."<<std::endl;
    TCanvas* c = new TCanvas("c", "Compton effect distributions", 1200, 380);
    c->Divide(3,1);
    c->cd(1);
    fH_photon_E_depos_->Draw();
    c->cd(2);
    fH_electron_E_ ->Draw();
    c->cd(3);
    fH_photon_theta_->Draw();
    TImage *img2 = TImage::Create();
    img2->FromPad(c);
    img2->WriteImage((filePrefix+std::string("_compton_distr_")+std::to_string(fNoOfGammas_)\
                      +std::string("gammas.png")).c_str());
    delete c;
    std::cout<<"[INFO] Distributions of electrons' energy, incident photons' energy and Compton scattering angle saved.\n"<<std::endl;
}

///
/// \brief ComptonScattering::Scatter Scatters a photon according to Klein-Nishina formula and fills histograms for electrons.
/// \param E Energy of incident photon.
///
void ComptonScattering::Scatter(double E)
{
    TRandom3 r(0); //set seed
    fH_photon_E_depos_->Fill(E);
    fPDF->SetParameter(0, E); //set incident photon energy
    double theta = fPDF->GetRandom(); //get scattering angle
    fH_photon_theta_->Fill(theta);
    double new_E = E * (1.0 - 1.0/(1.0+(E/(e_mass_MeV))*(1-TMath::Cos(theta)))); //E*(1-P) -- Compton electron's energy
    fH_electron_E_->Fill(new_E);
}

///
/// \brief ComptonScattering::KleinNishina_ Klein-Nishina formula
/// \param angle Scattering angle.
/// \param energy Incident's photon energy.
/// \return Compton effect cross section.
///
long double ComptonScattering::KleinNishina_(double* angle, double* energy)
{
    long double denom = 1L+(energy[0]/(e_mass_MeV))*(1-TMath::Cos(angle[0]));
    long double P = 1.0L/denom;
    return 0.5L*fine_structure_const_*fine_structure_const_*r_Compton_SI*r_Compton_SI\
            *P*P*(P + denom - TMath::Sin(angle[0])*TMath::Sin(angle[0]));
}

///
/// \brief ComptonScattering::KleinNishinaTheta_ PDF of occuring a Compton scattering with a given angle and energy.
/// \param angle Scattering angle.
/// \param energy Incident's photon energy.
/// \return PDF function value based on Klein-Nishina formula.
///
long double ComptonScattering::KleinNishinaTheta_(double* angle, double* energy)
{
    long double denom = 1L+(energy[0]/(e_mass_MeV))*(1-TMath::Cos(angle[0]));
    long double P = 1.0L/denom;
    return 0.5L*fine_structure_const_*fine_structure_const_*r_Compton_SI*r_Compton_SI\
            *P*P*(P + denom - TMath::Sin(angle[0])*TMath::Sin(angle[0]))\
            *2*TMath::Pi()*TMath::Sin(angle[0]); //corrections suggested by W.Krzemien
}



