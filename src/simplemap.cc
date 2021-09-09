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

#include "simplemap.h"
#include "tests.h"

using namespace std;

#define BALANCE 100000.0

template<class K, class V> 
void simplemap_t<K,V>::init(K key, int iterations, int threads) {
    /* Populate map with (key,values) */
    initialize_map(key);

    /* Map iterator */
    std::unordered_map<int, float>::iterator it;

     /* Start execution time */
    auto start = chrono::high_resolution_clock::now();

    
    /* Randomly switch between different set operations */
    srand(time(0));
    int read_only_ratio = 0;
    for (int counter = 0; counter < iterations; counter++) {
        long opt = rand() % 100;
        int rand_key = rand() % key; 
        float rand_value = BALANCE / (float)((key / 2)); 
        if (opt <= read_only_ratio) {
            lookup(rand_key);
        } 
        else if (opt > read_only_ratio && opt <= (((100-read_only_ratio)/2)+read_only_ratio)) {
            insert(rand_key, rand_value);
        }
        else if (opt > (((100-read_only_ratio)/2)+read_only_ratio) && opt <= 100) {
            remove(rand_key);
        }
    }

    /* Finish execution time */
    auto finish = chrono::high_resolution_clock::now();

    /* Duration represents time interval */
    chrono::duration<double> elapse_time = finish - start;

    /* Print elements of map */
    for (it = map.begin(); it != map.end(); ++it) {
        cout << '\t' << it->first
             << '\t' << it->second << '\n';
    }

    std::cout << "Execution time elapsed is: " << elapse_time.count() << endl;

    std::cout << "size of map is: " << map.size() << endl;
}

template<class K, class V> 
void simplemap_t<K, V>::initialize_map(K key) {
    int counter = 0;
    int balance = 0;
    srand(time(0));
    while (counter != key / 2) {
        int key_insert = rand() % key;
        if (!(lookup(key_insert).second)) {
            float val_insert = (float) (BALANCE / (key / 2));
            map.insert({key_insert, val_insert});
            counter++;
            balance += val_insert;
        }
        else {
            continue;
        }
    }
    cout << endl;
    cout << "Balance of all accounts is: " << balance << endl;
}

template<class K, class V> 
void simplemap_t<K, V>::insert(K key, V val) {
    /* insert (key, value) pair into map */
    if(!map.insert({key, val}).second) { 
        // std::cout << "Insert failed: " << (key, val) << endl;
    }
    else {
        // std::cout << "Insert succeeded: " << (key, val) << endl;

        /* push key and value to unsorted vectors */
        keys.push_back(key);
        values.push_back(val);
    }
}

template<class K, class V> 
bool simplemap_t<K, V>::update(K key, V val) {
    if(!map.insert({key, val}).second) { 
        auto update = map.find(key);
        update->second = val;
        // std::cout << "updated succeeded: " << (key, val) << endl; 
        return false;
    }
    // std::cout << "updated failed: " << (key, val) << endl; 
    return false;
}

template<class K, class V> 
bool simplemap_t<K, V>::remove(K key) {
    if (lookup(key).second) {
        map.erase(key);
        // std::cout << "remove succeeded: " << key << endl;
        return true;
    }
    // std::cout << "remove failed: " << key << endl;
    return false;
}

template<class K, class V> 
std::pair<V, bool> simplemap_t<K, V>::lookup(K key) {
    std::unordered_map<int,float>::iterator it = map.find(key);
    if (it == map.end()) {
        // std::cout << "key not in map: " << key << endl;
        return std::make_pair(0, false);
    }
    // std::cout << "found in map: " << key << endl;
    return std::make_pair(key, true);
}

template<class K, class V> 
void simplemap_t<K, V>::apply_balance(std::function<void(K, V)>& func) {
    for (const auto & [ key, value ] : map) {
        func(key,value);
    }
}

// Explicitly instantiate the simplemap_t, and its member definitions
template class simplemap_t<int,float>;
