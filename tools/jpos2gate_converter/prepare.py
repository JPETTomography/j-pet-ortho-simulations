#!/usr/bin/python
#author: Rafal Maselek
#email: rafal.maselek@ncbj.gov.pl

# this script generates EventHits files for a given .root file name, and folder name inside that file.

import sys
from ROOT import gROOT, TFile, TTree
import os
import shutil

# check for command line arguments
if(len(sys.argv) > 1):
    nameJPOS = sys.argv[1]
else:
    nameJPOS = 'data/jpos.root'
if(len(sys.argv) > 2):
    nameGATE = sys.argv[2]
else:
    nameGATE = 'data/gate.root'
if(len(sys.argv) == 4):
    folder = sys.argv[3]
else:
    folder = None

# generate files
gROOT.Reset()
gROOT.ProcessLine(".L ../../src/event.cpp")

#########################################################################################################
#########################################################################################################
#########################################################################################################

# FIRST DEAL WITH JPOS DATA
f = TFile(nameJPOS)
if folder:
	tree = f.Get(folder+"/tree")
else:
	tree = f.FindObjectAny("tree")
# tree.__class__ = TTree

tree.MakeClass("JPOSOutput")

thisFile = "JPOSOutput.C"
base = os.path.splitext(thisFile)[0]
newThisFile = base + ".cpp"
os.rename(thisFile, newThisFile)
# read the file
file = open("./JPOSOutput.h", "r")
contents = file.readlines()
file.close()

# add and write necessary content
contents.insert(14, "#include <vector>\n")
contents.insert(15, "#include <TLorentzVector.h>\n")
for ii in range(0, len(contents)):
    if "vector<" in contents[ii]:
        contents[ii] = contents[ii].replace("vector<", "std::vector<")
file = open("./JPOSOutput.h", "w")
contents = "".join(contents)
file.write(contents)
file.close()

path1 = os.path.abspath(newThisFile)
path2 = file.name
shutil.move(path1, os.path.join(os.path.dirname(path1), 'src/', newThisFile))
shutil.move(path2, os.path.join(os.path.dirname(path2), 'src/',  os.path.basename(path2)))
f.Close()
#########################################################################################################
#########################################################################################################
#########################################################################################################

# SECONDLY DEAL WITH GATE DATA
f = TFile(nameGATE)
tree = f.Get("Hits")
tree.MakeClass("GateOutput")

thisFile = "GateOutput.C"
base = os.path.splitext(thisFile)[0]
newThisFile = base + ".cpp"
os.rename(thisFile, newThisFile)

path1 = os.path.abspath(newThisFile)
path2 = os.path.abspath("GateOutput.h")
shutil.move(path1, os.path.join(os.path.dirname(path1), 'src/', newThisFile))
shutil.move(path2, os.path.join(os.path.dirname(path2), 'src/',  "GateOutput.h"))
f.Close()