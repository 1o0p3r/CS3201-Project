#include "stdafx.h"
#include "CppUnitTest.h"
#include "Follow.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(FollowTest)
	{
	public:

		TEST_METHOD(setFollowsCorrectly) {
			Follow follow;
			follow.setFollows(1, 2);
			Assert::AreEqual(2, follow.getFollows(1)[0]);
			Assert::AreEqual(1, follow.getFollowedBy(2)[0]);

			follow.setFollows(2, 3);
			vector<int> actual;
			actual.push_back(2);
			actual.push_back(3);
			Assert::AreEqual(actual[0], follow.getFollowsStar(1)[0]);
			Assert::AreEqual(actual[1], follow.getFollowsStar(1)[1]);
			actual.clear();
			actual.push_back(2);
			actual.push_back(1);
			Assert::AreEqual(actual[0], follow.getFollowedByStar(3)[0]);
			Assert::AreEqual(actual[1], follow.getFollowedByStar(3)[1]);
		}

	};
}