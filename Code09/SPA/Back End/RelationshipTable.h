//This class holds all the allowed arguments for every relationhips in the such that clause
#pragma once
#include <string>
#include "Relationship.h"
#include <unordered_map>

using namespace std;
class RelationshipTable
{
private:
	unordered_map<string, Relationship> relationshipTable;
	bool isValidArg1(string rel, string type);
	bool isValidArg2(string rel, string type);
public:
	RelationshipTable();
	bool isValidArg(string rel, string type, int numArg);
	bool isRelationshipExists(string rel);
};

