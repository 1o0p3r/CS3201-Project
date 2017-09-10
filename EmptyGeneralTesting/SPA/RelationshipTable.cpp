#include "RelationshipTable.h"
#include <unordered_map>;
#include <string>;

RelationshipTable::RelationshipTable() {

	const int NUM_TWO = 2;
	const string MODIFIES_STRING = "modfiies";
	const string USES_STRING = "uses";
	const string FOLLOWS_STRING = "follows";
	const string FOLLOWS_STAR_STRING = "follows*";
	const string PARENT_STRING = "parent";
	const string PARENT_STAR_STRING = "parent*";

	//variables to be used to add to relationship table
	vector<string> modifiesArg1;
	vector<string> modifiesArg2;
	vector<string> usesArg1;
	vector<string> usesArg2;
	vector<string> followsArg;
	vector<string> followsStarArg;
	vector<string> parentArg1;
	vector<string> parentArg2;
	vector<string> parentStarArg1;
	vector<string> parentStarArg2;

	//Add the relationship of Modifies to table
	modifiesArg1 = { "stmt", "assignment", "while", "prog_line", "if", "procedure", "string", "constant" };
	modifiesArg2 = { "variable", "string", "wildCard" };
	Relationship relationModifies(NUM_TWO, modifiesArg1, modifiesArg2);
	relationshipTable[MODIFIES_STRING] = relationModifies;

	//Add the relationship of Uses to table
	usesArg1 = { "stmt", "assignment", "while", "prog_line", "if", "procedure", "string", "constant" };
	usesArg2 = {"variable", "string", "wildCard"};
	Relationship relationUses(NUM_TWO, usesArg1, usesArg2);
	relationshipTable[USES_STRING] = relationUses;

	//Adds the relationship of Follows to table
	followsArg = { "stmt", "assignment", "while", "if", "constant", "prog_line", "call", "wildcard" };
	Relationship relationFollows(NUM_TWO, followsArg, followsArg);
	relationshipTable[FOLLOWS_STRING] = relationFollows;

	//Adds the relationship of Follows* to table
	followsStarArg = { "stmt", "assignment", "while", "if", "constant", "prog_line", "call", "wildcard" };
	Relationship relationFollowsStar(NUM_TWO, followsStarArg, followsStarArg);
	relationshipTable[FOLLOWS_STAR_STRING] = relationFollowsStar;

	//Adds the relationship of Parent to table
	parentArg1 = { "stmt", "while", "if", "constant", "prog_line" ,"wilcard" };
	parentArg2 = { "stmt", "while", "if", "constant", "wildcard", "prog_line", "call", "assignment" };
	Relationship relationParent(NUM_TWO, parentArg1, parentArg2);
	relationshipTable[PARENT_STRING] = relationParent;

	//Adds the relationship of Parent* to table
	parentStarArg1 = { "stmt", "while", "if", "constant", "prog_line", "wilcard" };
	parentStarArg2 = { "stmt", "while", "if", "constant", "wildcard", "prog_line", "call", "assignment" };
	Relationship relationParentStar(NUM_TWO, parentStarArg1, parentStarArg2);
	relationshipTable[PARENT_STAR_STRING] = relationParentStar;
}

bool RelationshipTable::isValidArg(string rel, string type, int argIndex) {
	if (argIndex == 1 && isValidArg1(rel,type)) {
		return true;
	}
	else if (argIndex == 2 && isValidArg2(rel, type)) {
		return true;
	}
	else {
		return false;
	}
}
bool RelationshipTable::isValidArg1(string rel, string typeArg) {
	if (isRelationshipExists(rel)) {
		Relationship relationship = relationshipTable.at(rel);
		vector<string> reqArg1 = relationship.getArg1;

		for (std::vector<string>::iterator it = reqArg1.begin(); it != reqArg1.end(); it++) {
			if (typeArg == (*it)) {
				return true;
			}
		}
		return false;
	}
	return false;
}
bool RelationshipTable::isValidArg2(string rel, string typeArg) {
	if (isRelationshipExists(rel)) {
		Relationship relationship = relationshipTable.at(rel);
		vector<string> reqArg2 = relationship.getArg2;

		for (std::vector<string>::iterator it = reqArg2.begin(); it != reqArg2.end(); it++) {
			if (typeArg == (*it)) {
				return true;
			}
		}
		return false;
	}
	return false;
}
bool RelationshipTable::isRelationshipExists(string rel) {
	if (relationshipTable.find(rel) == relationshipTable.end()) {
		return true;
	}
	return false;
}
bool RelationshipTable::isValidNumArgs(string rel, int numArgs) {
	
	return false;
}