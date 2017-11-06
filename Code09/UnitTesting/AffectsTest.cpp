#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
	TEST_CLASS(PKB_Affects) {
public:
	TEST_METHOD(getAffectsTwoLiterals) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsFalse(pkb.getAffectsTwoLiterals(1, 2));
		Assert::IsFalse(pkb.getAffectsTwoLiterals(2, 4));
		Assert::IsFalse(pkb.getAffectsTwoLiterals(1, 3));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(1, 4));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(3, 4));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-2.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(1, 4));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(4, 9));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(1, 11));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(5, 11));
	}

	TEST_METHOD(getAffectsFirstLiteral) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAffectsFirstLiteral(1) == vector<int>{4});
		Assert::IsTrue(pkb.getAffectsFirstLiteral(2) == vector<int>{});
		Assert::IsTrue(pkb.getAffectsFirstLiteral(3) == vector<int>{4});
		Assert::IsTrue(pkb.getAffectsFirstLiteral(4) == vector<int>{4});

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-2.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAffectsFirstLiteral(1) == vector<int>{4, 9, 11, 13});
		Assert::IsTrue(pkb.getAffectsFirstLiteral(5) == vector<int>{6, 11});
		Assert::IsTrue(pkb.getAffectsFirstLiteral(9) == vector<int>{11, 13});
		Assert::IsTrue(pkb.getAffectsFirstLiteral(6) == vector<int>{});
		Assert::IsTrue(pkb.getAffectsFirstLiteral(2) == vector<int>{11, 7});
	}

	TEST_METHOD(getAffectsSecondLiteral) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAffectsSecondLiteral(1) == vector<int>{});
		Assert::IsTrue(pkb.getAffectsSecondLiteral(2) == vector<int>{});
		Assert::IsTrue(pkb.getAffectsSecondLiteral(3) == vector<int>{});
		Assert::IsTrue(pkb.getAffectsSecondLiteral(4) == vector<int>{1, 4, 3});

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-2.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAffectsSecondLiteral(4) == vector<int>{1, 4});
		Assert::IsTrue(pkb.getAffectsSecondLiteral(11) == vector<int>{9, 1, 4, 2, 7, 10, 5});
		Assert::IsTrue(pkb.getAffectsSecondLiteral(12) == vector<int>{11});
		Assert::IsTrue(pkb.getAffectsSecondLiteral(13) == vector<int>{9, 1, 4, 12, 11});
	}

	TEST_METHOD(getAffectsTwoSynonyms) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAffectsTwoSynonyms() == tuple<vector<int>, vector<int>>{ {1, 3, 4}, { 4, 4, 4 }});

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-2.txt";
		Assert::IsTrue(Parse(filename, pkb));
		tuple<vector<int>, vector<int>> ans = pkb.getAffectsTwoSynonyms();
		sort(get<0>(ans).begin(), get<0>(ans).end());
		sort(get<1>(ans).begin(), get<1>(ans).end());
		tuple<vector<int>, vector<int>> actual = {
			{ 1, 1, 5, 9, 2, 1, 10, 2, 11, 9, 12, 11, 1, 4, 4, 7, 5, 4, 7, 4 },
			{ 4, 9, 6, 11, 11, 11, 11, 7, 12, 13, 13, 13, 13, 4, 9, 11, 11, 11, 7, 13 } };
		sort(get<0>(actual).begin(), get<0>(actual).end());
		sort(get<1>(actual).begin(), get<1>(actual).end());
		Assert::IsTrue(ans == actual);
	}

	TEST_METHOD(getAffectStarTwoLiterals) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsFalse(pkb.getAffectStarTwoLiterals(1, 2));
		Assert::IsFalse(pkb.getAffectStarTwoLiterals(2, 4));
		Assert::IsFalse(pkb.getAffectStarTwoLiterals(1, 3));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(1, 4));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(3, 4));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(4, 4));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-2.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(1, 4));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(4, 9));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(1, 11));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(1, 12));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(1, 13));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(5, 11));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(2, 12));
		Assert::IsTrue(pkb.getAffectStarTwoLiterals(2, 13));
	}
	};
}