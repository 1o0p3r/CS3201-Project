#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidator.h"
#include <assert.h>
#include <Util.h>

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
			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;

			query = "stmt s; Select s";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "stmt s; Select s such that Follows(s,4)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "variable v; Select v such that Uses(\"       Second\", v)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "   while w   ; assign a   ; Select w such that Follows(w, a)  ";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "while w; assign a; Select w such that Follows(w, a) pattern a(_, x)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; Select a pattern b#(_, _\"f - d + b - l\"_)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "while w; assign a; Select w such that Follows(w, a) pattern a(_, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "while w; assign a; Select x such that Follows(w, a) pattern a(_, \"x\")";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "while w; assign a; Select w such that Follows(\"w\", a) pattern a(_, \"x\")";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "prog_line pl, p#; constant c; Select c such that Follows(c,_)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "prog_line pl, p#; constant c, d; Select p# such that Follows(c,_)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; prog_line pl, p#; constant c, d; Select p# such that Follows(a,_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; Select a pattern a(_, _\"f - d + b - l\"_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "variable v1,v#; assign a1,a#; constant d; Select v1 such that Modifies(6,v1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,v1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a1(_, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a(_, _\"x\")";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a#(\"y\", \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") and Parent(1, _)pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

		}

		TEST_METHOD(isValidMultiplePatternQuery) {
			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 pattern a#(v#,_\"x+y\"_) such that Parent(1, _)pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 pattern a#(v#,_\"x+y\"_) and pattern a1(1, \"(x+y)\")pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#(v#,_\"x+y\"_)		and pattern a1(1, \"(x+y)\")		pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#(v#,_\"x+y\"_)	 pattern a1(1, \"(x+y)\")		pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#     (v#,_\"x+y\"_)	 pattern a1		(1, \"(x+y)\")		pattern a1		(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#     (	v#		,		_\"x+y\"_		)	 pattern a1		(	 1, \"(x+y)\"	)		pattern a1		(	v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#     (	v#		,		_		\"		x  +y		\"		_		)	 pattern a1		(	 1, \"(x+y)\"	)		pattern a1		(	v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#     (	v#		,		_		\"		(x  +y)*z-     4		\"		_		)	 pattern a1		(	 \"     box \", \"(x+y)\"	)		pattern a1		(	v1, \"   x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#		  (	v#		,		_		    	\"		   	(	x	   +y		)*		z-     4		\"		_		)	 pattern a1		(	 \"     box		\"	,	 \"(x	+	y)	\"	)		pattern a1		(	v1, \"   x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

		}
		TEST_METHOD(isValidQueryIfsAndProcDeclaration) {
			QueryValidator queryValidator;
			string query;
			QueryElement selectQueryElement, suchThatQueryElement,patternQueryElement, withQueryElement;
			QueryStatement expectedQueryStatement;
			QueryStatement queryStatement;

			query = "stmt s; assign a; procedure p; while w; if i;  call c; variable v; Select BOOLEAN such that Modifies(3, \"a\") and Modifies(a, v) and Modifies(\"x\", \"x\") and Modifies(s, v) such that Modifies(w, v) and Modifies(i, v) and  Modifies(p, v)";
			Assert::IsTrue(queryValidator.parseInput(query));
			selectQueryElement = QueryElement("empty", "empty", "BOOLEAN");
			expectedQueryStatement.addSelectQuery(selectQueryElement);
			suchThatQueryElement = QueryElement("3", "number", "empty", "a", "variable", "empty", "Modifies");
			expectedQueryStatement.addSuchThatQuery(suchThatQueryElement);
			suchThatQueryElement = QueryElement("a", "synonym", "assign", "v", "synonym", "variable", "Modifies");
			expectedQueryStatement.addSuchThatQuery(suchThatQueryElement);
			suchThatQueryElement = QueryElement("x", "variable", "empty", "x", "variable", "empty", "Modifies");
			expectedQueryStatement.addSuchThatQuery(suchThatQueryElement);
			suchThatQueryElement = QueryElement("s", "synonym", "stmt", "v", "synonym", "variable", "Modifies");
			expectedQueryStatement.addSuchThatQuery(suchThatQueryElement);
			suchThatQueryElement = QueryElement("w", "synonym", "while", "v", "synonym", "variable", "Modifies");
			expectedQueryStatement.addSuchThatQuery(suchThatQueryElement);
			suchThatQueryElement = QueryElement("i", "synonym", "if", "v", "synonym", "variable", "Modifies");
			expectedQueryStatement.addSuchThatQuery(suchThatQueryElement);
			suchThatQueryElement = QueryElement("p", "synonym", "procedure", "v", "synonym", "variable", "Modifies");
			expectedQueryStatement.addSuchThatQuery(suchThatQueryElement);
			queryStatement = queryValidator.getQueryStatement();

		}

		TEST_METHOD(isValidQueryWith) {
			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;

			query = "procedure p; constant c; assign a; Select BOOLEAN pattern a(_,_) with p.procName = \"First\"";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a, a1, a2; constant c; Select BOOLEAN pattern a (_,   _) such that Next(20, 40)   and  Next(30, 50) with c.value = 10";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "assign a, a1; constant c; Select BOOLEAN pattern a(_, _) such that Next(20,40) and Next*(a, 30) with c.value = 10 and a.stmt# = c.value";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; procedure p; if ifs; variable v; constant c; Select BOOLEAN pattern a(_,_) such that Calls*(\"First\", p) with p.procName = \"Second\" with v.varName = \"Hello\"";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "assign a; procedure p1, p2; if ifs; variable v; constant c; Select BOOLEAN with p1.procName = \"Second\" and p2.procName = p1.procName with v.varName = \"HELP\" with c.value = ifs.stmt#";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; prog_line n1,n2; procedure p1,p2; if ifs; variable v; constant c; Select BOOLEAN with p1.procName = v.varName and n1 = ifs.stmt# with n1 = 5";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; prog_line n1,n2; procedure p1,p2; while w; variable v; constant c; Select BOOLEAN with p1.procName = \"Second\" and \"Third\" = v.varName with p1.procName = v.varName with w.stmt# = 5 and w.stmt# = c.value";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
		}
		//This test method checks if the correct Query Elements are parse into the QueryStatement
		TEST_METHOD(isValidQueryStatement) {
			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;
			QueryElement expectedSelectQueryElement;
			vector<QueryElement> expectedSuchThatQueryElements;
			vector<QueryElement> expectedPatternQueryElements;

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") and Parent(1, _)pattern a1(v1, \"x\")";
			queryValidator.parseInput(query);
			queryStatement = queryValidator.getQueryStatement();
			expectedSelectQueryElement = QueryElement("variable", "v1", "synonym");
		}
		//This test method assumes that the input is already grammatically correct i.e. no commas out of nowhere
		TEST_METHOD(isValidParseEntityAndSynonym) {
			QueryValidator queryValidator;
			string str;

			str = "stmt s";
			Assert::IsTrue(queryValidator.isEntityAndSynonym(str));

			str = "while w";
			Assert::IsTrue(queryValidator.isEntityAndSynonym(str));

			str = "haha a";
			Assert::IsFalse(queryValidator.isEntityAndSynonym(str));

			str = "stmt s,a";
			Assert::IsTrue(queryValidator.isEntityAndSynonym(str));

			str = "stmt s, w, a";
			Assert::IsTrue(queryValidator.isEntityAndSynonym(str));
		}

		TEST_METHOD(isValidAssignPatternRegex) {
			QueryValidator queryValidator;
			string str;

			str = "a(_, \"x+y\")";
			Assert::IsTrue(queryValidator.isAssignPatternRegex(str));

			str = "a22(\"	x	\", \"x+y\")";
			Assert::IsTrue(queryValidator.isAssignPatternRegex(str));

			str = "a#( v, \"(x+y)\")";
			Assert::IsTrue(queryValidator.isAssignPatternRegex(str));

			str = "a#(2	, _\"(x+y)\"_)";
			Assert::IsTrue(queryValidator.isAssignPatternRegex(str));

		}

		TEST_METHOD(isInValidAssignPatternRegex) {
			QueryValidator queryValidator;
			string str;

			str = "a(_\"x\"_, \"x+y\")";
			Assert::IsFalse(queryValidator.isAssignPatternRegex(str));

			str = "a(_\"x%\"_, \"x+y\")";
			Assert::IsFalse(queryValidator.isAssignPatternRegex(str));

			str = "a(_\"x%\"_, \".,\")";
			Assert::IsFalse(queryValidator.isAssignPatternRegex(str));
	
		}

		TEST_METHOD(isValidSplit) {
			QueryValidator queryValidator;
			vector<string> vecStr;
			vector<string> expectedVecStr;
			string toPush, arg1, arg2, arg3, arg4, arg5, arg6;
			vector<string> temp = vecStr;
			
			toPush = "Select s";
			arg1 = "Select s";
			expectedVecStr.push_back(arg1);
			vecStr = queryValidator.splitToSentences(toPush);
			temp = vecStr;
			Assert::IsTrue(vecStr == expectedVecStr);
			
			vecStr.clear();
			expectedVecStr.clear();
		
			toPush = "Select s such that Follows (s,4)";
			arg1 = "Select s";
			arg2 = "such that Follows (s,4)";
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			vecStr = queryValidator.splitToSentences(toPush);
			temp = vecStr;
			Assert::IsTrue(vecStr == expectedVecStr);

			vecStr.clear();
			expectedVecStr.clear();

			toPush = "Select a pattern a(_, \"y\")";
			arg1 = "Select a";
			arg2 = "pattern a(_, \"y\")";
			temp = vecStr;
			vecStr = queryValidator.splitToSentences(toPush);
			vecStr.clear();
			expectedVecStr.clear();
			Assert::IsTrue(vecStr == expectedVecStr);

			vecStr.clear();
			expectedVecStr.clear();

			toPush = "Select s such that Follows(s,4) pattern a(""x"",_)";
			arg1 = "Select s";
			arg2 = "such that Follows(s,4)";
			arg3 = "pattern a(""x"",_)";
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			expectedVecStr.push_back(arg3);
			temp = vecStr;
			vecStr = queryValidator.splitToSentences(toPush);
			Assert::IsTrue(vecStr == expectedVecStr);

			vecStr.clear();
			expectedVecStr.clear();

			toPush = "Select s        pattern a(""x"",_)       such that         Follows(s,4)                    ";
			arg1 = "Select s";
			arg2 = "pattern a(""x"",_)";
			arg3 = "such that Follows(s,4)";
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			expectedVecStr.push_back(arg3);
			temp = vecStr;
			vecStr = queryValidator.splitToSentences(toPush);
			Assert::IsTrue(vecStr == expectedVecStr);

			vecStr.clear();
			expectedVecStr.clear();

			toPush = "Select s such that Follows(s,4) pattern a(\"x\", _) with p.procName = \"First\"";
			arg1 = "Select s";
			arg2 = "such that Follows(s,4)";
			arg3 = "pattern a(\"x\", _)";
			arg4 = "with p.procName = \"First\"";
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			expectedVecStr.push_back(arg3);
			expectedVecStr.push_back(arg4);
			vecStr = queryValidator.splitToSentences(toPush);
			temp = vecStr;
			Assert::IsTrue(vecStr == expectedVecStr);

			vecStr.clear();
			expectedVecStr.clear();
			
			toPush = "Select s pattern a(""x"",_)";
			arg1 = "Select s";
			arg2 = "pattern a(""x"",_)";
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			vecStr = queryValidator.splitToSentences(toPush);
			temp = vecStr;


			vecStr.clear();
			expectedVecStr.clear();
			temp.clear();

			toPush = "Select s such that Follows(s,4) and Parent(a,4) pattern a(\"x\", _) with p.procName = \"First\" such that Modifies(x, \"x\")";
			arg1 = "Select s";
			arg2 = "such that Follows(s,4) and Parent(a,4)";
			arg3 = "pattern a(\"x\", _)";
			arg4 = "with p.procName = \"First\"";
			arg5 = "such that Modifies(x, \"x\")";
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			expectedVecStr.push_back(arg3);
			expectedVecStr.push_back(arg4);
			expectedVecStr.push_back(arg5);
			vecStr = queryValidator.splitToSentences(toPush);
			temp = vecStr;
			Assert::IsTrue(vecStr == expectedVecStr);

			
			vecStr.clear();
			expectedVecStr.clear();
			temp.clear();

			toPush = "Select s such that Follows(s,4) pattern a(\"x\", _) and pattern a2(_, _\"y\"_) with p.procName = \"First\" and n= 1";
			arg1 = "Select s";
			arg2 = "such that Follows(s,4)";
			arg3 = "pattern a(\"x\", _) and pattern a2(_, _\"y\"_)";
			arg4 = "with p.procName = \"First\" and n= 1";
			vecStr = queryValidator.splitToSentences(toPush);
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			expectedVecStr.push_back(arg3);
			expectedVecStr.push_back(arg4);
			temp = vecStr;
			Assert::IsTrue(vecStr == expectedVecStr);
			
			vecStr.clear();
			expectedVecStr.clear();
			temp.clear();

			toPush = "Select s such that Follows(s,4) pattern a(\"x\", _) pattern a2(_, _\"y\"_) with p.procName = \"First\" and n= 1";
			arg1 = "Select s";
			arg2 = "such that Follows(s,4)";
			arg3 = "pattern a(\"x\", _) pattern a2(_, _\"y\"_)";
			arg4 = "with p.procName = \"First\" and n= 1";
			vecStr = queryValidator.splitToSentences(toPush);
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			expectedVecStr.push_back(arg3);
			expectedVecStr.push_back(arg4);
			temp = vecStr;
			Assert::IsTrue(vecStr == expectedVecStr);

			vecStr.clear();
			expectedVecStr.clear();
			temp.clear();

			toPush = "Select s such that Follows(s,4) pattern a(\"x\", _) pattern a2(_, _\"y\"_) with p.procName = \"First\" and n= 1 pattern a3(_, \"z\")";
			arg1 = "Select s";
			arg2 = "such that Follows(s,4)";
			arg3 = "pattern a(\"x\", _) pattern a2(_, _\"y\"_)";
			arg4 = "with p.procName = \"First\" and n= 1";
			arg5 = "pattern a3(_, \"z\")";
			vecStr = queryValidator.splitToSentences(toPush);
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			expectedVecStr.push_back(arg3);
			expectedVecStr.push_back(arg4);
			expectedVecStr.push_back(arg5);
			temp = vecStr;
			Assert::IsTrue(vecStr == expectedVecStr);

			vecStr.clear();
			expectedVecStr.clear();
			temp.clear();

			toPush = "Select s such that Follows(s,4) pattern a(\"pattern\", _) pattern a2(_, _\"y\"_) with p.procName = \"First\" and n= 1 pattern a3(_, \"pattern\")";
			arg1 = "Select s";
			arg2 = "such that Follows(s,4)";
			arg3 = "pattern a(\"pattern\", _) pattern a2(_, _\"y\"_)";
			arg4 = "with p.procName = \"First\" and n= 1";
			arg5 = "pattern a3(_, \"pattern\")";
			vecStr = queryValidator.splitToSentences(toPush);
			expectedVecStr.push_back(arg1);
			expectedVecStr.push_back(arg2);
			expectedVecStr.push_back(arg3);
			expectedVecStr.push_back(arg4);
			expectedVecStr.push_back(arg5);
			temp = vecStr;
			Assert::IsTrue(vecStr == expectedVecStr);

		}
		TEST_METHOD(isValidDeclarationRegex) {
			QueryValidator queryValidator;
			string str;

			str = "stmt sos;";
			Assert::IsTrue(queryValidator.isValidDeclarationRegex(str));

			str = "constant s,v;";
			Assert::IsTrue(queryValidator.isValidDeclarationRegex(str));

			str = "assign a1,a2,a;";
			Assert::IsTrue(queryValidator.isValidDeclarationRegex(str));

			str = "prog_line n;";
			Assert::IsTrue(queryValidator.isValidDeclarationRegex(str));

			str = "prog_line n123;";
			Assert::IsTrue(queryValidator.isValidDeclarationRegex(str));

			str = "stmt First;";
			Assert::IsTrue(queryValidator.isValidDeclarationRegex(str));

			str = "stmt First#;";
			Assert::IsTrue(queryValidator.isValidDeclarationRegex(str));

			str = "procedure First#;";
			Assert::IsTrue(queryValidator.isValidDeclarationRegex(str));

			str = "call wOdeCall#;";
			Assert::IsTrue(queryValidator.isValidDeclarationRegex(str));


		}
		TEST_METHOD(isInvalidDeclarationRegex) {
			QueryValidator queryValidator;
			string str;

			str = "while ,v;";
			Assert::IsFalse(queryValidator.isValidDeclarationRegex(str));


		}
		TEST_METHOD(isValidSuchThatRegex) {
			QueryValidator queryValidator;
			string str;

			str = "such that Uses(3,4)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Uses(3,s)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Parent(3,_)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Parent*(3,_)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Follows(s,4)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Follows(\"s#\",4)";
			Assert::IsFalse(queryValidator.isValidSuchThatRegex(str));

			str = "such that Modifies(\"s\",\"a\")";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Modifies(\"s\",\"a\") and Follows(s,4)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Modifies(\"s\",\"a\") and Follows(s,4) and Uses(3,s)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Parent*(s#,    4)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Follows*(_,_   )";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

		}
		TEST_METHOD(isValidSuchThatRegexExtended) {
			QueryValidator queryValidator;
			string str;

			str = "such that Uses(3,4)such that Modifies(2,4)";
			Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			str = "such that Modifies(\"s\",\"a\") and Follows(s,3)";
			Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));
			
			str = "such that Modifies(\"s\",\"a\") and Follows(s,3) such that Uses(3,4)";
			Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			str = "such that Modifies(\"s\",\"a\") and Follows(s,3) such that Uses(3,4)";
			Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			str = "such that Modifies(\"s\",\"a\") and Follows(s,3) such that Uses(3,4) and Parent(5,7)";
			Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			str = "such that Modifies(\"s\",\"a\") and Follows(s,3) such that Next(8,9) such that Uses(3,4) and Parent(5,7)";
			Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			str = "such that Modifies(uses, \"Next\") and Follows(s,3) such that Next(6, Parent)";
			Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));
		}
		TEST_METHOD(isInValidSuchThatRegexExtended) {
			QueryValidator queryValidator;
			string str;

			//Not allowed symbols
			str = "such that Uses(3,4) such that Modifies(2,4)#";
			Assert::IsFalse(queryValidator.isValidSuchThaExtendedRegex(str));

			//Not allowed relationships
			str = "such that Uses(3,4) such that Pathod(2,4)#";
			Assert::IsFalse(queryValidator.isValidSuchThaExtendedRegex(str));

			//Missing relationship
			str = "such that";
			Assert::IsFalse(queryValidator.isValidSuchThaExtendedRegex(str));

			//Missing relationship
			str = "such that Uses(4,5) and";
			Assert::IsFalse(queryValidator.isValidSuchThaExtendedRegex(str));

			//Missing 2 relationships
			str = "such that Uses(4,5) and such that";
			Assert::IsFalse(queryValidator.isValidSuchThaExtendedRegex(str));

			//Whitespaces
			str = "such that Uses (4,5) and such that";
			Assert::IsFalse(queryValidator.isValidSuchThaExtendedRegex(str));

			//Invalid symbols within arguments
			str = "such that Modifies(uses, \"Next(x, y)\") and Follows(s,3) such that Next(6, Parent)";
			Assert::IsFalse(queryValidator.isValidSuchThaExtendedRegex(str));
		}
		TEST_METHOD(isValidExtractSuchThatClauses) {
			QueryValidator queryValidator;
			string str;
			vector<string> expectedVec;
			vector<string> returnedVec;


			str = "such that Modifies(\"s\",\"a\") and Follows(s,4) and Uses(3,s)";
			expectedVec.push_back("Modifies(\"s\",\"a\")");
			expectedVec.push_back("Follows(s,4)");
			expectedVec.push_back("Uses(3,s)");
			returnedVec = queryValidator.extractSuchThatClauses(str);
			Assert::IsTrue(returnedVec == expectedVec);

			expectedVec.clear();
			returnedVec.clear();

			str = "such that Uses(3, 4)such that Modifies(2, 4)";
			expectedVec.push_back("Uses(3, 4)");
			expectedVec.push_back("Modifies(2, 4)");
			returnedVec = queryValidator.extractSuchThatClauses(str);
			Assert::IsTrue(returnedVec == expectedVec);

			expectedVec.clear();
			returnedVec.clear();

			str = "such that Modifies(\"s\",\"a\") and Follows(s,3) such that Next(8,9) such that Uses(3,4) and Parent(5,7)";
			expectedVec.push_back("Modifies(\"s\",\"a\")");
			expectedVec.push_back("Follows(s,3)");
			expectedVec.push_back("Next(8,9)");
			expectedVec.push_back("Uses(3,4)");
			expectedVec.push_back("Parent(5,7)");
			returnedVec = queryValidator.extractSuchThatClauses(str);
			Assert::IsTrue(returnedVec == expectedVec);

			expectedVec.clear();
			returnedVec.clear();

			str = "such that Modifies (\"s\",\"a\") and Follows(s,3) such that Next(8,9) such that Uses(3,4) and Parent(5,7)";
			expectedVec.push_back("Follows(s,3)");
			expectedVec.push_back("Next(8,9)");
			expectedVec.push_back("Uses(3,4)");
			expectedVec.push_back("Parent(5,7)");
			returnedVec = queryValidator.extractSuchThatClauses(str);
			Assert::IsTrue(returnedVec == expectedVec);
		}
		TEST_METHOD(isValidExtractWithClauses) {
			QueryValidator queryValidator;
			string str;
			vector<string> expectedVec;
			vector<string> returnedVec;

			str = "with p.procName = v1.varName";
			expectedVec.push_back("p.procName = v1.varName");
			returnedVec = queryValidator.extractWithClauses(str);
			Assert::IsTrue(returnedVec == expectedVec);
			expectedVec.clear();
			returnedVec.clear();

			str = "with v1.varName = v2.varName and v1.varName = \"hello\"";
			expectedVec.push_back("v1.varName = v2.varName");
			expectedVec.push_back("v1.varName = \"hello\"");
			returnedVec = queryValidator.extractWithClauses(str);
			Assert::IsTrue(returnedVec == expectedVec);

			expectedVec.clear();
			returnedVec.clear();

			str = "with v1.varName = v2.varName and v1.varName = \"hello\" with c.value = 1 with p.procName = \"First\"";
			expectedVec.push_back("v1.varName = v2.varName");
			expectedVec.push_back("v1.varName = \"hello\"");
			expectedVec.push_back("c.value = 1");
			expectedVec.push_back("p.procName = \"First\"");
			returnedVec = queryValidator.extractWithClauses(str);
			Assert::IsTrue(returnedVec == expectedVec);

			expectedVec.clear();
			returnedVec.clear();

			str = "with c.value = a.stmt# with v2.varName = p.procName and 4 = c.value with n = c.value and p.procName = \"HELL\"";
			expectedVec.push_back("c.value = a.stmt#");
			expectedVec.push_back("v2.varName = p.procName");
			expectedVec.push_back("4 = c.value");
			expectedVec.push_back("n = c.value");
			expectedVec.push_back("p.procName = \"HELL\"");
			returnedVec = queryValidator.extractWithClauses(str);
			Assert::IsTrue(returnedVec == expectedVec);


			expectedVec.clear();
			returnedVec.clear();

		}

		TEST_METHOD(isValidIfPatternRegex) {
			QueryValidator queryValidator;
			string str;

			str = " ifs(_,_,_)";
			Assert::IsTrue(queryValidator.isValidIfPatternRegex(str));

			str = " ifs(x,   _   ,     _)";
			Assert::IsTrue(queryValidator.isValidIfPatternRegex(str));

			str = "		ifs     (x,   _   ,     _)";
			Assert::IsTrue(queryValidator.isValidIfPatternRegex(str));


			str = " ifs     (	x,   _   ,     _			)";
			Assert::IsTrue(queryValidator.isValidIfPatternRegex(str));

			str = " ifs     (	pattern,   _   ,     _			)";
			Assert::IsTrue(queryValidator.isValidIfPatternRegex(str));
		}
		TEST_METHOD(isInValidIfPatternRegex) {
			QueryValidator queryValidator;
			string str;

			str = "ifs(@,_,_)";
			Assert::IsFalse(queryValidator.isValidIfPatternRegex(str));

			str = "ifs(_,3,4)";
			Assert::IsFalse(queryValidator.isValidIfPatternRegex(str));

			str = "f@s(_,3,4)";
			Assert::IsFalse(queryValidator.isValidIfPatternRegex(str));
		}
		TEST_METHOD(isValidIfMultipleRegex) {
			QueryValidator queryValidator;
			string str;

			str = "pattern ifs(@,_,_)";
			Assert::IsFalse(queryValidator.isValidIfPatternRegex(str));
		}
		TEST_METHOD(isValidSelectInitialRegex) {
			QueryValidator queryValidator;
			string str;

			str = "Select s";
			Assert::IsTrue(queryValidator.isValidSelectInitialRegex(str));

			str = "Select \@";
			Assert::IsFalse(queryValidator.isValidSelectInitialRegex(str));
		}
		TEST_METHOD(isValidWhilePattern) {
			QueryValidator queryValidator;
			string str;

			str = "w#(x,_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));

			str = "wwewew(\"x\",_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));

			str = "waaaa(\"	x	\",_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));

			str = "wddd(1,_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));

			str = "w(		_		,		_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));
		}
		TEST_METHOD(isInValidWhilePattern) {
			QueryValidator queryValidator;
			string str;

			str = "w#(__,_)";
			Assert::IsFalse(queryValidator.isValidWhilePatternRegex(str));

			str = "w#(_+_,_)";
			Assert::IsFalse(queryValidator.isValidWhilePatternRegex(str));

			str = "w#(_,2)";
			Assert::IsFalse(queryValidator.isValidWhilePatternRegex(str));
		}
		TEST_METHOD(isValidWithClause) {
			QueryValidator queryValidator;
			string input;


			input = "with p.procName = v.varName";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));
			
			input = "with p.procName = \"hello\"";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with p.procName = p2.procName";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with \"help\" = p.procName";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with p.procName=\"hello\"";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));
			
			
			
			input = "with v.varName = \"iAmvarName\"";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with v1.varName = v2.varName";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with \"varName\" = v1.varName";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with v.varName    =       \"hi\"";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));


			
			input = "with c.value = 1";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with c.value = a.stmt#";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with c.value = n";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with c1.value = c2.value";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));


			input = "with 2 = c1.value";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with a.stmt# = c1.value";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with w.stmt# = 3";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with     i.stmt# = a.stmt#";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));


			input = "with n = c2.value";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with n1 = n2";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with 2 = n1";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with i.stmt#      = n1";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			
			input = "with 2 = 2";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));


			input = "with v1.varName = v2.varName and v1.varName = \"hello\"";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with n1 = n2.varName and v2.procName = \"hello\"";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with v1.varName = v2.varName and v1.varName = p.procName";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with a.stmt# = 1       and v2.varName = \"hello\"";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with a.stmt# = 1       and v2.varName = \"hello\"";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));

			input = "with         p.procName = v2.varName      and v2.varName = \"hello\"    and i.stmt# = 5 and v1.varName = v2.varName";
			Assert::IsTrue(queryValidator.isValidWithRegex(input));
		}

		TEST_METHOD(isValidWithClauseExtended) {
			QueryValidator queryValidator;
			string input;

			input = "with v1.varName = v2.varName and v1.varName = \"hello\"";
			Assert::IsTrue(queryValidator.isValidWithExtendedRegex(input));

			input = "with v1.varName = v2.varName and v1.varName = \"hello\" with v3.varName = \"hi\"";
			Assert::IsTrue(queryValidator.isValidWithExtendedRegex(input));

			input = "with v1.varName = v2.varName with v1.varName = \"hello\" with v3.varName = \"hi\"";
			Assert::IsTrue(queryValidator.isValidWithExtendedRegex(input));

			input = "with v1.varName = v2.varName with v1.varName = \"hello\" and c.value = 1 with v3.varName = \"hi\"";
			Assert::IsTrue(queryValidator.isValidWithExtendedRegex(input));

			input = "with v1.varName = v2.varName with v1.varName = \"hello\" and cb.value = 1  and 4 = a.stmt# with v3.varName = \"hi\" and p.procName = \"First\"";
			Assert::IsTrue(queryValidator.isValidWithExtendedRegex(input));

		}
		TEST_METHOD(isValidPartialPatternMatch) {
			QueryValidator queryValidator;
			string input;

			input = "and pattern a(_,_)";
			Assert::IsTrue(queryValidator.isPartialPatternRegex(input));

			input = "and          pattern a(_,_)";
			Assert::IsTrue(queryValidator.isPartialPatternRegex(input));
		}
		TEST_METHOD(isInValidPartialPatternMatch) {
			QueryValidator queryValidator;
			string input;

			input = "andpattern a(_,_)";
			Assert::IsFalse(queryValidator.isPartialPatternRegex(input));
		}
	};
}