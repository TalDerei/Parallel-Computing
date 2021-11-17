/**
 * @file concurrent.h
 * 
 * Concurrent Implementation of Cuckoo Hash 
 */

#ifndef CONCURRENT_DEF
#define CONCURRENT_DEF

#include <iostream>
#include <vector>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <thread>
#include <algorithm>
#include <ctime>
#include <memory>
#include <atomic>
#include "config_t.h"

inline int REHASH_COUNT_CONCURRENT = 0;

#define HASHTABLES              2
#define EMPTY                   -1
#define RECURSION               10
#define RESIZE_PERCENTAGE       50

using namespace std;

template<typename K>
class concurrent {
    /** Bucket struct contianing key */
    struct bucket {
        std::atomic<K> key;
    };

    std::mutex mtx;
    std::mutex m1;

    /** 2D Vector with pointers to buckets as underlying data structure for hash table */
    std::vector<std::vector<shared_ptr<bucket>>> hashtable_t;

public: 
    /** Constructor initializing empty object */
    concurrent() {}

    /** Driver function for executing API calls */
    void driver(config_t &, concurrent<K> &);

    /** Spawn threads */
    void spawn_threads(config_t &, concurrent<K> &);

    /** Initialize hashtable */
    void initialize(K);

    /** Insert API function call */
    bool insert(K);

    /** Remove API function call */
    bool remove(K);

    /** Lookup API function call */
    std::pair<K, bool> lookup(K);

    /** Swap elements between hashtables */
    bool swap(int, K, int);

    /** Rehash and Resize hashtable */
    void rehash();

    /** Execute API calls */
    void run_tests(config_t &, concurrent<int> &);

    /** Hash function that switches between murmur_hash and bitwise_hash */
    int hash(int, K);

    /** Hash function adopted from CSE 109 */
    int bitwise_hash(K);

    /** Hash function using murmur hash library */
    int murmur_hash(K);

    /** Print hashtable */
    void print();
};

#endif