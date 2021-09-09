// CSE 375/475 Assignment #1
// Fall 2021
//
// Description: This file implements a function 'run_custom_tests' that should be able to use
// the configuration information to drive tests that evaluate the correctness
// and performance of the map_t object.

#include "tests.h"

void test_driver(config_t &cfg, simplemap_t<int, float> &simple_map) {
	cout << "entered test driver!!!!!!!!!!!!!!!!!" << endl;
	run_custom_tests(cfg, simple_map);
}

void run_custom_tests(config_t& cfg, simplemap_t<int, float> &simple_map) {
	cout << "entered run customer tests???????????????????????" << endl;
	for (int i = 0; i < cfg.iters; i++) {
		int opt = rand() % 100;
		if (opt < 95) {
			bool result = deposit(simple_map);
			cout << "deposit result: " << result << endl;
		}
		else {
			float acc_balances = balance(simple_map);
			cout << "balance of all accs is: " << acc_balances << endl;
		}
	}
}

float balance(simplemap_t<int, float> &simple_map) {
    float balance = 0.0;
	auto balance_lambda = [&](int k, float v) {
		balance += v;
	};
	simple_map.apply(balance_lambda);
	return balance;
}

bool deposit(simplemap_t<int, float> &simple_map) {
    assert("Not Implemented");
	return false;
}

void printer(int k, int v) {
	std::cout<<"<"<<k<<","<<v<<">"<< std::endl;
}