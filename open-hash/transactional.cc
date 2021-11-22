/**
 * @file transactional.cc
 * 
 * Software Transactional Memory (STM) Implementation of Cuckoo Hash 
 */

#include <iostream>
#include <vector>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <mutex>
#include <thread>
#include <algorithm>
#include <ctime>
#include <memory>
#include <atomic>
#include "config_t.h"
#include "murmurHash.h"
#include "hash.h"

#define EMPTY                   -1
#define HASHTABLES              2
#define RECURSION               10
#define RESIZE_PERCENTAGE       50

using namespace std;

template<typename K>
class transactional {
/** Bucket struct contianing key and lock */
struct bucket {
    std::atomic<int> key;
};

std::mutex m;

/** 2D Vector with pointers to buckets as underlying data structure for hash table */
std::vector<std::vector<shared_ptr<bucket>>> hashtable_t;

public: 
/** Constructor initializing empty object */
transactional() {}

void driver(config_t &config, transactional<K> &hashtable) {
    /** Spawn threads to execute API functions */    
    spawn_threads(config, hashtable);
}

void populate(K key_max) {
    /** Resize hashtable_t to include 2 hashtables */
    hashtable_t.resize(HASHTABLES);

    /** Populate each row with a 'key_max' number of pointers to buckets */
    for (int i = 0; i < HASHTABLES; i++) {
        for (int j = 0; j < key_max; j++) {
            /** Use push_back to construct the object with make_shared */
            hashtable_t[i].push_back(std::make_shared<bucket>());

            /** Set the value atomically */
            hashtable_t[i][j]->key.store(-1, std::memory_order_seq_cst);
        }
    }
}

void spawn_threads(config_t &config, transactional<K> &hashtable) {
    /** Vector of threads */
    std::vector<std::thread> thread_t;

    /** Spawn thread and insert into vector */
    for (int i = 0; i < config.threads; i++) {
        cout << "spawn threads!" << endl;
        thread_t.push_back(std::thread(run_tests, ref(config), ref(hashtable)));
    }

    // /** Synchronize threads by joining them */
    for (int i = 0; i < config.threads; i++) {
        cout << "Waiting for threads to finish!" << endl;
        thread_t[i].join();
    }
}

/** Lookup API function call */
std::pair<K, bool> lookup(K key) {
    if (hashtable_t[0][hash(0, key)]->key.load(std::memory_order_seq_cst) == key) {
        // cout << "Found " << key << " in first hashtable!" << endl;
        return std::make_pair(0, true);  
    }
    else if (hashtable_t[1][hash(1, key)]->key.load(std::memory_order_seq_cst) == key) {
        // cout << "Found " << key << " in second hashtable!" << endl;
        return std::make_pair(1, true);        
    }
    // cout << "Key " << key << " NOT found in either hashtable!" << endl;
    return std::make_pair(-1, false);
}

/** Insert API function call */
bool insert(K key) {
    int table = 0, recursion = 0;
    
    /** Check if key already exists in hashtable, return false */
    if (lookup(key).second) {
        return false;            
    }

    /** Swap() is a wrapper function for insert, using recursion to identify loops */
    swap(table, key, recursion);

    /** Return true if key inserted into hashtable */
    return true;
}

/** Remove API function call */
bool remove(K key) {
    std::pair<K, bool> found = lookup(key);
    int table = found.first;

    /** Check if key doesn't exist in hashtable, return false */
    if (!found.second) {
        return false;            
    }

    /** If key found in either table, remove key and return true */
    if (table == 0) {
        hashtable_t[table][hash(table, key)]->key.store(-1, std::memory_order_seq_cst);
        // cout << "Removed " << key << " successfully from " << "table " << table << endl;
    }
    else if (table == 1) {
        hashtable_t[table][hash(table, key)]->key.store(-1, std::memory_order_seq_cst);
        // cout << "Removed " << key << " successfully from " << "table " << table << endl;
    }
    return true;
}

/** Swap elements between hashtables */
// static __attribute__((transaction_pure))
bool swap(int table, K key, int recursion) {  
    /** Thread ID executing the API call */
    // cout << "Thread executing insert() operation is: " << std::this_thread::get_id() << endl;
    
    /** If loop is detected, rehash hashtables */ 
    if (recursion == RECURSION) {
        cout << "Loop detected. Triggering RESIZE AND REHASH!" << endl;

        /** Single-threaded access to rehash() function */
        rehash();

        return true;
    }

    /** Calculate index and lock the entry in the table */
    int index = hash(table, key);

    synchronized {
        /* If hashtable index already contains an element, swap the element */
        if (hashtable_t[table][index]->key.load(std::memory_order_seq_cst) != EMPTY) {
            // cout << "Index " << index << " in hashtable " << table << " is already populated! Swapping it!" << endl;

            K val = std::atomic_load(&hashtable_t[table][index]->key);
            hashtable_t[table][index]->key.store(key, std::memory_order_seq_cst);
            swap((table + 1) % HASHTABLES, val, recursion + 1);

            return true;
        }
        // /* Else insert the element into the empty index */
        else {
            hashtable_t[table][index]->key.store(key, std::memory_order_seq_cst);

            return true;
        }
    }
    return true;
}

/** Rehash and Resize hashtable */
bool rehash() {
    /** lock all the buckets in a transaction using STM */
    synchronized {
        /** Define temporary vector to hold elements */
        std::vector<std::vector<K> > temp_hashtable_one;
        std::vector<std::vector<K> > temp_hashtable_two;    

        /** Create temporary hashtables with new size */
        temp_hashtable_one.resize(1, vector<K>(hashtable_t[0].size()));
        temp_hashtable_two.resize(1, vector<K>(hashtable_t[1].size()));

        // /** Copy old hashtable to temporary hashtable */
        for (int i = 0; i < (int) hashtable_t[0].size(); i++) {
            temp_hashtable_one[0][i] = hashtable_t[0][i]->key.load(std::memory_order_seq_cst);
        }  
        for (int i = 0; i < (int) hashtable_t[1].size(); i++) {
            temp_hashtable_two[0][i] = hashtable_t[1][i]->key.load(std::memory_order_seq_cst);
        }       

        /** Save size of old hashtables */
        int hashtable_one = hashtable_t[0].size();
        int hashtable_two = hashtable_t[1].size();

        /** Resize old hashtable size */
        int hashtable_one_new = hashtable_t[0].size() * (1+((float)RESIZE_PERCENTAGE)/100);
        int hashtable_two_new = hashtable_t[1].size() * (1+((float)RESIZE_PERCENTAGE)/100);

        /** Set old hashtable values to -1, declaring them empty */
        for (int j = 0; j < hashtable_one; j++) {
            hashtable_t[0][j]->key.store(-1, std::memory_order_seq_cst);
        }
        for (int j = 0; j < hashtable_two; j++) {
            hashtable_t[1][j]->key.store(-1, std::memory_order_seq_cst);
        }

        /** Alloctate new space in resized hashtables */
        for (int j = hashtable_one; j < hashtable_one_new; j++) {
            hashtable_t[0].push_back(std::make_shared<bucket>());
            hashtable_t[0][j]->key.store(-1, std::memory_order_seq_cst);
        }

        for (int j = hashtable_two; j < hashtable_two_new; j++) {
            hashtable_t[1].push_back(std::make_shared<bucket>());
            hashtable_t[1][j]->key.store(-1, std::memory_order_seq_cst);
        }

        /** Copy values back to old hash table */
        for (int k = 0; k < hashtable_one; k++) {
            hashtable_t[0][hash(0, temp_hashtable_one[0][k])]->key.store(temp_hashtable_one[0][k], std::memory_order_seq_cst);
        }
        for (int k = 0; k < hashtable_two; k++) {
            hashtable_t[1][hash(1, temp_hashtable_two[0][k])]->key.store(temp_hashtable_two[0][k], std::memory_order_seq_cst);
        }
   }
    return true;
}

/** Hash function that switches between murmur_hash and bitwise_hash */
int hash(int table, K key) {
    switch (table) {
        case 0: return murmur_hash(key);
        case 1: return bitwise_hash(key);
    }
    return 0;
}

/** Hash function using murmur hash library */
int murmur_hash(K key) {
    uint64_t seed = 100;
    uint64_t hash_otpt[2]= {0};
    const int *key_hash = &key;
    MurmurHash3_x64_128(key_hash, sizeof(K), seed, hash_otpt); 
    int index = *hash_otpt % hashtable_t[0].size();
    return index;
}

/** Hash function from xxHash libraru */
int bitwise_hash(K key) {
    uint hash = (uint) xxh::xxhash<32>({key});
    int index = hash % hashtable_t[1].size();
    return index;
}

static void run_tests(config_t &config, transactional<K> &hashtable) {
    /* API counters */
    int lookup_true = 0, lookup_false = 0;
    int insert_true = 0, insert_false = 0;
    int remove_true = 0, remove_false = 0;

    /** Randomly call insert/remove/lookup APIs */ 
    srand(time(0));
    for (int i = 0; i < config.iterations / config.threads; i++) {
        int key = rand() % config.key_max; 
        int opt = rand() % 100;
        if (opt < 80) {
            // cout << "LOOKUP OPERATION!" << endl;
            if (hashtable.lookup(key).second) {
                lookup_true++;
            }
            else {
                lookup_false++;
            }
        } 
        else if ((80 < opt) && (opt < 90)) {
            // cout << "INSERT OPERATION!" << endl;
            if (hashtable.insert(key)) {
                insert_true++;
            }
            else {
                insert_false++;
            }            
        } 
        else {
            // cout << "REMOVE OPERATION!" << endl;
            if (hashtable.remove(key)) {
                remove_true++;
            }
            else {
                remove_false++;
            }
        }
    }

    /** Print the number of operations completed */
    cout << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "number of 'lookup' operations: " << lookup_true + lookup_false << endl;
    cout << "number of lookups succeeded: " << lookup_true << endl;
    cout << "number of lookups failed: " << lookup_false << endl;
    cout << "number of 'insert' operations: " << insert_true + insert_false << endl;
    cout << "number of inserts succeeded: " << insert_true << endl;
    cout << "number of inserts failed: " << insert_false << endl;
    cout << "number of 'remove' operations: " << remove_true + insert_false << endl;
    cout << "number of remove succeeded: " << remove_true << endl;
    cout << "number of remove failed: " << remove_false << endl;
    cout << endl;
}

/** Print hashtable */
void print() {
    cout << "First and Second Hashtables: " << endl;
    for (int i = 0; i < (int) hashtable_t.size(); i++) {
        for (int j = 0; j < (int) hashtable_t[i].size(); j++) {
            cout << hashtable_t[i][j]->key.load(std::memory_order_seq_cst) << " ";
        }
        cout << endl << endl;
    }
    cout << endl;
    /** Print size of hashtables */
    cout << "Size of first hashtable: " << hashtable_t[0].size() << endl;
    cout << "Size of second hashtable: " << hashtable_t[1].size() << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
}
};

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
    transactional<int> transaction_hashtable;

    /** Initialize data structure to half the size of the key_length */
    transaction_hashtable.populate(config.key_max);

    /* Start execution time */
    auto start = chrono::high_resolution_clock::now();

    /** Driver function for executing API calls */
    transaction_hashtable.driver(config, transaction_hashtable);

    /** Finish execution time */
    auto finish = chrono::high_resolution_clock::now();

    /** Duration represents time elapsed */
    chrono::duration<double> elapse_time = finish - start;

    transaction_hashtable.print();

    /** Print execution time */
    std::cout << "Execution time elapsed is: " << elapse_time.count() << endl;
}