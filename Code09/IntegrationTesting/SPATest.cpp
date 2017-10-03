#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
#include "QueryValidator.h"
#include "QueryEval.h"
#include "QueryAnalyzer.h"
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
		string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		Assert::IsTrue(Parse(filename, pkb));

		QueryValidator q;
		string query;
		QueryStatement qs;	
		QueryAnalyzer qa;
		vector<string> expected;
		vector<string> answer;
		
	
		/**
		query = "stmt s; Select s such that Follows(s, 3)";
		expected = {};
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);
	
	
		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
		qa = QueryAnalyzer();
		expected = vector<string>();
	
		query = "stmt s; Select s such that Follows(1, s)";
		expected = { "2" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);

		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
		qa = QueryAnalyzer();
		expected = vector<string>();
	
		query = "stmt s; Select s such that Follows*(s, 2)";
		expected = { "1" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);
	
		query = "variable v; Select v such that Modifies(a, v)";
		Assert::IsFalse(q.parseInput(query));
	
		query = "variable v; Select v such that Modifies(2, v)";
		expected = { "x", "y" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);

		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
		qa = QueryAnalyzer();
		expected = vector<string>();

		query = "variable v; Select v such that Uses(3,v)";
		expected = {};
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);

		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
		qa = QueryAnalyzer();
		expected = vector<string>();

		query = "variable v; Select v such that Uses(4,v)";
		expected = { "x", "y" };
		if (!q.parseInput(query)) {
			answer = { "Invalid" };
		} else {
			qs = q.getQueryStatement();
			qa.setPKB(pkb);
			qa.setQS(qs);
			answer = qa.runQueryEval();
		}
		Assert::IsTrue(expected == answer);
	
		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
		
		query = "variable v; stmt s; Select s such that pattern a(\"x\", _)";
		expected = { "Invalid" };
		if (!q.parseInput(query)) {
			answer = { "Invalid" };
		} else {
			qs = q.getQueryStatement();
			qa.setPKB(pkb);
			qa.setQS(qs);
			answer = qa.runQueryEval();
		}
		Assert::IsTrue(expected == answer);
		
		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();

		
		//PKB may have somme issues here
		query = "variable v; Select v such that Modifies(3, v)";
		expected = { "y" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);
	
		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();

		query = "assign a; variable v; Select v such that Modifies(4, v )";
		expected = { "x" };	
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);
		
		//Clear the objects that were used
		q = QueryValidator();
		qs = QueryStatement();
	
	

		query = "variable v; Select v such that Modifies(2, v)";
		expected = { "x", "y" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);
		
		q = QueryValidator();
		qs = QueryStatement();

		query = "variable v; Select v such that Modifies(2, v)";
		expected = { "x", "y" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);

		q = QueryValidator();
		qs = QueryStatement();

		//case 15
		query = "assign a; Select a pattern a(\"x\", _) such that Modifies(a，_)";
		expected = { "1", "4" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);
	
		**/
		q = QueryValidator();
		qs = QueryStatement();

		//cant even read parent*
		//Cuz wrong expected w
		query = "stmt s; assign a; Select a such that Parent*(s, 4) pattern a(_, \"x*y + 1\")";
		expected = { "4" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);
		/*
		//same problem with 12
		/*	q = QueryValidator();
		qs = QueryStatement();

		//cant even read parent* and parent
		query = "stmt a; Select a such that Parent(a，3）";
		expected = { "2" };
		q.parseInput(query);
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);
			*/
	
/*
		q = QueryValidator();
		qs = QueryStatement();

		query = "assign a; Select a such that Uses(a，\"y\")";
		expected = { "4" };
		Assert::IsTrue(q.parseInput(query));
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);
		*/

		q = QueryValidator();
		qs = QueryStatement();
		query = "stmt s; Select s such that Follows*(s, 2)";
			expected = { "1" };
		Assert::IsTrue(q.parseInput(query));
		qs = q.getQueryStatement();
		qa.setPKB(pkb);
		qa.setQS(qs);
		answer = qa.runQueryEval();
		Assert::IsTrue(expected == answer);

		//Assert::IsTrue(q.parseInput(query));
	}
	};
}

