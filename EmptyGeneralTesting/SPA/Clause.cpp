#include "Clause.h"
#include <string>

using namespace std;

//SELECT CLAUSE
Clause::Clause(string cType, vector<string> r)
{
	clauseType = cType;
	result = r;
}

//Relationship clause
Clause:: Clause(string cType, string rType, string arg1, string arg2) {
	clauseType = cType;	//e.g. Select clause, relationship clause, pattern clause
	relationshipType = rType;
	argument1 = arg1;
	argument2 = arg2;
}

Clause::Clause(string cType, string arg1, string arg2) {
	clauseType = cType;
	argument1 = arg1;
	argument2 = arg2;
}
string Clause::getRelationship(){
	return string();
}
string Clause::getClauseType() {
	return clauseType;
}
string Clause::getRelationshipType() {
	return relationshipType;
}
string Clause::getArg1() {
	return argument1;
}
string Clause::getArg2() {
	return argument2;
}