#include "stdafx.h"
#include "CppUnitTest.h"
#include "Use.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(UseTest)
	{
	public:

		TEST_METHOD(setUsesCorrectly_1) {

			Use use;
			use.setUses(1, 2);
			Assert::AreEqual(2, use.getUses(1)[0]);
			Assert::AreEqual(1, use.getUsedBy(2)[0]);
		}

		TEST_METHOD(setUsesCorrectly_2) {

			Use use;
			use.setUses(1, 1);
			use.setUses(1, 2);
			Assert::AreEqual(1, use.getUses(1)[0]);
			Assert::AreEqual(2, use.getUses(1)[1]);
		}

	};
}