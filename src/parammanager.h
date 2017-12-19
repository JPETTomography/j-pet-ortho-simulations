/// @file parammanager.h
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
#ifndef PARAMMANAGER_H
#define PARAMMANAGER_H
#include <string>
#include <vector>

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

class TwoAndNTestFixture; // for testing

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
        inline float GetEff() const {return fEff_;}
        inline float GetR() const {return fR_;}
        inline float GetL() const {return fL_;}
        inline float GetE() const {return fE_;} //in keV
        inline float GetP() const {return fP_;}
        inline float GetSmearLowLimit() const {return fSmearLowLimit_;}
        inline float GetSmearHighLimit() const {return fSmearHighLimit_;}
        inline int GetSeed() const {return fSeed_;}
        inline bool IsSilentMode() const {return fSilentMode_;}
        //methods used for 2&N decays
        inline bool Is2nNDataImported() const {return f2nNdataImported_;}
        inline int GetNumberOfDecayBranches() const {return fDecayBranchProbability_.size();}
        inline int GetBranchSize(const unsigned index) const
            {return fGammaEnergy_[index].size();}
        inline double GetDecayBranchProbabilityAt(const unsigned index) const
            {if(index<fDecayBranchProbability_.size()) return fDecayBranchProbability_[index]; else return 0;}
        inline double GetGammaEnergyAt(const unsigned branch, const unsigned gamma) const
            {if(branch<fDecayBranchProbability_.size() && gamma<(fGammaEnergy_.at(branch)).size()) return (fGammaEnergy_[branch])[gamma]; else return 0;}
        inline double GetPhantomNaive511Prob() const {return fPPhantom511_;}
        inline double GetPhantomNaivePromptProb() const {return fPPhantomPrompt_;}
        inline double GetPhantomUse() const {return fUsePhantom_;}
        inline bool GetPhantomSmear() const {return fPhantomSmear_;}
        //////////////////////////////////
        inline void SetR(float r) {fR_=r;}
        inline void SetL(float l) {fL_=l;}
        inline void SetEff(float eff) {fEff_=eff;}
        inline void SetE(float e) {fE_=e;} //in keV
        inline void SetP(float p) {fP_=p;}
        inline void SetSmearLowLimit(float limit) {fSmearLowLimit_=limit;}
        inline void SetSmearHighLimit(float limit) {fSmearHighLimit_=limit;}
        inline void SetNoOfGammas(int no) {fNoOfGammas_=no;}
        inline void EnableSilentMode() {fSilentMode_=true;}
        inline void DisableSilentMode() {fSilentMode_=false;}
        inline OutputOptions GetOutputType() const {return fOutput_;}
        inline void SetOutputType(OutputOptions type) {fOutput_=type;}
        inline EventTypeToSave GetEventTypeToSave() const {return fEventTypeToSave_;}
        inline void SetEventTypeToSave(EventTypeToSave type) {fEventTypeToSave_=type;}
        inline void SetSeed(int seed){fSeed_=seed;}
        inline void SetUseOfPhantom(bool isPhantom){fUsePhantom_=isPhantom;}
        inline void SetPhantomNaive511Prob(double p){fPPhantom511_=p;}
        inline void SetPhantomNaivePromptProb(double p){fPPhantomPrompt_=p;}
        inline void SetPhantomSmear(bool isSmear){fPhantomSmear_=isSmear;}
        //access source parameters
        std::vector<double> GetDataAt(const int index=0) const;

        //import parameters from external file

        void ImportParams(const std::string& inFile="simpar.par");
        void Import2nNdata(const std::string& inFile="2nN_data.dat");

        //print parameters to the stdout
        void PrintParams();
        void Print2nNdata();

    private:
        int fSimEvents_; //loaded from file
        int fSimRuns_; //calculated as the number of sets of source parameters
        int fNoOfGammas_;
        float fEff_; //scintillator's efficiency
        float fL_; //detector length
        float fR_; //detector radius
        float fE_; //energy in keV of additional gamma emitted in 2+1 event mode or gamma in 1-gamma mode
        float fP_; //probability of emitting an additional gamma in 2+1 event mode
        float fSmearLowLimit_; //lower limit for smearing effect
        float fSmearHighLimit_; //higher limit for smearing effect
        int fSeed_; //seed of the random generator, if set to 0 then different for different program executions
        bool fSilentMode_; //if set to true, less output to std::cout will be printed
        bool f2nNdataImported_; //set to true after importing 2&N data
        bool fUsePhantom_; //set true to use phantom
        double fPPhantom511_; //probability for a 511 keV phantom to scatter inside a phantom in naive mode
        double fPPhantomPrompt_; //probability for a prompt phantom to scatter inside a phantom in naive mode
        bool fPhantomSmear_;

        OutputOptions fOutput_; //what kind of output will be produced
        EventTypeToSave fEventTypeToSave_; //what kind of events should be saved
        std::vector<std::vector<double> > fData_; //this is where source parameters are stored
        //fields to store info for 2&N decays
        std::vector<double> fDecayBranchProbability_; //probability that a certain decay branch will be realized (can be abundance also)
        std::vector<std::vector<double> > fGammaEnergy_; //keV
        void ValidatePromptData_(); //validate the 2&N data

        friend class TwoAndNTestFixture; // for testing
};

#endif // PARAMMANAGER_H
