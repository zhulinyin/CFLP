#include "DataProcess.hpp"

void readInstance(const string fileName, vector<int> &capacity, vector<int> &openCost, vector<double> &demand,
	vector<vector<double>> &assignmentCost) {
	ifstream in(fileName);
	int facilityNum, customerNum;
	in >> facilityNum >> customerNum;
	for (int i = 0; i < facilityNum; i++) {
		int c, oc;
		in >> c >> oc;
		capacity.push_back(c);
		openCost.push_back(oc);

	}
	for (int i = 0; i < customerNum; i++) {
		double d;
		in >> d;
		demand.push_back(d);
	}
	assignmentCost = vector<vector<double>>(customerNum, vector<double>(facilityNum));
	for (int i = 0; i < facilityNum; i++) {
		for (int j = 0; j < customerNum; j++) {
			in >> assignmentCost[j][i];
		}
	}
	in.close();
}

void writeResult(const string resultTable, const string detailFile, int instance, const double &result, const double &time,
	const vector<double> &occupy, const vector<int> &assignment) {
	ofstream resultOut(resultTable, ios::app);
	resultOut << "p" + to_string(instance) << "," << result << "," << time << endl;
	resultOut.close();

	ofstream detailOut(detailFile);
	detailOut << result << endl;
	for (int i = 0; i < occupy.size(); i++) {
		detailOut << (occupy[i] > 0) ? 1 : 0 << ' ';
	}
	detailOut << endl;
	for (int i = 0; i < assignment.size(); i++) {
		detailOut << assignment[i] << ' ';
	}
	detailOut.close();
}