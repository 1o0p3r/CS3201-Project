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
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);
		qa.setPKB(pkb);
		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		//intint
		QueryElement intInt("_", "wildcard", "", "_", "wildcard", "", "Follows");
		
		qs.addSuchThatQuery(intInt);
		qa.setQS(qs);
		
		auto clauseResult = FollowsAnalyzer(intInt, pkb).solveClause();
		Assert::IsTrue(get<0>(clauseResult));
	}

	};
}