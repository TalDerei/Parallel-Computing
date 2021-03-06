/**
 * @file sequential.cc
 * 
 * Sequential Implementation of Cuckoo Hash 
 */

#include "sequential.h"
#include "murmurHash.h"
#include "hash.h"

using namespace std;

template<typename K>
void sequential<K>::driver(config_t &config, sequential<K> &hashtable) {
    /** Execute API functions */
    run_tests(config, hashtable);
}

/** Populate and initialize hashtable */
template<typename K>
void sequential<K>::populate(K key_max) {
    /** Resize hashtable_t to include 2 hashtables */
    hashtable_t.resize(HASHTABLES);

    /** Initialize each table with a 'key_max' number of pointers to buckets */
    for (int i = 0; i < HASHTABLES; i++) {
        for (int j = 0; j < key_max; j++) {
            hashtable_t[i].push_back(new bucket());
        }
    }

    /** Populate hashtables with random values */
    srand(time(0));
    for (int i = 0; i < HASHTABLES; i++) {
        for (int j = 0; j < key_max / 2; j++) {
            bool loop = true; 
            while (loop) {
                int key = rand() % key_max; 
                if (!(lookup(key).second)) {
                    hashtable_t[i][j]->key = key;
                    loop = false;
                }
            }
        }
    }
}

/** Insert API function call */
template<typename K>
bool sequential<K>::insert(K key) {
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
bool sequential<K>::remove(K key) {
    std::pair<K, bool> found = lookup(key);
    int table = found.first;

    /** Check if key doesn't exist in hashtable, return false */
    if (!found.second) {
        return false;            
    }

    /** If key found in either table, remove key and return true */
    if (table == 0) {
        hashtable_t[table].erase(hashtable_t[table].begin() + hash(table, key));
        // cout << "Removed " << key << " successfully from " << "table " << table << endl;
    }
    else if (table == 1) {
        hashtable_t[table].erase(hashtable_t[table].begin() + hash(table, key));
        // cout << "Removed " << key << " successfully from " << "table " << table << endl;
    }
    return true;
}

/** Lookup API function call */
template<typename K>
std::pair<K, bool> sequential<K>::lookup(K key) {
    if (hashtable_t[0][hash(0, key)]->key == key) {
        // cout << "Found " << key << " in first hashtable!" << endl;
        return std::make_pair(0, true);  
    }
    else if (hashtable_t[1][hash(1, key)]->key == key) {
        // cout << "Found " << key << " in second hashtable!" << endl;
        return std::make_pair(1, true);        
    }
    // cout << "Key " << key << " NOT found in either hashtable!" << endl;
    return std::make_pair(-1, false);
}

/** Swap elements between hashtables */
template<typename K>
bool sequential<K>::swap(int table, K key, int recursion) {  
    /** If loop is detected, rehash hashtables */ 
    if (recursion == RECURSION) {
        cout << "Loop detected. Triggering RESIZE AND REHASH!";
        rehash();
        REHASH_COUNT_SEQUENTIAL++;
        return true;
    }

    /* If hashtable index already contains an element, swap the element */
    int index = hash(table, key);
    if (hashtable_t[table][index]->key != EMPTY) {
        // cout << "Index " << index << " in hashtable " << table << " is already populated! Swapping it!" << endl;
        K val = hashtable_t[table][index]->key;
        hashtable_t[table][index]->key = key;
        swap((table + 1) % HASHTABLES, val, recursion + 1);
        return true;
    }
    /* Else insert the element into the empty index */
    else {
        hashtable_t[table][index]->key = key;
        // cout << "Inserted " << key << " in index " << index << " in table " << table << "!" << endl;
        return true;
    }
}

/** Rehash and Resize hashtable */
template<typename K>
void sequential<K>::rehash() {
    /** Define temporary vector to hold elements */
    std::vector<std::vector<K> > temp_hashtable_one;
    std::vector<std::vector<K> > temp_hashtable_two;    

    /** Create temporary hashtables with new size */
    temp_hashtable_one.resize(1, vector<K>(hashtable_t[0].size() * (1+((float)RESIZE_PERCENTAGE)/100), 0));
    temp_hashtable_two.resize(1, vector<K>(hashtable_t[1].size() * (1+((float)RESIZE_PERCENTAGE)/100), 0));   

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

    /** Save new hashtable sizes */
    int hashtable_one = hashtable_t[0].size();
    int hashtable_two = hashtable_t[1].size();

    /** Clear old hashtables */
    hashtable_t[0].clear();
    hashtable_t[1].clear();

    /** Alloctate new space in resized hashtables */
    for (int i = 0; i < (int) hashtable_one; i++) {
        hashtable_t[0].push_back(new bucket());
        hashtable_t[0][i]->key = 0;
    }
    for (int i = 0; i < (int) hashtable_two; i++) {
        hashtable_t[1].push_back(new bucket());
        hashtable_t[1][i]->key = 0;
    }

    /** Copy values back to old hash table */
    for (int k = 0; k < (int) hashtable_t[0].size(); k++) {
        hashtable_t[0][hash(0, temp_hashtable_one[0][k])]->key = temp_hashtable_one[0][k];
    }
    for (int k = 0; k < (int) hashtable_t[1].size(); k++) {
        hashtable_t[1][hash(1, temp_hashtable_two[0][k])]->key = temp_hashtable_two[0][k];
    }
}

/** Hash function that switches between murmur_hash and bitwise_hash */
template<typename K>
int sequential<K>::hash(int table, K key) {
    switch (table) {
        case 0: return murmur_hash(key);
        case 1: return bitwise_hash(key);
    }
    return 0;
}

/** Hash function from xxHash libraru */
template<typename K>
int sequential<K>::bitwise_hash(K key) {
    uint hash = (uint) xxh::xxhash<32>({key});
    int index = hash % hashtable_t[1].size();
    return index;
}

/** Hash function using murmur hash library */
template<typename K>
inline int sequential<K>::murmur_hash(K key) {
    uint64_t seed = 100;
    uint64_t hash_otpt[2]= {0};
    const int *key_hash = &key;
    MurmurHash3_x64_128(key_hash, sizeof(K), seed, hash_otpt); 
    int index = *hash_otpt % hashtable_t[0].size();
    return index;
}

/** Randomly call insert/remove/lookup APIs */ 
void run_tests(config_t &config, sequential<int> &hashtable) {
    /* API counters */
    int lookup_true = 0, lookup_false = 0;
    int insert_true = 0, insert_false = 0;
    int remove_true = 0, remove_false = 0;

    srand(time(0));
	for (int i = 0; i < config.iterations / config.threads; i++) {
        int key = rand() % config.key_max; 
		int opt = rand() % 100;
		if (opt < 80) {
			if (hashtable.lookup(key).second) {
                lookup_true++;
            }
            else {
                lookup_false++;
            }
		} 
        else if ((80 < opt) && (opt < 90)) {
            if (hashtable.insert(key)) {
                insert_true++;
            }
            else {
                insert_false++;
            }            
        } 
        else {
			if (hashtable.remove(key)) {
                remove_true++;
            }
            else {
                remove_false++;
            }
		}
	}

    /** Print the number of operations completed */
    cout << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Number of 'lookup' operations: " << lookup_true + lookup_false << endl;
    cout << "Number of lookups succeeded: " << lookup_true << endl;
    cout << "Number of lookups failed: " << lookup_false << endl;
    cout << "Number of 'insert' operations: " << insert_true + insert_false << endl;
    cout << "Number of inserts succeeded: " << insert_true << endl;
    cout << "Number of inserts failed: " << insert_false << endl;
    cout << "Number of 'remove' operations: " << remove_true + insert_false << endl;
    cout << "Number of remove succeeded: " << remove_true << endl;
    cout << "Number of remove failed: " << remove_false << endl;
    cout << endl;
}

/** Print hashtable */
template<typename K>
void sequential<K>::print() {
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
    cout << "Number of rehash operations executed is: " << REHASH_COUNT_SEQUENTIAL << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
}

template class sequential<int>;