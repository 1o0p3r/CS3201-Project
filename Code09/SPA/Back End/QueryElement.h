#pragma once
#include <string>

using namespace std;
class QueryElement
{	
private:
	string ent;
	string syn;
	string selectType;
	string synAttr;

	string argumentOneSuchThat;
	string argumentTwoSuchThat;
	string argumentOneTypeSuchThat;
	string argumentOneEntitySuchThat;
	string argumentTwoTypeSuchThat;
	string argumentTwoEntitySuchThat;
	string relSuchThatType;

	string argumentOnePattern;
	string argumentTwoPattern;
	string argumentThreePattern;
	string patternEnt;
	string patternSyn;
	string patternArg1Type;
	string patternArg2Type;
	string patternArg3Type;
	string patternArg1Ent;
	
	
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

	
	QueryElement(string entity, string synonym, string type);
	
	
	QueryElement(string arg1, string arg1Type, string arg1Entity, string arg2, string arg2Type, string arg2Entity, string rel); // such that
	
	


	//Pattern Clauses
	//E.g. pattern a2(_, "x")
	//arg1 = "_"	arg2 = "x"	arg3 = "empty"	patternEntity = assign	patternSynonym = a2	patternArg1Type = wildcard	patternArg2Type	= 
	QueryElement(string arg1, string arg2, string arg3, string patternEntity, string patternSynonym, string patternArg1Type, string patternArg2Type, string patternArg3Type, string patternEnt1); //Pattern
	
	//With Clauses
	//E.g. procedure p; variable v; 
	//with p.procName = v.varName
	//with n=1
	//arg 1 = p.procName	arg2 = v.varName	arg1Type = procName		arg2Type = varName		arg1Ent = procedure		arg2Ent = variable		arg1Syn = p		arg1Syn = v
	//arg1Type = prog_line	arg2Type = number	arg1Syn = n		arg2Syn = 1
	//arg1Type and arg2Type = prog_line/varName/procName/value/stmt#/stringLiteral/number
	QueryElement(string arg1, string arg2, string arg1Type, string arg2Type, string ent1Type, string ent2Type, string arg1Syn, string arg2Syn); //With

	string getSelectEntity();
	string getSelectSynonym();
	string getSelectType();
	string getSynAtrr();

	string getSuchThatArg1();
	string getSuchThatArg2();
	string getSuchThatArg1Type();
	string getSuchThatArg1Entity();
	string getSuchThatArg2Type();
	string getSuchThatArg2Entity();
	string getSuchThatRel();

	string getPatternArg1();
	string getPatternArg2();
	string getPatternArg3();
	string getPatternEntity();
	string getPatternSynonym();
	string getPatternArg1Type();
	string getPatternArg2Type();
	string getPatternArg3Type();
	string getPatternArg1Ent();

	string getWithArg1();
	string getWithArg2();
	string getWithArg1Type();
	string getWithArg2Type();
	string getWithArg1Ent();
	string getWithArg2Ent();
	string getWithArg1Syn();
	string getWithArg2Syn();

};

