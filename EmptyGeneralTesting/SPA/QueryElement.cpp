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
QueryElement::QueryElement(string arg1, string arg2, string patternEntity, string patternSynonym, int isPattern)
{
	argumentOnePattern = arg1;
	argumentTwoPattern = arg2;
	patternEnt = patternEntity;
	patternSyn = patternSynonym;
	isPatt = isPattern;
}

string QueryElement::getSelectEntity()
{
	return ent;
}

string QueryElement::getSelectSynonym()
{
	return syn;
}

string QueryElement::getSuchThatArg1(int idx)
{
	return argumentOneSuchThat;
}

string QueryElement::getSuchThatArg2(int idx)
{
	return argumentTwoSuchThat;
}

string QueryElement::getSuchThatArg1Type(int idx)
{
	return argumentOneTypeSuchThat;
}

string QueryElement::getSuchThatArg2Type(int idx)
{
	return argumentTwoTypeSuchThat;
}

string QueryElement::getPatternArg1(int idx)
{
	return argumentOnePattern;
}

string QueryElement::getPatternArg2(int idx)
{
	return argumentTwoPattern;
}

string QueryElement::getPatternEntity(int idx)
{
	return patternEnt;
}

string QueryElement::getPatternSynonym(int idx)
{
	return patternSyn;
}
string QueryElement :: getSuchThatRel() {
	return relSuchThatType;
}