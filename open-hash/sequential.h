/**
 * @file hashset.h
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

using namespace std;

template<typename K>
class sequential {
    struct bucket {
        K key;
    };

    /** Ordered Vector of type 'K' as underlying data structure for hash table */
    std::vector<std::vector<bucket *> > hashtable_t;

public: 
    /** Constructor initializing empty unordered set (hash set) */
    sequential() {}

    /** Initialize hashtable */
    void initialize(K key_max) {
        hashtable_t.resize(HASHTABLES);

        for (int i = 0; i < HASHTABLES; i++) {
            for (int j = 0; j < key_max; j++) {
                hashtable_t[i].push_back(new bucket());
                hashtable_t[i][j]->key = -1;
            }
        }
    }

    /** Insert (K) API function call */
    bool insert(K key) {
        int table = 0, recursion = 0;
        
        if (lookup(key).second) {
            return false;            
        }
        
        /** Swap is a wrapper for insert, using recursion to identify loops */
        swap(table, key, recursion);

        return true;
    }

    /** Remove (K) API function call */
    bool remove(K key) {
        std::pair<K, bool> found = lookup(key);
        int table = found.first;

        if (!found.second) {
            return false;            
        }

        if (table == 0) {
            hashtable_t[table].erase(hashtable_t[table].begin() + hash(table, key));
            cout << "removed " << key << " successfully from " << "table " << table << endl;
        }
        else if (table == 1) {
            hashtable_t[table].erase(hashtable_t[table].begin() + hash(table, key));
            cout << "removed " << key << " successfully from " << "table " << table << endl;
        }
        return true;
    }

    /** Lookup (K) API function call */
    std::pair<K, bool> lookup(K key) {
        if (hashtable_t[0][hash(0, key)]->key == key) {
            cout << key << " found key in hashtable one!" << endl;
            return std::make_pair(0, true);  
        }
        // else if (hashtable_t[1][hash(1, key)]->key == key) {
        //     cout << key << " found key in hashtable two!" << endl;
        //     return std::make_pair(1, true);        
        // }
        // cout << key << " NOT found in either hashtable!" << endl;
        return std::make_pair(-1, false);
    }

    /** Swap elements in hashtable */
    bool swap(int table, K key, int recursion) {   
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! recusion is: " << recursion << endl;
        cout << "TABLE is: " << table << endl;
        if (recursion == RECURSION) {
            cout << "Loop detected. Triggering RESIZE AND REHASH!";
            rehash();
            return true;
        }
        int index = hash(table, key);
        cout << "INDEX IS: " << index << endl;
        if (hashtable_t[table][index]->key != EMPTY) {
            cout << "Index contains: " << hashtable_t[table][index]->key << " in table: " << table << endl; 
            K val = hashtable_t[table][index]->key;
            hashtable_t[table][index]->key = key;
            cout << "NEW index contains: " << hashtable_t[table][index]->key << " in table: " << table << endl; 
            swap((table + 1) % HASHTABLES, val, recursion + 1);
            return true;
        }
        else {
            cout << "INSERTING ELEMENT " << key << endl;
            // memcpy(hashtable_t[table][index]->key, key, sizeof(key));
            hashtable_t[table][index]->key = key;
            cout << "INSERTED ELEMENT: " << key << endl;
            cout << "hashtable_t[table][index]->key = key is " << hashtable_t[table][index]->key << endl;
            cout << "hashtable_t[table][index]->key = key is " << hashtable_t[table][index]->key << endl;
            return true;
        }
    }

    /** Rehash and resize hashtable */
    void rehash() {
        // int old_size = hashtable_t[0].size();
        cout << "STARTING RESIZE AND REHASH!" << endl;

        /** Print old hashtable sizes */
        cout << "Size of table[0] before resize is: " << hashtable_t[0].size() << endl;
        cout << "Size of table[1] before resize is: " << hashtable_t[1].size() << endl;

        /** Define temporary vector to hold elements */
        std::vector<std::vector<K> > temp_hashtable_one;
        std::vector<std::vector<K> > temp_hashtable_two;    

        /** Create temporary hash table with new size */
        temp_hashtable_one.resize(1, vector<K>(hashtable_t[0].size() * (1+((float)RESIZE_PERCENTAGE)/100), -1));
        temp_hashtable_two.resize(1, vector<K>(hashtable_t[1].size() * (1+((float)RESIZE_PERCENTAGE)/100), -1));
 
        /** Print new temp sizes */
        cout << "size of temp_hashtable_one[0].size() " << temp_hashtable_one[0].size() << endl;
        cout << "size of temp_hashtable_two[0].size() " << temp_hashtable_two[0].size() << endl;     

        /** Copy old hash table to temp hash table */
        for (int i = 0; i < (int) hashtable_t[0].size(); i++) {
            temp_hashtable_one[0][i] = hashtable_t[0][i]->key;
        }  
        for (int i = 0; i < (int) hashtable_t[1].size(); i++) {
            temp_hashtable_two[0][i] = hashtable_t[1][i]->key;
            // [hash(1, hashtable_t[1][i]->key)]
        }   

        /* Resize old hash table */
        hashtable_t[0].resize(hashtable_t[0].size() * (1+((float)RESIZE_PERCENTAGE)/100));
        hashtable_t[1].resize(hashtable_t[1].size() * (1+((float)RESIZE_PERCENTAGE)/100));

        cout << "Size of table[0] after resize is: " << hashtable_t[0].size() << endl;
        cout << "Size of table[1] after resize is: " << hashtable_t[1].size() << endl;

        /** Save new hashtable sizes */
        int hashtable_one = hashtable_t[0].size();
        int hashtable_two = hashtable_t[1].size();

        /** Clear old hashtables */
        hashtable_t[0].clear();
        hashtable_t[1].clear();

        /** Alloctate new space in hashtable */
        for (int i = 0; i < (int) hashtable_one; i++) {
            hashtable_t[0].push_back(new bucket());
            hashtable_t[0][i]->key = -1;
        }

        for (int i = 0; i < (int) hashtable_two; i++) {
            hashtable_t[1].push_back(new bucket());
            hashtable_t[1][i]->key = -1;
        }

        /** Copy values back to old hash table (with new size) */
        for (int k = 0; k < (int) hashtable_t[0].size(); k++) {
            hashtable_t[0][hash(0, temp_hashtable_one[0][k])]->key = temp_hashtable_one[0][k];
        }
        for (int k = 0; k < (int) hashtable_t[1].size(); k++) {
            hashtable_t[1][hash(1, temp_hashtable_two[0][k])]->key = temp_hashtable_two[0][k];
        }

        print();
    }

    /** Hash function that switches between hash_func_one and hash_func_two */
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
        cout << "key is: " << key << endl;
        cout << "hash in TWO is: " << hash << endl;
        cout << "index in TWO is: " << index << endl;
        return index;
    }

    /** Hash function using murmur hash library */
    int murmur_hash(K key) {
        uint64_t seed = 100;
        uint64_t hash_otpt[2]= {0};
        const int *key_hash = &key;
        MurmurHash3_x64_128(key_hash, sizeof(K), seed, hash_otpt); // 0xb6d99cf8
        cout << *hash_otpt << endl;
        int index = *hash_otpt % hashtable_t[0].size();
        cout << "key in murmur: " << key << endl;
        cout << "hash in murmur is: " << *hash_otpt << endl;
        cout << "index in murmur is: " << index << endl;
        return index;
    }

    /** Print hashtable */
    void print() {
        for (int i = 0; i < (int) hashtable_t.size(); i++) {
            for (int j = 0; j < (int) hashtable_t[i].size(); j++) {
                cout << hashtable_t[i][j]->key << " ";
            }
            cout << endl;
            cout << endl;
        }
        cout << endl;

        /** Print size of hashtables */
        cout << "Size of table[0]: " << hashtable_t[0].size() << endl;
        cout << "Size of table[1]: " << hashtable_t[1].size() << endl;
    }
};

#endif

// create temp vectors
// map pointers from old 2d vector to new vectors after rehash
// copy old vectors to new vector