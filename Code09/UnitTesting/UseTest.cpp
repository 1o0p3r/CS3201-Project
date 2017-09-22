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
			vector<int> parentStar;
			parentStar.push_back(1);
			use.setUses(2, 2, parentStar);
			parentStar.push_back(1);
			Assert::AreEqual(2, use.getUses(1)[0]);
			Assert::AreEqual(2, use.getUses(2)[0]);
		}
	};
}