# Running tests on Linux

## Install gtest

    sudo apt-get install libgtest-dev

    sudo apt-get install cmake # install cmake
    cd /usr/src/gtest
    sudo cmake CMakeLists.txt
    sudo make
 
    # copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
    sudo cp *.a /usr/lib


## Compile tests

    g++ ../src/MCP342x.cpp MCP342x.test.cpp -I . -I ../src -l gtest -l pthread -std=c++11

