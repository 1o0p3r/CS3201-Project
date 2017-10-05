#include "Parent.h"

Parent::Parent() {
	vector<vector<int>> parentTable;
	vector<int> childTable;
	vector<vector<int>> parentStarTable;
	vector<vector<int>> childStarTable;
	set<int> allParentTable;
}

void Parent::setParent(int s1, int s2) {
	if (s1 <= 0 || s2 <= 0) {
		throw new invalid_argument("Statement Number must be more than 0.");
	}
	if (parentTable.size() <= s1) {
		parentTable.resize(s1 + 1);
	}
	parentTable[s1].push_back(s2);
	allParentTable.insert(s2);
	setChild(s2, s1);
	setParentStar(s1, s2);
	vector<int> holder = getParent(s1);
	while (!holder.empty()) {
		int grandparent = holder[0];
		setParentStar(grandparent, s2);
		holder = getParent(grandparent);
	}
}

void Parent::setChild(int s1, int s2) {
	if (childTable.size() <= s1) {
		childTable.resize(s1 + 1);
	}
	childTable[s1] = s2;
}

void Parent::setParentStar(int s1, int s2) {
	if (s1 <= 0 || s2 <= 0) {
		throw new invalid_argument("Statement Number must be more than 0.");
	}
	if (parentStarTable.size() <= s1) {
		parentStarTable.resize(s1 + 1);
	}
	parentStarTable[s1].push_back(s2);
	setChildStar(s2, s1);
}

void Parent::setChildStar(int s1, int s2) {
	if (childStarTable.size() <= s1) {
		childStarTable.resize(s1 + 1);
	}
	childStarTable[s1].push_back(s2);
}

vector<int> Parent::getChild(int statementNum) {
	if (parentTable.size() > statementNum) {
		return parentTable[statementNum];
	} else {
		return vector<int>();
	}
}

vector<int> Parent::getParent(int statementNum) {
	if (childTable.size() > statementNum) {
		vector<int> result(1);
		result[0] = childTable[statementNum];
		if (result[0] == 0) {
			return vector<int>();
		} else {
			return result;
		}
	} else {
		return vector<int>();
	}
}

vector<int> Parent::getChildStar(int statementNum) {
	if (parentStarTable.size() > statementNum) {
		return parentStarTable[statementNum];
	} else {
		return vector<int>();
	}
}

vector<int> Parent::getParentStar(int statementNum) {
	if (childStarTable.size() > statementNum) {
		return childStarTable[statementNum];
	} else {
		return vector<int>();
	}
}

vector<int> Parent::getAllParent() {
	if (allParentTable.size() > 0) {
		vector<int> result;
		result.insert(result.end(), allParentTable.begin(), allParentTable.end());
		return result;
	}
	else {
		return vector<int>();
	}
}