#include "parammanager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <iterator>
#include <cstring>

///
/// \brief ParamManager::ParamManager Basic constructor.
///
ParamManager::ParamManager() :
    fSimEvents_(0),
    fSimRuns_(0),
    fNoOfGammas_(0),
    fP_(0),
    fL_(700),
    fR_(500),
    fESc_(1160),
    fPSc_(0.98),
    fSilentMode_(false)
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
    fP_=est.fP_;
    fR_=est.fP_;
    fL_=est.fL_;
    fESc_=est.fESc_;
    fPSc_=est.fPSc_;
    fSilentMode_=est.fSilentMode_;
    fData_.resize(est.fData_.size());
    std::copy(est.fData_.begin(), est.fData_.end(), fData_.begin());
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
    fP_=est.fP_;
    fR_=est.fP_;
    fL_=est.fL_;
    fESc_=est.fESc_;
    fPSc_=est.fPSc_;
    fSilentMode_=est.fSilentMode_;
    fData_.resize(est.fData_.size());
    std::copy(est.fData_.begin(), est.fData_.end(), fData_.begin());
    return *this;
}

///
/// \brief ParamManager::operator == Compare operator. Compares values of member fields.
/// \param est Instance of ParamManager to be compared with this.
/// \return True if all parameter fields are equal.
///
bool ParamManager::operator==(const ParamManager &est) const
{
    return ((est.fData_ == fData_) && (fSimEvents_==est.fSimEvents_) && (fSimRuns_==est.fSimRuns_) && \
            (fP_==est.fP_) && (fL_==est.fL_) && (fR_==est.fR_) && (fNoOfGammas_==est.fNoOfGammas_) && \
            (fESc_==est.fESc_) && (fPSc_==est.fPSc_) && (fSilentMode_==est.fSilentMode_));
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
/// \brief ParamManager::ImportParams Imports prameters from file. It reads the number of simulation steps from the first line, and source's (x,y,z,px,py,pz) from the nect ones.
/// \param inFile Path to the file with parameters.
///
void ParamManager::ImportParams(std::string inFile)
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
          if (row.find(":=") != std::string::npos)
          {
              std::string segment;
              std::vector<std::string> token;

              while(std::getline(is, segment, ' '))
              {
                 token.push_back(segment);
              }
              if(token[0]=="p")
                fP_ = atof(token[2].c_str());
              else if (token[0]=="events")
                fSimEvents_ = atoi(token[2].c_str());
              else if (token[0]=="R")
                fR_ = atof(token[2].c_str());
              else if (token[0]=="L")
                fL_ = atof((token[2].c_str()));
              else if (token[0]=="gammas")
                fNoOfGammas_=atoi(token[2].c_str());
              else if (token[0]=="ESc")
                fESc_=atof(token[2].c_str());
              else if (token[0]=="pSc")
                fPSc_=atof(token[2].c_str());
              else if (token[0]=="silent")
                fSilentMode_= atoi(token[2].c_str()) == 0 ? false : true;
              else
                std::cerr<<"[WARNING] Unrecognized parameter in the param file: \""<<token[0]<<"\""<<std::endl;
          }
          else
          {
              fData_.push_back(std::vector<double>(std::istream_iterator<double>(is), std::istream_iterator<double>()));
          }
    }
    //The number of simulation runs is determined basing on the number of data sets with source's parameters.
    fSimRuns_=fData_.size();
    if(fNoOfGammas_==1)
        std::cout<<"[INFO] No of decay products: 2+1"<<std::endl;
    else if(fNoOfGammas_!=2 && fNoOfGammas_!=3)
        std::cout<<"[INFO] No of decay products: 2 and 3"<<std::endl;
    else
        std::cout<<"[INFO] No of decay products: "<<fNoOfGammas_<<std::endl;
    std::cout<<"[INFO] Events to generate: "<<fSimEvents_<<std::endl;
    std::cout<<"[INFO] Runs to simulate: "<<fSimRuns_<<std::endl;
    std::cout<<"[INFO] Detector radius: "<<fR_<<" [mm]"<<std::endl;
    std::cout<<"[INFO] Detector length: "<<fL_<<" [mm]"<<std::endl;
    std::cout<<"[INFO] Probability to interact with detector: "<<fP_<<std::endl;
    if(fNoOfGammas_==1)
    {
        std::cout<<"[INFO] Energy of additional gamma: "<<fESc_<<" [keV]"<<std::endl;
        std::cout<<"[INFO] Probability of emitting an additional gamma: "<<fPSc_<<std::endl;
    }
    std::cout<<"[INFO] Silent mode: ";
    if(fSilentMode_) std::cout<<"ENABLED"<<std::endl;
    else std::cout<<"DISABLED"<<std::endl;
    std::cout<<"[INFO] Parameters imported!\n"<<std::endl;
}

///
/// \brief ParamManager::getDataAt Used to get source's position and momentum.
/// \param index Number of the run, used to access apropriate data.
/// \return An array with a set of source's parameters: x, y, z, px, py, pz;
///
std::vector<double> ParamManager::GetDataAt(int index)
{
    if(index >= fSimRuns_)
        throw ("[ERROR] Invalid index to get from ParamManger!");

    std::vector<double> data=fData_.at(index);
//    std::cout<<data[0]<<" "<<data[1]<<" "<<data[2]<<" "<<data[3]<<" "<<data[4]<<" "<<data[5]<<std::endl;
    return data;
}
