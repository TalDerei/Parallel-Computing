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

// template<class K, class V> 
// void simplemap_t<K, V>::initialize_map(std::unordered_map<K, V> &map) {
//     map.insert(std::pair<K,V>(1,2.2));
//     std::cout << "1 => " << map.find(1)->second << '\n';
// }

template<class K, class V> 
void simplemap_t<K, V>::insert(K key, V val) {
    if(!map.insert({key, val}).second) { 
        std::cout << "Insert failed!";
    }
    else {
        std::cout << "Insert succeeded!";
        keys.push_back(key);
        values.push_back(val);
    }
}

template<class K, class V> 
bool simplemap_t<K, V>::update(K key, V val) {
    if(!map.insert({key, val}).second) { 
        auto update = map.find(key);
        update->second = val;
        return false;
    }
    return false;
}

template<class K, class V> 
bool simplemap_t<K, V>::remove(K key) {
    if (map.erase(key)) {
        return true;
    }
    return false;
}

template<class K, class V> 
std::pair<V, bool> simplemap_t<K, V>::lookup(K key) {
    std::unordered_map<int,float>::iterator it = map.find(key);
    if (it == map.end()) {
        std::cout << "not in map!";
        return std::make_pair(0, false);
    }
    std::cout << "found in map!";
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
template class simplemap_t<int,float>;
