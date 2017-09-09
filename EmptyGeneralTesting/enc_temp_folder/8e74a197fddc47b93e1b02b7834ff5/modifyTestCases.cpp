#include "stdafx.h"
#include "CppUnitTest.h"
#include "Modify.h"

#include <iostream>

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
			string x = "x";
			Assert::AreEqual(x, modify.getModifies(1)[0]);
		}

	};
}