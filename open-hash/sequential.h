/**
 * @file sequential.h
 * 
 * Sequential Implementation of Cuckoo Hash 
 */

#ifndef SEQUENTIAL_DEF
#define SEQUENTIAL_DEF

#include <iostream>
#include <vector>
#include <functional>
#include <stdio.h>
#include <string.h>
#include "config_t.h"

#define HASHTABLES              2
#define EMPTY                   0
#define RECURSION               10
#define RESIZE_PERCENTAGE       50

inline int REHASH_COUNT_SEQUENTIAL = 0;

using namespace std;

template<typename K>
class sequential {
    /** Bucket struct contianing key */
    struct bucket {
        bool empty;
        K key;
    };

    /** 2D Vector with pointers to buckets as underlying data structure for hash table */
    std::vector<std::vector<bucket *> > hashtable_t;

public: 
    /** Constructor initializing empty object */
    sequential() {}

    /** Driver function for executing API calls */
    void driver(config_t &, sequential<K> &);

    /** Initialize and populate hashtable */
    void populate(K);

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