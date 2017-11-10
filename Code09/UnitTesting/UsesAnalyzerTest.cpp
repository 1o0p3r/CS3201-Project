#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(UsesAnalyzerTest) {
public:
	TEST_METHOD(solveUsesAnalyzer) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;

		vector<string> test2;
		vector<int> test3;
		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		unordered_map<string, tuple<int, int>> qMap;
		vector<vector<vector<string>>> qTable;
		/*
		0 procedure Main{
		1 x = 5;
		2 while x{
		3 	y = 2;
		4 	x = x*y + 1; } }
		*/

		vector<int> test;
		int sizEUses;
		pkb.setUses(1, "x");
		pkb.setUses(2, "x");

		pkb.setUses(2, "y");
		pkb.setUses(4, "x");
		pkb.setUses(4, "y");

		sizEUses = test.size();
		pkb.setStatementType(1, "assign");
		pkb.setStatementType(2, "while");
		pkb.setStatementType(3, "assign");
		pkb.setStatementType(4, "assign");


		QueryElement synSyn("a", "synonym", "assign", "b", "synonym", "assign", "Uses", "suchThat");
		qs = QueryStatement();
		qs.addNormalQueryElement(synSyn);
		qa.setQS(qs);
		clauseResult = UsesAnalyzer(synSyn,pkb, qTable, qMap).solveClause();
		hardcode = { { "1","4","4","a" },{ "x","x","y","b" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}

	}

	};
}