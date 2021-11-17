/**
 * @file concurrent.h
 * 
 * Concurrent Implementation of Cuckoo Hash 
 */

#include "concurrent.h"
#include "tests.h"
#include "murmurHash.h"

using namespace std;

template<typename K>
void concurrent<K>::driver(config_t &config, concurrent<K> &hashtable) {
    /** Initialize data structure to half the size of the key_length */
    // hashtable.initialize(config.key_max);

    /** Spawn threads to execute API functions */    
    spawn_threads(config, hashtable);
}

template<typename K>
void concurrent<K>::initialize(K key_max) {
    /** Resize hashtable_t to include 2 hashtables */
    hashtable_t.resize(HASHTABLES);

    /** Populate each row with a 'key_max' number of pointers to buckets */
    for (int i = 0; i < HASHTABLES; i++) {
        for (int j = 0; j < key_max / 2; j++) {
            /** Use push_back to construct the object with make_shared */
            hashtable_t[i].push_back(std::make_shared<bucket>());

            /** Set the value atomically */
            hashtable_t[i][j]->key.store(-1, std::memory_order_seq_cst);
            // rand() % key_max
        }
    }
}

template<typename K>
void concurrent<K>::spawn_threads(config_t &config, concurrent<K> &hashtable) {
    /** Vector of threads */
    std::vector<std::thread> thread_t;

    /** Spawn thread and insert into vector */
    for (int i = 0; i < config.threads; i++) {
        cout << "spawn threads!" << endl;
        thread_t.push_back(std::thread(&concurrent<K>::run_tests, this, ref(config), ref(hashtable)));
    }

    // /** Synchronize threads by joining them */
    for (int i = 0; i < config.threads; i++) {
        cout << "Waiting for threads to finish!" << endl;
        thread_t[i].join();
    }
    cout << "Done!" << endl;
}

/** Lookup API function call */
template<typename K>
std::pair<K, bool> concurrent<K>::lookup(K key) {
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
template<typename K>
bool concurrent<K>::insert(K key) {
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
template<typename K>
bool concurrent<K>::remove(K key) {
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
template<typename K>
bool concurrent<K>::swap(int table, K key, int recursion) {  
    /** If loop is detected, rehash hashtables */ 
    if (recursion == RECURSION) {
        cout << "Loop detected. Triggering RESIZE AND REHASH!";
        rehash();
        REHASH_COUNT_CONCURRENT++;
        return true;
    }

    /* If hashtable index already contains an element, swap the element */
    int index = hash(table, key);
    /** atomically load in key, and check if index is populated */
    if (hashtable_t[table][index]->key.load(std::memory_order_seq_cst) != EMPTY) {
        // cout << "Index " << index << " in hashtable " << table << " is already populated! Swapping it!" << endl;

        /** Load the existing value atomically */
        K val = std::atomic_load(&hashtable_t[table][index]->key);

        /* allocate temporary 'newnode' to hold contents of old pointer */
        std::shared_ptr<bucket> newnode = std::atomic_load(&hashtable_t[table][index]); 

        /** Store new key in index */
        newnode->key.store(key, std::memory_order_seq_cst);

        swap((table + 1) % HASHTABLES, val, recursion + 1);
        
        return true;
    }
    /* Else insert the element into the empty index */
    else {
        hashtable_t[table][index]->key.store(key, std::memory_order_seq_cst);
        // hashtable_t[table][index]->key = key;
        // cout << "Inserted " << key << " in index " << index << " in table " << table << "!" << endl;
        return true;
    }
}

/** Rehash and Resize hashtable */
template<typename K>
void concurrent<K>::rehash() {
    // cout << "STARTING RESIZE AND REHASH!" << endl;
    mtx.lock();

    /** Define temporary vector to hold elements */
    std::vector<std::vector<K> > temp_hashtable_one;
    std::vector<std::vector<K> > temp_hashtable_two;    

    /** Create temporary hashtables with new size */
    temp_hashtable_one.resize(1, vector<K>(hashtable_t[0].size() * (1+((float)RESIZE_PERCENTAGE)/100), -1));
    temp_hashtable_two.resize(1, vector<K>(hashtable_t[1].size() * (1+((float)RESIZE_PERCENTAGE)/100), -1));

    /** Print size of temporary hashtables */
    // cout << "size of temp_hashtable_one[0].size() " << temp_hashtable_one[0].size() << endl;
    // cout << "size of temp_hashtable_two[0].size() " << temp_hashtable_two[0].size() << endl;     

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
    // cout << "Size of table[0] after resize is: " << hashtable_t[0].size() << endl;
    // cout << "Size of table[1] after resize is: " << hashtable_t[1].size() << endl;

    /** Save new hashtable sizes */
    int hashtable_one = hashtable_t[0].size();
    int hashtable_two = hashtable_t[1].size();

    /** Clear old hashtables */
    hashtable_t[0].clear();
    hashtable_t[1].clear();

    /** Alloctate new space in resized hashtables */
    for (int i = 0; i < (int) hashtable_one; i++) {
        hashtable_t[0].push_back(std::make_shared<bucket>());
        hashtable_t[0][i]->key = -1;
    }
    for (int i = 0; i < (int) hashtable_two; i++) {
        hashtable_t[1].push_back(std::make_shared<bucket>());
        hashtable_t[1][i]->key = -1;
    }

    /** Copy values back to old hash table */
    for (int k = 0; k < (int) hashtable_t[0].size(); k++) {
        hashtable_t[0][hash(0, temp_hashtable_one[0][k])]->key.store(temp_hashtable_one[0][k], std::memory_order_seq_cst);
    }
    for (int k = 0; k < (int) hashtable_t[1].size(); k++) {
        hashtable_t[1][hash(1, temp_hashtable_two[0][k])]->key.store(temp_hashtable_two[0][k], std::memory_order_seq_cst);
    }

    cout << "SUCCESSFULLY EXECUTED RESIZE AND REHASH!" << endl;

    mtx.unlock();
}

/** Hash function that switches between murmur_hash and bitwise_hash */
template<typename K>
int concurrent<K>::hash(int table, K key) {
    switch (table) {
        case 0: return murmur_hash(key);
        case 1: return bitwise_hash(key);
    }
    return 0;
}

/** Hash function adopted from CSE 109 */
template<typename K>
int concurrent<K>::bitwise_hash(K key) {
    uint32_t hash = 5381;
    unsigned char* rbytes = (unsigned char*)(&key); 
    for (size_t i = 0; i < 3; ++i) { 
        hash = ((hash << 5) + hash) ^ (rbytes[i]); 
    }
    int index = hash % hashtable_t[1].size();
    return index;
}

/** Hash function using murmur hash library */
template<typename K>
int concurrent<K>::murmur_hash(K key) {
    uint64_t seed = 100;
    uint64_t hash_otpt[2]= {0};
    const int *key_hash = &key;
    MurmurHash3_x64_128(key_hash, sizeof(K), seed, hash_otpt); 
    int index = *hash_otpt % hashtable_t[0].size();
    return index;
}

/** Print hashtable */
template<typename K>
void concurrent<K>::print() {
    // cout << "First and Second Hashtables: " << endl;
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
    // cout << "Number of rehash operations executed is: " << REHASH_COUNT_CONCURRENT << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
}

// Explicitly instantiate the simplemap_t, and its member definitions
template class concurrent<int>;