//This class holds all the allowed arguments for every relationhips in the such that clause
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
	const string AFFECTS_STRING = "Affects";
	const string AFFECTS_STAR_STRING = "Affects*";

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
	vector<string> affectsArg1;
	vector<string> affectsArg2;

	//Add the relationship of Modifies to table
	modifiesArg1 = { "stmt", "assign", "while", "prog_line", "if", "call", "procedure",
		"string", "number" };
	modifiesArg2 = { "variable", "string", "wildcard" };
	Relationship relationModifies = Relationship(modifiesArg1, modifiesArg2);
	relationshipTable[MODIFIES_STRING] = relationModifies;

	//Add the relationship of Uses to table
	usesArg1 = { "stmt", "assign", "while", "prog_line", "if", "call", "procedure",
		"string", "number"};
	usesArg2 = {"variable", "string", "wildcard"};
	Relationship relationUses = Relationship(usesArg1, usesArg2);
	relationshipTable[USES_STRING] = relationUses;

	//Adds the relationship of Follows to table
	followsArg = { "stmt", "assign", "while", "if", "prog_line", "call", "wildcard", "number" };
	Relationship relationFollows = Relationship(followsArg, followsArg);
	relationshipTable[FOLLOWS_STRING] = relationFollows;

	//Adds the relationship of Follows* to table
	followsStarArg = { "stmt", "assign", "while", "if", "prog_line", "call", "wildcard", "number" };
	Relationship relationFollowsStar = Relationship(followsStarArg, followsStarArg);
	relationshipTable[FOLLOWS_STAR_STRING] = relationFollowsStar;

	//Adds the relationship of Parent to table
	parentArg1 = { "stmt", "while", "if", "prog_line" ,"wildcard", "number" };
	parentArg2 = { "stmt", "while", "if", "wildcard", "prog_line", "call", "assign", "number" };
	Relationship relationParent = Relationship(parentArg1, parentArg2);
	relationshipTable[PARENT_STRING] = relationParent;

	//Adds the relationship of Parent* to table
	parentStarArg1 = { "stmt", "while", "if", "prog_line", "wildcard", "number" };
	parentStarArg2 = { "stmt", "while", "if", "wildcard", "prog_line", "call", "assign", "number" };
	Relationship relationParentStar = Relationship(parentStarArg1, parentStarArg2);
	relationshipTable[PARENT_STAR_STRING] = relationParentStar;

	//Adds the relationship of Next to table
	nextArg1 = { "stmt", "assign", "while", "if", "call", "prog_line", "number", "wildcard" };
	nextArg2 = { "stmt", "assign", "while", "if", "call", "prog_line", "number", "wildcard" };
	Relationship relationNext = Relationship(nextArg1, nextArg2);
	relationshipTable[NEXT_STRING] = relationNext;

	//Since Next and Next*
	nextStarArg1 = nextArg1;
	nextStarArg2 = nextArg2;
	Relationship relationNextStar = Relationship(nextStarArg1, nextStarArg2);
	relationshipTable[NEXT_STAR_STRING] = relationNextStar;

	//Adds the relationship of Calls to table
	callsArg1 = { "procedure", "string", "wildcard" };
	callsArg2 = { "procedure", "string", "wildcard" };
	Relationship relationCalls = Relationship(callsArg1, callsArg2);
	relationshipTable[CALLS_STRING] = relationCalls;

	callsStarArg1 = callsArg1;
	callsStarArg2 = callsArg2;
	Relationship relationCallsStar = Relationship(callsStarArg1, callsStarArg2);
	relationshipTable[CALLS_STAR_STRING] = relationCallsStar;

	//Adds the relationship of Affects to table
	affectsArg1 = { "prog_line", "stmt", "assign", "wildcard", "number" };
	affectsArg2 = { "prog_line", "stmt", "assign", "wildcard", "number" };
	Relationship relationAffects = Relationship(affectsArg1, affectsArg2);
	relationshipTable[AFFECTS_STRING] = relationAffects;

	//Adds the relationship of Affects* to table
	Relationship relationAffectsT = Relationship(affectsArg1, affectsArg2);
	relationshipTable[AFFECTS_STAR_STRING] = relationAffectsT;

}
//This Function takes in a relationship, the type and the argIdx(argNum) and checks if they are permitted with the particular relationship
//Returns true if so, else returns false
bool RelationshipTable::isValidArg(string rel, string type, int argIndex) {
	if (argIndex == 1 && (isValidArg1(rel,type))) {
		return true;
	} else if (argIndex == 2 && isValidArg2(rel, type)) {
		return true;
	} else {
		return false;
	}
}
//This function takes in a relationship and the type of argument it is and checks for the first argument with type of argument
//Returns true if check is valid i.e. exists in a relationship argNum1, else returns false
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
//This function takes in a relationship and the type of argument it is and checks for the second argument with type of argument
//Returns true if check is valid i.e. exists in a relationship argNum2, else returns false
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
//This function takes in a string which is a relationship and checks if it exists as a key in the relationshipTable Object
//Returns true if exists, else returns false;
bool RelationshipTable::isRelationshipExists(string rel) {
	std::unordered_map<std::string, Relationship>::iterator it;
	it = relationshipTable.find(rel);
	if (it != relationshipTable.end()) {
		return true;
	}
	return false;
}