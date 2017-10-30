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
		string filename = "..\\..\\Tests09\\Sample-Source(actual).txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAllCalls() == vector<string>{"q", "p"});
		Assert::IsTrue(pkb.getAllConstants() == vector<string>{"1", "2", "3", "5"});
		Assert::IsTrue(pkb.getAllProcedures() == vector<string>{"Example", "p", "q"});
		Assert::IsTrue(pkb.getAllStmt() == vector<int>{4, 14, 1, 2, 3, 5, 7, 8, 9, 11, 15, 17, 18, 19, 20, 21, 23, 24, 6, 13, 22, 10, 12, 16});
		Assert::IsTrue(pkb.getAllVariables() == vector<string>{"i", "x", "y", "z"});
		Assert::IsTrue(pkb.getAssign() == vector<int>{1, 2, 3, 5, 7, 8, 9, 11, 15, 17, 18, 19, 20, 21, 23, 24});
		Assert::IsTrue(pkb.getCall() == vector<int>{10, 12, 16});
		Assert::IsTrue(pkb.getCalledBy("p") == vector<string>{"Example"});
		Assert::IsTrue(pkb.getCalledByStar("q") == vector<string>{"Example", "p"});
		Assert::IsTrue(pkb.getCalls("p") == vector<string>{"q"});
		Assert::IsTrue(pkb.getCallsStar("Example") == vector<string>{"q", "p"});
		Assert::IsTrue(pkb.getChild(13) == vector<int>{14, 18, 19, 20});
		Assert::IsTrue(pkb.getChildStar(4) == vector<int>{5, 6, 7, 8, 9, 10, 11});
		Assert::IsTrue(pkb.getFirstline("p") == 13);
		Assert::IsTrue(pkb.getFollowedBy(12) == vector<int>{4});
		Assert::IsTrue(pkb.getFollowedByStar(21) == vector<int>{13});
		Assert::IsTrue(pkb.getFollows(13) == vector<int>{21});
		Assert::IsTrue(pkb.getFollowsStar(1) == vector<int>{2, 3, 4, 12});
		Assert::IsTrue(pkb.getIf() == vector<int>{6, 13, 22});
		Assert::IsTrue(pkb.getLastline("q") == 24);
		Assert::IsTrue(pkb.getModifiedBy("x") == vector<int>{1, 4, 5, 10, 12, 13, 14, 15, 16, 18, 22, 24});
		Assert::IsTrue(pkb.getModifies(13) == vector<string>{"x", "z", "i"});
		Assert::IsTrue(pkb.getParent(7) == vector<int>{6});
		Assert::IsTrue(pkb.getParentStar(8) == vector<int>{6, 4});
		Assert::IsTrue(pkb.getPatternVariable("i") == tuple<vector<int>, vector<string>> { {3, 11, 17}, {"(5)", "(i) (1) -", "(i) (1) -"} });
		Assert::IsTrue(pkb.getProcModifiedBy("y") == vector<string>{"Example"});
		Assert::IsTrue(pkb.getProcModifies("q") == vector<string>{"x", "z"});
		Assert::IsTrue(pkb.getProcUsedBy("x") == vector<string>{"Example", "q", "p"});
		Assert::IsTrue(pkb.getProcUses("p") == vector<string>{"x", "z", "i", "y"});
		Assert::IsTrue(pkb.getUsedBy("z") == vector<int>{4, 6, 8, 9, 10, 12, 13, 14, 15, 16, 19, 21, 22, 24});
		Assert::IsTrue(pkb.getUses(10) == vector<string>{"x", "z"});
		Assert::IsTrue(pkb.getWhile() == vector<int>{4, 14});
	}
	};
}