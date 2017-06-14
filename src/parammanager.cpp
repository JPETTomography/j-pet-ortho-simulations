#include "parammanager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <iterator>

///
/// \brief ParamManager::ParamManager Basic constructor.
///
ParamManager::ParamManager() : fSimSteps_(0), fSimRuns_(0) {} //DO NOT CHANGE THE DEFAULT VALUE FOR fSimStep!!!

///
/// \brief ParamManager::ParamManager Copy constructor.
/// \param est ParamManager instance from which fields will be copied.
///
ParamManager::ParamManager(const ParamManager &est)
{
    fSimSteps_=est.fSimSteps_;
    fSimRuns_=est.fSimRuns_;
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
    fData_.resize(est.fData_.size());
    std::copy(est.fData_.begin(), est.fData_.end(), fData_.begin());
    return *this;
}

///
/// \brief ParamManager::operator == Compare operator. Compares values of member fields.
/// \param est Instance of ParamManager to be compared with this.
/// \return True if all fields are equal.
///
bool ParamManager::operator==(const ParamManager &est) const
{
    return ((est.fData_ == fData_) && (fSimSteps_==est.fSimSteps_) && (fSimRuns_==est.fSimRuns_));
}


///////////////////////////////////////////////////////////////////

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
          if(row[0]=='#')
          {
              continue; //ignore lines starting with '#'
          }
          std::istringstream is(row);
          if(fSimSteps_ > 0) //if fSimSteps_ was changed (by default 0) it will read source's parameters. Otherwise it will read fSimSteps_.
              fData_.push_back(std::vector<double>(std::istream_iterator<double>(is), std::istream_iterator<double>()));
          else
              is>>fSimSteps_;
    }
    //The number of simulation runs is determined basing on the number of data sets with source's parameters.
    fSimRuns_=fData_.size();
    std::cout<<"[INFO] Sim steps: "<<fSimSteps_<<std::endl;
    std::cout<<"[INFO] Sim runs: "<<fSimRuns_<<std::endl;
    std::cout<<"[INFO] Parameters imported!\n"<<std::endl;
}

///
/// \brief ParamManager::getDataAt Used to get source's position and momentum.
/// \param index Number of the run, used to access apropriate data.
/// \return An array with a set of source's parameters: x, y, z, px, py, pz;
///
std::vector<double> ParamManager::getDataAt(int index)
{
    if(index >= fSimRuns_)
        throw ("[ERROR] Invalid index to get from ParamManger!");

    std::vector<double> data=fData_.at(index);
//    std::cout<<data[0]<<" "<<data[1]<<" "<<data[2]<<" "<<data[3]<<" "<<data[4]<<" "<<data[5]<<std::endl;
    return data;
}
