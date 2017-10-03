#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(ParserTest) {
	public:
		TEST_METHOD(parse) {
			PKB pkb;
			string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
			Assert::IsTrue(Parse(filename, pkb));

			pkb = PKB();
			filename = "..\\..\\Tests09\\Sample-Source(actual).txt";
			//Assert::IsTrue(Parse(filename, pkb));
		}
	};
}