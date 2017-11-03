#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryAnalyzerTest) {
public:
	TEST_METHOD(solveFollowsAnalyzer) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);
		pkb.setFollows(2, 3);
		qa.setPKB(pkb);
		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		unordered_map<string, tuple<int, int>> qMap;
		vector<vector<vector<string>>> qTable;
		//intint
		
		QueryElement wildWild("_", "wildcard", "", "_", "wildcard", "", "Follows", "suchThat");
		
		qs.addSuchThatQuery(wildWild);
		qa.setQS(qs);
		
		clauseResult = FollowsAnalyzer(wildWild,pkb, qTable, qMap).solveClause();
		Assert::IsTrue(get<0>(clauseResult));
		
		QueryElement wildInt("_", "wildcard", "wildcard", "3", "int", "assign", "Follows", "suchThat");
		qs = QueryStatement();
		qs.addSuchThatQuery(wildInt);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(wildInt,pkb, qTable, qMap).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intWild("2", "int", "assign","_", "wildcard", "", "Follows", "suchThat");
		qs = QueryStatement();
		qs.addSuchThatQuery(intWild);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(intWild,pkb, qTable, qMap).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intInt("1", "int", "assign", "3", "int", "", "Follows", "suchThat");
		qs = QueryStatement();
		qs.addSuchThatQuery(intInt);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(intInt,pkb, qTable, qMap).solveClause();
		Assert::IsFalse(get<0>(clauseResult));

		QueryElement intSyn("2", "int", "assign", "a", "synonym", "assign", "Follows", "suchThat");
		qs = QueryStatement();
		qs.addSuchThatQuery(intSyn);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(intSyn,pkb, qTable, qMap).solveClause();
		hardcode = { {"3"} };
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardcode[0][0], get<1>(clauseResult)[0][0]);

		QueryElement synInt("a", "synonym", "assign", "3", "integer", "assign", "Follows", "suchThat");
		qs = QueryStatement();
		qs.addSuchThatQuery(synInt);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(synInt,pkb, qTable, qMap).solveClause();
		hardcode = { { "2" } };
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardcode[0][0], get<1>(clauseResult)[0][0]);

		QueryElement synSyn("a", "synonym", "assign", "b", "synonym", "assign", "Follows", "suchThat");
		qs = QueryStatement();
		qs.addSuchThatQuery(synSyn);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(synSyn,pkb, qTable, qMap).solveClause();
		hardcode = { { "2" },{"3"} };
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardcode[0][0], get<1>(clauseResult)[0][0]);
		Assert::AreEqual(hardcode[1][0], get<1>(clauseResult)[1][0]);
		
		QueryElement synWild("a", "synonym", "assign", "_", "wildcard", "assign", "Follows", "suchThat");
		qs = QueryStatement();
		qs.addSuchThatQuery(synWild);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(synWild,pkb, qTable, qMap).solveClause();
		hardcode = { { "2" }};
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardcode[0][0], get<1>(clauseResult)[0][0]);

		QueryElement wildSyn("_", "wildcard", "assign", "a", "synonym", "assign", "Follows", "suchThat");
		qs = QueryStatement();
		qs.addSuchThatQuery(wildSyn);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(wildSyn,pkb, qTable, qMap).solveClause();
		hardcode = { { "3" } };
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardcode[0][0], get<1>(clauseResult)[0][0]);


	}

	};
}