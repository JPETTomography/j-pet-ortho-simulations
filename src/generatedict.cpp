#include "TROOT.h"
#include <string>
void generatedict(char* path)
{
    std::string macroPath(path);
    gSystem->cd(macroPath.c_str());
    const char* command = (".L "+macroPath+"event.cpp++").c_str();
    gROOT->ProcessLine(command);
}
