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
		string filename = "..\\..\\Tests09\\Sample-Source(actual).txt";
		Assert::IsTrue(Parse(filename, pkb));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		Assert::IsTrue(Parse(filename, pkb));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-2.txt";
		Assert::IsTrue(Parse(filename, pkb));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-1.txt";
		Assert::IsTrue(Parse(filename, pkb));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-6.txt";
		Assert::IsTrue(Parse(filename, pkb));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-6-v2.txt";
		Assert::IsTrue(Parse(filename, pkb));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-Next.txt";
		Assert::IsTrue(Parse(filename, pkb));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-Pattern.txt";
		Assert::IsTrue(Parse(filename, pkb));

		pkb = PKB();
		filename = "..\\..\\Tests09\\Sample-Source-Pattern-2.txt";
		Assert::IsTrue(Parse(filename, pkb));
	}

	TEST_METHOD(elseParse) {
		vector<string> input = { "else", "{" };
		Assert::IsTrue(isElseStatement(input));
		input = { "else{" };
		Assert::IsTrue(isElseStatement(input));
		input = { "else", "{", ";" };
		Assert::IsFalse(isElseStatement(input));
		input = { "else", "{", "{" };
		Assert::IsFalse(isElseStatement(input));
		input = { "else", "{{" };
		Assert::IsFalse(isElseStatement(input));
		input = { "else{{" };
		Assert::IsFalse(isElseStatement(input));
	}

	TEST_METHOD(procedureParse) {
		vector<string> input = { "procedure", "apple", "{" };
		Assert::IsTrue(isProcedure(input));
		input = { "procedure", "apple{" };
		Assert::IsTrue(isProcedure(input));
		input = { "procedure", "apple{{" };
		Assert::IsFalse(isProcedure(input));
		input = { "procedure", "3pple{" };
		Assert::IsFalse(isProcedure(input));
		input = { "procedure", "apple{", "{" };
		Assert::IsFalse(isProcedure(input));
	}

	TEST_METHOD(callParse) {
		vector<string> input = { "call", "apple", ";" };
		Assert::IsTrue(isCallStatement(input));
		input = { "call", "apple;" };
		Assert::IsTrue(isCallStatement(input));
		input = { "call", "apple", ";", "}", "}" };
		Assert::IsTrue(isCallStatement(input));
		input = { "call", "apple", ";", "}}" };
		Assert::IsTrue(isCallStatement(input));
		input = { "call", "apple", ";;", "}}" };
		Assert::IsFalse(isCallStatement(input));
		input = { "call", "apple", ";", ";", "}}" };
		Assert::IsFalse(isCallStatement(input));
		input = { "call", "apple", ";", ";", "};}" };
		Assert::IsFalse(isCallStatement(input));
	}

	TEST_METHOD(whileParse) {
		vector<string> input = { "while", "apple", "{" };
		Assert::IsTrue(isWhileStatement(input));
		input = { "while", "apple{" };
		Assert::IsTrue(isWhileStatement(input));
		input = { "while", "apple{{" };
		Assert::IsFalse(isWhileStatement(input));
		input = { "while", "3pple{" };
		Assert::IsFalse(isWhileStatement(input));
		input = { "while", "apple{", "{" };
		Assert::IsFalse(isWhileStatement(input));
	}

	TEST_METHOD(ifParse) {
		vector<string> input = { "if", "apple", "then" ,"{" };
		Assert::IsTrue(isIfStatement(input));
		input = { "if", "apple", "then{" };
		Assert::IsTrue(isIfStatement(input));
		input = { "if", "apple", "then{{" };
		Assert::IsFalse(isIfStatement(input));
		input = { "if", "3pple", "then{" };
		Assert::IsFalse(isIfStatement(input));
		input = { "if", "apple", "then{", "{" };
		Assert::IsFalse(isIfStatement(input));
	}

	TEST_METHOD(assignParse) {
		string input = "x=1+y;";
		Assert::IsTrue(isAssignStatement(input));
		input = "xyz=1+y;}";
		Assert::IsTrue(isAssignStatement(input));
		input = "x = 1 + z ;";
		Assert::IsTrue(isAssignStatement(input));
		input = "xyz = 1 + z ; }";
		Assert::IsTrue(isAssignStatement(input));
		input = "x=1; 2";
		Assert::IsFalse(isAssignStatement(input));
		input = "=1;";
		Assert::IsFalse(isAssignStatement(input));
		input = " a = f +s = 3;";
		Assert::IsFalse(isAssignStatement(input));
	}
	};
}