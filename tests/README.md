# NEEDS UPDATING!

# AUTHOR: Rafał Masełek
## EMAIL: rafal.maselek@ncbj.gov.pl

These are tests for j-pet-ortho-simulations repository.
They are based on a google test framework.
>Copyright 2008, Google Inc.
All rights reserved.

### Requirements:
+ Google test framework
+ Boost library

### Installation of Google test framework under Ubuntu:
`sudo apt-get install libgtest-dev`

`cd /usr/src/gtest`

`cmake CMakeList.txt`

`make`

`cp *.a /usr/lib`

### Installation of Boost library under Ubuntu:
`sudo apt-get install libboost-all-dev`

### Building tests:
Simply run `make` in the *tests/* directory.

### Usage:
To run all tests:
`./testAll`

To run all tests 100 times:
`./testAll --gtest_repeat=100`

TO run all tests 100 times and break if any of them fails:
`./testAll --gtest_repeat=100 --gtest_break_on_failure`


