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
			modify.setModifies(1, "x");
			string x = "x";
			Assert::AreEqual(x, modify.getModifies(1)[0]);
		}

	};
}