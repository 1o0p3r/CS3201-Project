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
		parent.setParent(2, 3);
		Assert::AreEqual(2, parent.getParent(1).at(0));
	}

	};
}