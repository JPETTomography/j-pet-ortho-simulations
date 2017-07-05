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

        inline int getSimEvents() const {return fSimEvents_;}
        inline int getSimRuns() const {return fSimRuns_;}
        inline int getNoOfGammas() const {return fNoOfGammas_;}
        inline float getP() const {return fP_;}
        inline float getR() const {return fR_;}
        inline float getL() const {return fL_;}
        inline bool isSilentMode() const {return fSilentMode_;}
        inline void setR(float r) {fR_=r;}
        inline void setL(float l) {fL_=l;}
        inline void setP(float p) {fP_=p;}
        inline void setNoOfGammas(int no) {fNoOfGammas_=no;}
        inline void enableSilentMode() {fSilentMode_=true;}
        inline void disableSilentMode() {fSilentMode_=false;}

        std::vector<double> getDataAt(int index=0);
        void ImportParams(std::string inFile="simulation_parameters.par");
        bool operator==(const ParamManager &pm) const;

    private:
        int fSimEvents_; //loaded from file
        int fSimRuns_; //calculated as the number of sets of source parameters
        int fNoOfGammas_;
        float fP_; //interaction probability
        float fL_; //detector length
        float fR_; //detector radius
        bool fSilentMode_;
        std::vector<std::vector<double> > fData_; //this is where source parameters are stored
};

#endif // PARAMMANAGER_H
