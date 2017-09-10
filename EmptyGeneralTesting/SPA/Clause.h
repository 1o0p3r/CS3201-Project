#pragma once
#include <string>
#include <vector>

using namespace std;;
class Clause
{
private:
	string clauseType;
	string relationshipType;
	string argument1;
	string argument2;
	vector<string> result;
public:
	Clause(string, vector<string>); //Selct clause
	Clause(string, string, string, string); //Relationship clause
	Clause(string, string, string); //Pattern clause
	string getRelationship();
	string getClauseType();
	string getRelationshipType();
	string getArg1();
	string getArg2();

};

