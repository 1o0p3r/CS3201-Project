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
	string patternArg1Type;
	string patternArg2Type;
	int isSubstring;
public:
	QueryElement();
	QueryElement(string entity, string synonym);
	QueryElement(string arg1, string arg1Type, string arg2, string arg2Type, string rel); //suchThat
	QueryElement(string arg1, string arg2, string patternEntity, string patternSynonym, string patternArg1Type, string patternArg2Type); //Pattern

	string getSelectEntity();
	string getSelectSynonym();
	string getSuchThatArg1(int idx);
	string getSuchThatArg2(int idx);
	string getSuchThatArg1Type(int idx);
	string getSuchThatArg2Type(int idx);

	string getPatternArg1(int idx);
	string getPatternArg2(int idx);
	string getPatternEntity(int idx);
	string getPatternArg1Type();
	string getPatternArg2Type();
	string getPatternSynonym(int idx);
	string getSuchThatRel();
};

