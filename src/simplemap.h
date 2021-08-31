// CSE 375/475 Assignment #1
// Fall 2021
//
// Description: This file specifies a custom map implemented using two vectors.
// << use templates for creating a map of generic types >>
// One vector is used for storing keys. One vector is used for storing values.
// The expectation is that items in equal positions in the two vectors correlate.
// That is, for all values of i, keys[i] is the key for the item in values[i].
// Keys in the map are not necessarily ordered.
//
// The specification for each function appears as comments.
// Students are responsible for implementing the simple map as specified.

#include <vector>
#include <cassert>


template <class K, class V>
class simplemap_t {

    // Define the two vectors of types K and V
	// << use std::vector<K> >>


  public:

    // The constructor should just initialize the vectors to be empty
    simplemap_t() {
        assert("Not Implemented");
    }


    // Insert (key, val) if and only if the key is not currently present in
    // the map.  Returns true on success, false if the key was
    // already present.
    bool insert(K key, V val) {

    	assert("Not Implemented");
    	return true;

    	// The following is just an example of using C++11 features,
    	// like the 'auto' type and lambda expression
    	/*
        for (auto i = keys->begin(); i != keys->end(); ++i)
            if (*i == key)
                return false;

        // insert the key and value.  Note that indices should, by default, match
        // between these two vectors.
        keys->push_back(key);
        values->push_back(val);
		*/


    }

    // If key is present in the data structure, replace its value with val
    // and return true; if key is not present in the data structure, return
    // false.
    bool update(K key, V val) {
        assert("Not Implemented");
        return false;
    }

    // Remove the (key, val) pair if it is present in the data structure.
    // Returns true on success, false if the key was not already present.
    bool remove(K key) {
        assert("Not Implemented");
        return false;
    }

    // If key is present in the map, return a pair consisting of
    // the corresponding value and true. Otherwise, return a pair with the
    // boolean entry set to false.
    // Be careful not to share the memory of the map with application threads, you might
    // get unexpected race conditions
    std::pair<V, bool> lookup(K key) {
        assert("Not Implemented");
        //TO DO: the following is a default return value, do not use it!
        return std::make_pair(0, false);
    }

    // Apply a function to each key in the map
    void apply(void (*f)(K, V)) {
    	for (auto i : *keys) {
    		f(i, values->at(i));
    	}
    }
};



/* Header file for Set.cc */

// #ifndef INITIALIZATION_DEF
// #define INITIALIZATION_DEF

// #include <map>

// using namespace std;

// template <typename K, typename V>
// class Initialization {
//     public:
//         void initialize_map(map<K, V> &);

//     struct Account {
//         int id = 0;
//         float balance = 0.0;
//     };
// };

// #endif 

// /**
//  * @file initialization.cc
//  * Initialize map<int, float> 
//  */

// #include <iostream>
// #include <string>
// #include <unistd.h>
// #include "initialization.h"

// using namespace std;

// template <typename K, typename V>
// void Initialization<K,V>::initialize_map(map<K, V> &my_map) {
//       my_map.insert(pair<int,float>(1,2.2));
//       std::cout << "1 => " << my_map.find(1)->second << '\n';
// }

// map<int, float> my_map;
//     Initialization<int,float> i;
//     i.initialize_map(my_map);