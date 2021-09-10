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
#include <string>
#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <vector>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>
#include <functional>
#include <mutex>
#include <thread>

#define BALANCE 100000.0
#define AMOUNT 10.0
#define NUMBUCKETS 5

template <class K, class V>
class simplemap_t {    
  // The map of key/value pairs
  std::unordered_map<K,V> map;

  // Lockable unordered_map of key/value pairs
  struct bucket_t;

  // Number of buckets, each bucket representing an account
  size_t const num_buckets = NUMBUCKETS;

  // Vector of buckets
  std::vector<bucket_t *>buckets;

  // Vector of threads
  std::vector<std::thread> thread_vec;

  public:
    // The constructor initializes an empty simplemap_t object 
    simplemap_t() {}

    // Construct concurrent hash table
    void concurrent_hashtable(size_t);

    // Insert (key, val) if and only if the key is not currently present in
    // the map.  Returns true on success, false if the key was
    // already present.
    void insert(K, V);

    // Remove the (key, val) pair if it is present in the data structure.
    // Returns true on success, false if the key was not already present.
    void remove(K);

    // If key is present in the map, return a pair consisting of
    // the corresponding value and true. Otherwise, return a pair with the
    // boolean entry set to false.
    // Be careful not to share the memory of the map with application threads, you might
    // get unexpected race conditions
    std::pair<V, bool> lookup(K);

    // If key is present in the data structure, replace its value with val
    // and return true; if key is not present in the data structure, return
    // false.
    void update_subtract(K, V, K);

    void update_add(K, V, K);

    // Apply a balance function to each key in the map
    void apply_balance(std::function<void(K,V)>);

    // Apply a deposit function to each key in the map
    void apply_deposit(std::function<void(K,K,V,K,K)>);

    // Print unordered map
    void print();

    // Record exeuction time for unordered map
    void init(K);
};

#endif