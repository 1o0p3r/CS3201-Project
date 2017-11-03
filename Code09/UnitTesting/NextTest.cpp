#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(NextTest) {
	public:
		TEST_METHOD(getNextStarTwoLiterals) {
			PKB pkb;
			string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
			Assert::IsTrue(Parse(filename, pkb));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(1, 2));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(1, 3));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(1, 4));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(4, 2));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(4, 3));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(4, 4));
			Assert::IsFalse(pkb.getNextStarTwoLiterals(4, 1));

			pkb = PKB();
			filename = "..\\..\\Tests09\\Sample-Source-2.txt";
			Assert::IsTrue(Parse(filename, pkb));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(1, 2));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(5, 4));
			Assert::IsFalse(pkb.getNextStarTwoLiterals(4, 2));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(10, 12));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(9, 13));
			Assert::IsTrue(pkb.getNextStarTwoLiterals(5, 11));
		}

		TEST_METHOD(getNextStarFirstLiteral) {
			PKB pkb;
			string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
			Assert::IsTrue(Parse(filename, pkb));
			Assert::IsTrue(pkb.getNextStarFirstLiteral(1) == vector<int>{2, 3, 4});
			Assert::IsTrue(pkb.getNextStarFirstLiteral(2) == vector<int>{2, 3, 4});
			Assert::IsTrue(pkb.getNextStarFirstLiteral(3) == vector<int>{2, 3, 4});
			Assert::IsTrue(pkb.getNextStarFirstLiteral(4) == vector<int>{2, 3, 4});

			pkb = PKB();
			filename = "..\\..\\Tests09\\Sample-Source-2.txt";
			Assert::IsTrue(Parse(filename, pkb));
			Assert::IsTrue(pkb.getNextStarFirstLiteral(3) == vector<int>{3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13});
			Assert::IsTrue(pkb.getNextStarFirstLiteral(6) == vector<int>{3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13});
			Assert::IsTrue(pkb.getNextStarFirstLiteral(8) == vector<int>{9, 10, 11, 12, 13});
			Assert::IsTrue(pkb.getNextStarFirstLiteral(10) == vector<int>{11, 12, 13});
			Assert::IsTrue(pkb.getNextStarFirstLiteral(13) == vector<int>{});
		}

		TEST_METHOD(getNextStarSecondLiteral) {
			PKB pkb;
			string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
			Assert::IsTrue(Parse(filename, pkb));
			Assert::IsTrue(pkb.getNextStarSecondLiteral(1) == vector<int>{});
			Assert::IsTrue(pkb.getNextStarSecondLiteral(2) == vector<int>{1, 2, 3, 4});
			Assert::IsTrue(pkb.getNextStarSecondLiteral(3) == vector<int>{1, 2, 3, 4});
			Assert::IsTrue(pkb.getNextStarSecondLiteral(4) == vector<int>{1, 2, 3, 4});

			pkb = PKB();
			filename = "..\\..\\Tests09\\Sample-Source-2.txt";	
			Assert::IsTrue(Parse(filename, pkb));
			Assert::IsTrue(pkb.getNextStarSecondLiteral(3) == vector<int>{1, 2, 3, 4, 5, 6, 7});
			Assert::IsTrue(pkb.getNextStarSecondLiteral(6) == vector<int>{1, 2, 3, 4, 5, 6, 7});
			Assert::IsTrue(pkb.getNextStarSecondLiteral(8) == vector<int>{1, 2, 3, 4, 5, 6, 7});
			Assert::IsTrue(pkb.getNextStarSecondLiteral(10) == vector<int>{1, 2, 3, 4, 5, 6, 7, 8});
			Assert::IsTrue(pkb.getNextStarSecondLiteral(11) == vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
		}

		TEST_METHOD(getNextStarTwoSynonyms) {
			PKB pkb;
			string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
			Assert::IsTrue(Parse(filename, pkb));
			Assert::IsTrue(pkb.getNextStarTwoSynonyms() == tuple<vector<int>, vector<int>>{ {1, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4, 4}, {2, 3, 3, 4, 4, 4, 2, 2, 2, 3, 3, 4}});

			pkb = PKB();
			filename = "..\\..\\Tests09\\Sample-Source-2.txt";
			Assert::IsTrue(Parse(filename, pkb));
			Assert::IsTrue(pkb.getNextStarTwoSynonyms() == tuple<vector<int>, vector<int>>{ 
				{1, 1, 2, 1, 2, 3, 1, 2, 3, 1, 2, 3, 4, 1, 2, 3, 8, 1, 2, 3, 8, 1, 2, 3, 4, 5, 1, 2, 3, 8, 9, 10, 1, 2, 3, 4, 5, 6, 1, 2, 3, 8, 9, 11, 10, 3, 4, 5, 6, 7, 1, 2, 3, 8, 9, 11, 12, 10, 4, 5, 6, 7, 4, 5, 6, 7, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 6, 7, 4, 5, 6, 7, 7, 4, 5, 6, 7, 4, 5, 6, 7},
				{2, 3, 3, 4, 4, 4, 8, 8, 8, 5, 5, 5, 5, 9, 9, 9, 9, 10, 10, 10, 10, 6, 6, 6, 6, 6, 11, 11, 11, 11, 11, 11, 7, 7, 7, 7, 7, 7, 12, 12, 12, 12, 12, 12, 12, 3, 3, 3, 3, 3, 13, 13, 13, 13, 13, 13, 13, 13, 4, 4, 4, 4, 8, 8, 8, 8, 5, 5, 5, 9, 9, 9, 9, 10, 10, 10, 10, 6, 6, 11, 11, 11, 11, 7, 12, 12, 12, 12, 13, 13, 13, 13}
			});
		}
	};
}