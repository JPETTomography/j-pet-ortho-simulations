/// @file parammanager.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <iterator>
#include <cstring>
#include <algorithm>
#include <TMath.h>
#include "parammanager.h"
///
/// \brief ParamManager::ParamManager Basic constructor.
///
ParamManager::ParamManager() :
    fSimEvents_(0),
    fSimRuns_(0),
    fNoOfGammas_(0),
    fEff_(0),
    fL_(700),
    fR_(500),
    fE_(1157),
    fP_(0.98),
    fSmearLowLimit_(0.0),
    fSmearHighLimit_(2.0),
    fSeed_(0),
    fSilentMode_(false),
    f2nNdataImported_(false),
    fOutput_(PNG),
    fEventTypeToSave_(ALL)
    {}

///
/// \brief ParamManager::ParamManager Copy constructor.
/// \param est ParamManager instance from which fields will be copied.
///
ParamManager::ParamManager(const ParamManager &est)
{
    fSimEvents_=est.fSimEvents_;
    fSimRuns_=est.fSimRuns_;
    fNoOfGammas_=est.fNoOfGammas_;
    fEff_=est.fEff_;
    fR_=est.fP_;
    fL_=est.fL_;
    fE_=est.fE_;
    fP_=est.fP_;
    fSmearLowLimit_=est.fSmearLowLimit_;
    fSmearHighLimit_=est.fSmearHighLimit_;
    fSeed_=est.fSeed_;
    fSilentMode_=est.fSilentMode_;
    f2nNdataImported_=est.f2nNdataImported_;
    fOutput_=est.fOutput_;
    fEventTypeToSave_=est.fEventTypeToSave_;
    fData_.resize(est.fData_.size());
    std::copy(est.fData_.begin(), est.fData_.end(), fData_.begin());
    fDecayBranchProbability_.resize(est.fDecayBranchProbability_.size());
    std::copy(est.fDecayBranchProbability_.begin(), est.fDecayBranchProbability_.end(), fDecayBranchProbability_.begin());
    for(unsigned ii=0; ii<fGammaEnergy_.size(); ii++)
        fGammaEnergy_.push_back(est.fGammaEnergy_[ii]);
}

///
/// \brief ParamManager::operator = Assignement operator.
/// \param est ParamManager instance from which member fields will be copied.
/// \return Reference to ParamManager instance with values of fields from est.
///
ParamManager & ParamManager::operator= (const ParamManager &est) {
    if (this == &est) return *this;
    fSimEvents_=est.fSimEvents_;
    fSimRuns_=est.fSimRuns_;
    fNoOfGammas_=est.fNoOfGammas_;
    fEff_=est.fEff_;
    fR_=est.fP_;
    fL_=est.fL_;
    fE_=est.fE_;
    fP_=est.fP_;
    fSmearLowLimit_=est.fSmearLowLimit_;
    fSmearHighLimit_=est.fSmearHighLimit_;
    fSeed_=est.fSeed_;
    fSilentMode_=est.fSilentMode_;
    f2nNdataImported_=est.f2nNdataImported_;
    fOutput_=est.fOutput_;
    fEventTypeToSave_=est.fEventTypeToSave_;
    fData_.resize(est.fData_.size());
    std::copy(est.fData_.begin(), est.fData_.end(), fData_.begin());
    fDecayBranchProbability_.resize(est.fDecayBranchProbability_.size());
    std::copy(est.fDecayBranchProbability_.begin(), est.fDecayBranchProbability_.end(), fDecayBranchProbability_.begin());
    for(unsigned ii=0; ii<fGammaEnergy_.size(); ii++)
        fGammaEnergy_.push_back(est.fGammaEnergy_[ii]);
    return *this;
}

///
/// \brief ParamManager::operator == Compare operator. Compares values of member fields.
/// \param est Instance of ParamManager to be compared with this.
/// \return True if all fields are equal.
///
bool ParamManager::operator==(const ParamManager &est) const
{
    bool params = ((est.fData_ == fData_) && (fSimEvents_==est.fSimEvents_) && (fSimRuns_==est.fSimRuns_) && \
            (fEff_==est.fEff_) && (fL_==est.fL_) && (fR_==est.fR_) && (fNoOfGammas_==est.fNoOfGammas_) && \
            (fE_==est.fE_) && (fP_==est.fP_) && (fSilentMode_==est.fSilentMode_) && fOutput_==est.fOutput_)&&\
            (fEventTypeToSave_==est.fEventTypeToSave_) && (fSmearLowLimit_==est.fSmearLowLimit_) && \
            (fSmearHighLimit_==est.fSmearHighLimit_) && (f2nNdataImported_==est.f2nNdataImported_) && fSeed_==est.fSeed_;
    return params && std::equal(fData_.begin(), fData_.end(), est.fData_.begin())\
            && std::equal(fDecayBranchProbability_.begin(), fDecayBranchProbability_.end(), est.fDecayBranchProbability_.begin())\
            && std::equal(fGammaEnergy_.begin(), fGammaEnergy_.end(), est.fGammaEnergy_.begin());
}


///////////////////////////////////////////////////////////////////
/// \brief trim Removes preceding and following white spaces from a string object.
/// \param str String to be trimmed.
/// \param whitespace What chars should be treates as a whitespace.
/// \return Substring without preceding and follwing whitespaces.
///
std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

///
/// \brief reduce Removes all whitespaces and optionally places fill strings in place of them, except for the following/preceding ones.
/// \param str Input string.
/// \param fill Place it where whitespaces are.
/// \param whitespace What characters should be treated as whitespaces.
/// \return String withou unnecessary whitespaces.
///
std::string reduce(const std::string& str,
                   const std::string& fill = " ",
                   const std::string& whitespace = " \t")
{
    // trim first
    auto result = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}

///
/// \brief ParamManager::ImportParams Imports prameters from file.
/// \param inFile Path to the file with parameters.
///
void ParamManager::ImportParams(const std::string& inFile)
{
    std::cout<<"[INFO] Importing parameters from file: "<<inFile<<std::endl;
    std::ifstream param_file(inFile.c_str());
    std::string row;

    //read line by line
    while (getline(param_file, row))
    {
          if(!row.length() || row[0]=='#')
          {
              continue; //ignore lines starting with '#'
          }
          row = reduce(row);
          std::istringstream is(row);
          if (row.find(":=") != std::string::npos) // parse parameters
          {
              std::string segment;
              std::vector<std::string> token;

              while(std::getline(is, segment, ' '))
              {
                 token.push_back(segment);
              }
              if(token[0]=="eff")
                fEff_ = atof(token[2].c_str());
              else if (token[0]=="events")
                fSimEvents_ = atoi(token[2].c_str());
              else if (token[0]=="R")
                fR_ = atof(token[2].c_str());
              else if (token[0]=="L")
                fL_ = atof((token[2].c_str()));
              else if (token[0]=="gammas")
                {
                  fNoOfGammas_=atoi(token[2].c_str());
                }
              else if (token[0]=="E")
                fE_=atof(token[2].c_str());
              else if (token[0]=="p")
                fP_=atof(token[2].c_str());
              else if (token[0]=="seed")
                fSeed_=atof(token[2].c_str());
              else if(token[0]=="smearLow")
                fSmearLowLimit_=atof(token[2].c_str());
              else if(token[0]=="smearHigh")
                fSmearHighLimit_=atof(token[2].c_str());
              else if (token[0]=="silent")
                fSilentMode_= atoi(token[2].c_str()) == 0 ? false : true;
              else if (token[0]=="output")
              {
                  if(token[2]=="tree")
                      fOutput_=TREE;
                  else if(token[2]=="png")
                      fOutput_=PNG;
                  else if(token[2]=="both")
                      fOutput_=BOTH;
                  else
                  {
                      std::cerr<<"[WARNING] Unrecognized output type! Setting to default (png)."<<std::endl;
                      fOutput_=PNG;
                  }
              }
              else if (token[0]=="eventType")
              {
                  if(token[2]=="all")
                      fEventTypeToSave_=ALL;
                  else if(token[2]=="pass")
                      fEventTypeToSave_=PASS;
                  else if(token[2]=="fail")
                      fEventTypeToSave_=FAIL;
                  else
                  {
                      std::cerr<<"[WARNING] Unrecognized event type to save! Setting to default (all)."<<std::endl;
                      fEventTypeToSave_=ALL;
                  }
              }
              else
                std::cerr<<"[WARNING] Unrecognized parameter in the param file: \""<<token[0]<<"\""<<std::endl;
          }
          else //parse source position, momentum and radius
          {
              fData_.push_back(std::vector<double>(std::istream_iterator<double>(is), std::istream_iterator<double>()));
          }
    }
    //The number of simulation runs is dete#include <algorithm>rmined basing on the number of data sets with source's parameters.
    fSimRuns_=fData_.size();
    if(!fSilentMode_)
        PrintParams();
    else
        std::cout<<"[WARNING] Silent mode is enabled! Some information will not be printed!"<<std::endl;
    std::cout<<"[INFO] Parameters imported.\n"<<std::endl;
}

void ParamManager::Import2nNdata(const std::string& inFile)
{
    std::cout<<"[INFO] Importing 2&Ndata from file: "<<inFile<<std::endl;
    std::ifstream param_file(inFile.c_str());
    std::string row;

    //read line by line
    while (getline(param_file, row))
    {
          if(!row.length() || row[0]=='#')
          {
              continue; //ignore lines starting with '#'
          }
          row = reduce(row);
          std::istringstream is(row);
          double pB;
          double eG;
          is >> pB;
          if(pB > 0.0)
          {
            fDecayBranchProbability_.push_back(pB);
            fGammaEnergy_.push_back(std::vector<double>());
          }
          int index = fGammaEnergy_.size()-1;
          while(is>> eG)
          {
              (fGammaEnergy_.at(index)).push_back(eG);
          }
    f2nNdataImported_=true;
    }
    ValidatePromptData_();
}

///
/// \brief PrintParams Prints parameters values to the standard output.
///
void ParamManager::PrintParams()
{
    if(fNoOfGammas_==1)
        std::cout<<"[INFO] No of decay products: 1"<<std::endl;
    else if(fNoOfGammas_==4)
        std::cout<<"[INFO] No of decay products: 2+1"<<std::endl;
    else if(fNoOfGammas_!=2 && fNoOfGammas_!=3)
        std::cout<<"[INFO] No of decay products: 2 and 3"<<std::endl;
    else
        std::cout<<"[INFO] No of decay products: "<<fNoOfGammas_<<std::endl;
    std::cout<<"[INFO] Events to generate: "<<fSimEvents_<<std::endl;
    std::cout<<"[INFO] Runs to simulate: "<<fSimRuns_<<std::endl;
    std::cout<<"[INFO] Detector radius: "<<fR_<<" [mm]"<<std::endl;
    std::cout<<"[INFO] Detector length: "<<fL_<<" [mm]"<<std::endl;
    std::cout<<"[INFO] Scintillator's efficiency: "<<fEff_<<std::endl;
    if(fNoOfGammas_==4)
    {
        std::cout<<"[INFO] Energy of single gamma: "<<fE_<<" [keV]"<<std::endl;
        std::cout<<"[INFO] Probability of emitting an additional gamma: "<<fP_<<std::endl;
    }
    std::string seedToShow = fSeed_==0 ? "random" : std::to_string(fSeed_);
    std::cout<<"[INFO] Seed: "<<seedToShow<<std::endl;
    std::cout<<"[INFO] Smearing lower limit: "<<fSmearLowLimit_<<" [MeV]"<<std::endl;
    std::cout<<"[INFO] Smearing higher limit: "<<fSmearHighLimit_<<" [MeV]"<<std::endl;
    std::cout<<"[INFO] Silent mode: ";
    if(fSilentMode_) std::cout<<"ENABLED"<<std::endl;
    else std::cout<<"DISABLED"<<std::endl;
    std::cout<<"[INFO] Output type: ";
    switch (fOutput_)
    {
        case TREE:
            std::cout<<"ROOT TREE"<<std::endl;
            break;
        case PNG:
            std::cout<<"PNG IPromptMAGES"<<std::endl;
            break;
        case BOTH:
            std::cout<<"ROOT TREE & PNG IMAGES"<<std::endl;
            break;
        default:
            break;
    }
    std::cout<<"[INFO] Event type saved to tree: ";
    switch (fEventTypeToSave_)
    {
        case ALL:
            std::cout<<"ALL"<<std::endl;
            break;
        case PASS:
            std::cout<<"PASS"<<std::endl;
            break;
        case FAIL:
            std::cout<<"FAIL"<<std::endl;
            break;
        default:
            break;
    }
}

void ParamManager::Print2nNdata()
{
    std::cout<<"[INFO] Printing decay branches:"<<std::endl;
    for(unsigned ii = 0; ii< fDecayBranchProbability_.size(); ii++)
    {
        std::cout<<fDecayBranchProbability_.at(ii)<<" ";
        for(unsigned jj = 0; jj < fGammaEnergy_.at(ii).size(); jj++)
        {
            double eG = (fGammaEnergy_.at(ii)).at(jj);
            std::cout<<eG<<" ";
        }
        std::cout<<std::endl;
    }
}


///
/// \brief ParamManager::getDataAt Used to get source's position and momentum and radius.
/// \param index Number of the run, used to access apropriate data.
/// \return An array with a set of source's parameters: x, y, z, px, py, pz, r;
///
std::vector<double> ParamManager::GetDataAt(const int index) const
{
    if(index >= fSimRuns_)
        throw ("[ERROR] Invalid index to get from ParamManger!");

    std::vector<double> data=fData_.at(index);
    return data;
}

void ParamManager::ValidatePromptData_()
{
    double sum = 0;
    for(std::vector<double>::iterator it = fDecayBranchProbability_.begin(); it!=fDecayBranchProbability_.end(); ++it)
    {
        sum+=*it;
    }
    if(TMath::Abs(sum - 1.00) > 1e-6)
    {
        std::cout<<"[WARNING] Decay branch probabilities don\'t sum to 1! Renormalizing."<<std::endl;
        std::for_each(fDecayBranchProbability_.begin(), fDecayBranchProbability_.end(), [sum](double &p){ p/=sum; });
    }

}
