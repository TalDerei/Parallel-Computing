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

#include "sequential.h"
#include "concurrent.h"
#include "config_t.h"

/** Helper method displaying help message */
void usage() {
    std::cout
        << "Command-Line Options:" << std::endl
        << "  -a <string> : a phrase to help differentiate experiments" << std::endl
        << "  -b <int>    : the upper maximum key value when choosing random keys" << std::endl
        << "  -c <int>    : the number of iterations per thread" << std::endl
        << "  -d <int>    : the number of threads in the experiment" << std::endl
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

    /** Instantiate hashtable objects and call constructor */ 
    sequential<int> seq_hashtable;
    concurrent<int> conc_hashtable;

    /** Initialize data structure to half the size of the key_length */
    if (config.method == "sequential") {
        seq_hashtable.populate(config.key_max);
    } 
    else if (config.method == "concurrent") {
        conc_hashtable.populate(config.key_max);
    }

    /* Start execution time */
    auto start = chrono::high_resolution_clock::now();

    /** Driver function for executing API calls */
    if (config.method == "sequential") {
        seq_hashtable.driver(config, seq_hashtable);
    } 
    else if (config.method == "concurrent") {
        conc_hashtable.driver(config, conc_hashtable);
    }

    /** Finish execution time */
    auto finish = chrono::high_resolution_clock::now();

    /** Duration represents time elapsed */
    chrono::duration<double> elapse_time = finish - start;

    /** Print hashtable */
    if (config.method == "sequential") {
        seq_hashtable.print();
    } 
    else if (config.method == "concurrent") {
        conc_hashtable.print();
    }

    /** Print execution time */
    std::cout << "Execution time elapsed is: " << elapse_time.count() << endl;
}