#include "RelationshipTable.h"
#include <unordered_map>
#include <string>

RelationshipTable::RelationshipTable() {
	const int NUM_ONE = 1;
	const int NUM_TWO = 2;
	const string MODIFIES_STRING = "Modifies";
	const string USES_STRING = "Uses";
	const string FOLLOWS_STRING = "Follows";
	const string FOLLOWS_STAR_STRING = "Follows*";
	const string PARENT_STRING = "Parent";
	const string PARENT_STAR_STRING = "Parent*";
	const string NEXT_STRING = "Next";
	const string CALLS_STRING = "Calls";
	const string NEXT_STAR_STRING = "Next*";
	const string CALLS_STAR_STRING = "Calls*";

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
	vector<string> nextArg1;
	vector<string> nextArg2;
	vector<string> nextStarArg1;
	vector<string> nextStarArg2;
	vector<string> callsArg1;
	vector<string> callsArg2;
	vector<string> callsStarArg1;
	vector<string> callsStarArg2;

	//Add the relationship of Modifies to table
	modifiesArg1 = { "stmt", "assign", "while", "prog_line", "if", "call", "procedure",
		"string", "number" };
	modifiesArg2 = { "variable", "string", "wildcard" };
	Relationship relationModifies = Relationship(NUM_TWO, modifiesArg1, modifiesArg2);
	relationshipTable[MODIFIES_STRING] = relationModifies;

	//Add the relationship of Uses to table
	usesArg1 = { "stmt", "assign", "while", "prog_line", "if", "call", "procedure",
		"string", "number"};
	usesArg2 = {"variable", "string", "wildcard"};
	Relationship relationUses = Relationship(NUM_TWO, usesArg1, usesArg2);
	relationshipTable[USES_STRING] = relationUses;

	//Adds the relationship of Follows to table
	followsArg = { "stmt", "assign", "while", "if", "prog_line", "call", "wildcard", "number" };
	Relationship relationFollows = Relationship(NUM_TWO, followsArg, followsArg);
	relationshipTable[FOLLOWS_STRING] = relationFollows;

	//Adds the relationship of Follows* to table
	followsStarArg = { "stmt", "assign", "while", "if", "prog_line", "call", "wildcard", "number" };
	Relationship relationFollowsStar = Relationship(NUM_TWO, followsStarArg, followsStarArg);
	relationshipTable[FOLLOWS_STAR_STRING] = relationFollowsStar;

	//Adds the relationship of Parent to table
	parentArg1 = { "stmt", "while", "if", "prog_line" ,"wilcard", "number" };
	parentArg2 = { "stmt", "while", "if", "wildcard", "prog_line", "call", "assign", "number" };
	Relationship relationParent = Relationship(NUM_TWO, parentArg1, parentArg2);
	relationshipTable[PARENT_STRING] = relationParent;

	//Adds the relationship of Parent* to table
	parentStarArg1 = { "stmt", "while", "if", "prog_line", "wilcard", "number" };
	parentStarArg2 = { "stmt", "while", "if", "wildcard", "prog_line", "call", "assign", "number" };
	Relationship relationParentStar = Relationship(NUM_TWO, parentStarArg1, parentStarArg2);
	relationshipTable[PARENT_STAR_STRING] = relationParentStar;

	//Adds the relationship of Next to table
	nextArg1 = { "stmt", "assign", "while", "if", "call", "prog_line", "number", "wildcard" };
	nextArg2 = { "stmt", "assign", "while", "if", "call", "prog_line", "number", "wildcard" };
	Relationship relationNext = Relationship(NUM_TWO, nextArg1, nextArg2);
	relationshipTable[NEXT_STRING] = relationNext;

	//Since Next and Next*
	nextStarArg1 = nextArg1;
	nextStarArg2 = nextArg2;
	Relationship relationNext = Relationship(NUM_TWO, nextStarArg1, nextStarArg2);
	relationshipTable[NEXT_STAR_STRING] = relationNext;

	//Adds the relationship of Calls to table
	callsArg1 = { "procedure", "string", "wildcard" };
	callsArg2 = { "procedure", "string", "wildcard" };
	Relationship relationCalls = Relationship(NUM_TWO, callsArg1, callsArg2);
	relationshipTable[CALLS_STRING] = relationCalls;

	callsStarArg1 = callsArg1;
	callsStarArg2 = callsArg2;
	Relationship relationCallsStar = Relationship(NUM_TWO, callsStarArg1, callsStarArg2);
	relationshipTable[CALLS_STAR_STRING] = relationCallsStar;

}

bool RelationshipTable::isValidArg(string rel, string type, int argIndex) {
	if (argIndex == 1 && (isValidArg1(rel,type))) {
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

		vector<string> reqArg1 = relationship.getArg1();
		
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
	if (isRelationshipExists(rel) == true) {
		Relationship relationship = relationshipTable.at(rel);
		vector<string> reqArg2 = relationship.getArg2();

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
	std::unordered_map<std::string, Relationship>::iterator it;

	it = relationshipTable.find(rel);

	if (it != relationshipTable.end()) {
		return true;
	}
	return false;
}