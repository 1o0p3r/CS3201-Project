#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryAnalyzerTest) {
public:
	TEST_METHOD(solveModifiesAnalyzer) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;

		string filename = "Sample-Source.txt";
		Parse(filename, pkb);

		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		/*
		0 procedure Main{
		1 x = 5;
		2 while x{
		3 	y = 2;
		4 	x = x*y + 1; } }
		*/

		QueryElement synSyn("a", "synonym", "assign", "b", "synonym", "assign", "FollowsStar");
		qs = QueryStatement();
		qs.addSuchThatQuery(synSyn);
		qa.setQS(qs);
		clauseResult = ModifiesAnalyzer(synSyn, pkb).solveClause();
		hardcode = { { "2","2","1","3","4","a" },
		{ "x","y","x","y","x","b" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}
		
	}
	
	};
}