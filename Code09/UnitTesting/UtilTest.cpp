#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "Util.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(UtilTest) {
	public:
		TEST_METHOD(insertBrackets) {
			string test1 = "x * y * h * f";
			string results = Util::insertBrackets(test1);
			Assert::AreEqual(string("(((x*y)*h)*f)"), results);
			string test2 = "x*y*h+f*g";
			results = Util::insertBrackets(test2);
			Assert::AreEqual(string("(((x*y)*h)+(f*g))"), results);
			Assert::AreEqual(string("((x-y)+z)"), Util::insertBrackets("x-y+z"));
			Assert::AreEqual(string("((x-(y*g))+z)"), Util::insertBrackets("x-y*g+z"));
			Assert::AreEqual(string("((((x*y)*g)+z)+k)"), Util::insertBrackets("x*y*g+z+k"));
		}

		TEST_METHOD(splitLine) {
			string test = "This is  a test";
			vector<string> actual = { "This", "is", "a", "test" };
			Assert::IsTrue(actual == Util::splitLine(test,' '));
		}

		TEST_METHOD(isValidName) {
			Assert::IsTrue(Util::isValidName("Correct1"));
			Assert::IsFalse(Util::isValidName("1Wrong"));
			Assert::IsFalse(Util::isValidName("Wrong!"));
		}

		TEST_METHOD(trim) {
			Assert::IsTrue("Test" == Util::trim("Test"));
			Assert::IsTrue("Test" == Util::trim("   Test   "));
			Assert::IsTrue("Test" == Util::trim("\t   Test  \t"));
			Assert::IsTrue("Test" == Util::trim("\t\tTest\t\t"));
		}
		
		TEST_METHOD(getExpression) {
			Assert::AreEqual("5", Util::getExpression("5;}").c_str());
			Assert::AreEqual("x+5", Util::getExpression(" x + 5 ;").c_str());
		}
	};
}