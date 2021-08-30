/**
 * @file main.cc
 * Transaction system that performs bank deposit trasnfers between accounts using parallelism
 * (multiple threads) and concurrency (safe, synchronized access to shared resources)
 */
#include <iostream>
#include "initialization.h"

int main(int argc, char **argv) {
    std::map<int, float> my_map;
    Initialization initialization;
    initialization.initialize_map(my_map);
}