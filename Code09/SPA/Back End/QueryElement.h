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
	string argumentOneEntitySuchThat;
	string argumentTwoTypeSuchThat;
	string argumentTwoEntitySuchThat;
	string relSuchThatType;

	string argumentOnePattern;
	string argumentTwoPattern;
	string patternEnt;
	string patternSyn;
	string patternArg1Type;
	string patternArg2Type;
	
	string argumentOneWith;
	string argumentTwoWith;
	string argumentOneTypeWith;
	string argumentTwoTypeWith;
	string argumentOneEntWith;
	string argumentTwoEntWith;
	string argumentOneSynWith;
	string argumentTwoSynWith;


public:
	QueryElement();
	QueryElement(string entity, string synonym);
	QueryElement(string arg1, string arg1Type, string arg1Entity, string arg2, string arg2Type, string arg2Entity, string rel); // such that
	QueryElement(string arg1, string arg2, string patternEntity, string patternSynonym, string patternArg1Type, string patternArg2Type); //Pattern
	QueryElement(string arg1, string arg2, string arg1Type, string arg2Type, string ent1Type, string ent2Type, string arg1Syn, string arg2Syn);

	string getSelectEntity();
	string getSelectSynonym();

	string getSuchThatArg1();
	string getSuchThatArg2();
	string getSuchThatArg1Type();
	string getSuchThatArg1Entity();
	string getSuchThatArg2Type();
	string getSuchThatArg2Entity();
	string getSuchThatRel();

	string getPatternArg1();
	string getPatternArg2();
	string getPatternEntity();
	string getPatternSynonym();
	string getPatternArg1Type();
	string getPatternArg2Type();

};

