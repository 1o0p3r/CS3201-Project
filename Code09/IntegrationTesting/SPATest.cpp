#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "QueryValidator.h"
#include "QueryEval.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	TEST_CLASS(SPATest) {
public:

	TEST_METHOD(IntegrationTesting) {
		/*
		Simple Code 1

		0 procedure Main {
		1 x = 5;
		2 while x {
		3 	y = 2;
		4 	x = x*y+1;}}

		stmt s;
		Select s such that Follows(s,3)

		stmt s;
		Select s such that Follows(1,s)

		stmt s;
		Select s such that Follows*(s,2)

		variable v;
		Select v such that Modifies (a, v)

		variable v;
		Select v such that Modifies (2, v)

		variable v;
		Select v such that Modifies(3, v)

		variable v;
		Select v such that Uses(3，v)

		variable v; stmt s;
		Select s such that pattern a("x",_)
		*/
		PKB pkb;
		
		//line 1
		pkb.setStatementType(1, "assign");
		pkb.setModifies(1, "x");
		pkb.setProcModifies("Main", "x");
		pkb.addConstant("5");
		pkb.addPattern(1, "x", "5");
		//line 2
		pkb.setFollows(1, 2);
		pkb.setStatementType(2, "while");
		pkb.setUses(2, "x");
		pkb.setProcUses("Main", "x");
		//line 3
		pkb.setParent(2, 3);
		pkb.setStatementType(3, "assign");
		pkb.setModifies(3, "y");
		pkb.setProcModifies("Main", "y");
		pkb.addConstant("2");
		pkb.addPattern(3, "y", "2");
		//line 4
		pkb.setFollows(3, 4);
		pkb.setParent(2, 4);
		pkb.setStatementType(4, "assign");
		pkb.setModifies(4, "x");
		pkb.setProcModifies("Main", "x");
		pkb.setUses(4, "x");
		pkb.setProcUses("Main", "x");
		pkb.setUses(4, "y");
		pkb.setProcUses("Main", "y");
		pkb.addConstant("1");
		pkb.addPattern(4, "x", "x*y+1");

		QueryValidator q;
		string query;
		QueryStatement qs;	
		QueryEval qe;
		vector<string> expected;
		vector<string> answer;
		/**
		query = "stmt s; Select s such that Follows(s, 3)";
		expected = { "None" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qe.setPKB(pkb);
		qe.setQueryStatement(qs);
		answer = qe.runQueryEval();
		Assert::IsTrue(expected == answer);
		
		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
		qe = QueryEval();
		expected = vector<string>();
		
		query = "stmt s; Select s such that Follows(1, s)";
		expected = { "2" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qe.setPKB(pkb);
		qe.setQueryStatement(qs);
		answer = qe.runQueryEval();
		Assert::IsTrue(expected == answer);
		
		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
		qe = QueryEval();
		expected = vector<string>();
	
		query = "stmt s; Select s such that Follows*(s, 2)";
		expected = { "1" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qe.setPKB(pkb);
		qe.setQueryStatement(qs);
		answer = qe.runQueryEval();
		Assert::IsTrue(expected == answer);
	
		query = "variable v; Select v such that Modifies(a, v)";
		Assert::IsFalse(q.parseInput(query));
	
		query = "variable v; Select v such that Modifies(2, v)";
		expected = { "x", "y" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qe.setPKB(pkb);
		qe.setQueryStatement(qs);
		answer = qe.runQueryEval();
		Assert::IsTrue(expected == answer);
		
		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
		qe = QueryEval();
		expected = vector<string>();

		query = "variable v; Select v such that Uses(3,v)";
		expected = { "None" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qe.setPKB(pkb);
		qe.setQueryStatement(qs);
		answer = qe.runQueryEval();
		Assert::IsTrue(expected == answer);

		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
		qe = QueryEval();
		expected = vector<string>();

		query = "variable v; Select v such that Uses(4,v)";
		expected = { "x", "y" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qe.setPKB(pkb);
		qe.setQueryStatement(qs);
		answer = qe.runQueryEval();
		Assert::IsTrue(expected == answer);
		**/
		query = "variable v; stmt s; Select s such that pattern a(\"x\", _)";
		expected = { "2, 4" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qe.setPKB(pkb);
		qe.setQueryStatement(qs);
		answer = qe.runQueryEval();
		Assert::IsTrue(expected == answer);

		/**
		//PKB may have somme issues here
		query = "variable v; Select v such that Modifies(3, v)";
		expected = { "y" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qe.setPKB(pkb);
		qe.setQueryStatement(qs);
		answer = qe.runQueryEval();
		Assert::IsTrue(expected == answer);
		
		query = "assign a; variable v; Select v such that Modifies(a, v)";
		expected = { "x", "y" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qe.setPKB(pkb);
		qe.setQueryStatement(qs);
		answer = qe.runQueryEval();
		Assert::IsTrue(expected == answer);
		**/
	
	
		//Assert::IsTrue(q.parseInput(query));
	}
	};
}