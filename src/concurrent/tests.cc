// CSE 375/475 Assignment #1
// Fall 2021
//
// Description: This file implements a function 'run_custom_tests' that should be able to use
// the configuration information to drive tests that evaluate the correctness
// and performance of the map_t object.

#include "tests.h"

void do_work(simplemap_t<int, float> &simple_map) {
	config_t config;
	srand(time(0));
	int deposit_counter = 0;
	int balance_counter = 0;
	for (int i = 0; i < (config.iters / config.threads); i++) {
		int opt = rand() % 100;
		if (opt < 95) {
			deposit(simple_map);
			deposit_counter++;
		}
		else {
			balance(simple_map);
			balance_counter++;
		}
	}

	float acc_balances = balance(simple_map);
	cout << "Balance of all accounts is: " << acc_balances << endl;
	cout << "Deposit counter is: " << deposit_counter << endl;
	cout << "Balance counter is: " << balance_counter << endl;
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
	simple_map.apply_deposit();
}