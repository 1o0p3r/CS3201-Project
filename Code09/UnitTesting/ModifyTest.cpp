#include "stdafx.h"
#include "CppUnitTest.h"
#include "Modify.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ModifyTest)
	{
	public:

		TEST_METHOD(setModifiesCorrectly_wParent) { //check is parent has added variable
			
			Modify modify;
			vector<int> parentStar;
			parentStar.push_back(1);
			modify.setModifies(2, 2, parentStar);
			Assert::AreEqual(2, modify.getModifies(1)[0]);
		}

		TEST_METHOD(setModifiesCorrectly_woParent) {

			Modify modify;
			vector<int> parentStar;
			modify.setModifies(1, 2, parentStar);
			Assert::AreEqual(2, modify.getModifies(1)[0]);
			Assert::IsTrue(parentStar.empty());
		}

	};
}