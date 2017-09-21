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
		vector<int> actual;
		actual.push_back(2);
		actual.push_back(3);
		Assert::AreEqual(actual[0], parent.getChildStar(1)[0]);
		Assert::AreEqual(actual[1], parent.getChildStar(1)[1]);

		parent.setParent(1, 4);
		Assert::IsTrue(parent.getChild(1) == vector<int>{2, 4});
	}
	};
}