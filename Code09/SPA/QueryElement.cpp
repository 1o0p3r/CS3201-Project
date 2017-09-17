#include "QueryElement.h"
#include <string>

using namespace std;

QueryElement::QueryElement()
{
}

QueryElement::QueryElement(string entity, string synonym)
{
	ent = entity;
	syn = synonym;

}

//Such that
QueryElement::QueryElement(string arg1, string arg1Type, string arg2, string arg2Type, string rel)
{
	argumentOneSuchThat = arg1;
	argumentOneTypeSuchThat = arg1Type;
	argumentTwoSuchThat = arg2;
	argumentTwoTypeSuchThat = arg2Type;
	relSuchThatType = rel;

}

//Pattern
QueryElement::QueryElement(string arg1, string arg2, string patternEntity, string patternSynonym, string argOneType, string argTwoType)
{
	argumentOnePattern = arg1;
	argumentTwoPattern = arg2;
	patternEnt = patternEntity;
	patternSyn = patternSynonym;
	patternArg1Type = argOneType;
	patternArg2Type = argTwoType;
}

string QueryElement::getSelectEntity()
{
	return ent;
}

string QueryElement::getSelectSynonym()
{
	return syn;
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

string QueryElement::getSuchThatArg2Type()
{
	return argumentTwoTypeSuchThat;
}

string QueryElement::getPatternArg1()
{
	return argumentOnePattern;
}

string QueryElement::getPatternArg2()
{
	return argumentTwoPattern;
}

string QueryElement::getPatternEntity()
{
	return patternEnt;
}
string QueryElement::getPatternArg1Type() {
	return patternArg1Type;
}
string QueryElement::getPatternArg2Type() {
	return patternArg2Type;
}
string QueryElement::getPatternSynonym()
{
	return patternSyn;
}
string QueryElement :: getSuchThatRel() {
	return relSuchThatType;
}