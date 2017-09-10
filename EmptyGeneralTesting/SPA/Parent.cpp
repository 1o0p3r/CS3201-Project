#include "Parent.h"

Parent::Parent() {
	vector<int> parentTable;
	vector<int> childTable;
	vector<vector<int>> parentStarTable;
	vector<vector<int>> childStarTable;
}

void Parent::setParent(int s1, int s2) {
	if (s1 <= 0 || s2 <= 0) {
		throw new invalid_argument("Statement Number must be more than 0.");
	}
	parentTable.resize(s1 + 1);
	parentTable[s1] = s2;
	setChild(s2, s1);
	setParentStar(s1, s2);
	int grandparent = getParent(s1)[0];
	while (grandparent != 0) {
		setParentStar(grandparent, s2);
		grandparent = getParent(grandparent)[0];
	}
}

void Parent::setChild(int s1, int s2) {
	childTable.resize(s1 + 1);
	childTable[s1] = s2;
}

void Parent::setParentStar(int s1, int s2) {
	if (s1 <= 0 || s2 <= 0) {
		throw new invalid_argument("Statement Number must be more than 0.");
	}
	parentStarTable.resize(s1 + 1);
	parentStarTable[s1].push_back(s2);
	setChildStar(s2, s1);
}

void Parent::setChildStar(int s1, int s2) {
	childStarTable.resize(s1 + 1);
	childStarTable[s1].push_back(s2);
}

vector<int> Parent::getChild(int statementNum) {
	vector<int> result(1);
	result[0] = parentTable[statementNum];
	return result;
}

vector<int> Parent::getParent(int statementNum) {
	vector<int> result(1);
	result[0] = childTable[statementNum];
	return result;
}

vector<int> Parent::getChildStar(int statementNum) {
	return parentStarTable[statementNum];
}

vector<int> Parent::getParentStar(int statementNum) {
	return childStarTable[statementNum];
}