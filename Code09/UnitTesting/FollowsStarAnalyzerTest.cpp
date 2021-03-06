#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryAnalyzerTest) {
public:
	TEST_METHOD(solveFollowsStarAnalyzer) {
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


		QueryElement wildWild("_", "wildcard", "", "_", "wildcard", "", "FollowsStar", "suchThat");

		qs.addNormalQueryElement(wildWild);
		qa.setQS(qs);

		clauseResult = FollowsStarAnalyzer(wildWild,pkb, qTable, qMap).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement wildInt("_", "wildcard", "wildcard", "3", "int", "assign", "FollowsStar", "suchThat");
		qs = QueryStatement();
		qs.addNormalQueryElement(wildInt);
		qa.setQS(qs);
		clauseResult = FollowsStarAnalyzer(wildInt,pkb, qTable, qMap).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intWild("2", "int", "assign", "_", "wildcard", "", "FollowsStar", "suchThat");
		qs = QueryStatement();
		qs.addNormalQueryElement(intWild);
		qa.setQS(qs);
		clauseResult = FollowsStarAnalyzer(intWild,pkb, qTable, qMap).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intInt("2", "int", "assign", "6", "int", "", "FollowsStar", "suchThat");
		qs = QueryStatement();
		qs.addNormalQueryElement(intInt);
		qa.setQS(qs);
		clauseResult = FollowsStarAnalyzer(intInt,pkb, qTable, qMap).solveClause();
		Assert::IsFalse(get<0>(clauseResult));

		QueryElement intSyn("2", "int", "assign", "a", "synonym", "assign", "FollowsStar", "suchThat");
		qs = QueryStatement();
		qs.addNormalQueryElement(intSyn);
		qa.setQS(qs);
		clauseResult = FollowsStarAnalyzer(intSyn,pkb, qTable, qMap).solveClause();
		hardcode = { { "3","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement synInt("a", "synonym", "assign", "4", "integer", "assign", "FollowsStar", "suchThat");
		qs = QueryStatement();
		qs.addNormalQueryElement(synInt);
		qa.setQS(qs);
		clauseResult = FollowsStarAnalyzer(synInt,pkb, qTable, qMap).solveClause();
		hardcode = { { "2","3","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}

		QueryElement synSyn("a", "synonym", "assign", "b", "synonym", "assign", "FollowsStar", "suchThat");
		qs = QueryStatement();
		qs.addNormalQueryElement(synSyn);
		qa.setQS(qs);
		clauseResult = FollowsStarAnalyzer(synSyn,pkb, qTable, qMap).solveClause();
		hardcode = { { "2","a" },
		{ "3","b" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement synWild("a", "synonym", "assign", "_", "wildcard", "assign", "FollowsStar", "suchThat");
		qs = QueryStatement();
		qs.addNormalQueryElement(synWild);
		qa.setQS(qs);
		clauseResult = FollowsStarAnalyzer(synWild,pkb, qTable, qMap).solveClause();
		hardcode = { { "2","3","5","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement wildSyn("_", "wildcard", "assign", "a", "synonym", "assign", "FollowsStar", "suchThat");
		qs = QueryStatement();
		qs.addNormalQueryElement(wildSyn);
		qa.setQS(qs);
		clauseResult = FollowsStarAnalyzer(wildSyn,pkb, qTable, qMap).solveClause();
		hardcode = { { "3","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


	}

	};
}