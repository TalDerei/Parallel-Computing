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
void simplemap_t<K,V>::init(K key) {
    int counter = 0;
    int balance = 0;
    srand(time(0));
    while (counter != key) {
        int key_insert = rand() % key;
        if (!(lookup(key_insert).second)) {
            float val_insert = (float) (BALANCE / (key));
            insert(key_insert, val_insert);
            counter++;
            balance += val_insert;
        }
        else {
            continue;
        }
    }
}

template<class K, class V> 
void simplemap_t<K, V>::insert(K key, V val) {
    size_t bucket_index = std::hash<K>{}(key) % num_buckets;
    std::lock_guard<std::mutex> (buckets[bucket_index]->lock);
    if(!buckets[bucket_index]->map.insert({key, val}).second) {}
}

template<class K, class V> 
void simplemap_t<K, V>::remove(K key) {
    size_t bucket_index = std::hash<K>{}(key) % num_buckets;
    std::lock_guard<std::mutex> (buckets[bucket_index]->lock);
    if (lookup(key).second) {
        buckets[bucket_index]->map.erase(key);
    }
}

template<class K, class V> 
std::pair<V, bool> simplemap_t<K, V>::lookup(K key) {
    size_t bucket_index = std::hash<K>{}(key) % num_buckets;
    std::lock_guard<std::mutex> (buckets[bucket_index]->lock);
    if (buckets[bucket_index]->map.find(key) == buckets[bucket_index]->map.end()) {
        return std::make_pair(0, false);
    }
    return std::make_pair(key, true);
}

template<class K, class V> 
void simplemap_t<K, V>::update_subtract(K key, V val, K bucket_index) {
    std::lock_guard<std::mutex> (buckets[bucket_index]->lock);
        auto update = buckets[bucket_index]->map.find(key);
        update->second -= val;
}

template<class K, class V> 
void simplemap_t<K, V>::update_add(K key, V val, K bucket_index) {
    std::lock_guard<std::mutex> (buckets[bucket_index]->lock);
        auto update = buckets[bucket_index]->map.find(key);
        update->second += val;
}

template<class K, class V> 
void simplemap_t<K, V>::apply_balance(std::function<void(K,V)> func) {
    // Lock buckets
    for (int index = 0; index < num_buckets; index++) {
        buckets[index]->lock.lock();
        for (const auto & [ key, value ] : buckets[index]->map) {
            func(key,value);
        }
    }

    // Unlock buckets
    for (int index = 0; index < num_buckets; index++) {
        buckets[index]->lock.unlock();
    }
}

template<class K, class V> 
void simplemap_t<K, V>::apply_deposit() {
    for (int index = 0; index < num_buckets; index++) {
        buckets[index]->lock.lock();
    }

    int rand_bucket_one = rand() % num_buckets;
    int rand_bucket_two = rand() % num_buckets;
    auto iterator_one = buckets[rand_bucket_one]->map.begin();
    auto iterator_two = buckets[rand_bucket_two]->map.begin();
    std::advance(iterator_one, rand() % buckets[rand_bucket_one]->map.size());
    std::advance(iterator_two, rand() % buckets[rand_bucket_two]->map.size());
    K random_key_first_acc = iterator_one->first;
    K random_key_second_acc = iterator_two->first;
    V random_val = AMOUNT;

    if (random_key_first_acc != random_key_second_acc) {
        // cout << "?????????????????????????" << endl;
        iterator_one->second -= random_val;
        iterator_two->second += random_val;
        // update_subtract(random_key_first_acc, random_val, rand_bucket_one);
        // update_add(random_key_second_acc, random_val, rand_bucket_two);
    } 
    else {
        // cout << "never!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        apply_deposit();
    }

    for (int index = 0; index < num_buckets; index++) {
        buckets[index]->lock.unlock();
    }
}

template<class K, class V> 
void simplemap_t<K, V>::print() {
    for (int index = 0; index < num_buckets; index++) {
        buckets[index]->lock.lock();
    }

    std::unordered_map<int, float>::iterator it;
    int index = 0;
    for (int i = 0; i < num_buckets; i++) {
        for (it = buckets[index]->map.begin(); it != buckets[index]->map.end(); ++it) {
            cout << '\t' << it->first
                << '\t' << it->second << '\n';
        }
        index++;
    }

    for (int index = 0; index < num_buckets; index++) {
        buckets[index]->lock.unlock();
    }
}

// Explicitly instantiate the simplemap_t, and its member definitions
template class simplemap_t<int,float>;
