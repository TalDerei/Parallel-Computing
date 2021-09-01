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

template<class K, class V> 
void simplemap_t<K, V>::initialize_map(std::unordered_map<K, V> &map) {
    map.insert(std::pair<K,V>(1,2.2));
    std::cout << "1 => " << map.find(1)->second << '\n';
}

template<class K, class V> 
void simplemap_t<K, V>::insert(K key, V val) {
    // std::cout << "key is: " << key;
    // std::cout << "value is: " << val;
    // if (!map.insert({key, val}).second) {
    //     std::cout << "insert failed!";
    // }
    // else {
    //     std::cout << "insert succeedeed!";
    // }
    // for (auto i = keys->begin(); i != keys->end(); ++i) {
    //     if (*i == key) {
    //         return false;
    //     }
    // }
    // keys->push_back(key);
    // values->push_back(val);
    // return true;
}

template<class K, class V> 
bool simplemap_t<K, V>::update(K key, V val) {
    assert("Not Implemented");
    return false;
}

template<class K, class V> 
bool simplemap_t<K, V>::remove(K key) {
    assert("Not Implemented");
    return false;
}

template<class K, class V> 
std::pair<V, bool> simplemap_t<K, V>::lookup(K key) {
    assert("Not Implemented");
    return std::make_pair(0, false);
}

template<class K, class V> 
void simplemap_t<K, V>::apply(void (*f)(K, V)) {
    // for (auto i : *keys) {
    //     f(i, values->at(i));
    // }
    assert("Not Implemented");
}

// Explicitly instantiate the template, and its member definitions
template class simplemap_t<int,float>;
