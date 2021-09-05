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

using namespace std;

template<class K, class V> 
void simplemap_t<K,V>::init(K key, V value) {
    /* Populate map with (key,values) */
    initialize_map(key, value);

    /* Map iterator */
    std::unordered_map<int, int>::iterator it;

     /* Start execution time */
    auto start = chrono::high_resolution_clock::now();

    int read_only_ratio = 0;
    int iterations = 1000000;
    srand(time(0));
    /* Randomly switch between different set operations */
    for (int counter = 0; counter < iterations; counter++) {
        long opt = rand() % 100;
        int rand_key = rand() % key; 
        int rand_value = rand() % value; 
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
void simplemap_t<K, V>::initialize_map(K key, V val) {
    int counter = 0;
    srand(time(0));
    while (counter != key / 2) {
        int key_insert = rand() % key;
        int val_insert = rand() % val;
        if (!lookup(key_insert).second) {
            map.insert({key_insert, val_insert});
            counter++;
        }
        else {
            continue;
        }
    }
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
    std::unordered_map<int,int>::iterator it = map.find(key);
    if (it == map.end()) {
        // std::cout << "key not in map: " << key << endl;
        return std::make_pair(0, false);
    }
    // std::cout << "found in map: " << key << endl;
    return std::make_pair(key, true);
}

template<class K, class V> 
void simplemap_t<K, V>::apply(void (*f)(K, V)) {
    assert("Not Implemented");
    // for (auto i : *keys) {
    //     f(i, values->at(i));
    // }
}

// Explicitly instantiate the simplemap_t, and its member definitions
template class simplemap_t<int,int>;
