// CSE 375/475 Assignment #1
// Fall 2021
//
// Description: This file declares a struct for storing per-execution configuration information.

#include <iostream>
#include <string>

// store all of our command-line configuration parameters

struct config_t {

  // The maximum key value
    int key_max;

    // The number of iterations for which a test should run
    int iters;

    // A string that is output with all the other information
    std::string  name;

    // The number of threads to use
    int threads;

    // simple constructor
    config_t() : key_max(256), iters(1024), name("no_name"), threads(1) { }

    // Print the values of the iters, and name fields
    void dump();
};
