#include "DataProcess.hpp"
#include "SA.hpp"
#include "Greedy.hpp"
#include <ctime>
#define DATAPATH "Instances/p"
#define SARESULTPATH "SAresult/"
#define GREEDYRESULTPATH "Greedyresult/"

/*int main() {
	ofstream resultOut(GREEDYRESULTPATH + string("result.csv"));
	resultOut << "," << "Result" << "," << "Time(s)" << endl;
	resultOut.close();
	for (int i = 1; i < 72; i++) {
		cout << "Instance p" << i << endl;
		double startTime = clock();
		vector<int> capacity;
		vector<int> openCost;
		vector<double> demand;
		vector<vector<double>> assignmentCost;
		readInstance(DATAPATH + to_string(i), capacity, openCost, demand, assignmentCost);
		Greedy greedy(capacity, openCost, demand, assignmentCost);
		State bestState = greedy.getBestState();
		double endTime = clock();
		writeResult(GREEDYRESULTPATH + string("result.csv"), GREEDYRESULTPATH + string("p") + to_string(i), i, bestState.cost,
			(endTime - startTime) / CLOCKS_PER_SEC, bestState.occupy, bestState.assign);
	}
}*/

int main() {
	ofstream resultOut(SARESULTPATH + string("result.csv"));
	resultOut << "," << "Result" << "," << "Time(s)" << endl;
	resultOut.close();
	for (int i = 1; i < 2; i++) {
		cout << "Instance p" << i << endl;
		double startTime = clock();
		vector<int> capacity;
		vector<int> openCost;
		vector<double> demand;
		vector<vector<double>> assignmentCost;
		readInstance(DATAPATH + to_string(i), capacity, openCost, demand, assignmentCost);
		SA sa(capacity, openCost, demand, assignmentCost);
		sa.run(300, 0.01, 0.99, 200);
		double endTime = clock();
		writeResult(SARESULTPATH + string("result.csv"), SARESULTPATH + string("p") + to_string(i), i, sa.getBestState().cost,
			(endTime - startTime) / CLOCKS_PER_SEC, sa.getBestState().occupy, sa.getBestState().assign);
	}
}