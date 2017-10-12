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
	TEST_METHOD(addAssignPattern) {
		PKB pkb;
		pkb.addAssignPattern(1, "x", "5");
		pkb.addAssignPattern(3, "y", "2");
		pkb.addAssignPattern(4, "x", "x*y+1");
		Assert::AreEqual(1, get<0>(pkb.getPattern("x")[0]));
		string exp = "5";
		Assert::AreEqual(exp, get<1>(pkb.getPattern("x")[0]));
		Assert::AreEqual(4, get<0>(pkb.getPattern("x")[1]));
		exp = "((x*y)+1)";
		Assert::AreEqual(exp, get<1>(pkb.getPattern("x")[1]));
		Assert::AreEqual(3, get<0>(pkb.getPattern("y")[0]));
	}
	TEST_METHOD(addProcedure) {
		PKB pkb;
		pkb.setProcModifies("First", "x");
		pkb.setProcUses("Second", "y");
		vector<string> results;
		results.push_back("First");
		results.push_back("Second");
		Assert::AreEqual(results[0], pkb.getAllProcedures()[0]);
		Assert::AreEqual(results[1], pkb.getAllProcedures()[1]);
	}
	TEST_METHOD(addVariable) {
		PKB pkb;
		pkb.setModifies(1, "x");
		pkb.setUses(2, "y");
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
	TEST_METHOD(setProcModifiesForMultiplePrcs) {
		PKB pkb;
		pkb.setProcModifies("first", "x");
		pkb.setCalls(1, "first", "second");
		pkb.setProcModifies("second", "y");

		vector<string> results;
		Assert::IsTrue(pkb.getProcModifies("first") == vector<string>{"x", "y"});
		Assert::IsTrue(pkb.getProcModifiedBy("y") == vector<string>{"first", "second"});
	}

	TEST_METHOD(setProcUsesForMultiplePrcs) {
		PKB pkb;
		pkb.setProcUses("first", "x");
		pkb.setProcUses("first", "y");
		pkb.setCalls(1, "first", "second");
		pkb.setProcUses("second", "z");

		vector<string> results;
		Assert::IsTrue(pkb.getProcUses("first") == vector<string>{"x", "y", "z"});
		Assert::IsTrue(pkb.getProcUsedBy("z") == vector<string>{"first", "second"});
	}

	TEST_METHOD(setModifies) {
		PKB pkb;
		pkb.setModifies(1, "x");
		pkb.setModifies(2, "x");
		pkb.setModifies(3, "y");
		pkb.setModifies(4, "x");
		Assert::AreEqual(3, pkb.getModifiedBy("y")[0]);
	}
	};
}