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
		//intint
		QueryElement wildWild("_", "wildcard", "", "_", "wildcard", "", "Follows");
		
		qs.addSuchThatQuery(wildWild);
		qa.setQS(qs);
		
		clauseResult = FollowsAnalyzer(wildWild, pkb).solveClause();
		Assert::IsTrue(get<0>(clauseResult));
		
		QueryElement wildInt("_", "wildcard", "wildcard", "3", "int", "assign", "Follows");
		qs = QueryStatement();
		qs.addSuchThatQuery(wildInt);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(wildInt, pkb).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intWild("2", "int", "assign","_", "wildcard", "", "Follows");
		qs = QueryStatement();
		qs.addSuchThatQuery(intWild);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(intWild, pkb).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intInt("1", "int", "assign", "3", "int", "", "Follows");
		qs = QueryStatement();
		qs.addSuchThatQuery(intInt);
		qa.setQS(qs);
		clauseResult = FollowsAnalyzer(intInt, pkb).solveClause();
		Assert::IsFalse(get<0>(clauseResult));

	}

	};
}