#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(ParentTest) {
public:

	TEST_METHOD(setParentCorrectly) {
		Parent parent;
		parent.setParent(1, 2);
		Assert::AreEqual(2, parent.getChild(1)[0]);
		Assert::AreEqual(1, parent.getParent(2)[0]);

		parent.setParent(2, 3);
		vector<int> actual = {2, 3};
		Assert::IsTrue(actual == parent.getChildStar(1));
		Assert::IsTrue(parent.getParentStar(3) == vector<int>{2, 1});

		parent.setParent(1, 4);
		Assert::IsTrue(parent.getChild(1) == vector<int>{2, 4});
	}
	};
}