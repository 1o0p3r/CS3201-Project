#pragma once

#include "Follow.h"
#include "Parent.h"
#include "Modify.h"
#include "Use.h"
#include <vector>

using namespace std;

class QueryEval
{
public:
	static const int ASSIGNMOD = 0;
	static const int ASSIGNUSE = 0;
	static const int VARMODBY = 1;
	static const int VARUSEBY = 1;
	static const int PROCMOD = 2;
	static const int PROCUSE = 2;
	static const int PROCMODBY = 3;
	static const int PROCUSEBY = 3;

	QueryEval();
	~QueryEval();

private:
	vector<int> followResult(int s1, int opt);
	vector<int> followStarResult(int s1, int opt);
	vector<int> parentResult(int s1, int opt);
	vector<int> parenStarResult(int s1, int opt);
	vector<int> modifiesResult(int s1, int opt);
	vector<int> usesResult(int s1, int opt);
};

