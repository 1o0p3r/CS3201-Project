#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryAnalyzerTest) {
public:
	TEST_METHOD(solveCallsAnalyzer) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;

		vector<vector<string>> pkbHardCode;
		vector<vector<string>> hardCodeResult;
		vector<vector<string>> result;
		vector<vector<string>> hardcode;

		QueryElement wildWild("_", "wildcard", "", "_", "wildcard", "", "Parent");
		pkbHardCode = { { "true" } };
		ModifiesAnalyzer testWild(wildWild, pkb);
		testWild.setUnitTestInputs(pkbHardCode);
		clauseResult = testWild.solveClause();

		Assert::IsTrue(get<0>(clauseResult));
		qs.addSuchThatQuery(wildWild);
		qa.setQS(qs);

		clauseResult = ParentAnalyzer(wildWild, pkb).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement wildInt("_", "wildcard", "wildcard", "3", "int", "assign", "Parent");
		qs = QueryStatement();
		qs.addSuchThatQuery(wildInt);
		qa.setQS(qs);
		clauseResult = ParentAnalyzer(wildInt, pkb).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intWild("2", "int", "assign", "_", "wildcard", "", "Parent");
		qs = QueryStatement();
		qs.addSuchThatQuery(intWild);
		qa.setQS(qs);
		clauseResult = ParentAnalyzer(intWild, pkb).solveClause();
		Assert::IsFalse(get<0>(clauseResult));

		QueryElement intInt("1", "int", "assign", "3", "int", "", "Parent");
		qs = QueryStatement();
		qs.addSuchThatQuery(intInt);
		qa.setQS(qs);
		clauseResult = ParentAnalyzer(intInt, pkb).solveClause();
		Assert::IsTrue(get<0>(clauseResult));

		QueryElement intSyn("4", "int", "assign", "a", "synonym", "assign", "Parent");
		qs = QueryStatement();
		qs.addSuchThatQuery(intSyn);
		qa.setQS(qs);
		clauseResult = ParentAnalyzer(intSyn, pkb).solveClause();
		hardcode = { { "5","6","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement synInt("a", "synonym", "assign", "7", "integer", "assign", "Parent");
		qs = QueryStatement();
		qs.addSuchThatQuery(synInt);
		qa.setQS(qs);
		clauseResult = ParentAnalyzer(synInt, pkb).solveClause();
		hardcode = { { "6","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}

		QueryElement synSyn("a", "synonym", "assign", "b", "synonym", "assign", "Parent");
		qs = QueryStatement();
		qs.addSuchThatQuery(synSyn);
		qa.setQS(qs);
		clauseResult = ParentAnalyzer(synSyn, pkb).solveClause();
		hardcode = { { "1","1","1","4","4","6","a" },
		{ "2","3","4","5","6","7","b" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement synWild("a", "synonym", "assign", "_", "wildcard", "assign", "Parent");
		qs = QueryStatement();
		qs.addSuchThatQuery(synWild);
		qa.setQS(qs);
		clauseResult = ParentAnalyzer(synWild, pkb).solveClause();
		hardcode = { { "1","4","6","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement wildSyn("_", "wildcard", "assign", "a", "synonym", "assign", "Parent");
		qs = QueryStatement();
		qs.addSuchThatQuery(wildSyn);
		qa.setQS(qs);
		clauseResult = ParentAnalyzer(wildSyn, pkb).solveClause();
		hardcode = { { "2","3","4","5","6","7","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


	}

	};
}