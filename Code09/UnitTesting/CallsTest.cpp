#include "stdafx.h"
#include "CppUnitTest.h"
#include "Calls.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting
{
	TEST_CLASS(CallsTest)
	{
	public:

		TEST_METHOD(setCallsCorrectly) { 

			Calls call;

			call.setCalls(1, 1, 2);
			Assert::AreEqual(2, call.getCalls(1)[0]);
			Assert::AreEqual(1, call.getCalledBy(2)[0]);
			call.setCalls(5, 2, 3);
			Assert::AreEqual(3, call.getCalls(2)[0]);
			Assert::AreEqual(2, call.getCalledBy(3)[0]);
			Assert::IsTrue(call.getCallsStar(1) == vector<int>{2, 3});
			Assert::IsTrue(call.getCalledByStar(3) == vector<int>{2, 1});
		}

		TEST_METHOD(getAllCallsWorking) {

			Calls call;
			call.setCalls(1, 1, 2);
			call.setCalls(5, 2, 3);
			Assert::IsTrue(call.getAllCalls() == vector<int>{2, 3});
		}

		TEST_METHOD(setCallsIncorrectly) {

			Assert::ExpectException<invalid_argument> ([&] {
				int a = 1;
				int b = 1;
				if (a == b)
					throw invalid_argument("Procedure calls itself. Program is recursive");
			});
		}
	};
}