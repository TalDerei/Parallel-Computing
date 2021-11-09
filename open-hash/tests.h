/**
 * @file tests.h
 */

#ifndef TESTS_DEF
#define TESTS_DEF

#include "config_t.h"
#include "sequential.h"

using namespace std;

template<typename K>
void run_tests(config_t &config, sequential<K> &hashtable) {
    /* Function counts */
    int lookup_count = 0;
    int insert_count = 0;
    int remove_count = 0;

    /** Initialize data structure to half the size of the key_length */
    hashtable.initialize(config.key_max);

    /** Randomly call insert/remove/lookup APIs */ 
    srand(time(0));
	for (int i = 0; i < config.iterations; i++) {
        int key = rand() % config.key_max; 
		int opt = rand() % 100;
		if (opt < 80) {
			hashtable.lookup(key);
            lookup_count ++;
		} else if ((80 < opt) && (opt < 90)) {
            hashtable.insert(key);
            insert_count++;
        } else {
			hashtable.remove(key);
            remove_count++;
		}
	}

    cout << "number of 'contains' operations: " << lookup_count << endl;
    cout << "number of 'contains' operations: " << insert_count << endl;
    cout << "number of 'contains' operations: " << remove_count << endl;
}

#endif