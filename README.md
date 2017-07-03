#DEVELOPMENT BRANCH

# j-pet-ortho-simulations

## Author: Rafał Masełek
## Email: rafal.maselek@ncbj.gov.pl

### Requirements:
+ ROOT 5 (You may have to manually load the environment, if it is not set in *~/.bashrc* To learn how to do it see ROOT installation manual.
+ GCC (default version on cluster is too old, load more recent one; tested on 5.4.0)
+ Unix (tested on Kubuntu 16.04)

### Installation:
To build the application download all files from the repository into a destination folder. Cd to that folder
and type *make* in the bash console.

### Running:
To run the application type 
>./sim -i param_file 
**param_file** is a path to a file, where simulation parameters are stored. If the flag '-i'  is not provided, the program will try to read file "simulation_parameters.par".

### Changing the simulation parameters
For details see simulation_parameters.par file.

### Results 
By deault all results will be saved to the *results/* directory. You can change it by editing src/simulate.cpp file. There are static variables at the beginning of the file called:
_globalPrefix_, _comptonPrefix_ and _decaysPrefix_.

### Documentation
Documentation, both HTML and LATEX, is stored in the doc/ directory. Comments inside the code are also provided for developers and advanced users. 
