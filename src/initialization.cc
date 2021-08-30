/**
 * @file initialization.cc
 * Initialize map<int, float> 
 */

#include <iostream>
#include <string>
#include <unistd.h>
#include "initialization.h"

void Initialization::initialize_map(std::map<int, float> &my_map) {
      my_map.insert(std::pair<int,float>(1,2.2));
      std::cout << "1 => " << my_map.find(1)->second << '\n';
}