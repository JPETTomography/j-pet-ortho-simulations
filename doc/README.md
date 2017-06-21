# Generating documentation

To generate automatic documentation you have to have [Doxygen](http://www.stack.nl/~dimitri/doxygen/) installed.
On Debian-family systems it can be done using **apt-get**:
>sudo apt-get install doxygen

Then cd to the main directory, where **Doxyfile** is placed. Generate documentation by typing:
>doxygen Doxyfile

It should appear in the doc/ directory.
