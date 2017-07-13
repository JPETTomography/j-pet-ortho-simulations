/// @file generatedict.cpp
/// @author Rafal Maselek <rafal.maselek@ncbj.gov.pl>
/// @date 13.07.2017
///
/// This root macro generates a dictionary needed to correctly used user's classes that inherit from TObject.
/// It is called by the Makefile.
///
#include <string>
#include "TROOT.h"
///
/// \brief generatedict Generates dictionary files.
/// \param path Absolute path to the src folder.
///
void generatedict(char* path)
{
    std::string macroPath(path);
    gSystem->cd(macroPath.c_str());
    const char* command = (".L "+macroPath+"event.cpp++").c_str();
    gROOT->ProcessLine(command);
}
