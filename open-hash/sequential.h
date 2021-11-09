/**
 * @file hashset.h
 */

#ifndef SEQUENTIAL_DEF
#define SEQUENTIAL_DEF

#include <iostream>
#include <vector>
#include <functional>
#include "config_t.h"

#define HASHTABLES 2
#define EMPTY -1

using namespace std;

template<typename K>
class sequential {
    /** Ordered Vector of type 'K' as underlying data structure for hash table */
    std::vector<std::vector<K> > hashtable_t;
    
public: 
    /** Constructor initializing empty unordered set (hash set) */
    sequential() {}

    /** Initialize unordered set (hash set) */
    void initialize(K key_max) {
        hashtable_t.resize(HASHTABLES, vector<K>(key_max, -1));
    }

    /** Insert (K) API call */
    bool insert(K key) {
        if (lookup(key).second) {
            return false;            
        }

        for (int i = 0; i < HASHTABLES; i++) {
            if (hashtable_t[i][hash(key)] == EMPTY) {
                hashtable_t[i][hash(key)] = key;
                cout << "inserted key in hashtable!" << endl;
            }
            else if (hashtable_t[i][hash(key)] != EMPTY) {
                cout << "swap!" << endl;
            }
        }
        return true;
    }

    /** Remove (K) API call */
    bool remove(K key) {
        for (int i = 0; i < HASHTABLES; i++) {
            if (lookup(key).second) {
                hashtable_t[i].erase(hashtable_t[i].begin() + lookup(key).first);
                cout << "removed " << key << " successfully!" << endl;
                return true;
            }
        }
        cout << "trying to remove " << key << " but not found!" <<endl;
        return false;
    }

    /** Find (K) API call */
    std::pair<K, bool> lookup(K key) {
        for (int i = 0; i < HASHTABLES; i++) {
            int index = hash(key);
            if (hashtable_t[i][index] == key) {
                cout << key << " found key in hashtable!" << endl;
                return std::make_pair(index, true);      
            }
        }
        cout << key << " NOT found in either hashtable!" << endl;
        return std::make_pair(key, false);
    }

    /** Print hashset */
    void print() {
        for (int i = 0; i < (int) hashtable_t.size(); i++) {
            for (int j = 0; j < (int) hashtable_t[i].size(); j++) {
                cout << hashtable_t[i][j] << " ";
            }
        }
        cout << endl;
    }

    /** Hash function */
    K hash(K key) {
        std::hash<K> hash_value;
        K index = hash_value(key) % hashtable_t[0].size();
        return index;
    }
};

#endif