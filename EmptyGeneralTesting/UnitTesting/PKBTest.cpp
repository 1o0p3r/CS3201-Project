#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(PKBTest) {
public:
	TEST_METHOD(addConstant) {
		PKB pkb;
		pkb.addConstant("1");
		pkb.addConstant("2");
		vector<string> results;
		results.push_back("1");
		results.push_back("2");
		Assert::AreEqual(results[0], pkb.getAllConstants()[0]);
		Assert::AreEqual(results[1], pkb.getAllConstants()[1]);
	}
	TEST_METHOD(addPattern) {
		PKB pkb;
		pkb.addPattern(1, "x", "3+5*y");
		Assert::AreEqual(1, get<0>(pkb.getPattern("x")[0]));
		string exp = "(3+(5*y))";
		Assert::AreEqual(exp, get<1>(pkb.getPattern("x")[0]));
	}
	TEST_METHOD(addProcedure) {
		PKB pkb;
		pkb.addProcedure("First");
		pkb.addProcedure("Second");
		vector<string> results;
		results.push_back("First");
		results.push_back("Second");
		Assert::AreEqual(results[0], pkb.getAllProcedures()[0]);
		Assert::AreEqual(results[1], pkb.getAllProcedures()[1]);
	}
	TEST_METHOD(addVariable) {
		PKB pkb;
		pkb.addVariable("x");
		pkb.addVariable("y");
		vector<string> results;
		results.push_back("x");
		results.push_back("y");
		Assert::AreEqual(results[0], pkb.getAllVariables()[0]);
		Assert::AreEqual(results[1], pkb.getAllVariables()[1]);
	}
	TEST_METHOD(getStatments) {
		PKB pkb;
		pkb.setStatementType(1, "assign");
		pkb.setStatementType(2, "while");
		pkb.setStatementType(3, "if");
		Assert::AreEqual(1, pkb.getAssign()[0]);
		Assert::AreEqual(2, pkb.getWhile()[0]);
		Assert::AreEqual(3, pkb.getIf()[0]);
		Assert::AreEqual(2, pkb.getAllStmt()[0]);
		Assert::AreEqual(1, pkb.getAllStmt()[1]);
		Assert::AreEqual(3, pkb.getAllStmt()[2]);
	}
	};
}