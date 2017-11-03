#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryAnalyzerTest) {
public:
	TEST_METHOD(solveParentStarAnalyzer) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setStatementType(4, "while");
		pkb.setStatementType(5, "assign");
		pkb.setStatementType(6, "while");
		pkb.setStatementType(7, "assign");
		pkb.setParent(1, 2);
		pkb.setParent(1, 3);
		pkb.setParent(1, 4);
		pkb.setParent(4, 5);
		pkb.setParent(4, 6);
		pkb.setParent(6, 7);

		pkb.setFollows(2, 3);
		pkb.setFollows(3, 4);
		pkb.setFollows(5, 6);
		qa.setPKB(pkb);
		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		unordered_map<string, tuple<int, int>> qMap;
		vector<vector<vector<string>>> qTable;

		//intint
		/*
		1.while {
		2.  assign 
		3.  assign
		4.	while
		5.		assign
		6.		while
		7.			assign
		}
		*/


		QueryElement wildWild("_", "wildcard", "", "_", "wildcard", "", "ParentStar");

		qs.addSuchThatQuery(wildWild);
		qa.setQS(qs);

		clauseResult = ParentStarAnalyzer(wildWild,pkb, qTable, qMap).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement wildInt("_", "wildcard", "wildcard", "3", "int", "assign", "ParentStar");
		qs = QueryStatement();
		qs.addSuchThatQuery(wildInt);
		qa.setQS(qs);
		clauseResult = ParentStarAnalyzer(wildInt,pkb, qTable, qMap).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intWild("2", "int", "assign", "_", "wildcard", "", "ParentStar");
		qs = QueryStatement();
		qs.addSuchThatQuery(intWild);
		qa.setQS(qs);
		clauseResult = ParentStarAnalyzer(intWild,pkb, qTable, qMap).solveClause();
		Assert::IsFalse(get<0>(clauseResult));

		QueryElement intInt("1", "int", "assign", "3", "int", "", "ParentStar");
		qs = QueryStatement();
		qs.addSuchThatQuery(intInt);
		qa.setQS(qs);
		clauseResult = ParentStarAnalyzer(intInt,pkb, qTable, qMap).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intSyn("4", "int", "while", "a", "synonym", "assign", "ParentStar");
		qs = QueryStatement();
		qs.addSuchThatQuery(intSyn);
		qa.setQS(qs);
		clauseResult = ParentStarAnalyzer(intSyn,pkb, qTable, qMap).solveClause();
		hardcode = { {"5","7","a"} };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement synInt("a", "synonym", "while", "7", "integer", "assign", "ParentStar");
		qs = QueryStatement();
		qs.addSuchThatQuery(synInt);
		qa.setQS(qs);
		clauseResult = ParentStarAnalyzer(synInt,pkb, qTable, qMap).solveClause();
		hardcode = { { "1","4","6","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}

		QueryElement synSyn("a", "synonym", "while", "b", "synonym", "assign", "ParentStar");
		qs = QueryStatement();
		qs.addSuchThatQuery(synSyn);
		qa.setQS(qs);
		clauseResult = ParentStarAnalyzer(synSyn,pkb, qTable, qMap).solveClause();
		hardcode = { { "1","1","1","1","4","4","6","a" },
					{ "2","3","5","7","5","7","7","b" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement synWild("a", "synonym", "while", "_", "wildcard", "assign", "ParentStar");
		qs = QueryStatement();
		qs.addSuchThatQuery(synWild);
		qa.setQS(qs);
		clauseResult = ParentStarAnalyzer(synWild,pkb, qTable, qMap).solveClause();
		hardcode = { { "1","4","6","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}
	

		QueryElement wildSyn("_", "wildcard", "", "a", "synonym", "assign", "ParentStar");
		qs = QueryStatement();
		qs.addSuchThatQuery(wildSyn);
		qa.setQS(qs);
		clauseResult = ParentStarAnalyzer(wildSyn,pkb, qTable, qMap).solveClause();
		hardcode = { { "2","3","5","7","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


	}

	};
}