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

#ifndef SIMPLEMAP_DEF
#define SIMPLEMAP_DEF

#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>

template <class K, class V>
class simplemap_t {
    
    // The map of key/value pairs
    std::unordered_map<K,V> map;
    
    // Define the two vectors of types K and V
	// << use std::vector<K> >>
    std::vector<K> keys;
    std::vector<V> values;

  public:
    // The constructor should just initialize the vectors to be empty
    simplemap_t() {
        std::vector<K> keys;
        std::vector<V> values;
    }
    
    // initialize unordered map
    void initialize_map(K, V);

    // Insert (key, val) if and only if the key is not currently present in
    // the map.  Returns true on success, false if the key was
    // already present.
    void insert(K, V);

    // If key is present in the data structure, replace its value with val
    // and return true; if key is not present in the data structure, return
    // false.
    bool update(K, V);

    // Remove the (key, val) pair if it is present in the data structure.
    // Returns true on success, false if the key was not already present.
    bool remove(K);

    // If key is present in the map, return a pair consisting of
    // the corresponding value and true. Otherwise, return a pair with the
    // boolean entry set to false.
    // Be careful not to share the memory of the map with application threads, you might
    // get unexpected race conditions
    std::pair<V, bool> lookup(K);

    // Apply a function to each key in the map
    void apply(void (*f)(K, V));

    void init(K, V);
};

#endif