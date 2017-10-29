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
			Assert::AreEqual(string("(a*((x+y)))"), Util::insertBrackets("a*(x+y)"));
			Assert::AreEqual(string("(((x*y)*h)*f)"), Util::insertBrackets("x*y*h*f"));
			Assert::AreEqual(string("(((x*y)*h)+(f*g))"), Util::insertBrackets("x*y*h+f*g"));
			Assert::AreEqual(string("((x-y)+z)"), Util::insertBrackets("x-y+z"));
			Assert::AreEqual(string("((x-(y*g))+z)"), Util::insertBrackets("x-y*g+z"));
			Assert::AreEqual(string("((((x*y)*g)+z)+k)"), Util::insertBrackets("x*y*g+z+k"));
			Assert::AreEqual(string("(((x+1))*y)"), Util::insertBrackets("(x+1)*y"));
			Assert::AreEqual(string("(abc+xyz)"), Util::insertBrackets("abc+xyz"));
			Assert::AreEqual(string("((abc+((xyz*d5f)*s66))+ds321321515)"), Util::insertBrackets("abc+xyz*d5f*s66+ds321321515"));
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

		TEST_METHOD(isValidExpression) {
			Assert::IsTrue(Util::isValidExpression("x+7"));
			Assert::IsTrue(Util::isValidExpression("(y+5)"));
			Assert::IsTrue(Util::isValidExpression("(a+		t3)"));
			Assert::IsTrue(Util::isValidExpression("(a+		t3) * (b)"));
			Assert::IsTrue(Util::isValidExpression("c4*(a+		t3) * (b)"));
			Assert::IsTrue(Util::isValidExpression("((ct))+	(a+		t3) * (b)"));
			Assert::IsFalse(Util::isValidExpression(")(x+7"));
			Assert::IsFalse(Util::isValidExpression("(x+7"));
			Assert::IsFalse(Util::isValidExpression("(x++7)"));
			Assert::IsFalse(Util::isValidExpression("*x+7"));
			Assert::IsFalse(Util::isValidExpression("*)x+7"));
			Assert::IsFalse(Util::isValidExpression("x#+7"));
			Assert::IsFalse(Util::isValidExpression(")(x+7"));
			Assert::IsFalse(Util::isValidExpression("(x+7)*"));
			Assert::IsFalse(Util::isValidExpression("X1   X2"));
			Assert::IsFalse(Util::isValidExpression("XQ - 1A"));
			Assert::IsFalse(Util::isValidExpression("()"));
			Assert::IsFalse(Util::isValidExpression("(	)"));
			Assert::IsFalse(Util::isValidExpression(""));
		}

		TEST_METHOD(inFixToPosFix)
		{
			//"a+b*(c*d-e)*(f+g*h)-i";
			//"a b c d * e - * f g h * + * + i -";
			string exp = "radius    +  (   china *(   dog -   fog ) +   war)";
			string result = Util::getPostFixExp(exp);
			string expected = "radius china dog fog - * war + +";
			Assert::AreEqual(expected.length(), result.length());
			Assert::AreEqual(expected, result);
			
			exp = "radius    +  (   china *(   dog -   fog ) +   war)";
			result = Util::getPostFixExp(exp);
			expected = "radius china dog fog - * war + +";
			Assert::AreEqual(expected, result);

			exp = "radi    +  (   uschi *(   nado -   gfog ) +   war)";
			result = Util::getPostFixExp(exp);
			expected = "radi uschi nado gfog - * war + +";
			Assert::AreEqual(expected, result);
			
			
		}
	};
}