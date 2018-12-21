#include "Greedy.hpp"
#include <algorithm>


Greedy::Greedy(const vector<int> &capacity, const vector<int> &openCost, const vector<double> &demand,
	const vector<vector<double>> &assignmentCost):capacity(capacity), openCost(openCost), demand(demand),
	assignmentCost(assignmentCost) {
	facility = capacity.size();
	customer = demand.size();
}

/*
获取贪心算法执行后的状态
State.cost:该状态的费用
State.occupy:每个设施已分配的空间
State.assign:每个顾客所分配的设施
*/
State Greedy::getBestState() {
	vector<int> assign(customer, -1);
	vector<double> occupy(facility, 0);
	vector<vector<bool>> alloc(customer, vector<bool>(facility, true));
	for (int i = 0; i < customer; i++) {
		int custom, faci;
		while (true) {
			findMin(alloc, custom, faci);
			/*该设施容量足够*/
			if (occupy[faci] + demand[custom] <= capacity[faci]) {
				for (int j = 0; j < alloc[custom].size(); j++) {
					alloc[custom][j] = false;
				}
				occupy[faci] += demand[custom];
				assign[custom] = faci;
				break;
			}
			/*该设施容量不够*/
			else {
				alloc[custom][faci] = false;
			}
		}
	}
	double cost = calculateCost(occupy, assign); //计算cost
	return State(cost, occupy, assign);
}
/*
找到cost最小且可以被分配的顾客和设施
alloc[i][j]:顾客i是否可以分配到设施j
custom:找到的顾客
faci:找到的设施
*/
void Greedy::findMin(const vector<vector<bool>> &alloc, int &custom, int &faci) {
	int m = INT_MAX;
	for (int i = 0; i < assignmentCost.size(); i++) {
		for (int j = 0; j < assignmentCost[i].size(); j++) {
			if (alloc[i][j] && assignmentCost[i][j] < m) {
				m = assignmentCost[i][j];
				custom = i;
				faci = j;
			}
		}
	}
}
/*
计算cost
occupy:每个设施已分配的空间
assign:每个顾客分配的设施
*/
double Greedy::calculateCost(const vector<double> &occupy, const vector<int> &assign) {
	double cost = 0;
	for (int i = 0; i < facility; i++) {
		cost += (occupy[i] > 0) ? openCost[i] : 0; //判断该设施是否已分配，如果是，则加上开启该设施的代价
	}
	for (int i = 0; i < customer; i++) {
		cost += assignmentCost[i][assign[i]];
	}
	return cost;
}
