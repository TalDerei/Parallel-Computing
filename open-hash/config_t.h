/**
 * @file config_t.h 
 */

#ifndef CONFIG_DEF
#define CONFIG_DEF

#include <iostream>
#include <string>

/**
 * Configuration information for command-line parameters
 */
struct config_t {
    /** Maximum key-value */
    int key_max = 0;

    /** Number of iterations */
    int iterations = 0;

    /** Number of threads */
    int threads = 0;

    /** Execution method -- sequential, parallel, transactional */
    std::string method = "";

    /** Display help message */
    bool usage = false;

    /** Data type (int, float, string) used in data structure */
    std::string data_type = "";
};

#endif