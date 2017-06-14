#ifndef PARAMMANAGER_H
#define PARAMMANAGER_H
#include <string>
#include<vector>

///
/// \brief The ParamManager class Class responsible for loadind simulation parameters from external file. (by default simulation_parameters.par)
///
class ParamManager
{
    public:
        ParamManager();
        ParamManager(const ParamManager &pm);
        ParamManager& operator= (const ParamManager &est);

        double getSimSteps() {return fSimSteps_;}
        double getSimRuns() {return fSimRuns_;}
        std::vector<double> getDataAt(int index=0);
        void ImportParams(std::string inFile="simulation_parameters.par");
        bool operator==(const ParamManager &pm) const;

    private:
        int fSimSteps_; //loaded from file
        int fSimRuns_; //calculated as the number of sets of source parameters
        std::vector<std::vector<double> > fData_; //this is where source parameters are stored
};

#endif // PARAMMANAGER_H
