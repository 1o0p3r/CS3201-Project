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

		//test wildcard true
		QueryElement intWild("3", "number", "", "_", "wildcard", "", "Modifies", "suchThat");
		pkbHardCode = { {"TRUE"} };
		ModifiesAnalyzer testWild(intWild, pkb);
		testWild.setUnitTestInputs(pkbHardCode);
		clauseResult = testWild.solveClause();
		Assert::IsTrue(get<0>(clauseResult));


		//test arg1 = procedure synonym, arg2 = string literal
		QueryElement procSynVar("procd", "synonym", "procedure", "x", "variable", "", "Modifies", "suchThat");
		pkbHardCode = {{"Example","p"} };
		hardCodeResult = { { "Example","p","procd"} };
		ModifiesAnalyzer test(procSynVar, pkb);
		test.setUnitTestInputs(pkbHardCode);
		clauseResult = test.solveClause();
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		for (int i = 0; i < hardCodeResult[0].size(); i++)
			Assert::AreEqual(hardCodeResult[0][i], get<1>(clauseResult)[0][i]);
		
		//test arg1 = stmt synonym, arg2 = string literal
		QueryElement stmtSynVar("haha", "synonym", "call", "x", "variable", "", "Modifies", "suchThat");
		pkbHardCode = {{ "1","4","5","6","10","12","13","14","15","16","18","22","24" } };
		hardCodeResult = { { "1","4","5","6","10","12","13","14","15","16","18","22","24","haha" } };
		ModifiesAnalyzer test1(stmtSynVar, pkb);
		test1.setUnitTestInputs(pkbHardCode);
		clauseResult = test1.solveClause();
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		for (int i = 0; i < hardCodeResult[0].size(); i++)
			Assert::AreEqual(hardCodeResult[0][i], get<1>(clauseResult)[0][i]);



		//test arg1 = procedure, arg2 = synonym
		QueryElement procVarSyn("p", "variable", "", "x", "synonym", "", "Modifies", "suchThat");
		pkbHardCode = { { "x","i","z" } };
		hardCodeResult = { { "x","i","z","x" } };
		ModifiesAnalyzer test2(procVarSyn, pkb);
		test2.setUnitTestInputs(pkbHardCode);
		clauseResult = test2.solveClause();
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		for (int i = 0; i < hardCodeResult[0].size(); i++)
			Assert::AreEqual(hardCodeResult[0][i], get<1>(clauseResult)[0][i]);

		//test arg1 = stmt, arg2 = synonym
		QueryElement stmtVarSyn("10", "number", "", "shy", "synonym", "", "Modifies", "suchThat");
		pkbHardCode = {{ "z","x" } };
		hardCodeResult = { { "z","x","shy" } };
		ModifiesAnalyzer test3(stmtVarSyn, pkb);
		test3.setUnitTestInputs(pkbHardCode);
		clauseResult = test3.solveClause();
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		for (int i = 0; i < hardCodeResult[0].size(); i++)
			Assert::AreEqual(hardCodeResult[0][i], get<1>(clauseResult)[0][i]);
		
		//test arg1 = stmt, arg2 = synonym
		QueryElement procSynSyn("waow", "synonym", "procedure", "happy", "synonym", "", "Modifies", "suchThat");
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
			for (int j = 0; j < hardCodeResult[0].size(); j++)
				Assert::AreEqual(hardCodeResult[i][j], get<1>(clauseResult)[i][j]);

		//test arg1 = proc, arg2 = wildcard
		QueryElement procSynWild("waow", "synonym", "procedure", "_", "wildcard", "", "Modifies", "suchThat");
		pkbHardCode = { { "x","z","i","y" },{ "Example","p","q" },{ "Example","p","q" },
		{ "Example","p" },{ "Example" } };
		hardCodeResult = { { "Example","p","q","Example","p","q","Example","p","Example","waow" } };
		ModifiesAnalyzer test5(procSynWild, pkb);
		test5.setUnitTestInputs(pkbHardCode);
		clauseResult = test5.solveClause();
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		for (int i = 0; i < hardCodeResult.size(); i++)
			for (int j = 0; j < hardCodeResult[0].size(); j++)
				Assert::AreEqual(hardCodeResult[i][j], get<1>(clauseResult)[i][j]);
	
		QueryElement stmtSynSyn("waow", "synonym", "stmt", "dead", "synonym", "", "Modifies", "suchThat");
		pkbHardCode = { { "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16",
				"17","18","19","20","21","22","23","24" },{ "x" },{ "z" },{ "i" },{ "x","z","y","i" },
				{ "x" },{ "z","y" },{ "z" },{ "y" },{ "z" },{ "z","x" },{ "i" },{ "x","z","i" },
				{ "x","z","i" },{ "x","z","i" },{ "x" },{ "x","z" },{ "i" },
				{ "x" },{ "z" },{ "z" },{ "z" },{ "z","x" },{ "z","x" },{ "x" }
		};
		hardCodeResult = { { "x","z","i","x","z","y","i",
			"x","z","y","z","y","z","z","x","i","x","z","i","x","z","i","x","z","i","x","x","z","i",
			"x","z","z","z","z","x","z","x","x","waow" } ,{ "1","2","3","4","4","4","4","5","6","6","7","8","9","10","10","11",
				"12","12","12","13","13","13","14","14","14","15","16","16","17","18","19","20",
				"21","22","22","23","23","24","dead" }
				
		};
		ModifiesAnalyzer test6(stmtSynSyn, pkb);
		test6.setUnitTestInputs(pkbHardCode);
		clauseResult = test6.solveClause();
		Assert::IsTrue(get<0>(clauseResult));
		Assert::AreEqual(hardCodeResult[0].size(), get<1>(clauseResult)[0].size());
		for (int i = 0; i < hardCodeResult.size(); i++)
			for (int j = 0; j < hardCodeResult[0].size(); j++)
				Assert::AreEqual(hardCodeResult[i][j], get<1>(clauseResult)[i][j]);

	}
	};
}