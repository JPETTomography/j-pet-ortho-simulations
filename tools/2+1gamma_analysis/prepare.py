#!/usr/bin/python
#author: Rafal Maselek
#email: rafal.maselek@ncbj.gov.pl

# this script generates EventHits files for a given .root file name, and folder name inside that file.

import sys
from ROOT import gROOT, TFile, TTree

# check for command line arguments
if(len(sys.argv) > 1):
    name = sys.argv[1]
else:
    name = 'sc44/sc44.root'
if(len(sys.argv) == 3):
    folder = sys.argv[2]
else:
    folder = "0_0_0_0_0_0"

# generate files
gROOT.Reset()
gROOT.ProcessLine(".L ../../src/event.cpp")
f = TFile(name)
tree = f.Get(folder+"/tree")
tree.MakeClass("EventHits")

# read the file
file = open("EventHits.h", "r")
contents = file.readlines()
file.close()

# add and write necessary content
contents.insert(14, "#include <vector>\n")
contents.insert(15, "#include <TLorentzVector.h>\n")
for ii in range(0, len(contents)):
    if "vector<" in contents[ii]:
        contents[ii] = contents[ii].replace("vector<", "std::vector<")
file = open("EventHits.h", "w")
contents = "".join(contents)
file.write(contents)
file.close()

