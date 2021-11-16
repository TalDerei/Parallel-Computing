/**
 * @file main.cc
 */

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <unordered_map>

#include "sequential.h"
#include "tests.h"
#include "config_t.h"

/** Helper method displaying help message */
void usage() {
    std::cout
        << "Command-Line Options:" << std::endl
        << "  -a <string> : a phrase to help differentiate experiments" << std::endl
        << "  -b <int>    : the upper maximum key value when choosing random keys" << std::endl
        << "  -c <int>    : the number of iterations per thread" << std::endl
        << "  -d <int>    : the number of threads in the experiment" << std::endl
        << "  -e <int>    : data type used in the data structure (int, float)" << std::endl
        << "  -h          : display this message and exit" << std::endl << std::endl;
    exit(0);
}

/** Parse command-line arguments */
void parseargs(int argc, char** argv, config_t& config) {
    int opt;
    while ((opt = getopt(argc, argv, "a:b:c:d:e:h")) != -1) {
        switch (opt) {
            case 'a': config.method = std::string(optarg); break;
            case 'b': config.key_max = atoi(optarg); break;  
            case 'c': config.iterations = atoi(optarg); break;
            case 'd': config.threads = atoi(optarg); break;
            case 'e': config.data_type = atoi(optarg); break;
            case 'h': usage(); break;
        }
    }
}

/** Main function */
int main(int argc, char **argv) {
    /** CLI configuration parameters */
    config_t config;
    parseargs(argc, argv, config);

    /** Instantiate hashtable object and call constructor */ 
    sequential<int> hashtable;

    /** Insert/Remove/Lookup API function calls */
    run_tests(config, hashtable);

    /** Print hashtable */
    hashtable.print();
}