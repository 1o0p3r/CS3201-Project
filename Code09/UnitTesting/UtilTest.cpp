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
			Assert::AreEqual(string("(((x*y)*h)*f)"), Util::insertBrackets("x * y * h * f"));
			Assert::AreEqual(string("(((x*y)*h)+(f*g))"), Util::insertBrackets("x*y*h+f*g"));
			Assert::AreEqual(string("((x-y)+z)"), Util::insertBrackets("x-y+z"));
			Assert::AreEqual(string("((x-(y*g))+z)"), Util::insertBrackets("x-y*g+z"));
			Assert::AreEqual(string("((((x*y)*g)+z)+k)"), Util::insertBrackets("x*y*g+z+k"));
			Assert::AreEqual(string("(((x+1))*y)"), Util::insertBrackets("(x+1)*y"));
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
			Assert::AreEqual("(x+5)*y", Util::getExpression(" (x + 5) * y ;").c_str());
		}

		TEST_METHOD(constructExpression) {
			vector<string> actual = { "x", "5", "y" };
			Assert::IsTrue(actual == Util::constructExpression("(x+5)*y"));
			actual = { "x", "5", "y" };
			Assert::IsTrue(actual == Util::constructExpression("((x+5)*y)"));
		}
	};
}