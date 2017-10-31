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
	string clauseType;

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
	
	string argumentOneTypeWith;
	string argumentTwoTypeWith;
	string argumentOneEntWith;
	string argumentTwoEntWith;
	string argumentOneSynWith;
	string argumentTwoSynWith;


public:
	QueryElement();

	
	//Select
	//E.g. assign a; procedure p; constant c; stmt s; prog_line n; variable v; Select < p.procName, c.value, s, v, v.varName > with n=1
	//Ent = procedure,constant,stmt,variable,variable	 synonym = p,c,s,v,v		type = tuple	synAttr = 	procName,value,synonym,synonym,varName
	//E.g. procedure p; constant c; prog_line n; Select  <p,	c > with n=1
	//Ent = procedure,constant		synonym = p,c		type = tuple	synAttrr = synonym,synonym
	//procedure p; constant c; prog_line n; Select BOOLEAN with n=1
	//Ent = empty	synonym = empty		type = BOOLEAN		attrName = empty
	//E.G. procedure p; constant c; stmt s; prog_line n; Select p.procName with n=1
	//Ent = procedure	Synonym = p		type = attrRef	attrName = procName
	QueryElement(string entity, string synonym, string type, string attrName, string clause);
	//Such that clauses
	//procedure p, variable v
	//E.g. such that Modifies(p,v)
	//arg1 = p arg1Type = synonym arg1Ent = procedure	arg2 = v	arg2Type = synonym	arg2Entity = variable	rel = Modifies
	QueryElement(string arg1, string arg1Type, string arg1Entity, string arg2, string arg2Type, string arg2Entity, string rel, string clause); // such that
	//Pattern Clauses
	//E.g. pattern a2(_, "x")
	//arg1 = "_"	arg2 = "x"	arg3 = "empty"	patternEntity = assign	patternSynonym = a2	patternArg1Type = wildcard	patternArg2Type	= variable patternArg3Type = empty	patternent1 = assign
	QueryElement(string arg1, string arg2, string arg3, string patternEntity, string patternSynonym, string patternArg1Type, string patternArg2Type, string patternArg3Type, string patternEnt1, string clause); //Pattern
	//With Clauses
	//E.g. procedure p; variable v; 
	//with p.procName = v.varName
	//with n=1
	//arg 1 = p.procName	arg2 = v.varName	arg1Type = procName		arg2Type = varName		arg1Ent = procedure		arg2Ent = variable		arg1Syn = p		arg1Syn = v
	//arg1Type = prog_line	arg2Type = number	arg1Syn = n		arg2Syn = 1
	//arg1Type and arg2Type = prog_line/varName/procName/value/stmt#/stringLiteral/number
	QueryElement(string arg1Type, string arg2Type, string ent1Type, string ent2Type, string arg1Syn, string arg2Syn, string clause); //With

	string getSelectEntity();
	string getSelectSynonym();
	string getSelectType();
	string getSynAtrr();
	string getClauseType();

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

	string getWithArg1Type();
	string getWithArg2Type();
	string getWithArg1Ent();
	string getWithArg2Ent();
	string getWithArg1Syn();
	string getWithArg2Syn();

};

