//This class holds the representation of information of clause i.e. result-cl, suchthat-cl. pattern-cl, with-cl 
#include "QueryElement.h"
#include <string>

using namespace std;

QueryElement::QueryElement() {
}

//Select
//type can be synonym, tuple, BOOLEAN
QueryElement::QueryElement(string entity, string synonym, string type, string attrName) {
	ent = entity;
	syn = synonym;
	selectType = type;
	synAttr = attrName;
}

//Such that: 7 parameteres
QueryElement::QueryElement(string arg1, string arg1Type, string arg1Entity, string arg2, string arg2Type, string arg2Entity, string rel, string clause) {
	argumentOneSuchThat = arg1;
	argumentOneTypeSuchThat = arg1Type;
	argumentOneEntitySuchThat = arg1Entity;
	argumentTwoSuchThat = arg2;
	argumentTwoTypeSuchThat = arg2Type;
	argumentTwoEntitySuchThat = arg2Entity;
	relSuchThatType = rel;
	clauseType = clause;
}

//Pattern  9 paramaters
QueryElement::QueryElement(string arg1, string arg2, string arg3, string patternEntity, string patternSynonym, string argOneType, string argTwoType, string argThreeType, string argOneEntity, string clause) {
	argumentOnePattern = arg1;
	argumentTwoPattern = arg2;
	argumentThreePattern = arg3;
	patternEnt = patternEntity;
	patternSyn = patternSynonym;
	patternArg1Type = argOneType;
	patternArg2Type = argTwoType;
	patternArg3Type = argThreeType;
	patternArg1Ent = argOneEntity;
	clauseType = clause;
}

//With:8 parameters
QueryElement::QueryElement(string arg1Type, string arg2Type, string ent1Type, string ent2Type, string arg1Syn, string arg2Syn, string clause) {
	argumentOneTypeWith = arg1Type;
	argumentTwoTypeWith = arg2Type;
	argumentOneEntWith = ent1Type;
	argumentTwoEntWith = ent2Type;
	argumentOneSynWith = arg1Syn;
	argumentTwoSynWith = arg2Syn;
	clauseType = clause;
}
//This function returns the select entity of given queryElement object
string QueryElement::getSelectEntity() {
	return ent;
}
//This function returns the select synonym of given queryElement object
string QueryElement::getSelectSynonym() {
	return syn;
}
//This function returns the Select Type(synonym|tuples|BOOLEAN) of given queryElement object
string QueryElement::getSelectType() {
	return selectType;
}
//This function returns the synAttr of given queryElement object
string QueryElement::getSynAtrr() {
	return synAttr;
}
//This function returns the relationship of the associated suchThat queryElement object
string QueryElement::getSuchThatRel() {
	return relSuchThatType;
}
//This function returns the arg1 of given such that queryElement object
string QueryElement::getSuchThatArg1() {
	return argumentOneSuchThat;
}
//This function returns the arg2 of given such that queryElement object
string QueryElement::getSuchThatArg2() {
	return argumentTwoSuchThat;
}
//This function returns the arg1 Type of given such that queryElement object
string QueryElement::getSuchThatArg1Type() {
	return argumentOneTypeSuchThat;
}
//This function returns the arg1 design entity of given such that queryElement object
string QueryElement::getSuchThatArg1Entity() {
	return argumentOneEntitySuchThat;
}
//This function returns the arg2 Type of given such that queryElement object
string QueryElement::getSuchThatArg2Type() {
	return argumentTwoTypeSuchThat;
}
//This function returns the arg2 design entity of given such that queryElement object
string QueryElement::getSuchThatArg2Entity() {
	return argumentTwoEntitySuchThat;
}
//This function returns the synonym of the associated pattern queryElement object
string QueryElement::getPatternSynonym() {
	return patternSyn;
}
//This function returns the arg1 of a pattern queryElement object
string QueryElement::getPatternArg1() {
	return argumentOnePattern;
}
//This function returns the arg2 of a pattern queryElement object
string QueryElement::getPatternArg2() {
	return argumentTwoPattern;
}
//This function returns the arg3 of a pattern queryElement object
string QueryElement::getPatternArg3() {
	return argumentThreePattern;
}
//This function returns the design entity of a pattern queryElement object
string QueryElement::getPatternEntity() {
	return patternEnt;
}
//This function returns the arg1 type of a pattern queryElement object
string QueryElement::getPatternArg1Type() {
	return patternArg1Type;
}
//This function returns the arg2 type of a pattern queryElement object
string QueryElement::getPatternArg2Type() {
	return patternArg2Type;
}
//This function returns the arg3 type of a pattern queryElement object
string QueryElement::getPatternArg3Type() {
	return patternArg3Type;
}
//This function returns the arg1 design Entity of a pattern queryElement object
string QueryElement::getPatternArg1Ent() {
	return patternArg1Ent;
}
//This function returns the arg1 Type of a with queryElement object
string QueryElement::getWithArg1Type() {
	return argumentOneTypeWith;
}
//This function returns the arg2 Type of a with queryElement object
string QueryElement::getWithArg2Type() {
	return argumentTwoTypeWith;
}
//This function returns the arg1 design entity of a with queryElement object
string QueryElement::getWithArg1Ent() {
	return argumentOneEntWith;
}
//This function returns the arg2 design entity of a with queryElement object
string QueryElement::getWithArg2Ent() {
	return argumentTwoEntWith;
}
//This function returns the arg1 Synonym of a with queryElement object
string QueryElement::getWithArg1Syn() {
	return argumentOneSynWith;
}
//This function returns the arg2 Synonym of a with queryElement object
string QueryElement::getWithArg2Syn() {
	return argumentTwoSynWith;
}
//This function returns the clauseType(suchThat, pattern, with) of a queryElement object
string QueryElement::getClauseType() {
	return clauseType;
}