#pragma once
#include <vector>
#include <tuple>
#include <set>

using namespace std;

class Next {
public:
	Next();
	void setNext(int stmtNum1, int stmtNum2);
	vector<int> getNext(int stmtNum);
	vector<int> getPrevious(int stmtNum);
	vector<int> Next::getAllNext();

	int getNextCount();

private:
	void setPrevious(int stmtNum1, int stmtNum2);
	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<int> allNextTable;
	int count;
};
