#pragma once
#include <vector>

using namespace std;

class Parent {
public:
	Parent();
	void setParent(int s1, int s2);

	vector<int> getParent(int statementNum);
	vector<int> getChild(int statementNum);
	vector<int> getParentStar(int statementNum);
	vector<int> getChildStar(int statementNum);

private:
	vector<int> parentTable;
	vector<int> childTable;
	vector<vector<int>> parentStarTable;
	vector<vector<int>> childStarTable;

	void setChild(int s1, int s2);
	void setParentStar(int s1, int s2);
	void setChildStar(int s1, int s2);
};