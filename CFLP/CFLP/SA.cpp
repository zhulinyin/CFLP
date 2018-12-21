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
ģ���˻����
beginTem:����
endTem:ĩ��
cool:����ϵ��
iteration:��ѭ����������
*/
void SA::run(double beginTem, double endTem, double cool, int iteration) {
	/*ʮ��ģ���˻����ȡ������õ�һ��*/
	for (int count = 0; count < 10; count++) {
		genRandomState(); //�������һ����ǰ״̬
		bestStateForEveryIteration = curState; //��¼ÿ��ѭ�������״̬
		double tem = beginTem;
		/*ģ���˻�������*/
		while (tem >= endTem) {
			for (int i = 0; i < iteration; i++) {
				int ran = rand() % 5; //����һ������������������ѡ���������
				/*����ر�һ����ʩ*/
				if (ran < 1) {
					State nextState = closeRandomFacility();
					if (exp((curState.cost - nextState.cost) / tem) >= rand() % 100 / 100.0) {
						curState = nextState;
					}
				}
				/*���������λ�˿�*/
				else if (ran < 3) {
					State nextState = exchangeTwoCustomer();
					if (exp((curState.cost - nextState.cost) / tem) >= rand() % 100 / 100.0) {
						curState = nextState;
					}
				}
				/*�����һλ�˿�ת�Ƶ���һ����ʩ*/
				else {
					State nextState = moveCustomerToAnotherFacility();
					if (exp((curState.cost - nextState.cost) / tem) >= rand() % 100 / 100.0) {
						curState = nextState;
					}
				}
				/*���±���ѭ�������״̬*/
				if (curState.cost < bestStateForEveryIteration.cost) {
					bestStateForEveryIteration = curState;
				}
			}
			tem *= cool; //����
		}
		cout << bestStateForEveryIteration.cost << endl;
		/*����ʮ��ѭ�������״̬*/
		if (bestStateForEveryIteration.cost < bestState.cost)
			bestState = bestStateForEveryIteration;
	}
}

/*�������һ����ʼ״̬*/
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

/*�����һλ�˿�ת�Ƶ���һ����ʩ*/
State SA::moveCustomerToAnotherFacility() {
	while (true) {
		int index = rand() % customer; //���ѡ��һλ�˿�
		int newFacility = rand() % facility; //���ѡ��һ����ʩ
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

/*���������λ�˿�*/
State SA::exchangeTwoCustomer() {
	while (true) {
		int index1 = rand() % customer; //���ѡ��˿�1
		int index2 = rand() % customer; //���ѡ��˿�2
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

/*����ر�һ����ʩ*/
State SA::closeRandomFacility() {
	int closeFacility = rand() % facility; //���ѡ��һ��Ҫ�رյ���ʩ
	State nextState = curState;
	nextState.occupy[closeFacility] = 0;
	for (int j = 0; j < customer; j++) {
		/*�ҵ����䵽Ҫ�ر���ʩ�Ĺ˿�*/
		if (nextState.assign[j] == closeFacility) {
			while (true) {
				int newFacility = rand() % facility; //���Ϊ�ù˿�ѡ��һ������ʩ
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
�жϵ�ǰ��ʩ��ռ��״̬�Ƿ����
occupy:��ʩ��ռ��״̬
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