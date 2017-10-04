#include "stdafx.h"
#include "CppUnitTest.h"
#include "Next.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(NextTest)
	{
	public:

		/*
		TEST_METHOD(checkNextStmt) {

			Next next;
			vector<int> stmtsAndType = { 0, 2, 2, 2 };
			next.createCFGTable(stmtsAndType); //wrong declaration
			Assert::AreEqual(2, next.getNext(1)[0]);
		}
		*/

		TEST_METHOD(checkNextStmtWithWhile) {

			PKB pkb;
			pkb.setParent(1, 2);
			pkb.setParent(2, 3);
			pkb.setParent(1, 4);

			Next next;
			vector<int> stmtsAndType = { 0, 1, 1, 2, 2 };
			next.createCFGTable(stmtsAndType, pkb);
			Assert::AreEqual(2, pkb.getParent(3)[0]);
			Assert::IsTrue(next.getNext(2) == vector<int>{3, 4});
		}

	};
}