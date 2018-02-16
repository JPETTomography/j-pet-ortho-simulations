#!/usr/bin/python 
"""
@author: Rafal Maselek
@email: rafalmaselek@gmail.com
This script copies input data files into data folder, change their name to meet the requirements
of the converter application and runs the program.
"""
import sys
import os

if len(sys.argv) < 4:
	print("Insufficient number of arguments!")
	print("python convert.py [input file path] [output file name] [1 if goja2jpos, 0 otherwise]")
	sys.exit(1)
else:
	# input file must have specific name
	if sys.argv[3]:
		in_name = "gate.root"
	else:
		in_name="jpos.root"
	# check if user provided a name of the output file with or without extension
	if sys.argv[2][-5:] == ".root":
		out_name = sys.argv[2]
	else:
		out_name = sys.argv[2]+".root"
	# prepare and execute bash commands
	cp_command = "cp "+sys.argv[1]+" ./data/"+in_name
	command = "./converter"+" "+out_name+" "+sys.argv[3]
	os.system(cp_command)
	os.system(command)
