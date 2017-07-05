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

        inline int GetSimEvents() const {return fSimEvents_;}
        inline int GetSimRuns() const {return fSimRuns_;}
        inline int GetNoOfGammas() const {return fNoOfGammas_;}
        inline float GetP() const {return fP_;}
        inline float GetR() const {return fR_;}
        inline float GetL() const {return fL_;}
        inline bool IsSilentMode() const {return fSilentMode_;}
        inline void SetR(float r) {fR_=r;}
        inline void SetL(float l) {fL_=l;}
        inline void SetP(float p) {fP_=p;}
        inline void SetNoOfGammas(int no) {fNoOfGammas_=no;}
        inline void EnableSilentMode() {fSilentMode_=true;}
        inline void DisableSilentMode() {fSilentMode_=false;}

        std::vector<double> GetDataAt(int index=0);
        void ImportParams(std::string inFile="simpar.par");
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
