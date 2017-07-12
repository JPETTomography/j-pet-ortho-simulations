#ifndef PARAMMANAGER_H
#define PARAMMANAGER_H
#include <string>
#include<vector>

enum OutputOptions
{
    TREE = 0,
    PNG = 1,
    BOTH = 2
};

enum EventTypeToSave
{
    PASS = 0,
    FAIL = 1,
    ALL = 2
};

///
/// \brief The ParamManager class Class responsible for loadind simulation parameters from external file. (by default simpar.par)
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
        inline float GetESc() const {return fESc_;} //in keV
        inline float GetPSc() const {return fPSc_;}
        inline bool IsSilentMode() const {return fSilentMode_;}
        inline void SetR(float r) {fR_=r;}
        inline void SetL(float l) {fL_=l;}
        inline void SetP(float p) {fP_=p;}
        inline void SetESc(float e) {fESc_=e;} //in keV
        inline void SetPSc(float p) {fPSc_=p;}
        inline void SetNoOfGammas(int no) {fNoOfGammas_=no;}
        inline void EnableSilentMode() {fSilentMode_=true;}
        inline void DisableSilentMode() {fSilentMode_=false;}
        inline OutputOptions GetOutputType() const {return fOutput_;}
        inline void SetOutputType(OutputOptions type) {fOutput_=type;}
        inline EventTypeToSave GetEventTypeToSave() const {return fEventTypeToSave_;}
        inline void SetEventTypeToSave(EventTypeToSave type) {fEventTypeToSave_=type;}
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
        float fESc_; //energy in keV of additional gamma emitted in 2+1 event mode
        float fPSc_; //probability of emitting an additional gamma in 2+1 event mode
        bool fSilentMode_; //if set to true, less output to std::cout will be printed
        OutputOptions fOutput_; //what kind of output will be produced
        EventTypeToSave fEventTypeToSave_; //what kind of events should be saved
        std::vector<std::vector<double> > fData_; //this is where source parameters are stored
};

#endif // PARAMMANAGER_H
