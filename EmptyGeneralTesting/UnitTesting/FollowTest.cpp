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
			Assert::AreEqual(2, follow.getFollows(1)[0]);
			Assert::AreEqual(1, follow.getFollowedBy(2)[0]);
		}

	};
}