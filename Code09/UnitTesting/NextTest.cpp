#include "stdafx.h"
#include "CppUnitTest.h"
#include "Next.h"
#include <tuple>

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
			vector<tuple<int, int>> procFirstAndLastLines = {};
			next.createCFGTable(stmtsAndType, parentOfStmtVec, procFirstAndLastLines); //, vector<tuple<int, int>> procFirstAndLastLines
			Assert::AreEqual(2, next.getNext(1)[0]);
		}

		TEST_METHOD(checkNextStmtWithWhile) {

			Next next;
			vector<int> stmtsAndType = { 0, 1, 1, 2, 2 };
			vector<int> parentOfStmtVec = { 0, 0, 1, 2, 1 };
			vector<tuple<int, int>> procFirstAndLastLines = {};

			next.createCFGTable(stmtsAndType, parentOfStmtVec, procFirstAndLastLines);
			Assert::IsTrue(next.getNext(2) == vector<int>{3, 4});
		}

		TEST_METHOD(checkNextStmtWithWhileIfElse) {

			Next next;
			vector<int> stmtsAndType = { 0, 2, 3, 1, 2, 5, 2 };
			vector<int> parentOfStmtVec = { 0, 0, 0, 2, 3, 2, 0 };
			vector<tuple<int, int>> procFirstAndLastLines = {};

			next.createCFGTable(stmtsAndType, parentOfStmtVec, procFirstAndLastLines);
			Assert::IsTrue(next.getNext(2) == vector<int>{3, 5});
			Assert::IsTrue(next.getNext(3) == vector<int>{4, 6});
			Assert::IsTrue(next.getNext(3) == vector<int>{4, 6});
			Assert::IsTrue(next.getPrevious(5) == vector<int>{2});
			Assert::IsTrue(next.getPrevious(3) == vector<int>{2, 4});
		}

		TEST_METHOD(checkLastLine) {

			Next next;
			vector<int> stmtsAndType = { 0, 2, 1, 2 };
			vector<int> parentOfStmtVec = { 0, 0, 0, 2 };
			vector<tuple<int, int>> procFirstAndLastLines = {};

			next.createCFGTable(stmtsAndType, parentOfStmtVec, procFirstAndLastLines);
			Assert::IsTrue(next.getPrevious(0) == vector<int>{2});
		}

		TEST_METHOD(checkProcEndLine) {

			Next next;
			vector<int> stmtsAndType = { 0, 2, 4, 2, 2 };
			vector<int> parentOfStmtVec = { 0, 0, 0, 0, 0 };
			vector<tuple<int, int>> procFirstAndLastLines = { make_tuple(0, 0), make_tuple(0, 0) , make_tuple(3, 4) };

			next.createCFGTable(stmtsAndType, parentOfStmtVec, procFirstAndLastLines);
			Assert::IsTrue(next.getProcEndLine(2) == vector<int>{4});
		}

		/*
		TEST_METHOD(checkProcEndLineWMultipleProc) {

		Next next;
		vector<int> stmtsAndType = { 0, 2, 4, 2, 4, 2, 2 };
		vector<int> parentOfStmtVec = { 0, 0, 0, 0, 0 };
		vector<tuple<int, int>> procFirstAndLastLines = { make_tuple(0, 0), make_tuple(0, 0) , make_tuple(5, 6), make_tuple(0, 0) , make_tuple(5, 6) };

		next.createCFGTable(stmtsAndType, parentOfStmtVec, procFirstAndLastLines);
		Assert::IsTrue(next.getProcEndLine(2) == vector<int>{6});
		Assert::IsTrue(next.getProcEndLine(4) == vector<int>{6});
		}
		*/
	};
}