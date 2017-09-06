#include "stdafx.h"
#include "CppUnitTest.h"
#include "Follow.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(FollowTest)
	{
	public:

		TEST_METHOD(setFollowsCorrectly)
		{
			Follow follow;
			follow.setFollows(1, 2);
			follow.setFollows(2, 3);
			Assert::AreEqual(2, follow.getFollows(1).at(0));
		}

	};
}