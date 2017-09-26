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
			string filename = "Sample-Source.txt";
			Assert::IsTrue(Parse(filename, pkb));

			pkb = PKB();
			filename = "Sample-Source(actual).txt";
			Assert::IsTrue(Parse(filename, pkb));
		}
	};
}