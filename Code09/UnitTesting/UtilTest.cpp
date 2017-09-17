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
			string test1 = "x*y*h*f";
			string results = Util::insertBrackets(test1);
			Assert::AreEqual(string("(((x*y)*h)*f)"), results);
			string test2 = "x*y*h+f*g";
			results = Util::insertBrackets(test2);
			Assert::AreEqual(string("(((x*y)*h)+(f*g))"), results);
			Assert::AreEqual(string("((x-y)+z)"), Util::insertBrackets("x-y+z"));
			Assert::AreEqual(string("((x-(y*g))+z)"), Util::insertBrackets("x-y*g+z"));
			Assert::AreEqual(string("((((x*y)*g)+z)+k)"), Util::insertBrackets("x*y*g+z+k"));
		}
	};
}