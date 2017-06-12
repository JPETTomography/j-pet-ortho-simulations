#include "parammanager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <map>
#include <iterator>

ParamManager::ParamManager() : fSimSteps_(10000), fSimRuns_(0) {}

ParamManager & ParamManager::operator= (const ParamManager &est) {
    if (this == &est) return *this;
    fData_.resize(est.fData_.size());
    std::copy(est.fData_.begin(), est.fData_.end(), fData_.begin());
    return *this;
}

bool ParamManager::operator==(const ParamManager &est) const { return est.fData_ == fData_; }

ParamManager::ParamManager(const ParamManager &est)
{
    fData_.resize(est.fData_.size());
    std::copy(est.fData_.begin(), est.fData_.end(), fData_.begin());
}

///////////////////////////////////////////////////////////////////

void ParamManager::ImportParams(std::string inFile)
{
    std::cout<<"[INFO] Importing parameters from file: "<<inFile<<std::endl;
    std::ifstream param_file(inFile.c_str());
    std::string row;
    for(int ii=0; ii<6; ii++)
        std::getline(param_file, row);
    std::istringstream iss1(row);
    iss1>>fSimSteps_;
    std::cout<<"[INFO] Sim steps: "<<fSimSteps_<<std::endl;

    while (getline(param_file, row))
    {
          std::istringstream is(row);
          fData_.push_back(std::vector<double>(std::istream_iterator<double>(is), std::istream_iterator<double>()));
    }

    fSimRuns_=fData_.size();
    std::cout<<"[INFO] Parameters imported!"<<std::endl;
}


std::vector<double> ParamManager::getDataAt(int index)
{
    if(index >= fSimRuns_)
        throw ("[ERROR] Invalid index to get from ParamManger!");

    std::vector<double> data=fData_.at(index);
//    std::cout<<data[0]<<" "<<data[1]<<" "<<data[2]<<" "<<data[3]<<" "<<data[4]<<" "<<data[5]<<std::endl;
    return data;
}
