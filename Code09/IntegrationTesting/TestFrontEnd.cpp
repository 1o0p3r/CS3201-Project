#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	TEST_CLASS(UnitTest1) {
public:
	TEST_METHOD(FrontEndTest) {
		PKB pkb;
		string filename = "Sample-Source.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAllConstants() == vector<string>{"1", "2", "5"});
		Assert::IsTrue(pkb.getAllProcedures() == vector<string>{"Main"});
		Assert::IsTrue(pkb.getAllStmt() == vector<int>{2, 1, 3, 4});
		Assert::IsTrue(pkb.getAllVariables() == vector<string>{"x", "y"});
		Assert::IsTrue(pkb.getAssign() == vector<int>{1, 3, 4});
		Assert::IsTrue(pkb.getChild(2) == vector<int>{3, 4});
		Assert::IsTrue(pkb.getFollowedBy(4) == vector<int>{3});
		Assert::IsTrue(pkb.getFollows(1) == vector<int>{2});
		Assert::IsTrue(pkb.getIf() == vector<int>{});
		Assert::IsTrue(pkb.getModifiedBy("x") == vector<int>{1, 2, 4});
		Assert::IsTrue(pkb.getModifies(2) == vector<string>{"x", "y"});
		Assert::IsTrue(pkb.getParent(1) == vector<int>{});
		Assert::IsTrue(pkb.getPattern("x") == vector<tuple<int, string>>{tuple<int, string>{1, "5"}, tuple<int, string>{4, "((x*y)+1)"}});
		Assert::IsTrue(pkb.getProcModifiedBy("y") == vector<string>{"Main"});
		Assert::IsTrue(pkb.getProcModifies("Main") == vector<string>{"x", "y"});
		Assert::IsTrue(pkb.getProcUsedBy("x") == vector<string>{"Main"});
		Assert::IsTrue(pkb.getProcUses("Main") == vector<string>{"x", "y"});
		Assert::IsTrue(pkb.getUsedBy("x") == vector<int>{2, 4});
		Assert::IsTrue(pkb.getUses(2) == vector<string>{"x", "y"});
		Assert::IsTrue(pkb.getWhile() == vector<int>{2});
	}
	};
}