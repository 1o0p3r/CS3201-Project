#include "stdafx.h"
#include "CppUnitTest.h"
#include "Modify.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(modifyTest)
	{
	public:

		TEST_METHOD(setModifies_Correct)
		{
			Modify modify;
			modify.setModifies(1, "x");
			modify.setModifies(2, "y");
			Assert::AreEqual(1, modify.getModifies(1).at(0));
		}

	};
}