/**
 * @file hashset.h
 * 
 * Sequential Implementation of Cuckoo Hash 
 */

#ifndef SEQUENTIAL_DEF
#define SEQUENTIAL_DEF

#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <stdio.h>
#include <string.h>
#include "config_t.h"
#include "murmurHash.h"

#define HASHTABLES 2
#define EMPTY -1
#define RECURSION 10
#define RESIZE_PERCENTAGE 50

int REHASH_COUNT = 0;

using namespace std;

template<typename K>
class sequential {
    /** Bucket struct contianing key */
    struct bucket {
        K key;
    };

    /** 2D Vector with pointers to buckets as underlying data structure for hash table */
    std::vector<std::vector<bucket *> > hashtable_t;

public: 
    /** Constructor initializing empty object */
    sequential() {}

    /** Initialize hashtable */
    void initialize(K key_max) {
        /** Resize hashtable_t to include 2 hashtables */
        hashtable_t.resize(HASHTABLES);

        /** Populate each row with a 'key_max' number of pointers to buckets */
        for (int i = 0; i < HASHTABLES; i++) {
            for (int j = 0; j < key_max; j++) {
                hashtable_t[i].push_back(new bucket());
                hashtable_t[i][j]->key = -1;
            }
        }
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
            hashtable_t[table].erase(hashtable_t[table].begin() + hash(table, key));
            cout << "Removed " << key << " successfully from " << "table " << table << endl;
        }
        else if (table == 1) {
            hashtable_t[table].erase(hashtable_t[table].begin() + hash(table, key));
            cout << "Removed " << key << " successfully from " << "table " << table << endl;
        }
        return true;
    }

    /** Lookup API function call */
    std::pair<K, bool> lookup(K key) {
        if (hashtable_t[0][hash(0, key)]->key == key) {
            cout << "Found " << key << " in first hashtable!" << endl;
            return std::make_pair(0, true);  
        }
        else if (hashtable_t[1][hash(1, key)]->key == key) {
            cout << "Found " << key << " in second hashtable!" << endl;
            return std::make_pair(1, true);        
        }
        cout << "Key " << key << " NOT found in either hashtable!" << endl;
        return std::make_pair(-1, false);
    }

    /** Swap elements between hashtables */
    bool swap(int table, K key, int recursion) {  
        /** If loop is detected, rehash hashtables */ 
        if (recursion == RECURSION) {
            cout << "Loop detected. Triggering RESIZE AND REHASH!";
            rehash();
            REHASH_COUNT++;
            return true;
        }

        /* If hashtable index already contains an element, swap the element */
        int index = hash(table, key);
        if (hashtable_t[table][index]->key != EMPTY) {
            cout << "Index " << index << " in hashtable " << table << " is already populated! Swapping it!" << endl;
            K val = hashtable_t[table][index]->key;
            hashtable_t[table][index]->key = key;
            swap((table + 1) % HASHTABLES, val, recursion + 1);
            return true;
        }
        /* Else insert the element into the empty index */
        else {
            hashtable_t[table][index]->key = key;
            cout << "Inserted " << key << " in index " << index << " in table " << table << "!" << endl;
            return true;
        }
    }

    /** Rehash and Resize hashtable */
    void rehash() {
        cout << "STARTING RESIZE AND REHASH!" << endl;

        /** Define temporary vector to hold elements */
        std::vector<std::vector<K> > temp_hashtable_one;
        std::vector<std::vector<K> > temp_hashtable_two;    

        /** Create temporary hashtables with new size */
        temp_hashtable_one.resize(1, vector<K>(hashtable_t[0].size() * (1+((float)RESIZE_PERCENTAGE)/100), -1));
        temp_hashtable_two.resize(1, vector<K>(hashtable_t[1].size() * (1+((float)RESIZE_PERCENTAGE)/100), -1));
 
        /** Print size of temporary hashtables */
        cout << "size of temp_hashtable_one[0].size() " << temp_hashtable_one[0].size() << endl;
        cout << "size of temp_hashtable_two[0].size() " << temp_hashtable_two[0].size() << endl;     

        /** Copy old hashtable to temporary hashtable */
        for (int i = 0; i < (int) hashtable_t[0].size(); i++) {
            temp_hashtable_one[0][i] = hashtable_t[0][i]->key;
        }  
        for (int i = 0; i < (int) hashtable_t[1].size(); i++) {
            temp_hashtable_two[0][i] = hashtable_t[1][i]->key;
        }   

        /* Resize old hashtables */
        hashtable_t[0].resize(hashtable_t[0].size() * (1+((float)RESIZE_PERCENTAGE)/100));
        hashtable_t[1].resize(hashtable_t[1].size() * (1+((float)RESIZE_PERCENTAGE)/100));

        /** Print new size of old hashtables */
        cout << "Size of table[0] after resize is: " << hashtable_t[0].size() << endl;
        cout << "Size of table[1] after resize is: " << hashtable_t[1].size() << endl;

        /** Save new hashtable sizes */
        int hashtable_one = hashtable_t[0].size();
        int hashtable_two = hashtable_t[1].size();

        /** Clear old hashtables */
        hashtable_t[0].clear();
        hashtable_t[1].clear();

        /** Alloctate new space in resized hashtables */
        for (int i = 0; i < (int) hashtable_one; i++) {
            hashtable_t[0].push_back(new bucket());
            hashtable_t[0][i]->key = -1;
        }
        for (int i = 0; i < (int) hashtable_two; i++) {
            hashtable_t[1].push_back(new bucket());
            hashtable_t[1][i]->key = -1;
        }

        /** Copy values back to old hash table */
        for (int k = 0; k < (int) hashtable_t[0].size(); k++) {
            hashtable_t[0][hash(0, temp_hashtable_one[0][k])]->key = temp_hashtable_one[0][k];
        }
        for (int k = 0; k < (int) hashtable_t[1].size(); k++) {
            hashtable_t[1][hash(1, temp_hashtable_two[0][k])]->key = temp_hashtable_two[0][k];
        }

        cout << "SUCCESSFULLY EXECUTED RESIZE AND REHASH!" << endl;
    }

    /** Hash function that switches between murmur_hash and bitwise_hash */
    int hash(int table, K key) {
       switch (table) {
           case 0: return murmur_hash(key);
           case 1: return bitwise_hash(key);
       }
       return 0;
    }

    /** Hash function adopted from CSE 109 */
    int bitwise_hash(K key) {
        uint32_t hash = 5381;
        unsigned char* rbytes = (unsigned char*)(&key); 
        for (size_t i = 0; i < 3; ++i) { 
            hash = ((hash << 5) + hash) ^ (rbytes[i]); 
        }
        int index = hash % hashtable_t[1].size();
        return index;
    }

    /** Hash function using murmur hash library */
    int murmur_hash(K key) {
        uint64_t seed = 100;
        uint64_t hash_otpt[2]= {0};
        const int *key_hash = &key;
        MurmurHash3_x64_128(key_hash, sizeof(K), seed, hash_otpt); 
        cout << *hash_otpt << endl;
        int index = *hash_otpt % hashtable_t[0].size();
        return index;
    }

    /** Print hashtable */
    void print() {
        cout << "First and Second Hashtables: " << endl;
        for (int i = 0; i < (int) hashtable_t.size(); i++) {
            for (int j = 0; j < (int) hashtable_t[i].size(); j++) {
                cout << hashtable_t[i][j]->key << " ";
            }
            cout << endl << endl;
        }
        cout << endl;

        /** Print size of hashtables */
        cout << "Size of first hashtable: " << hashtable_t[0].size() << endl;
        cout << "Size of second hashtable: " << hashtable_t[1].size() << endl;
        cout << "Number of rehash operations executed is: " << REHASH_COUNT << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
    }
};

#endif

// create temp vectors
// map pointers from old 2d vector to new vectors after rehash
// copy old vectors to new vector