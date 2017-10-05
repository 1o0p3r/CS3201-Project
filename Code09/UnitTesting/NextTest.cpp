#include "stdafx.h"
#include "CppUnitTest.h"
#include "Next.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(NextTest)
	{
	public:

		TEST_METHOD(checkNextStmt) {

			Next next;
			vector<int> stmtsAndType = { 0, 2, 2, 2 };
			vector<int> parentOfStmtVec = { 0, 0, 0, 0 };
			next.createCFGTable(stmtsAndType, parentOfStmtVec);
			Assert::AreEqual(2, next.getNext(1)[0]);
		}

		TEST_METHOD(checkNextStmtWithWhile) {

			Next next;
			vector<int> stmtsAndType = { 0, 1, 1, 2, 2 };
			vector<int> parentOfStmtVec = { 0, 0, 1, 2, 1 };

			next.createCFGTable(stmtsAndType, parentOfStmtVec);
			Assert::IsTrue(next.getNext(2) == vector<int>{3, 4});
		}

	};
}