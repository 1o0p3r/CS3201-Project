#pragma once
#include <vector>
#include <tuple>
#include <set>

using namespace std;

class Next {
public:
	Next();

	//void createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, int firstLine, int lastLine);
	void setNext(int stmtNum1, int stmtNum2);
	vector<int> getNext(int stmtNum);
	vector<int> getPrevious(int stmtNum);
	vector<int> Next::getAllNext();

private:
	void setPrevious(int stmtNum1, int stmtNum2);
	vector<set<int>> nextTable;
	vector<set<int>> previousTable;
	set<int> allNextTable;
};
