/**
 * @file concurrent.h
 * 
 * Concurrent Implementation of Cuckoo Hash 
 */

#include "concurrent.h"
#include "tests.h"

using namespace std;

template<typename K>
void concurrent<K>::driver(config_t &config, concurrent<K> &hashtable) {
    /** Initialize data structure to half the size of the key_length */
    hashtable.initialize(config.key_max);

    /** Spawn threads to execute API functions */    
    spawn_threads(config, hashtable);
}

template<typename K>
void concurrent<K>::initialize(K key_max) {
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

template<typename K>
void concurrent<K>::spawn_threads(config_t &config, concurrent<K> &hashtable) {
    /** Vector of threads */
    std::vector<std::thread> thread_t;

    /** Spawn thread and insert into vector */
    for (int i = 0; i < config.threads; i++) {
        thread_t.push_back(std::thread(run_tests, ref(config), ref(hashtable)));
    }

    // /** Synchronize threads by joining them */
    for (int i = 0; i < config.threads; i++) {
        cout << "Waiting for threads to finish!" << endl;
        thread_t[i].join();
    }
    cout << "Done!" << endl;
}

template<typename K>
bool concurrent<K>::insert(K key) {
    cout << "NOT IMPLEMENTED YET!" << endl;
    return false;
}

template<typename K>
bool concurrent<K>::remove(K key) {
    cout << "NOT IMPLEMENTED YET!" << endl;
    return false;
}

template<typename K>
std::pair<K, bool> concurrent<K>::lookup(K key) {
    cout << "NOT IMPLEMENTED YET!" << endl;
    return std::make_pair(0, false);   
}

// Explicitly instantiate the simplemap_t, and its member definitions
template class concurrent<int>;