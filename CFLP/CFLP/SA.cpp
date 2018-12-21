#include "SA.hpp"
#include <time.h>

SA::SA(const vector<int> &capacity, const vector<int> &openCost, const vector<double> &demand,
	const vector<vector<double>> &assignmentCost) :capacity(capacity), openCost(openCost), demand(demand),
	assignmentCost(assignmentCost) {
	facility = capacity.size();
	customer = demand.size();
	bestState = init();
}

/*
模拟退火过程
beginTem:初温
endTem:末温
cool:降温系数
iteration:内循环迭代次数
*/
void SA::run(double beginTem, double endTem, double cool, int iteration) {
	/*十次模拟退火过程取其中最好的一次*/
	for (int count = 0; count < 10; count++) {
		genRandomState(); //随机产生一个当前状态
		bestStateForEveryIteration = curState; //记录每次循环的最好状态
		double tem = beginTem;
		/*模拟退火主过程*/
		while (tem >= endTem) {
			for (int i = 0; i < iteration; i++) {
				int ran = rand() % 5; //产生一个随机数，根据随机数选择邻域策略
				/*随机关闭一个设施*/
				if (ran < 1) {
					State nextState = closeRandomFacility();
					if (exp((curState.cost - nextState.cost) / tem) >= rand() % 100 / 100.0) {
						curState = nextState;
					}
				}
				/*随机交换两位顾客*/
				else if (ran < 3) {
					State nextState = exchangeTwoCustomer();
					if (exp((curState.cost - nextState.cost) / tem) >= rand() % 100 / 100.0) {
						curState = nextState;
					}
				}
				/*随机将一位顾客转移到另一个设施*/
				else {
					State nextState = moveCustomerToAnotherFacility();
					if (exp((curState.cost - nextState.cost) / tem) >= rand() % 100 / 100.0) {
						curState = nextState;
					}
				}
				/*更新本次循环的最好状态*/
				if (curState.cost < bestStateForEveryIteration.cost) {
					bestStateForEveryIteration = curState;
				}
			}
			tem *= cool; //降温
		}
		cout << bestStateForEveryIteration.cost << endl;
		/*保存十次循环的最好状态*/
		if (bestStateForEveryIteration.cost < bestState.cost)
			bestState = bestStateForEveryIteration;
	}
}

/*随机产生一个初始状态*/
void SA::genRandomState() {
	while (true) {
		curState.assign = vector<int>(customer);
		curState.occupy = vector<double>(facility, 0);
		for (int i = 0; i < customer; i++) {
			curState.assign[i] = rand() % facility;
			curState.occupy[curState.assign[i]] += demand[i];
		}
		if (isFeasible(curState.occupy)) {
			curState.cost = calculateCost(curState.occupy, curState.assign);
			break;
		}
	}
}

/*随机将一位顾客转移到另一个设施*/
State SA::moveCustomerToAnotherFacility() {
	while (true) {
		int index = rand() % customer; //随机选择一位顾客
		int newFacility = rand() % facility; //随机选择一个设施
		if (curState.occupy[newFacility] + demand[index] <= capacity[newFacility]) {
			State nextState = curState;
			nextState.occupy[nextState.assign[index]] -= demand[index];
			nextState.assign[index] = newFacility;
			nextState.occupy[newFacility] += demand[index];
			nextState.cost = calculateCost(nextState.occupy, nextState.assign);
			return nextState;
		}
	}
}

/*随机交换两位顾客*/
State SA::exchangeTwoCustomer() {
	while (true) {
		int index1 = rand() % customer; //随机选择顾客1
		int index2 = rand() % customer; //随机选择顾客2
		if (curState.occupy[curState.assign[index1]] - demand[index1] + demand[index2] <= capacity[curState.assign[index1]]
			&& curState.occupy[curState.assign[index2]] - demand[index2] + demand[index1] <= capacity[curState.assign[index2]]) {
			State nextState = curState;
			nextState.occupy[nextState.assign[index1]] = nextState.occupy[nextState.assign[index1]] - demand[index1] + demand[index2];
			nextState.occupy[nextState.assign[index2]] = nextState.occupy[nextState.assign[index2]] - demand[index2] + demand[index1];
			int temp = nextState.assign[index1];
			nextState.assign[index1] = nextState.assign[index2];
			nextState.assign[index2] = temp;
			nextState.cost = calculateCost(nextState.occupy, nextState.assign);
			return nextState;
		}
	}
}

/*随机关闭一个设施*/
State SA::closeRandomFacility() {
	int closeFacility = rand() % facility; //随机选择一个要关闭的设施
	State nextState = curState;
	nextState.occupy[closeFacility] = 0;
	for (int j = 0; j < customer; j++) {
		/*找到分配到要关闭设施的顾客*/
		if (nextState.assign[j] == closeFacility) {
			while (true) {
				int newFacility = rand() % facility; //随机为该顾客选择一个新设施
				if (nextState.occupy[newFacility] + demand[j] <= capacity[newFacility]) {
					nextState.assign[j] = newFacility;
					nextState.occupy[newFacility] += demand[j];
					break;
				}
			}
		}
	}
	nextState.cost = calculateCost(nextState.occupy, nextState.assign);
	return nextState;
}

State SA::init() {
	return State(INT_MAX, vector<double>(facility, 0), vector<int>(customer, -1));
}

/*
判断当前设施的占用状态是否可行
occupy:设施的占用状态
*/
bool SA::isFeasible(const vector<double> &occupy) {
	for (int i = 0; i < occupy.size(); i++) {
		if (occupy[i] > capacity[i]) return false;
	}
	return true;
}
double SA::calculateCost(const vector<double> &occupy, const vector<int> &assign) {
	double cost = 0;
	for (int i = 0; i < facility; i++) {
		cost += (occupy[i] > 0) ? openCost[i] : 0;
	}
	for (int i = 0; i < customer; i++) {
		cost += assignmentCost[i][assign[i]];
	}
	return cost;
}

State SA::getBestState() {
	return bestState;
}