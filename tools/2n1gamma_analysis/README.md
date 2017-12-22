# AUTHOR: Rafał Masełek

# This is a tool that draws histograms that help to estimate the treshold energy forisotopes like Sc44, which decays into positron and an additional gamma.

## To use the software do the following:
* Copy data file (e.g. "result.root") somewhere into this directory (e.g. "sc44/result.root")
* Chceck via TBrowser the name of the folder inside the .root file (e.g. "0_0_0_0_0_0")
* Run the _prepare.py_ script like this:
`python prepare.py sc44/result.root 0_0_0_0_0_0`
* Build the tool by typing `make`
* Run the software `./analysis`
* Results will be saved into this directory as .png files