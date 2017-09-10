#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(PKBTest)
	{
	public:

		TEST_METHOD(setvarIndexTableCorrectly) {

			PKB pkb;
			pkb.setVarIndex("x");
			pkb.setVarIndex("y");
			Assert::AreEqual(0, pkb.getVarIndex("x"));
			Assert::AreEqual(1, pkb.getVarIndex("y"));

		}

	};
}