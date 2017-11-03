#include "QueryElement.h"
#include <string>

using namespace std;

QueryElement::QueryElement()
{
}

//Select
//type can be synonym, tuple, BOOLEAN
QueryElement::QueryElement(string entity, string synonym, string type, string attrName)
{
	ent = entity;
	syn = synonym;
	selectType = type;
	synAttr = attrName;
}

//Such that: 7 parameteres
QueryElement::QueryElement(string arg1, string arg1Type, string arg1Entity, string arg2, string arg2Type, string arg2Entity, string rel, string clause)
{
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
QueryElement::QueryElement(string arg1, string arg2, string arg3, string patternEntity, string patternSynonym, string argOneType, string argTwoType, string argThreeType, string argOneEntity, string clause)
{
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

string QueryElement::getSelectEntity()
{
	return ent;
}

string QueryElement::getSelectSynonym()
{
	return syn;
}

string QueryElement::getSelectType()
{
	return selectType;
}

string QueryElement::getSynAtrr()
{
	return synAttr;
}

string QueryElement::getSuchThatArg1()
{
	return argumentOneSuchThat;
}

string QueryElement::getSuchThatArg2()
{
	return argumentTwoSuchThat;
}

string QueryElement::getSuchThatArg1Type()
{
	return argumentOneTypeSuchThat;
}

string QueryElement::getSuchThatArg1Entity()
{
	return argumentOneEntitySuchThat;
}

string QueryElement::getSuchThatArg2Type()
{
	return argumentTwoTypeSuchThat;
}

string QueryElement::getSuchThatArg2Entity()
{
	return argumentTwoEntitySuchThat;
}

string QueryElement::getPatternArg1()
{
	return argumentOnePattern;
}

string QueryElement::getPatternArg2()
{
	return argumentTwoPattern;
}

string QueryElement::getPatternArg3()
{
	return argumentThreePattern;
}

string QueryElement::getPatternEntity()
{
	return patternEnt;
}
string QueryElement::getPatternArg1Type() 
{
	return patternArg1Type;
}
string QueryElement::getPatternArg2Type() 
{
	return patternArg2Type;
}
string QueryElement::getPatternArg3Type()
{
	return patternArg3Type;
}
string QueryElement::getPatternArg1Ent()
{
	return patternArg1Ent;
}
string QueryElement::getWithArg1Type()
{
	return argumentOneTypeWith;
}
string QueryElement::getWithArg2Type()
{
	return argumentTwoTypeWith;
}
string QueryElement::getWithArg1Ent()
{
	return argumentOneEntWith;
}
string QueryElement::getWithArg2Ent()
{
	return argumentTwoEntWith;
}
string QueryElement::getWithArg1Syn()
{
	return argumentOneSynWith;
}
string QueryElement::getWithArg2Syn()
{
	return argumentTwoSynWith;
}
string QueryElement::getPatternSynonym()
{
	return patternSyn;
}
string QueryElement :: getSuchThatRel() 
{
	return relSuchThatType;
}
string QueryElement::getClauseType() 
{
	return clauseType;
}