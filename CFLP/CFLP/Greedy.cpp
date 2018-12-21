#include "Greedy.hpp"
#include <algorithm>


Greedy::Greedy(const vector<int> &capacity, const vector<int> &openCost, const vector<double> &demand,
	const vector<vector<double>> &assignmentCost):capacity(capacity), openCost(openCost), demand(demand),
	assignmentCost(assignmentCost) {
	facility = capacity.size();
	customer = demand.size();
}

/*
��ȡ̰���㷨ִ�к��״̬
State.cost:��״̬�ķ���
State.occupy:ÿ����ʩ�ѷ���Ŀռ�
State.assign:ÿ���˿����������ʩ
*/
State Greedy::getBestState() {
	vector<int> assign(customer, -1);
	vector<double> occupy(facility, 0);
	vector<vector<bool>> alloc(customer, vector<bool>(facility, true));
	for (int i = 0; i < customer; i++) {
		int custom, faci;
		while (true) {
			findMin(alloc, custom, faci);
			/*����ʩ�����㹻*/
			if (occupy[faci] + demand[custom] <= capacity[faci]) {
				for (int j = 0; j < alloc[custom].size(); j++) {
					alloc[custom][j] = false;
				}
				occupy[faci] += demand[custom];
				assign[custom] = faci;
				break;
			}
			/*����ʩ��������*/
			else {
				alloc[custom][faci] = false;
			}
		}
	}
	double cost = calculateCost(occupy, assign); //����cost
	return State(cost, occupy, assign);
}
/*
�ҵ�cost��С�ҿ��Ա�����Ĺ˿ͺ���ʩ
alloc[i][j]:�˿�i�Ƿ���Է��䵽��ʩj
custom:�ҵ��Ĺ˿�
faci:�ҵ�����ʩ
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
����cost
occupy:ÿ����ʩ�ѷ���Ŀռ�
assign:ÿ���˿ͷ������ʩ
*/
double Greedy::calculateCost(const vector<double> &occupy, const vector<int> &assign) {
	double cost = 0;
	for (int i = 0; i < facility; i++) {
		cost += (occupy[i] > 0) ? openCost[i] : 0; //�жϸ���ʩ�Ƿ��ѷ��䣬����ǣ�����Ͽ�������ʩ�Ĵ���
	}
	for (int i = 0; i < customer; i++) {
		cost += assignmentCost[i][assign[i]];
	}
	return cost;
}
