#pragma once
#include <vector>
#include <limits.h>
using namespace std;

struct State
{
	double cost;
	vector<double> occupy;
	vector<int> assign;
	State(const double &cost, const vector<double> &occupy, const vector<int> &assign) :
		occupy(occupy), assign(assign), cost(cost) {}
	State() {
		cost = INT_MAX;
	}
};