# jpos2gate converter

## Author: Rafał Masełek
## Email: rafalmaselek@gmail.com

### About:
This a program to convert root outputs between JPOS and GATE. 
!!! It is Beta version: no optimalization, little tests were made.

### Requirements:
+ JPOS
+ pyROOT
+ Python 2.7 or newer
+ bash concole

### Installation:
To build the application type *make* in the console. Some file from the JPOS src directory will be copied and application will be built.

### Running:
Instead of directly running builded C++ application, use the provided convert.py Python script:
>python convert.py [path to input file] [name of output file] [1 for JPOS->GATE conversion, 0 for GATE->JPOS]

Example GATE->JPOS:
>python convert ~/misie_pysie/gate_output.root "my_jpos_output.root" 0


Example JPOS->GATE:
>python convert ~/pysie_mysie/jpos_output.root "my_gate_output" 1

Note that providing root extension is non-mandatory.

### JPOS->GATE:
Since it is Beta version, there are some constraints. The following fields in Hits tree (GATE output) will be filled with data:
+ time
+ sourcePosX
+ sourcePosY
+ sourcePosZ
+ posX
+ posY
+ posZ
+ edep
+ eventID

The rest will be filled with some default values, that can be checked in main::jpos2gate function.

If the folder name inside JPOS root is not "0_0_0_0_0_0", see section *"More on input files"*

### GATE->JPOS:
Since it is Beta version, there are some constraints. All JPOS files created by this converter will contain
folder named "0_0_0_0_0_0" no matter what was the source in input (GATE) file.


### More on input files
jpos2gate converter uses automatically generated ROOT template classes to read data. It allows to use program for different data files without the need to recompile, as long as they have the same structure as the original ones. Unfortunately, JPOS output files contain subdirectories with names indicating the position and momentum of the source, so if one wants to convert JPOS file with subdirectory other than "0_0_0_0_0_0", one must regenerate template classes and recompile the converter. The instruction (algorithm) of what to do is provided below:

#### You want to convert GATE->JPOS or JPOS->GATE with subdirectory named "0_0_0_0_0_0"? 

Simply use the convert.py script as described in section *"Running"*.

#### You want to convert JPOS->GATE with subdirectory named OTHER than "0_0_0_0_0_0"?

You have to:
1. Use prepare.py Python script to generate template classes. Type in bash console:
>python prepare.py [path to JPOS root file] [path to GATE root file] [name of the subdirectory]

**NOTE1:** You can use "data/gate.root" as a second parameter if you want.

**NOTE2:** If your JPOS output file contains only one subdirectory, then providing only one parameter -- the path to JPOS
root file should be sufficient.

2. Compile the converter:

>make

3. Use convert.py Python script as described in section *"Running"*

**IMPORTANT NOTE:** When you generate template classes for some subdirectory name, conversion 
JPOS->GATE will be available only for JPOS root files containing subdirectories with the same name.
To use it for different name, you have to redo the procedure (Do it if you want to be able to convert files
with subdirectory named "0_0_0_0_0_0").