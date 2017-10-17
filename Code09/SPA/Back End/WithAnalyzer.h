#pragma once
#include "PKB.h"
#include "QueryElement.h"

#include <unordered_map>

class WithAnalyzer
{
private:
	PKB pkbPtr;
	unordered_map<string, int> argSynTypeMap;
	void initArgSynTypeMap();

	//extract out of class
	vector<string> getStmtResults(string);
	vector<string> getAllProcName(string);

	void addSynToVec(vector<vector<string>> &, const vector<string>&, const vector<string>&,bool &);

	string arg1; //arg1Syn
	string arg1Type;
	string arg1Ent;

	string arg2; //arg2Syn
	string arg2Type;
	string arg2Ent;

public:
	WithAnalyzer(QueryElement, PKB&);
	tuple<bool, vector<vector<string>>> analyze();
};

