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
    /* API counters */
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
            cout << "LOOKUP OPERATION!" << endl;
			hashtable.lookup(key);
            lookup_count ++;
		} else if ((80 < opt) && (opt < 90)) {
            cout << "INSERT OPERATION!" << endl;
            hashtable.insert(key);
            insert_count++;
        } else {
            cout << "REMOVE OPERATION!" << endl;
			hashtable.remove(key);
            remove_count++;
		}
	}

    /** Print the number of operations completed */
    cout << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "number of 'lookup' operations: " << lookup_count << endl;
    cout << "number of 'insert' operations: " << insert_count << endl;
    cout << "number of 'remove' operations: " << remove_count << endl;
    cout << endl;
}

#endif