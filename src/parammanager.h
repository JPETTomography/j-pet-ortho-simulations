/// @file parammanager.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
#ifndef PARAMMANAGER_H
#define PARAMMANAGER_H
#include <string>
#include<vector>

///
/// \brief The OutputOptions enum Specifies type of output.
///
enum OutputOptions
{
    TREE = 0,
    PNG = 1,
    BOTH = 2
};

///
/// \brief The EventTypeToSave enum Specifies what kind of Event data should be stored inside a TTree.
///
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
        bool operator==(const ParamManager &pm) const;
        //setters and getters
        inline int GetSimEvents() const {return fSimEvents_;}
        inline int GetSimRuns() const {return fSimRuns_;}
        inline int GetNoOfGammas() const {return fNoOfGammas_;}
        inline float GetP() const {return fP_;}
        inline float GetR() const {return fR_;}
        inline float GetL() const {return fL_;}
        inline float GetESc() const {return fESc_;} //in keV
        inline float GetPSc() const {return fPSc_;}
        inline float GetSmearLowLimit() const {return fSmearLowLimit_;}
        inline float GetSmearHighLimit() const {return fSmearHighLimit_;}
        inline bool IsSilentMode() const {return fSilentMode_;}
        inline void SetR(float r) {fR_=r;}
        inline void SetL(float l) {fL_=l;}
        inline void SetP(float p) {fP_=p;}
        inline void SetESc(float e) {fESc_=e;} //in keV
        inline void SetPSc(float p) {fPSc_=p;}
        inline void SetSmearLowLimit(float limit) {fSmearLowLimit_=limit;}
        inline void SetSmearHighLimit(float limit) {fSmearHighLimit_=limit;}
        inline void SetNoOfGammas(int no) {fNoOfGammas_=no;}
        inline void EnableSilentMode() {fSilentMode_=true;}
        inline void DisableSilentMode() {fSilentMode_=false;}
        inline OutputOptions GetOutputType() const {return fOutput_;}
        inline void SetOutputType(OutputOptions type) {fOutput_=type;}
        inline EventTypeToSave GetEventTypeToSave() const {return fEventTypeToSave_;}
        inline void SetEventTypeToSave(EventTypeToSave type) {fEventTypeToSave_=type;}
        //access source parameters
        std::vector<double> GetDataAt(int index=0);
        //import parameters from external file
        void ImportParams(std::string& inFile="simpar.par");
        //print parameters to the stdout
        void PrintParams();

    private:
        int fSimEvents_; //loaded from file
        int fSimRuns_; //calculated as the number of sets of source parameters
        int fNoOfGammas_;
        float fP_; //interaction probability
        float fL_; //detector length
        float fR_; //detector radius
        float fESc_; //energy in keV of additional gamma emitted in 2+1 event mode
        float fPSc_; //probability of emitting an additional gamma in 2+1 event mode
        float fSmearLowLimit_; //lower limit for smearing effect
        float fSmearHighLimit_; //higher limit for smearing effect
        bool fSilentMode_; //if set to true, less output to std::cout will be printed
        OutputOptions fOutput_; //what kind of output will be produced
        EventTypeToSave fEventTypeToSave_; //what kind of events should be saved
        std::vector<std::vector<double> > fData_; //this is where source parameters are stored
};

#endif // PARAMMANAGER_H
