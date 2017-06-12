#ifndef PARAMMANAGER_H
#define PARAMMANAGER_H
#include <string>
#include<vector>

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

        int fSimSteps_;
        int fSimRuns_;
        std::vector<std::vector<double> > fData_;
};

#endif // PARAMMANAGER_H
