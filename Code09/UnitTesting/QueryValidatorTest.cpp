#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidator.h"
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

const string WHITESPACE__STRING = " ";
const string COMMA_STRING = ",";
const char INVERTED_COMMA = '\"';
const string DOUBLE_QUOTATION_STRING = "\"";
namespace UnitTesting
{
	TEST_CLASS(queryValidatorTest)
	{
	public:
		TEST_METHOD(isValidQuery)
		{
			QueryValidator q;
			string query;
			
			query = "stmt s; Select s";
			Assert::IsTrue(q.parseInput(query));
			
			query = "stmt s; Select s such that Follows(s,4)";
			Assert::IsTrue(q.parseInput(query));
			
			query = "   while w   ; assign a   ; Select w such that Follows(w, a)  ";
			Assert::IsTrue(q.parseInput(query));
			
			query = "while w; assign a; Select w such that Follows(w, a) pattern a(_, x)";
			Assert::IsFalse(q.parseInput(query));
			
			query = "assign a; Select a pattern b#(_, _\"f - d + b - l\"_)";
			Assert::IsFalse(q.parseInput(query));

			query = "while w; assign a; Select w such that Follows(w, a) pattern a(_, \"x\")";
			Assert::IsTrue(q.parseInput(query));
			
			query = "while w; assign a; Select x such that Follows(w, a) pattern a(_, \"x\")";
			Assert::IsFalse(q.parseInput(query));
			
			query = "while w; assign a; Select w such that Follows(\"w\", a) pattern a(_, \"x\")";
			Assert::IsFalse(q.parseInput(query));
			
			query = "prog_line pl, p#; constant c; Select c such that Follows(c,_)";
			Assert::IsFalse(q.parseInput(query));

			query = "prog_line pl, p#; constant c, d; Select p# such that Follows(c,_)";
			Assert::IsFalse(q.parseInput(query));

			query = "assign a; prog_line pl, p#; constant c, d; Select p# such that Follows(a,_)";
			Assert::IsTrue(q.parseInput(query));

			query = "assign a; Select a pattern a(_, _\"f - d + b - l\"_)";
			Assert::IsTrue(q.parseInput(query));
			
			query = "variable v1,v#; assign a1,a#; constant d; Select v1 such that Modifies(6,v1)";
			Assert::IsTrue(q.parseInput(query));

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,v1)";
			Assert::IsTrue(q.parseInput(query));

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\")";
			Assert::IsTrue(q.parseInput(query));

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a(_, \"x\")";
			Assert::IsTrue(q.parseInput(query));

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a(_, _\"x\")";
			Assert::IsFalse(q.parseInput(query));
			
			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a(\"y\", \"x\")";
			Assert::IsTrue(q.parseInput(query));

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a(v1, \"x\")";
			Assert::IsTrue(q.parseInput(query));
			
		}
		//This test method assumes that the input is already grammatically correct i.e. no commas out of nowhere
		TEST_METHOD(isValidParseEntityAndSynonym) {
			QueryValidator q;
			string str;

			str = "stmt s";
			Assert::IsTrue(q.isEntityAndSynonym(str));

			str = "while w";
			Assert::IsTrue(q.isEntityAndSynonym(str));

			str = "haha a";
			Assert::IsFalse(q.isEntityAndSynonym(str));

			str = "stmt s,a";
			Assert::IsTrue(q.isEntityAndSynonym(str));

			str = "stmt s, w, a";
			Assert::IsTrue(q.isEntityAndSynonym(str));
		}
		TEST_METHOD(isValidRemoveSymbols){
			QueryValidator q;
			string str, expectedStr;

			str = "s, w";
			expectedStr = "s,w";
			Assert::IsTrue(q.removeSymbols(str, WHITESPACE__STRING) == expectedStr);
			str = " s    ,    w";
			expectedStr = "s,w";
			Assert::IsTrue(q.removeSymbols(str, WHITESPACE__STRING) == expectedStr);

			str = "\"wewewewe\"";
			expectedStr = "wewewewe";
			Assert::IsTrue(q.removeSymbols(str, DOUBLE_QUOTATION_STRING) == expectedStr);

		}
		TEST_METHOD(isValidSplit) {
			QueryValidator q;
			vector<string> vecStr;
			vector<string> expectedVecStr;
			string toPush, arg1, arg2;


			toPush = "Select s such that Follows(s,4)";
			vecStr.push_back(toPush);
			arg1 = "Select s ";
			arg2 = "such that Follows(s,4)";
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			Assert::IsTrue(q.split(vecStr, "such that") == expectedVecStr);

			vecStr.clear();
			expectedVecStr.clear();
						
			toPush = "Select s such that Follows(s,4) pattern a(""x"",_)";
			vecStr.push_back(toPush);
			arg1 = "Select s ";
			arg2 = "such that Follows(s,4) pattern a(""x"",_)";
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			Assert::IsTrue(q.split(vecStr, "such that") == expectedVecStr);
			
			vecStr.clear();
			expectedVecStr.clear();
			
			toPush = "Select s pattern a(""x"",_)";
			vecStr.push_back(toPush);
			arg1 = "Select s ";
			arg2 = "pattern a(""x"",_)";
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			Assert::IsTrue(q.split(vecStr, "pattern") == expectedVecStr);
			
			vecStr.clear();
			expectedVecStr.clear();

			toPush = "Select s";
			vecStr.push_back(toPush);
			arg1 = "Select s";
			expectedVecStr.push_back(arg1);
			Assert::IsTrue(q.split(vecStr, "pattern") == expectedVecStr);
			
			vecStr.clear();
			expectedVecStr.clear();
		}
		//This checks if we get the correct corresponding entity
		TEST_METHOD(is_number) {
			QueryValidator q;
			string str;

			str = "123";
			Assert::IsTrue(q.is_number(str));

			str = "1";
			Assert::IsTrue(q.is_number(str));

			str = "aaa";
			Assert::IsFalse(q.is_number(str));
			
			str = "b";
			Assert::IsFalse(q.is_number(str));
		}
		TEST_METHOD(removeDuplicatesWhiteSpaces) {
			QueryValidator q;
			string str;
			string expected;

			str = "This is a      dummy     string";
			expected = "This is a dummy string";
			Assert::IsTrue(q.removeDuplicatesWhiteSpaces(str) == expected);
			
			str = "stmt    s;     while w; Select s such    that Follows(s   ,4)";
			expected = "stmt s; while w; Select s such that Follows(s ,4)";
			Assert::IsTrue(q.removeDuplicatesWhiteSpaces(str) == expected);

			str = " stmt s;  while  w; Select s such  that  Follows(  s,4) pattern a(_,   \"x\")";
			expected = " stmt s; while w; Select s such that Follows( s,4) pattern a(_, \"x\")";
			Assert::IsTrue(q.removeDuplicatesWhiteSpaces(str) == expected);
		}
		TEST_METHOD(isValidDeclarationRegex) {
			QueryValidator q;
			string str;

			str = "stmt s;";
			Assert::IsTrue(q.isValidDeclarationRegex(str));

			str = "while ,v;";
			Assert::IsFalse(q.isValidDeclarationRegex(str));

			str = "constant s,v;";
			Assert::IsTrue(q.isValidDeclarationRegex(str));

			str = "assign a1,a2,a;";
			Assert::IsTrue(q.isValidDeclarationRegex(str));

		}
		TEST_METHOD(isValidSuchThatRegex) {
			QueryValidator q;
			string str;

			str = "such that Uses(3,4)";
			Assert::IsFalse(q.isValidSuchThatRegex(str));

			str = "such that Uses(3,s)";
			Assert::IsTrue(q.isValidSuchThatRegex(str));

			str = "such that Parent(3,_)";
			Assert::IsTrue(q.isValidSuchThatRegex(str));

			str = "such that Parent*(3,_)";
			Assert::IsTrue(q.isValidSuchThatRegex(str));

			str = "such that Follows(s,4)";
			Assert::IsTrue(q.isValidSuchThatRegex(str));

			str = "such that Follows(\"s#\",4)";
			Assert::IsFalse(q.isValidSuchThatRegex(str));

			str = "such that Modifies(\"s\",\"a\")";
			Assert::IsFalse(q.isValidSuchThatRegex(str));

			str = "such that Parent*(s#,    4)";
			Assert::IsTrue(q.isValidSuchThatRegex(str));

			str = "such that Follows*(_,_   )";
			Assert::IsTrue(q.isValidSuchThatRegex(str));

		}

		TEST_METHOD(isValidSelectInitialRegex) {
			QueryValidator q;
			string str;

			str = "Select s";
			Assert::IsTrue(q.isValidSelectInitialRegex(str));

			str = "Select \@";
			Assert::IsFalse(q.isValidSelectInitialRegex(str));
		}
		TEST_METHOD(isValidPatternRegex) {
			QueryValidator q;
			string str;

			str = "pattern a(_,_)";
			Assert::IsTrue(q.isValidPatternRegex(str));

			str = "pattern a(_,\"x\")";
			Assert::IsTrue(q.isValidPatternRegex(str));

			str = "pattern a(_,\"x\")";
			Assert::IsTrue(q.isValidPatternRegex(str));

			str = "pattern a(_, _\"x+y\"_)";
			Assert::IsTrue(q.isValidPatternRegex(str));

			str = "pattern a(_, _\"x\")";
			Assert::IsFalse(q.isValidPatternRegex(str));

			str = "pattern a(s, _\"x\"_)";
			Assert::IsTrue(q.isValidPatternRegex(str));

			str = "pattern a(\"b#\", _\"x\"_)";
			Assert::IsTrue(q.isValidPatternRegex(str));
		}
		TEST_METHOD(isValidPatternArgsTrim) {
			QueryValidator q;
			string str;
			string expectedStr;

			str = "pattern a(s, _\"x\"_)";
			expectedStr = "s,_\"x\"_";
			Assert::IsTrue(q.trimPatternArgs(str) == expectedStr);

			str = "pattern a(s, _\"x+y\"_)";
			expectedStr = "s,_\"x+y\"_";
			Assert::IsTrue(q.trimPatternArgs(str) == expectedStr);

			str = "pattern a(s, _\"x+y\"_ )";
			expectedStr = "s,_\"x+y\"_";
			Assert::IsTrue(q.trimPatternArgs(str) == expectedStr);
		}
		TEST_METHOD(isValidModifiesP) {
			QueryValidator q;
			string str;

			str = "Modifies(\"x\",\"y\")";
			Assert::IsTrue(q.isValidModifiesP(str));
		}
	};
}