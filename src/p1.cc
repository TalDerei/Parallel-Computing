// CSE 375/475 Assignment #1
// Fall 2021
//
// Description: This file parses command line options, outputs them, and
// then runs the test function.

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <stdlib.h>
#include <chrono>

#include "simplemap.h"
#include "config_t.h"
#include "tests.h"

// Report on how to use the command line to configure this program
void usage() {
    std::cout
        << "Command-Line Options:" << std::endl
        << "  -n <string> : a phrase to help differentiate experiments" << std::endl
        << "  -k <int>    : the upper maximum key value when choosing random keys" << std::endl
        << "  -i <int>    : the number of iterations per thread" << std::endl
        << "  -t <int>    : the number of threads in the experiment" << std::endl
        << "  -h          : display this message and exit" << std::endl << std::endl;
    exit(0);
}

// Parse command line arguments using getopt()
void parseargs(int argc, char** argv, config_t& cfg) {
    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "n:k:i:t:h")) != -1) {
        switch (opt) {
          case 'n': cfg.name = std::string(optarg); break;
          case 'k': cfg.key_max = atoi(optarg); break;
          case 'i': cfg.iters = atoi(optarg); break;
          case 't': cfg.threads = atoi(optarg); break;
          case 'h': usage(); break;
        }
    }
}

// Main routine simply parses the arguments, dumps the arguments, populates the map
int main(int argc, char** argv) {
    // Configuration parameters
    config_t config;
    parseargs(argc, argv, config);
    config.dump();

    // Instantiate simplemap_t object and call constructor 
    simplemap_t<int, float> simple_map;

    // crash if the iterations are negative
    assert(config.iters > 0);

    // launch the tests
    test_driver(config);
}
