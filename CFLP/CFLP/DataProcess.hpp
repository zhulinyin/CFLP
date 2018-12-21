#include <vector>
#include <fstream>
#include <string>
using namespace std;

void readInstance(const string fileName, vector<int> &capacity, vector<int> &openCost, vector<double> &demand,
	vector<vector<double>> &assignmentCost);

void writeResult(const string resultTable, const string detailFile, int instance, const double &result, const double &time,
	const vector<double> &occupy, const vector<int> &assignment);