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
#define AMOUNT 10.0

template<class K, class V> 
struct simplemap_t<K, V>::bucket_t {
    std::mutex lock;
    std::unordered_map<K,V> map;
};

template<class K, class V> 
void simplemap_t<K, V>::concurrent_hashtable(size_t _buckets) {
    for (int i = 0; i < _buckets; i++) {
        buckets.push_back(new bucket_t());
    }
}

template<class K, class V> 
void simplemap_t<K, V>::insert(K key, V val) {
    size_t bucket_index = std::hash<K>{}(key) % num_buckets;
    std::lock_guard<std::mutex> sync(buckets[bucket_index]->lock);
    if(!buckets[bucket_index]->map.insert({key, val}).second) {}
}

template<class K, class V> 
void simplemap_t<K, V>::update_subtract(K key, V val) {
    size_t bucket_index = std::hash<K>{}(key) % num_buckets;
    std::lock_guard<std::mutex> sync(buckets[bucket_index]->lock);
    if(!buckets[bucket_index]->map.insert({key, val}).second) { 
        auto update = map.find(key);
        update->second = update->second - val;
    }
}

template<class K, class V> 
void simplemap_t<K, V>::update_add(K key, V val) {
    size_t bucket_index = std::hash<K>{}(key) % num_buckets;
    std::lock_guard<std::mutex> sync(buckets[bucket_index]->lock);
    if(!buckets[bucket_index]->map.insert({key, val}).second) { 
        auto update = buckets[num_buckets]->map.find(key);
        update->second = update->second + val;
    }
}

template<class K, class V> 
bool simplemap_t<K, V>::remove(K key) {
    size_t bucket_index = std::hash<K>{}(key) % num_buckets;
    std::lock_guard<std::mutex> sync(buckets[bucket_index]->lock);
    if (lookup(key).second) {
        buckets[bucket_index]->map.erase(key);
        return true;
    }
    return false;
}

template<class K, class V> 
std::pair<V, bool> simplemap_t<K, V>::lookup(K key) {
    size_t bucket_index = std::hash<K>{}(key) % num_buckets;
    std::lock_guard<std::mutex> sync(buckets[bucket_index]->lock);
    std::unordered_map<int,float>::iterator it = buckets[num_buckets]->map.find(key);
    if (it == buckets[bucket_index]->map.end()) {
        return std::make_pair(0, false);
    }
    return std::make_pair(key, true);
}

template<class K, class V> 
void simplemap_t<K, V>::apply_balance(std::function<void(K,V)> func) {
    size_t bucket_index = std::hash<K>{}(K) % num_buckets;
    std::lock_guard<std::mutex> sync(buckets[bucket_index]->lock);
    for (const auto & [ key, value ] : buckets[bucket_index]->map) {
        func(key,value);
    }
}

template<class K, class V> 
void simplemap_t<K, V>::apply_deposit(std::function<void(K,K,V)> func) {
    std::unordered_map<int, float>::iterator it;
    config_t cfg;
    auto iterator_one = map.begin();
    auto iterator_two = map.begin();
    int counter = rand() % map.size();
    std::advance(iterator_one, rand() % map.size());
    std::advance(iterator_two, rand() % map.size());
    K random_key_first_acc = iterator_one->first;
    K random_key_second_acc = iterator_two->first;
    V random_val = AMOUNT;

    if (random_key_first_acc != random_key_second_acc) {
        func(random_key_first_acc, random_key_second_acc, random_val);
    } 
    else {
        apply_deposit(func);
    }
}

template<class K, class V> 
void simplemap_t<K, V>::print() {
    std::unordered_map<int, float>::iterator it;
    for (it = map.begin(); it != map.end(); ++it) {
        cout << '\t' << it->first
             << '\t' << it->second << '\n';
    }
}

// Explicitly instantiate the simplemap_t, and its member definitions
template class simplemap_t<int,float>;
