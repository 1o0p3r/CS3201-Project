#pragma once
#include <string>

using namespace std;
class QueryElement
{	
private:
	string ent;
	string syn;
	string argumentOneSuchThat;
	string argumentTwoSuchThat;
	string argumentOneTypeSuchThat;
	string argumentTwoTypeSuchThat;
	string relSuchThatType;
	string argumentOnePattern;
	string argumentTwoPattern;
	string patternEnt;
	string patternSyn;
	int isPatt;
public:
	QueryElement();
	QueryElement(string entity, string synonym);
	QueryElement(string arg1, string arg1Type, string arg2, string arg2Type, string rel); //suchThat
	QueryElement(string arg1, string arg2, string patternEntity, string patternSynonym, int isPattern); //Pattern

	string getSelectEntity();
	string getSelectSynonym();
	string getSuchThatArg1(int idx);
	string getSuchThatArg2(int idx);
	string getSuchThatArg1Type(int idx);
	string getSuchThatArg2Type(int idx);

	string getPatternArg1(int idx);
	string getPatternArg2(int idx);
	string getPatternEntity(int idx);
	string getPatternSynonym(int idx);
	string getSuchThatRel();
};

