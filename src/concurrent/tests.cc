// CSE 375/475 Assignment #1
// Fall 2021
//
// Description: This file implements a function 'run_custom_tests' that should be able to use
// the configuration information to drive tests that evaluate the correctness
// and performance of the map_t object.

#include "tests.h"

void test_driver(config_t &cfg, simplemap_t<int, float> &simple_map) {
	run_custom_tests(cfg, simple_map);
}

void run_custom_tests(config_t& cfg, simplemap_t<int, float> &simple_map) {
	srand(time(0));
	for (int i = 0; i < cfg.iters; i++) {
		int opt = rand() % 100;
		if (opt < 95) {
			deposit(simple_map);
		}
		else {
			float acc_balances = balance(simple_map);
		}
	}

	float acc_balances = balance(simple_map);
	cout << "Balance of all accounts is: " << acc_balances << endl;
}

float balance(simplemap_t<int, float> &simple_map) {
    float balance = 0.0;
	auto balance_lambda = [&](int k, float v) {
		balance += v;
	};
	simple_map.apply_balance(balance_lambda);
	return balance;
}

void deposit(simplemap_t<int, float> &simple_map) {
	auto deposit_lambda = [&](int key_one, int key_two, float v) {
		if (simple_map.lookup(key_one).second && simple_map.lookup(key_two).second) {
			simple_map.update_subtract(key_one, v);
			simple_map.update_add(key_two, v);
		}
	};
	simple_map.apply_deposit(deposit_lambda);
}