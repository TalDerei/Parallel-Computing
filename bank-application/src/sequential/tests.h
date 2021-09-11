// CSE 375/475 Assignment #1
// Fall 2021

// Description: This file declares a function that should be able to use the
// configuration information to drive tests that evaluate the correctness and
// performance of the map type you create.

#ifndef TESTS_DEF
#define TESTS_DEF

#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <functional>
#include "config_t.h"
#include "simplemap.h"

using namespace std;

// Run a bunch of tests to ensure the data structure works correctly
void test_driver(config_t &, simplemap_t<int, float> &);

// Execute deposit and balance APIs
void run_custom_tests(config_t &, simplemap_t<int, float> &);

// Balance of an account
float balance(simplemap_t<int, float> &);

// Deposit from one another to another
void deposit(simplemap_t<int, float> &);

#endif