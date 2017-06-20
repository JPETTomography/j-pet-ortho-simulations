/// @file psdecay.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 01.06.2017
#include "psdecay.h"
//#ifndef __CINT__
//ClassImp(PsDecay);
//#endif
#include <typeinfo>
#include <cstdio>
#include "TText.h"
#include <iomanip>
#include "TRandom3.h"


///
/// \brief PsDecay::PsDecay The main constructor of PsDecay class. Assigns some fields; creates histograms and sets their names and labels.
/// \param noOfGammas How many gammas are created in a decay (2 or 3 must be provided).
/// \param sourcePos An array containing 3 spacial coordinates of the source (x,y,z).
/// \param R Radius of the detector in m.
/// \param L Length of the detector in m.
///
PsDecay::PsDecay(int noOfGammas, double* sourceXYZ,  float p, float R, float L) : fR_(R), fL_(L), fAcceptedEvents_(0), fAcceptedGammas_(0), fNumberOfEvents_(0), fNumberOfGammas_(0), fDetectionProbability_(p)
{
    // checking if: 1) source coordinates provided 2) and 3) if they are inside the detector
    if(sourceXYZ && (sourceXYZ[0]*sourceXYZ[0]+sourceXYZ[1]*sourceXYZ[1]<R*R) && (TMath::Abs(sourceXYZ[2])<L/2.0))
    {
        std::cout<<"[INFO] Position of the source: ("<<std::scientific<<std::setprecision(2);
        for(int ii=0; ii<3; ii++)
        {
            sourcePos_[ii]=sourceXYZ[ii];
            std::cout<<sourceXYZ[ii];
            if(ii<2)
                std::cout<<", ";
            else
                std::cout<<")"<<std::endl;
        }
    }
    //default behavior if position of the source is incorrect -- placing it in the middle of detector.
    //NOTE THAT THE NAME OF THE FOLDER WITH RESULTS WILL SUGGEST DIFFERENT POSITION!!!
    else
    {
        for(int ii=0; ii<3; ii++)
        {
            sourcePos_[ii]=0.0;
            std::cout<<"XYZ:"<<sourceXYZ[0]<<" "<<sourceXYZ[1]<<" "<<sourceXYZ[2]<<std::endl;
        }
        std::cout<<"[WARNING] Source position set incorrectly! Setting default value (0,0,0)."<<std::endl;
    }

    //Creating histograms for angle distributions
    if(noOfGammas==3)
    {
        //histograms for all events
        fH_12_23_ = new TH2F("fH_12_23_","fH_12_23_all", 50,0, 3.15, 50,0,3.15);
        fH_12_23_ -> SetTitle("Polar angle distr, 12 vs 23");
        fH_12_23_ -> GetXaxis()->SetTitle("#theta_{12}");
        fH_12_23_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_23_ -> GetYaxis()->SetTitle("#theta_{23}");
        fH_12_23_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_31_ = new TH2F("fH_12_31_","fH_12_31_all", 50,0, 3.15, 50,0,3.15);
        fH_12_31_ -> SetTitle("Polar angle distr, 12 vs 31");
        fH_12_31_ -> GetXaxis()->SetTitle("#theta_{12}");
        fH_12_31_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_31_ -> GetYaxis()->SetTitle("#theta_{31}");
        fH_12_31_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_23_31_ = new TH2F("fH_23_31_","fH_23_31_all", 50,0, 3.15, 50,0,3.15);
        fH_23_31_ -> SetTitle("Polar angle distr, 23 vs 31");
        fH_23_31_ -> GetXaxis()->SetTitle("#theta_{23}");
        fH_23_31_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_31_ -> GetYaxis()->SetTitle("#theta_{31}");
        fH_23_31_ -> GetYaxis()->SetTitleOffset(1.4);

        // histograms for events tha passed cuts
        fH_12_23_pass_ = new TH2F("fH_12_23_pass","fH_12_23_pass", 50,0, 3.15, 50,0,3.15);
        fH_12_23_pass_ -> SetTitle("Polar angle distr, 12 vs 23");
        fH_12_23_pass_ -> GetXaxis()->SetTitle("#theta_{12}");
        fH_12_23_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_23_pass_ -> GetYaxis()->SetTitle("#theta_{23}");
        fH_12_23_pass_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_31_pass_ = new TH2F("fH_12_31_pass","fH_12_31_pass", 50,0, 3.15, 50,0,3.15);
        fH_12_31_pass_ -> SetTitle("Polar angle distr, 12 vs 31");
        fH_12_31_pass_ -> GetXaxis()->SetTitle("#theta_{12}");
        fH_12_31_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_31_pass_ -> GetYaxis()->SetTitle("#theta_{31}");
        fH_12_31_pass_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_23_31_pass_ = new TH2F("fH_23_31_pass","fH_23_31_pass", 50,0, 3.15, 50,0,3.15);
        fH_23_31_pass_ -> SetTitle("Polar angle distr, 23 vs 31");
        fH_23_31_pass_ -> GetXaxis()->SetTitle("#theta_{23}");
        fH_23_31_pass_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_31_pass_ -> GetYaxis()->SetTitle("#theta_{31}");
        fH_23_31_pass_ -> GetYaxis()->SetTitleOffset(1.4);

        //histograms for events that failed cuts
        fH_12_23_fail_ = new TH2F("fH_12_23_fail","fH_12_23_fail", 50,0, 3.15, 50,0,3.15);
        fH_12_23_fail_ -> SetTitle("Polar angle distr, 12 vs 23");
        fH_12_23_fail_ -> GetXaxis()->SetTitle("#theta_{12}");
        fH_12_23_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_23_fail_ -> GetYaxis()->SetTitle("#theta_{23}");
        fH_12_23_fail_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_31_fail_ = new TH2F("fH_12_31_fail","fH_12_31_fail", 50,0, 3.15, 50,0,3.15);
        fH_12_31_fail_ -> SetTitle("Polar angle distr, 12 vs 31");
        fH_12_31_fail_ -> GetXaxis()->SetTitle("#theta_{12}");
        fH_12_31_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_12_31_fail_ -> GetYaxis()->SetTitle("#theta_{31}");
        fH_12_31_fail_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_23_31_fail_ = new TH2F("fH_23_31_fail","fH_23_31_fail", 50,0, 3.15, 50,0,3.15);
        fH_23_31_fail_ -> SetTitle("Polar angle distr, 23 vs 31");
        fH_23_31_fail_ -> GetXaxis()->SetTitle("#theta_{23}");
        fH_23_31_fail_ -> GetXaxis()->SetTitleOffset(1.4);
        fH_23_31_fail_ -> GetYaxis()->SetTitle("#theta_{31}");
        fH_23_31_fail_ -> GetYaxis()->SetTitleOffset(1.4);

        fH_12_=nullptr;
        fH_12_pass_=nullptr;
        fH_12_fail_=nullptr;
    }
    else if(noOfGammas==2)
    {
        fH_12_23_ = nullptr;
        fH_12_31_ = nullptr;
        fH_23_31_ = nullptr;
        fH_12_23_pass_ = nullptr;
        fH_12_31_pass_ = nullptr;
        fH_23_31_pass_ = nullptr;
        fH_12_23_fail_ = nullptr;
        fH_12_31_fail_ = nullptr;
        fH_23_31_fail_ = nullptr;

        //histogram for all events generated
        fH_12_ = new TH1F("fH_12_all", "fH_12_all", 50, 0, 3.15);
        fH_12_ -> SetTitle("Polar angle distr, 1 vs 2");
        fH_12_ -> GetXaxis()->SetTitle("#theta_2");
        fH_12_ -> GetXaxis()->SetTitleOffset(1.6);
        fH_12_ -> GetYaxis()->SetTitle("#theta_1");
        fH_12_ -> GetYaxis()->SetTitleOffset(1.6);
        //histogram for events that passed cuts
        fH_12_pass_ = new TH1F("fH_12_pass", "fH_12_pass", 50, 0, 3.15);
        fH_12_ -> SetTitle("Polar angle distr, 1 vs 2");
        fH_12_ -> GetXaxis()->SetTitle("#theta_2");
        fH_12_ -> GetXaxis()->SetTitleOffset(1.6);
        fH_12_ -> GetYaxis()->SetTitle("#theta_1");
        fH_12_ -> GetYaxis()->SetTitleOffset(1.6);
        // histogram for events that did not passed cuts
        fH_12_fail_ = new TH1F("fH_12_fail", "fH_12_fail", 50, 0, 3.15);
        fH_12_ -> SetTitle("Polar angle distr, 1 vs 2");
        fH_12_ -> GetXaxis()->SetTitle("#theta_2");
        fH_12_ -> GetXaxis()->SetTitleOffset(1.6);
        fH_12_ -> GetYaxis()->SetTitle("#theta_1");
        fH_12_ -> GetYaxis()->SetTitleOffset(1.6);
    }
    else
    {
        throw("Invalid no of decay products!");
    }

    // histograms common for all particles
    fH_en_ = new TH1F((std::string("fH_en_")+std::to_string(noOfGammas)).c_str(), "fH_en_", 100, 0.0, 0.6);
    fH_en_ -> SetTitle("Energy distribution");
    fH_en_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_ -> GetXaxis()->SetTitleOffset(2.);
    fH_en_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_ -> GetYaxis()->SetTitleOffset(2.);
    fH_p_ = new TH1F((std::string("fH_p_")+std::to_string(noOfGammas)).c_str(), "fH_p_", 100, 0.0, 0.6);
    fH_p_ -> SetTitle("Momentum distribution");
    fH_p_ -> GetXaxis()->SetTitle("p [MeV/c]");
    fH_p_ -> GetXaxis()->SetTitleOffset(2.);
    fH_p_ -> GetYaxis()->SetTitle("dN/dp");
    fH_p_ -> GetYaxis()->SetTitleOffset(2.);
    fH_phi_ = new TH1F((std::string("fH_phi_")+std::to_string(noOfGammas)).c_str(), "fH_phi_", 100, -3.2, 3.2);
    fH_phi_ -> SetTitle("Azimuthal angle distribution");
    fH_phi_ -> GetXaxis()->SetTitle("#phi");
    fH_phi_ -> GetXaxis()->SetTitleOffset(2.);
    fH_phi_ -> GetYaxis()->SetTitle("dN/d #phi");
    fH_phi_ -> GetYaxis()->SetTitleOffset(2.);
    fH_cosTheta_ = new TH1F((std::string("fH_cosTheta_")+std::to_string(noOfGammas)).c_str(), "fH_cosTheta_", 100, -1.01, 1.01);
    fH_cosTheta_ -> SetTitle("Cosine of polar angle distribution");
    fH_cosTheta_ -> GetXaxis()->SetTitle("cos(#theta)");
    fH_cosTheta_ -> GetXaxis()->SetTitleOffset(2.);
    fH_cosTheta_ -> GetYaxis()->SetTitle("dN/d cos(#theta)");
    fH_cosTheta_ -> GetYaxis()->SetTitleOffset(2.);

    //histograms for particles that passed through cuts
    fH_en_pass_ = new TH1F((std::string("fH_en_pass_")+std::to_string(noOfGammas)).c_str(), "fH_en_", 100, 0.0, 0.6);
    fH_en_pass_ -> SetTitle("Energy distribution");
    fH_en_pass_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_pass_ -> GetXaxis()->SetTitleOffset(2.);
    fH_en_pass_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_pass_ -> GetYaxis()->SetTitleOffset(2.);
    fH_p_pass_ = new TH1F((std::string("fH_p_pass_")+std::to_string(noOfGammas)).c_str(), "fH_p_", 100, 0.0, 0.6);
    fH_p_pass_ -> SetTitle("Momentum distribution");
    fH_p_pass_ -> GetXaxis()->SetTitle("p [MeV/c]");
    fH_p_pass_ -> GetXaxis()->SetTitleOffset(2.);
    fH_p_pass_ -> GetYaxis()->SetTitle("dN/dp");
    fH_p_pass_ -> GetYaxis()->SetTitleOffset(2.);
    fH_phi_pass_ = new TH1F((std::string("fH_phi_pass_")+std::to_string(noOfGammas)).c_str(), "fH_phi_", 100, -3.2, 3.2);
    fH_phi_pass_ -> SetTitle("Azimuthal angle distribution");
    fH_phi_pass_ -> GetXaxis()->SetTitle("#phi");
    fH_phi_pass_ -> GetXaxis()->SetTitleOffset(2.);
    fH_phi_pass_ -> GetYaxis()->SetTitle("dN/d #phi");
    fH_phi_pass_ -> GetYaxis()->SetTitleOffset(2.);
    fH_cosTheta_pass_ = new TH1F((std::string("fH_cosTheta_pass_")+std::to_string(noOfGammas)).c_str(), "fH_cosTheta_", 100, -1.01, 1.01);
    fH_cosTheta_pass_ -> SetTitle("Cosine of polar angle distribution");
    fH_cosTheta_pass_ -> GetXaxis()->SetTitle("cos(#theta)");
    fH_cosTheta_pass_ -> GetXaxis()->SetTitleOffset(2.);
    fH_cosTheta_pass_ -> GetYaxis()->SetTitle("dN/d cos(#theta)");
    fH_cosTheta_pass_ -> GetYaxis()->SetTitleOffset(2.);

    //histograms for particles that did not pass through cuts
    fH_en_fail_ = new TH1F((std::string("fH_en_fail_")+std::to_string(noOfGammas)).c_str(), "fH_en_", 100, 0.0, 0.6);
    fH_en_fail_ -> SetTitle("Energy distribution");
    fH_en_fail_ -> GetXaxis()->SetTitle("E [MeV]");
    fH_en_fail_ -> GetXaxis()->SetTitleOffset(2.);
    fH_en_fail_ -> GetYaxis()->SetTitle("dN/dE");
    fH_en_fail_ -> GetYaxis()->SetTitleOffset(2.);
    fH_p_fail_ = new TH1F((std::string("fH_p_fail_")+std::to_string(noOfGammas)).c_str(), "fH_p_", 100, 0.0, 0.6);
    fH_p_fail_ -> SetTitle("Momentum distribution");
    fH_p_fail_ -> GetXaxis()->SetTitle("p [MeV/c]");
    fH_p_fail_ -> GetXaxis()->SetTitleOffset(2.);
    fH_p_fail_ -> GetYaxis()->SetTitle("dN/dp");
    fH_p_fail_ -> GetYaxis()->SetTitleOffset(2.);
    fH_phi_fail_ = new TH1F((std::string("fH_phi_fail_")+std::to_string(noOfGammas)).c_str(), "fH_phi_", 100, -3.2, 3.2);
    fH_phi_fail_ -> SetTitle("Azimuthal angle distribution");
    fH_phi_fail_ -> GetXaxis()->SetTitle("#phi");
    fH_phi_fail_ -> GetXaxis()->SetTitleOffset(2.);
    fH_phi_fail_ -> GetYaxis()->SetTitle("dN/d #phi");
    fH_phi_fail_ -> GetYaxis()->SetTitleOffset(2.);
    fH_cosTheta_fail_ = new TH1F((std::string("fH_cosTheta_fail_")+std::to_string(noOfGammas)).c_str(), "fH_cosTheta_", 100, -1.01, 1.01);
    fH_cosTheta_fail_ -> SetTitle("Cosine of polar angle distribution");
    fH_cosTheta_fail_ -> GetXaxis()->SetTitle("cos(#theta)");
    fH_cosTheta_fail_ -> GetXaxis()->SetTitleOffset(2.);
    fH_cosTheta_fail_ -> GetYaxis()->SetTitle("dN/d cos(#theta)");
    fH_cosTheta_fail_ -> GetYaxis()->SetTitleOffset(2.);


    fNoOfDecayProducts_=noOfGammas;
    for(int ii=0; ii<noOfGammas; ii++)
        fMasses_.push_back(0.0);

    //histograms to monitor cuts passing
    fH_event_cuts_ = new TH1F((std::string("fH_event_cuts_")+std::to_string(fNoOfDecayProducts_)).c_str(),\
            "fH_event_cuts_", 2, 0.0, 2.0);
    fH_event_cuts_->SetTitle("Passing cuts by events");
    fH_event_cuts_->GetYaxis()->SetTitle("% passed");
    fH_event_cuts_->GetYaxis()->SetTitleOffset(1.4);
    fH_event_cuts_->GetXaxis()->SetLabelSize(0);
    fH_event_cuts_->GetXaxis()->SetTickLength(0);
    fH_gamma_cuts_ = new TH1F((std::string("fH_gamma_cuts_")+std::to_string(fNoOfDecayProducts_)).c_str(),\
            "fH_gamma_cuts_", 2, 0.0, 2.0);
    fH_gamma_cuts_->SetTitle("Passing cuts by gammas");
    fH_gamma_cuts_->GetYaxis()->SetTitle("% passed");
    fH_gamma_cuts_->GetYaxis()->SetTitleOffset(1.4);
    fH_gamma_cuts_->GetXaxis()->SetLabelSize(0);
    fH_gamma_cuts_->GetXaxis()->SetTickLength(0);

    std::cout<<"[INFO] PsDecay object created!"<<std::endl;
}

///
/// \brief PsDecay::PsDecay Copy constructor.
/// \param est Instance of PsDecay to be copied.
///
PsDecay::PsDecay(const PsDecay& est)
{
    for(int ii=0; ii<3; ii++)
        sourcePos_[ii] = est.sourcePos_[ii];
    fR_ = est.fR_;  //radius in m
    fL_ = est.fL_;  //length in m
    fDetectionProbability_ = est.fDetectionProbability_;
    fNoOfDecayProducts_ = est.fNoOfDecayProducts_;
    fMasses_.resize(est.fMasses_.size());
    std::copy(est.fMasses_.begin(), est.fMasses_.end(), fMasses_.begin());
    fH_12_ = new TH1F(*est.fH_12_);
    fH_12_23_ = new TH2F(*est.fH_12_23_);
    fH_12_31_ = new TH2F(*est.fH_12_31_);
    fH_23_31_ = new TH2F(*est.fH_23_31_);
    fH_12_pass_ = new TH1F(*est.fH_12_pass_);
    fH_12_23_pass_ = new TH2F(*est.fH_12_23_pass_);
    fH_12_31_pass_ = new TH2F(*est.fH_12_31_pass_);
    fH_23_31_pass_ = new TH2F(*est.fH_23_31_pass_);
    fH_12_fail_ = new TH1F(*est.fH_12_fail_);
    fH_12_23_fail_ = new TH2F(*est.fH_12_23_fail_);
    fH_12_31_fail_ = new TH2F(*est.fH_12_31_fail_);
    fH_23_31_fail_ = new TH2F(*est.fH_23_31_fail_);
    fH_en_ = new TH1F(*est.fH_en_);
    fH_p_ = new TH1F(*est.fH_p_);
    fH_phi_= new TH1F(*est.fH_phi_);
    fH_cosTheta_ = new TH1F(*est.fH_cosTheta_);
    fH_en_pass_ = new TH1F(*est.fH_en_pass_);
    fH_p_pass_ = new TH1F(*est.fH_p_pass_);
    fH_phi_pass_= new TH1F(*est.fH_phi_pass_);
    fH_cosTheta_pass_ = new TH1F(*est.fH_cosTheta_pass_);
    fH_en_fail_ = new TH1F(*est.fH_en_fail_);
    fH_p_fail_ = new TH1F(*est.fH_p_fail_);
    fH_phi_fail_= new TH1F(*est.fH_phi_fail_);
    fH_cosTheta_fail_ = new TH1F(*est.fH_cosTheta_fail_);
    fH_event_cuts_ = new TH1F(*est.fH_event_cuts_);
    fH_gamma_cuts_ = new TH1F(*est.fH_gamma_cuts_);
    fNumberOfEvents_ = est.fNumberOfEvents_;
    fNumberOfGammas_ = est.fNumberOfGammas_;
    fAcceptedEvents_ = est.fAcceptedEvents_;
    fAcceptedGammas_ = est.fAcceptedGammas_;

}

///
/// \brief PsDecay::operator = Assignement operator.
/// \param est Instance of PsDecay to be copied.
/// \return Reference to instance of PsDecay with values of member fields copied.
///
PsDecay& PsDecay::operator=(const PsDecay& est)
{
    for(int ii=0; ii<3; ii++)
        sourcePos_[ii]=est.sourcePos_[ii];
    fR_=est.fR_;  //radius in m
    fL_=est.fL_;  //length in m
    fDetectionProbability_ = est.fDetectionProbability_;
    fNoOfDecayProducts_=est.fNoOfDecayProducts_;
    fNoOfDecayProducts_ = est.fNoOfDecayProducts_;
    fMasses_.resize(est.fMasses_.size());
    std::copy(est.fMasses_.begin(), est.fMasses_.end(), fMasses_.begin());
    fH_12_ = new TH1F(*est.fH_12_);
    fH_12_23_ = new TH2F(*est.fH_12_23_);
    fH_12_31_ = new TH2F(*est.fH_12_31_);
    fH_23_31_ = new TH2F(*est.fH_23_31_);
    fH_12_pass_ = new TH1F(*est.fH_12_pass_);
    fH_12_23_pass_ = new TH2F(*est.fH_12_23_pass_);
    fH_12_31_pass_ = new TH2F(*est.fH_12_31_pass_);
    fH_23_31_pass_ = new TH2F(*est.fH_23_31_pass_);
    fH_12_fail_ = new TH1F(*est.fH_12_fail_);
    fH_12_23_fail_ = new TH2F(*est.fH_12_23_fail_);
    fH_12_31_fail_ = new TH2F(*est.fH_12_31_fail_);
    fH_23_31_fail_ = new TH2F(*est.fH_23_31_fail_);
    fH_en_ = new TH1F(*est.fH_en_);
    fH_p_ = new TH1F(*est.fH_p_);
    fH_phi_= new TH1F(*est.fH_phi_);
    fH_cosTheta_ = new TH1F(*est.fH_cosTheta_);
    fH_en_pass_ = new TH1F(*est.fH_en_pass_);
    fH_p_pass_ = new TH1F(*est.fH_p_pass_);
    fH_phi_pass_= new TH1F(*est.fH_phi_pass_);
    fH_cosTheta_pass_ = new TH1F(*est.fH_cosTheta_pass_);
    fH_en_fail_ = new TH1F(*est.fH_en_fail_);
    fH_p_fail_ = new TH1F(*est.fH_p_fail_);
    fH_phi_fail_= new TH1F(*est.fH_phi_fail_);
    fH_cosTheta_fail_ = new TH1F(*est.fH_cosTheta_fail_);
    fH_event_cuts_ = new TH1F(*est.fH_event_cuts_);
    fH_gamma_cuts_ = new TH1F(*est.fH_gamma_cuts_);
    fNumberOfEvents_ = est.fNumberOfEvents_;
    fNumberOfGammas_ = est.fNumberOfGammas_;
    fAcceptedEvents_ = est.fAcceptedEvents_;
    fAcceptedGammas_ = est.fAcceptedGammas_;
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
    if(fH_12_23_pass_) delete fH_12_23_pass_;
    if(fH_12_31_pass_) delete fH_12_31_pass_;
    if(fH_23_31_pass_) delete fH_23_31_pass_;
    if(fH_12_pass_) delete fH_12_pass_;
    if(fH_12_23_fail_) delete fH_12_23_fail_;
    if(fH_12_31_fail_) delete fH_12_31_fail_;
    if(fH_23_31_fail_) delete fH_23_31_fail_;
    if(fH_12_fail_) delete fH_12_fail_;
    if(fH_en_) delete fH_en_;
    if(fH_p_) delete fH_p_;
    if(fH_phi_) delete fH_phi_;
    if(fH_cosTheta_) delete fH_cosTheta_;
    if(fH_en_pass_) delete fH_en_pass_;
    if(fH_p_pass_) delete fH_p_pass_;
    if(fH_phi_pass_) delete fH_phi_pass_;
    if(fH_cosTheta_pass_) delete fH_cosTheta_pass_;
    if(fH_en_fail_) delete fH_en_fail_;
    if(fH_p_fail_) delete fH_p_fail_;
    if(fH_phi_fail_) delete fH_phi_fail_;
    if(fH_cosTheta_fail_) delete fH_cosTheta_fail_;
    if(fH_event_cuts_) delete fH_event_cuts_;
    if(fH_gamma_cuts_) delete fH_gamma_cuts_;
}

///
/// \brief PsDecay::AddEvent Checks whether a particle passes all cuts and fills histograms.
/// \param weight Event's weight from the generator.
/// \param gamma1 Four vector of the first gamma.
/// \param gamma2 Four vector of the second gamma.
/// \param gamma3 Four vector of the third gamma. For two-gamma decays it should be null.
/// \return An array of bools indicating if particular gammas passed all cuts.
///
std::vector<bool> PsDecay::AddEvent(Double_t weight, TLorentzVector* gamma1,  TLorentzVector* gamma2,  TLorentzVector* gamma3)
{
        fNumberOfEvents_++;
        fNumberOfGammas_+=fNoOfDecayProducts_;
        //Boolean variables below indicate whether a gamma passed cuts.
        bool pass1 = false;
        bool pass2 = false;
        bool pass3 = false;
        TLorentzVector* gammas[] = {gamma1, gamma2, gamma3};
        for(int ii=0; ii<3; ii++)
        {
            if(gammas[ii])
            {
                //filling histograms for all gammas
                fH_en_->Fill(gammas[ii]->Energy()*1000);
                fH_p_->Fill(gammas[ii]->P()*1000);
                fH_phi_->Fill(gammas[ii]->Phi());
                fH_cosTheta_->Fill(gammas[ii]->CosTheta());
                fH_gamma_cuts_->Fill(0);
            }
            if(AddCuts_(gammas[ii]))
            {
                fAcceptedGammas_++;
                switch (ii)
                {
                    case 0:
                        pass1 = true;
                        break;
                    case 1:
                        pass2 = true;
                        break;
                    case 2:
                        pass3 = true;
                        break;
                }
                //filling histograms only for gammas that passed cuts
                fH_en_pass_->Fill(gammas[ii]->Energy()*1000);
                fH_p_pass_->Fill(gammas[ii]->P()*1000);
                fH_phi_pass_->Fill(gammas[ii]->Phi());
                fH_cosTheta_pass_->Fill(gammas[ii]->CosTheta());
            }
            else if(gammas[ii])
            {
                //filling histograms only for gammas that did not passed cuts
                fH_en_fail_->Fill(gammas[ii]->Energy()*1000);
                fH_p_fail_->Fill(gammas[ii]->P()*1000);
                fH_phi_fail_->Fill(gammas[ii]->Phi());
                fH_cosTheta_fail_->Fill(gammas[ii]->CosTheta());
            }
        }

        //Taking care of event histograms.
        fH_event_cuts_->Fill(0); //filling the first bin of event cut histogram -- means that there was an event generated
        if(fNoOfDecayProducts_==2)
        {
            fH_12_->Fill(gamma1->Angle((gamma2)->Vect()), weight);
            if(pass1 && pass2) // two-gamma event could be correctly detected
            {
                fH_12_pass_->Fill(gamma1->Angle((gamma2)->Vect()), weight);
                fH_event_cuts_->Fill(1);
                fAcceptedEvents_++;
            }
            else
            {
                fH_12_fail_->Fill(gamma1->Angle((gamma2)->Vect()), weight);
            }
        }
        else if(fNoOfDecayProducts_==3)
        {
            fH_12_23_->Fill(gamma1->Angle(gamma2->Vect()), gamma2->Angle(gamma3->Vect()), weight);
            fH_12_31_->Fill(gamma1->Angle(gamma2->Vect()), gamma3->Angle(gamma1->Vect()), weight);
            fH_23_31_->Fill(gamma2->Angle(gamma3->Vect()), gamma3->Angle(gamma1->Vect()), weight);
            if(pass1 && pass2 && pass3) // three-gamma event could be correctly detected
            {
                fH_12_23_pass_->Fill(gamma1->Angle(gamma2->Vect()), gamma2->Angle(gamma3->Vect()), weight);
                fH_12_31_pass_->Fill(gamma1->Angle(gamma2->Vect()), gamma3->Angle(gamma1->Vect()), weight);
                fH_23_31_pass_->Fill(gamma2->Angle(gamma3->Vect()), gamma3->Angle(gamma1->Vect()), weight);
                fAcceptedEvents_++;
                fH_event_cuts_->Fill(1);
            }
            else
            {
                fH_12_23_fail_->Fill(gamma1->Angle(gamma2->Vect()), gamma2->Angle(gamma3->Vect()), weight);
                fH_12_31_fail_->Fill(gamma1->Angle(gamma2->Vect()), gamma3->Angle(gamma1->Vect()), weight);
                fH_23_31_fail_->Fill(gamma2->Angle(gamma3->Vect()), gamma3->Angle(gamma1->Vect()), weight);
            }
        }
    // returning info, if gammas passed the cuts
    std::vector<bool> ifPassed;
    ifPassed.push_back(pass1);
    ifPassed.push_back(pass2);
    ifPassed.push_back(pass3);
    return ifPassed;
}

///
/// \brief PsDecay::DrawHistograms
/// \param prefix Prefix of the output file name. The number indicating type of decay and ".png" extension will be added afterwards.
/// \param all If true, histograms for all generated gammas will be created and saved to file.
/// \param pass If true, histograms for gammas that passed cuts will be created and saved to file.
/// \param fail If true, histograms for gammas that did not pass cuts will be created and saved to file.
/// \param compare Currently unused.
/// \param cuts If true, histograms showing how many gammas and events passed cuts will be created and saved to file.
///
void PsDecay::DrawHistograms(std::string prefix, bool all, bool pass, bool fail, bool compare, bool cuts)
{
    std::string outFile;
    if(cuts)
    {
        std::cout<<"[INFO] Drawing histograms for cuts passing."<<std::endl;
        TCanvas* cuts = new TCanvas((std::string("cuts_passed")+std::to_string(fNoOfDecayProducts_)).c_str(),\
                                    (std::string("Fraction of events/gammas that passed cuts, ")+std::to_string(fNoOfDecayProducts_)+std::string("-gamma")).c_str(),\
                                    800,400);
        cuts->Divide(2,1);
        cuts->cd(1);
        fH_event_cuts_->Scale(100.0/fNumberOfEvents_);
        fH_event_cuts_->GetYaxis()->SetRangeUser(0.0, 101.0);
        fH_event_cuts_->SetStats(kFALSE);
        fH_event_cuts_->Draw();
        TText *labelBefore = new TText();
        labelBefore-> SetNDC();
        labelBefore -> DrawText(0.2, 0.8, "before cuts");
        TText *labelAfter = new TText();
        labelAfter-> SetNDC();
        labelAfter -> DrawText(0.6, 0.8, "after cuts");

        cuts->cd(2);
        fH_gamma_cuts_->Scale(100.0/fNumberOfGammas_);
        fH_gamma_cuts_->GetYaxis()->SetRangeUser(0.0, 101.0);
        fH_gamma_cuts_->SetStats(kFALSE);
        fH_gamma_cuts_->Draw();
        TText *labelBefore2 = new TText();
        labelBefore2-> SetNDC();
        labelBefore2 -> DrawText(0.2, 0.8, "before cuts");
        TText *labelGeo = new TText();
        labelGeo -> SetNDC();
        labelGeo -> DrawText(0.55, 0.8, "geom. accept.");

        std::cout<<"[INFO] Saving histograms for cuts passing."<<std::endl;
        TImage *img_cut = TImage::Create();
        img_cut->FromPad(cuts);
        outFile=prefix+std::string("_cuts_")+std::to_string(fNoOfDecayProducts_)+std::string("gammas.png");
        img_cut->WriteImage(outFile.c_str());

        delete cuts;
        delete labelBefore;
        delete labelAfter;
        delete labelBefore2;
    }

    if(all)
    {
        std::cout<<"[INFO] Drawing histograms for all generated events."<<std::endl;
        TCanvas* angles_all;

        TCanvas* dist_all = new TCanvas((std::string("dist_all")+std::to_string(fNoOfDecayProducts_)).c_str(), \
                                        (std::string("Basic distributions for all generated ")\
                                        +std::to_string(fNoOfDecayProducts_)+std::string("-gamma events")).c_str(), 800, 800);
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

        std::cout<<"[INFO] Saving histograms for all events"<<std::endl;
        TImage *img = TImage::Create();
        img->FromPad(dist_all);
        outFile=prefix+std::string("_dist_all_")+std::to_string(fNoOfDecayProducts_)+std::string("gammas.png");
        img->WriteImage(outFile.c_str());

        //angle distribution depends on the number of decay products
        if(fNoOfDecayProducts_==3)
        {
            angles_all = new TCanvas((std::string("angles_all")+std::to_string(fNoOfDecayProducts_)).c_str(), \
                                                      (std::string("Angle ditribution for all generated ")\
                                                      +std::to_string(fNoOfDecayProducts_)+std::string("-gamma events")).c_str(), 1200, 400);
            angles_all->Divide(3,1);
            angles_all->cd(1);
            outFile = prefix+std::string("_angles_all_3gammas.png");
            fH_12_23_->Draw("colz");
            angles_all->cd(2);
            fH_12_31_->Draw("colz");
            angles_all->cd(3);
            fH_23_31_->Draw("colz");
        }
        else
        {
            angles_all = new TCanvas((std::string("angles_all")+std::to_string(fNoOfDecayProducts_)).c_str(), \
                                                   (std::string("Angle ditribution for all generated ")\
                                                   +std::to_string(fNoOfDecayProducts_)+std::string("-gamma events")).c_str(), 400, 400);
            fH_12_->Draw();
            outFile = prefix+std::string("_angles_all_2gammas.png");
        }
        angles_all->Update();
        TImage *img2 = TImage::Create();
        img2->FromPad(angles_all);
        img2->WriteImage(outFile.c_str());

        delete dist_all;
        delete angles_all;
    }

    if(pass)
    {
        std::cout<<"[INFO] Drawing histograms for events that passed cuts."<<std::endl;
        TCanvas* angles_pass;

        TCanvas* dist_pass = new TCanvas((std::string("dist_pass")+std::to_string(fNoOfDecayProducts_)).c_str(),\
                                         ("Basic distributions for passed "\
                                         +std::to_string(fNoOfDecayProducts_)+std::string("-gamma events")).c_str(), 800, 800);
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

        std::cout<<"[INFO] Saving histograms for passed events"<<std::endl;
        TImage *img = TImage::Create();
        img->FromPad(dist_pass);
        outFile=prefix+std::string("_dist_pass_")+std::to_string(fNoOfDecayProducts_)+std::string("_gammas.png");
        img->WriteImage(outFile.c_str());

        //angle distribution depends on the number of decay products
        if(fNoOfDecayProducts_==3)
        {
            angles_pass = new TCanvas((std::string("angles_pass")+std::to_string(fNoOfDecayProducts_)).c_str(),\
                                        ("Angle ditribution for all generated "\
                                        +std::to_string(fNoOfDecayProducts_)+std::string("-gamma events")).c_str(), 1200, 400);
            angles_pass->Divide(3,1);
            angles_pass->cd(1);
            outFile = prefix+std::string("_angles_pass_3gammas.png");
            fH_12_23_pass_->Draw("colz");
            angles_pass->cd(2);
            fH_12_31_pass_->Draw("colz");
            angles_pass->cd(3);
            fH_23_31_pass_->Draw("colz");
        }
        else
        {
            angles_pass = new TCanvas((std::string("angles_pass")+std::to_string(fNoOfDecayProducts_)).c_str(),\
                                      ("Angle ditribution for all generated "\
                                      +std::to_string(fNoOfDecayProducts_)+std::string("-gamma events")).c_str(), 400, 400);
            fH_12_pass_->Draw();
            outFile = prefix+std::string("_angles_pass_2gammas.png");
        }
        angles_pass->Update();
        TImage *img2 = TImage::Create();
        img2->FromPad(angles_pass);
        img2->WriteImage(outFile.c_str());

        delete dist_pass;
        delete angles_pass;
    }

    if(fail)
    {
        std::cout<<"[INFO] Drawing histograms for events that did not pass cuts."<<std::endl;
        TCanvas* angles_fail;
        TCanvas* dist_fail= new TCanvas((std::string("dist_fail")+std::to_string(fNoOfDecayProducts_)).c_str(), ("Basic distributions for passed "\
                                         +std::to_string(fNoOfDecayProducts_)+std::string("-gamma events")).c_str(), 800, 800);
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

        std::cout<<"[INFO] Saving histograms for failed events"<<std::endl;
        TImage *img = TImage::Create();
        img->FromPad(dist_fail);
        outFile=prefix+std::string("_dist_fail_")+std::to_string(fNoOfDecayProducts_)+std::string("_gammas.png");
        img->WriteImage(outFile.c_str());

        //angle distribution depends on the number of decay products
        if(fNoOfDecayProducts_==3)
        {
            angles_fail = new TCanvas((std::string("angles_fail")+std::to_string(fNoOfDecayProducts_)).c_str(), \
                                      ("Angle ditribution for all generated "\
                                      +std::to_string(fNoOfDecayProducts_)+std::string("-gamma events")).c_str(), 1200, 400);

            angles_fail->Divide(3,1);
            angles_fail->cd(1);
            outFile = prefix+std::string("_angles_fail_3gammas.png");
            fH_12_23_fail_->Draw("colz");
            angles_fail->cd(2);
            fH_12_31_fail_->Draw("colz");
            angles_fail->cd(3);
            fH_23_31_fail_->Draw("colz");
        }
        else
        {
            angles_fail = new TCanvas((std::string("angles_fail")+std::to_string(fNoOfDecayProducts_)).c_str(), \
                                      ("Angle ditribution for all generated "\
                                      +std::to_string(fNoOfDecayProducts_)+std::string("-gamma events")).c_str(), 400, 400);

            fH_12_fail_->Draw();
            outFile = prefix+std::string("_angles_fail_2gammas.png");
        }
        angles_fail->Update();
        TImage *img2 = TImage::Create();
        img2->FromPad(angles_fail);
        img2->WriteImage(outFile.c_str());

        delete dist_fail;
        delete angles_fail;
    }

}

///
/// \brief PsDecay::AddCuts_ Applies all cuts to a particle.
/// \param gamma Four vector of a particle (gamma quant).
/// \return True if particle passed through all cuts, false if it did not.
///
bool PsDecay::AddCuts_(TLorentzVector* gamma)
{     
    if(!gamma)
        return false;
    return GeometricalAcceptance(gamma) && DetectionCut();
}

///
/// \brief PsDecay::GeometricalAcceptance Applies geometrical acceptance cut to a particle. It calculates intersection point on the basis surface of the cyllinder.
/// \param gamma Four vector of a particle (gamma quant).
/// \return True if particle hit the side surface of the detector's barrel, false otherwise.
///
bool PsDecay::GeometricalAcceptance(TLorentzVector *gamma)
{
    int side = (gamma->Pz()>0) ? 1 : -1;
    double s = (side*fL_/2.0 - sourcePos_[2])/gamma->Pz();
    double x = sourcePos_[0] + s*(gamma->Px());
    double y = sourcePos_[1] +s*(gamma->Py());

    if(x*x + y*y >= fR_*fR_)
    {
        fH_gamma_cuts_->Fill(1);
        return true;
    }
    else return false;
}

///
/// \brief PsDecay::DetectionCut A particle not always interacts with a detector. This cut checks if it interacts, with a priori probability given by a user.
/// \return True if particle interacts with a detector, false if not.
///
bool PsDecay::DetectionCut()
{
    if(fDetectionProbability_==1.0)
        return true;
    else
    {
        TRandom3 r(0); //set seed
        float p = r.Uniform(0.0, 1.0);
        return p <= fDetectionProbability_;
    }
}
