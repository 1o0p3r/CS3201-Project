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

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-2.txt";
		Assert::IsTrue(Parse(filename, pkb));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(1, 4));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(4, 9));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(1, 11));
		Assert::IsTrue(pkb.getAffectsTwoLiterals(5, 11));
	}
	};
}