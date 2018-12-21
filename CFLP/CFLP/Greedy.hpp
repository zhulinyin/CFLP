#pragma once
#include "State.hpp"
using namespace std;

class Greedy
{
public:
	Greedy(const vector<int> &capacity, const vector<int> &openCost, const vector<double> &demand,
		const vector<vector<double>> &assignmentCost);
	State getBestState();
	double calculateCost(const vector<double> &occupy, const vector<int> &assign);
	void findMin(const vector<vector<bool>> &alloc, int &custom, int &faci);

private:
	const vector<int> capacity;
	const vector<int> openCost;
	const vector<double> demand;
	const vector<vector<double>> assignmentCost;
	int facility;
	int customer;
};

