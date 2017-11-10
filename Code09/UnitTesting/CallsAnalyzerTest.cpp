#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Parser.h"
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
		vector<string> hardCodeResult;
		vector<vector<string>> result;
		vector<vector<string>> hardcode;

		unordered_map<string, tuple<int, int>> qMap;
		vector<vector<vector<string>>> qTable;

		string filename = "..\\..\\Tests09\\Sample-Source(actual).txt";
		Assert::IsTrue(Parse(filename,pkb));

		//QueryElement wildWild("_", "wildcard", "", "_", "wildcard", "", "Calls");
		//clauseResult = CallsAnalyzer(wildWild,pkb, qTable, qMap).solveClause();
		//Assert::IsTrue(get<0>(clauseResult));

		//QueryElement wildString("_", "wildcard", "wildcard", "q", "string", "procedure", "Calls");
		//qs = QueryStatement();
		//qs.addSuchThatQuery(wildString);
		//qa.setQS(qs);
		//clauseResult = CallsAnalyzer(wildString,pkb, qTable, qMap).solveClause();
		//Assert::IsTrue(get<0>(clauseResult));

		//QueryElement stringWild("Example", "string", "procedure", "_", "wildcard", "", "Calls");
		//qs = QueryStatement();
		//qs.addSuchThatQuery(stringWild);
		//qa.setQS(qs);
		//clauseResult = CallsAnalyzer(stringWild,pkb, qTable, qMap).solveClause();
		//Assert::IsTrue(get<0>(clauseResult));

		//QueryElement stringString("Example", "string", "procedure", "q", "string", "", "Calls");
		//qs = QueryStatement();
		//qs.addSuchThatQuery(stringString);
		//qa.setQS(qs);
		//clauseResult = CallsAnalyzer(stringString,pkb, qTable, qMap).solveClause();
		//Assert::IsTrue(get<0>(clauseResult));

		QueryElement synSyn("try", "synonym", "procedure", "fry", "synonym", "procedure", "Calls");
		qs = QueryStatement();
		qs.addNormalQueryElement(synSyn);
		qa.setQS(qs);
		clauseResult = CallsAnalyzer(synSyn,pkb, qTable, qMap).solveClause();
		hardcode = { { "Example","p","Example","try" }, {"q","q","p","fry"} };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement synString("a", "synonym", "procedure", "p", "string", "procedure", "Calls");
		qs = QueryStatement();
		qs.addNormalQueryElement(synString);
		qa.setQS(qs);
		clauseResult = CallsAnalyzer(synString,pkb, qTable, qMap).solveClause();
		hardcode = { { "Example","a" },{"p","p" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}

		QueryElement synWild("a", "synonym", "procedure", "_", "wildcard", "procedure", "Calls");
		qs = QueryStatement();
		qs.addNormalQueryElement(synWild);
		qa.setQS(qs);
		clauseResult = CallsAnalyzer(synWild,pkb, qTable, qMap).solveClause();
		hardcode = { { "Example","p","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}


		QueryElement wildSyn("_", "wildcard", "procedure", "a", "synonym", "procedure", "Calls");
		qs = QueryStatement();
		qs.addNormalQueryElement(wildSyn);
		qa.setQS(qs);
		clauseResult = CallsAnalyzer(wildSyn,pkb, qTable, qMap).solveClause();
		hardcode = { {"p","q","a" } };
		Assert::IsTrue(get<0>(clauseResult));
		for (int i = 0; i < get<1>(clauseResult).size(); i++)
			for (int j = 0; j < get<1>(clauseResult)[i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], get<1>(clauseResult)[i][j]);
			}

	}

	};
}