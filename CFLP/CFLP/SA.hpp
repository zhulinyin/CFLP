#pragma once
#include "State.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

class SA
{
public:
	SA(const vector<int> &capacity, const vector<int> &openCost, const vector<double> &demand, 
		const vector<vector<double>> &assignmentCost);
	State init();
	void genRandomState();
	double calculateCost(const vector<double> &occupy, const vector<int> &assign);
	bool isFeasible(const vector<double> &occupy);
	void run(double beginTem, double endTem, double cool, int iteration);
	State moveCustomerToAnotherFacility();
	State exchangeTwoCustomer();
	State closeRandomFacility();
	State getBestState();

private:
	State bestState;
	State bestStateForEveryIteration;
	State curState;
	const vector<int> capacity;
	const vector<int> openCost;
	const vector<double> demand;
	const vector<vector<double>> assignmentCost;
	int facility;
	int customer;
};