#include "stdafx.h"
#include "CppUnitTest.h"
#include "Modify.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ModifyTest)
	{
	public:

		TEST_METHOD(setModifiesCorrectly) {
			
			Modify modify;
			modify.setModifies(1, 2);
			Assert::AreEqual(2, modify.getModifies(1)[0]);
			Assert::AreEqual(1, modify.getModifiedBy(2)[0]);
		}

	};
}