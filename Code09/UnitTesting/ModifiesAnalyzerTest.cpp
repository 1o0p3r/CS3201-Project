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

		//string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		//Parse(filename, pkb);

		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		/*
		0 procedure Main{
		1 x = 5;
		2 while x{
		3 	y = 2;
		4 	x = x*y + 1; } }
		*/

		vector<int> test;
		int sizEMOdifies;
		pkb.setModifies(1, "x");
		pkb.setModifies(2, "x");
		
		pkb.setModifies(2, "y");
		pkb.setModifies(3, "y");
		pkb.setModifies(4, "x");
		test = pkb.getModifiedBy("y");
		sizEMOdifies = test.size();
		pkb.setStatementType(1, "assign");
		pkb.setStatementType(2, "while");
		pkb.setStatementType(3, "assign");
		pkb.setStatementType(4, "assign");


		QueryElement synSyn("a", "synonym", "assign", "b", "synonym", "assign", "Modifies");
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