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
	
public:
	QueryElement();
	QueryElement(string entity, string synonym);
	QueryElement(string arg1, string arg1Type, string arg2, string arg2Type, string rel); //suchThat
	QueryElement(string arg1, string arg2, string patternEntity, string patternSynonym, string patternArg1Type, string patternArg2Type); //Pattern

	string getSelectEntity();
	string getSelectSynonym();

	string getSuchThatArg1();
	string getSuchThatArg2();
	string getSuchThatArg1Type();
	string getSuchThatArg2Type();
	string getSuchThatRel();

	string getPatternArg1();
	string getPatternArg2();
	string getPatternEntity();
	string getPatternSynonym();
	string getPatternArg1Type();
	string getPatternArg2Type();

};

