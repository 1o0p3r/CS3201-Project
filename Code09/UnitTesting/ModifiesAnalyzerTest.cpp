#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(ModifiesAnalyzerTest) {
public:
	TEST_METHOD(solveModifiesAnalyzer) {
	// using sample-source(actual)
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		vector<vector<string>> pkbHardCode;
		vector<vector<string>> hardCodeResult;

		////test wildCard true
		//QueryElement wildWild("_", "wildcard", "", "_", "wildcard", "", "Modifies");
		//pkbHardCode = { {"true"} };
		//ModifiesAnalyzer testWild(wildWild, pkb);
		//testWild.setUnitTestInputs(pkbHardCode);
		//clauseResult = testWild.solveClause();
		//Assert::IsTrue(get<0>(clauseResult));


		////test arg1 = procedure synonym, arg2 = string literal
		//QueryElement procSynVar("procd", "synonym", "procedure", "x", "variable", "", "Modifies");
		//pkbHardCode = {{"Example","p"} };
		//hardCodeResult = { { "Example","p","procd"} };
		//ModifiesAnalyzer test(procSynVar, pkb);
		//test.setUnitTestInputs(pkbHardCode);
		//clauseResult = test.solveClause();
		//Assert::IsTrue(get<0>(clauseResult));
		//Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		//for (int i = 0; i < hardCodeResult[0].size(); i++)
		//	Assert::AreEqual(hardCodeResult[0][i], get<1>(clauseResult)[0][i]);
		//
		////test arg1 = stmt synonym, arg2 = string literal
		//QueryElement stmtSynVar("haha", "synonym", "call", "x", "variable", "", "Modifies");
		//pkbHardCode = {{ "1","4","5","6","10","12","13","14","15","16","18","22","24" } };
		//hardCodeResult = { { "1","4","5","6","10","12","13","14","15","16","18","22","24","haha" } };
		//ModifiesAnalyzer test1(stmtSynVar, pkb);
		//test1.setUnitTestInputs(pkbHardCode);
		//clauseResult = test1.solveClause();
		//Assert::IsTrue(get<0>(clauseResult));
		//Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		//for (int i = 0; i < hardCodeResult[0].size(); i++)
		//	Assert::AreEqual(hardCodeResult[0][i], get<1>(clauseResult)[0][i]);



		////test arg1 = procedure, arg2 = synonym
		//QueryElement procVarSyn("p", "variable", "", "x", "synonym", "", "Modifies");
		//pkbHardCode = { { "x","i","z" } };
		//hardCodeResult = { { "x","i","z","x" } };
		//ModifiesAnalyzer test2(procVarSyn, pkb);
		//test2.setUnitTestInputs(pkbHardCode);
		//clauseResult = test2.solveClause();
		//Assert::IsTrue(get<0>(clauseResult));
		//Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		//for (int i = 0; i < hardCodeResult[0].size(); i++)
		//	Assert::AreEqual(hardCodeResult[0][i], get<1>(clauseResult)[0][i]);

		////test arg1 = stmt, arg2 = synonym
		//QueryElement stmtVarSyn("10", "number", "", "shy", "synonym", "", "Modifies");
		//pkbHardCode = {{ "z","x" } };
		//hardCodeResult = { { "z","x","shy" } };
		//ModifiesAnalyzer test3(stmtVarSyn, pkb);
		//test3.setUnitTestInputs(pkbHardCode);
		//clauseResult = test3.solveClause();
		//Assert::IsTrue(get<0>(clauseResult));
		//Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		//for (int i = 0; i < hardCodeResult[0].size(); i++)
		//	Assert::AreEqual(hardCodeResult[0][i], get<1>(clauseResult)[0][i]);
		//
		//test arg1 = stmt, arg2 = synonym
		QueryElement procSynSyn("waow", "synonym", "procedure", "happy", "synonym", "", "Modifies");
		pkbHardCode = { {"x","z","i","y"},{"Example","p","q"},{ "Example","p","q" },
			{ "Example","p"},{"Example"} };
		hardCodeResult = { { "Example","p","q","Example","p","q","Example","p","Example","waow" }, 
			{"x","x","x","z","z","z","i","i","y","happy" } };
		ModifiesAnalyzer test4(procSynSyn, pkb);
		test4.setUnitTestInputs(pkbHardCode);
		clauseResult = test4.solveClause();
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		for (int i = 0; i < hardCodeResult.size(); i++)
			for(int j = 0; j < hardCodeResult[0].size(); j++)
			Assert::AreEqual(hardCodeResult[i][j], get<1>(clauseResult)[i][j]);
	}

	};
}