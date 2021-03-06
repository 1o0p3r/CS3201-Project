#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidator.h"
#include <assert.h>
#include <Util.h>
#include <QueryStatement.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

const string WHITESPACE__STRING = " ";
const string COMMA_STRING = ",";
const char INVERTED_COMMA = '\"';
const string DOUBLE_QUOTATION_STRING = "\"";
const bool TRUE = true;
const bool FALSE = false;
namespace UnitTesting
{
	TEST_CLASS(queryValidatorTest)
	{
	public:
		TEST_METHOD(testQuerySuchThat)
		{
			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;

			query = "assign a1; Select BOOLEAN such that Affects*(_,_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1; Select BOOLEAN such that Follows*(_,_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select BOOLEAN such that Follows(s,s)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == TRUE);

			query = "stmt s; Select BOOLEAN such that Affects(s,s)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select BOOLEAN such that Affects(5,5)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select BOOLEAN such that Modifies(\"hi\",\"hi\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select s such that Modifies(\"hi\",\"hi\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select BOOLEAN such that Follows(\"hi\",\"hi\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == TRUE);

			query = "if ifs, ifs2,ifs3 ; Select ifs";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select s";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "if ifs; Select ifs";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "while whiles; Select whiles";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);
			
			query = "while c; constant c; Select c";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "while c; constant c; Select c";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "Select BOOLEAN";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select s such that Follows(s,4)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "   while w   ; assign a   ; Select w such that Follows(w, a)  ";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p; stmt s;  Select <p.procName, s>";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);
			
			query = "assign a; Select a pattern b#(_, _\"f - d + b - l\"_)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);
			
			query = "prog_line pl, p#; constant c; Select c such that Follows(c,_)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "prog_line pl, p#; constant c, d; Select p# such that Follows(c,_)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a; prog_line pl, p#; constant c, d; Select p# such that Follows(a,_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "if ifs; Select ifs such that Next*(ifs, _)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "variable v1,v#; assign a1,a#; constant d; Select v1 such that Modifies(6,v1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,v1) and Parent(1, w1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") such that Parent(1, w1) and Uses(1,v1) such that Follows(a1, _)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s1, s2; assign a; Select s1 such that Next*(s1, s2) and Parent(s2, a)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select s such that Follows(s,s)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a; stmt s; Select s such that Follows(a,a)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select s such that Follows(*s,s)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a; stmt s; Select s such that Follows*(a,a)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a;stmt s; Select s such that Parent(a,a)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select s such that Parent(s,s)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a; stmt s; Select s such that Parent*(a,a)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select s such that Parent*(s,s)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; if ifs; Select s such that Parent*(ifs,ifs)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "prog_line l; Select l such that Next(l,l)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "prog_line l; Select l such that Next(1,l)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "prog_line l; Select l such that Next(1,1)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; if ifs; Select s such that Next*(ifs,ifs)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; if ifs; Select s such that Next*(ifs,ifs)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; if ifs; Select s such that Calls(\"aa\",\"a\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; if ifs; Select s such that Calls(\"a\",\"a\")";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; if ifs; Select s such that Calls*(\"a\",\"a\")";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; if ifs; Select s such that Modifies(\"First\",\"Second\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; if ifs; Select s such that Modifies(\"First\",\"First\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s1,s2; Select s1 such that Parent(_,s1) and Parent(_,_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s1,s2; Select s1 such that Parent(_,s1) and Parent(s1,s2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s1,s2; Select s1 such that Parent(_,s1) and Parent(_,s2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; prog_line n; stmt s; Select s such that Affects(s, a1) and Affects(n , a1) and Affects(a2, a1) and Affects(_ , s)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt a1; Select a1 such that Affects(1,2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt a1; Select a1 such that Affects(1,1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt a1; Select a1 such that Affects(1,a1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1; Select a1 such that Affects(a1,2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; Select a1 such that Affects(a1,a2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; Select a1 such that Affects(a1,_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; Select a1 such that Affects(a1,a1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; Select a1 such that Affects*(1,2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; Select a1 such that Affects*(a1,a2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; Select a1 such that Affects*(_,a2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1; prog_line p1, p2; Select p1 such that Affects*(_,p2) and Affects(_,a1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1; prog_line p1, p2; Select p1 such that Affects*(_,p2) such that Affects(_,a1) and Affects*(p1,p2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p1; Select p1 such that Calls*(p1, \"SystemTestFour\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p1; Select p1 such that Calls*(p1, \"SystemTestFive\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p1; Select p1 such that Calls*(_, \"SystemTestTwo\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p1, p2; Select p2 such that Calls*(p1, p2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p1, p2, p3; Select p2 such that Calls*(p2, p3) and Calls*(p3, \"SystemTestThree\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p1, p2, p3; Select p1 such that Calls(p3, p2) and Calls(p2, \"SystemTestThree\") and Calls*(p3, \"SystemTestFour\") and Calls*(p1, p2)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "while w; if ifs;Select <w, ifs> such that Parent*(w, ifs)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; if ifs; while w; Select w such that Next*(ifs, a1) an Next*(a1, w) and Next*(w, a2)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; if ifs; while w; Select w such that Nex(ifs, a1) ";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1.varName such that Modifies(6,v1) and Parent(1, w1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; prog_line n; stmt s; Select s such that Affects(s, a1) and Affects(n , a1) and Affects(a2, a1) and Affects(_ , s)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);
		}
		TEST_METHOD(testQueryAll) {

			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;

			query = "while w; assign a; Select w such that Follows(w, a) pattern a(_, x)";
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

			query = "assign a;variable v; Select v such that Uses(\"       Second\", v)  pattern a(,) and pattern a(v,_)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a#(\"y\", \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a1(_, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") pattern a(_, _\"x\")";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
		
			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") and Parent(1, _)pattern a1(v1, \"x\") with d.value = 4";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 pattern a#(v#,_\"x+y\"_) such that Parent(1, _)pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a1, a2; if ifs1; variable v1, v2; procedure p1, p2; while w1, w2; call c1, c2; Select <p1, c1> such that Uses(p1, v1) with p1.procName = \"SystemTestThree\" pattern a1(v1, _\"hThree\"_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "stmt s; assign a; Select <s, a> pattern a (_, _\"x\"_) such that Parent* (s, a) and Next (s, a) and Next* (a, s)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "call c; if ifs; Select <c, ifs> such that Next*(ifs, c)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a1; if ifs; variable v; Select v such that Next*(a1, ifs) pattern ifs(v, _ , _ ) with v.varName = \"bOne\"";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; if ifs1; variable v1, v2; procedure p1, p2; while w1, w2; call c1, c2; stmt s; Select <a1,ifs1,p1,w1,v2> such that Next*(w1, s) and Parent*(s, c1) and Modifies(c1, v1) and Uses(p1, v1) pattern a1(v2, _\"hOne\"_) and a2(v1, _\"hOne\"_) with ifs1.stmt# = 12 and p1.procName = \"SystemTestOne\"";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; if ifs1; variable v1, v2; procedure p1, p2; while w1, w2; call c1, c2; Select <p1, c1> such that Uses(p1, v1) with p1.procName = \"SystemTestThree\" pattern a1(v1, _\"hThree\"_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; if ifs1; variable v1, v2; procedure p1, p2; while w1, w2; call c1, c2; Select <v1, ifs1> such that Next*(ifs1, w1) and Modifies(p1, v1) and Calls(p1, p2) and Parent*(ifs1, w1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a1, a2; if ifs1; variable v1, v2; procedure p1, p2; while w1, w2; call c1, c2; Select <v1, ifs1> such that Calls(p1, p2) and Modifies(p2, v1) and Uses(a1, v1) and Parent*(w1, ifs1)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);
		}

		TEST_METHOD(testPatternQuery) {
			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;

			query = "variable v; assign a, a#; Select BOOLEAN pattern a#(_,_) and a(_,_";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == TRUE);

			query = "assign a; variable v; Select a pattern a(_,_) and a(v,_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; Select a pattern a(_, _\"f - d + b - l\"_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "while w; variable v; Select BOOLEAN pattern w(\"aOne\", _)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();                                       

			query = "variable v; assign a, a#; Select BOOLEAN pattern a#(1, _)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == TRUE);

			query = "variable v; assign a, a#; Select BOOLEAN pattern a#(\"aa\"a, _)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == TRUE);

			query = "variable v; assign a, a#; Select v pattern a#(_,_) and a(_,_";
			Assert::IsFalse(queryValidator.parseInput(query));

			query = "variable v; assign a, a#; Select v pattern a#(_,_) and a(_,_\"(x+y)\"_";
			Assert::IsFalse(queryValidator.parseInput(query));
		
			query = "variable v; assign a, a#; Select v pattern a#(_,_) and a(_,_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v; assign a, a#; Select v pattern a#(_,_) and a(_,_\"(x+y)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v; assign a, a#; Select v pattern a#(_,\"x+y\"_) and a(_,\"x\")";
			Assert::IsFalse(queryValidator.parseInput(query));

			query = "variable v; assign a, a#, Select v pattern a#(_, __\"x+y\"_)";
			Assert::IsFalse(queryValidator.parseInput(query));

			query = "variable v; assign a, a#, Select v pattern a#(_, _\"_x+y\"_)";
			Assert::IsFalse(queryValidator.parseInput(query));

			query = "variable v; assign a, a#; Select v pattern a#(_,_) and a(v,_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v; assign a, a#; Select v pattern a#(_,_) and a(v,\"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v; assign a, a#; Select v pattern a#(v,_) and a(v,\"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "variable v; assign a, a#; Select v pattern a#(\"aa\",_) and a(v,\"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v; assign a, a#; Select v pattern a#(\"aa\",_) and a(v,_\"x\"_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v; assign a, a#; Select v pattern a#(\"aa\",_) and a(v,_\"(x+y)*z\"_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; variable v; Select a pattern a(,) and pattern(v,_)";
			Assert::IsFalse(queryValidator.parseInput(query));

			query = "assign a; variable v; Select a pattern a(_,_) and pattern a(v,_)";
			Assert::IsFalse(queryValidator.parseInput(query));

			query = "variable v; assign a, a#; Select v pattern a#(\"aa\",_) and a(v,_\"(x+y)*z\"_) and a(v,_\"x\"_";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v; assign a, a#; Select v pattern a#(\"aa\",_) and a(1,_\"(x+y)*z\"_) and a(v,_\"x\"_)";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v; assign a, a#; Select v pattern a#(\"aa\",_) and a(v,_\"(x+y)*z\"_)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; Select v1 pattern a#(v#,_\"x+y\"_) pattern a1(v1, \"(x+y)\")pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 pattern w1(\"x\", _)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; if ifs1; constant d; while w1, w2; Select v1 pattern ifs1(\"xor  \", _, _)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#(v#,_\"x+y\"_)		and a1(v1, \"(x+y)\")		pattern a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#(v#,_\"x+y\"_)	 pattern a1(v1, \"(x+y)\")		and		 a1(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#(v#,_\"x+y\"_)	 and pattern a1(v1, \"(x+y)\")		and		 a1(v1, \"x\")";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#     (v#,_\"x+y\"_)	 pattern a1		(v1, \"(x+y)\")		pattern a1		(v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#     (	v#		,		_\"x+y\"_		)	 pattern a1		(	 v1, \"(x+y)\"	)		pattern a1		(	v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			
			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#     (	v#		,		_		\"		x  +y		\"		_		)	 and	 a1		(	 v1, \"(x+y)\"	)		pattern a1		(	v1, \"x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
		
			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#     (	v#		,		_		\"		(x  +y)*z-     4		\"		_		)	 pattern a1		(	 \"     box \", \"(x+y)\"	)		pattern a1		(	v1, \"   x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
		
			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1			pattern a#		  (	v#		,		_		    	\"		   	(	x	   +y		)*		z-     4		\"		_		)	 pattern a1		(	 \"     box		\"	,	 \"(x	+	y)	\"	)		pattern a1		(	v1, \"   x\")";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
		

		}
		TEST_METHOD(testQueryWith) {
			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;

			query = "stmt s; Select BOOLEAN with \"string\" = \"string\"";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt s; Select BOOLEAN with 1=1";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);
			
			query = "prog_line n1, n2; Select n1 such that Next*(n1, n2) and Uses(n2, \"b\") with n2 = 43";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "stmt a; constant c; prog_line n; Select BOOLEAN with a.stmt# = c.value";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p; constant c; prog_line n; Select BOOLEAN with n=1";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p; constant c; prog_line n; Select BOOLEAN with \"string\"=1";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == TRUE);

			query = "procedure p; constant c; prog_line n; Select BOOLEAN with p.procName = p.procName";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p; constant c; prog_line n; Select p with p.procName = p.procName";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p; constant c; prog_line n; Select BOOLEAN with \"string\"= p.procName";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p; constant c; prog_line n; Select BOOLEAN with \"string\"= \"string\"";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p; constant c; prog_line n; Select BOOLEAN with \"string\"= \"string2\"";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == TRUE);

			query = "procedure p; constant c; prog_line n; Select BOOLEAN with 5=1";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == TRUE);

			query = "procedure p; constant c; prog_line n; Select p with 5=1";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p; constant c; assign a; Select BOOLEAN pattern a(_,_) with p.procName = \"First\"";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a, a1, a2; constant c; Select BOOLEAN pattern a (_,   _) such that Next(20, 40)   and  Next(30, 50) with c.value = 10";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a, a1; constant c; Select BOOLEAN pattern a(_, _) such that Next(20,40) and Next*(a, 30) with c.value = 10 and a.stmt# = c.value";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a; procedure p; if ifs; variable v; constant c; Select BOOLEAN pattern a(_,_) such that Calls*(\"First\", p) with p.procName = \"Second\" with v.varName = \"Hello\"";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a; procedure p1, p2; if ifs; variable v; constant c; Select BOOLEAN with p1.procName = \"Second\" and p2.procName = p1.procName with v.varName = \"HELP\" with c.value = ifs.stmt#";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a; prog_line n1,n2; procedure p1,p2; if ifs; variable v; constant c; Select BOOLEAN with p1.procName = v.varName and n1 = ifs.stmt# with n1 = 5";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "assign a; prog_line n1,n2; procedure p1,p2; while w; variable v; constant c; Select BOOLEAN with p1.procName = \"Second\" and \"Third\" = v.varName with p1.procName = v.varName with w.stmt# = 5 and w.stmt# = c.value";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure pro1, pro2; Select pro1 with pro2.procName = Pear such that Calls(pro1,pro2) with pro1.procName = \"Nana\"";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);

			query = "procedure p; variable v1,v#; assign a1,a#; constant d; while w1, w2; Select		 v1	 with v1.procName = \"hi\"";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
			Assert::IsTrue(queryStatement.getInvalidQueryBoolean() == FALSE);
		}
		TEST_METHOD(testQueryTuple) {

			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;

			query = "procedure p; constant c; prog_line n; Select <p,c> with n=1";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "procedure p; constant c; prog_line n; Select  <p,	c > with n=1";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "procedure p; constant c; stmt s; prog_line n; Select < p.procName, c.value, s > with n=1";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "procedure p; constant c; stmt s; prog_line n; Select p.procName with n=1";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; procedure p; constant c; stmt s; prog_line n; variable v; Select < p.procName, c.value, s, v, v.varName > with n=1";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "call c; while w; if ifs; Select <c, ifs, w> such that Follows*(ifs, w)";
			Assert::IsTrue(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();

			query = "assign a; procedure p; constant c; stmt s; prog_line n; variable v; Select < c.procName, c.value, s, v, v.varName > with n=1";
			Assert::IsFalse(queryValidator.parseInput(query));
			queryStatement = queryValidator.getQueryStatement();
		}
		TEST_METHOD(testValidQueryIfsAndProcDeclaration) {
			QueryValidator queryValidator;
			string query;
			QueryElement selectQueryElement, suchThatQueryElement,patternQueryElement, withQueryElement;
			QueryStatement expectedQueryStatement;
			QueryStatement queryStatement;

			query = "stmt s; assign a; procedure p; while w; if i;  call c; variable v; Select BOOLEAN such that Modifies(3, \"a\") and Modifies(a, v)" 
				"and Modifies(\"x\", \"x\") and Modifies(s, v) such that Modifies(w, v) and Modifies(i, v) and  Modifies(p, v)";
			Assert::IsTrue(queryValidator.parseInput(query));
			selectQueryElement = QueryElement("empty", "empty", "BOOLEAN", "empty");
			expectedQueryStatement.addSelectQuery(selectQueryElement);
			suchThatQueryElement = QueryElement("3", "number", "empty", "a", "variable", "empty", "Modifies", "suchThat");
			expectedQueryStatement.addNormalQueryElement(suchThatQueryElement);
			suchThatQueryElement = QueryElement("a", "synonym", "assign", "v", "synonym", "variable", "Modifies", "suchThat");
			expectedQueryStatement.addNormalQueryElement(suchThatQueryElement);
			suchThatQueryElement = QueryElement("x", "variable", "empty", "x", "variable", "empty", "Modifies", "suchThat");
			expectedQueryStatement.addNormalQueryElement(suchThatQueryElement);
			suchThatQueryElement = QueryElement("s", "synonym", "stmt", "v", "synonym", "variable", "Modifies", "suchThat");
			expectedQueryStatement.addNormalQueryElement(suchThatQueryElement);
			suchThatQueryElement = QueryElement("w", "synonym", "while", "v", "synonym", "variable", "Modifies", "suchThat");
			expectedQueryStatement.addNormalQueryElement(suchThatQueryElement);
			suchThatQueryElement = QueryElement("i", "synonym", "if", "v", "synonym", "variable", "Modifies", "suchThat");
			expectedQueryStatement.addNormalQueryElement(suchThatQueryElement);
			suchThatQueryElement = QueryElement("p", "synonym", "procedure", "v", "synonym", "variable", "Modifies", "suchThat");
			expectedQueryStatement.addNormalQueryElement(suchThatQueryElement);
			queryStatement = queryValidator.getQueryStatement();

		}

		
		//This test method checks if the correct Query Elements are parse into the QueryStatement
		TEST_METHOD(testValidQueryStatement) {
			QueryValidator queryValidator;
			string query;
			QueryStatement queryStatement;
			QueryElement expectedSelectQueryElement;
			vector<QueryElement> expectedSuchThatQueryElements;
			vector<QueryElement> expectedPatternQueryElements;

			query = "variable v1,v#; assign a1,a#; constant d; while w1, w2; Select v1 such that Modifies(6,\"x\") and Parent(1, _)pattern a1(v1, \"x\")";
			queryValidator.parseInput(query);
			queryStatement = queryValidator.getQueryStatement();
			expectedSelectQueryElement = QueryElement("variable", "v1", "synonym", "empty");
		}
		//This test method assumes that the input is already grammatically correct i.e. no commas out of nowhere
		TEST_METHOD(testValidParseEntityAndSynonym) {
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

		TEST_METHOD(testValidAssignPatternRegex) {
			QueryValidator queryValidator;
			string str;

			str = "a(_, \"x+y\")";
			Assert::IsTrue(queryValidator.isAssignPatternRegex(str));

			str = "a22(\"	x	\", \"x+y\")";
			Assert::IsTrue(queryValidator.isAssignPatternRegex(str));

			str = "a#( v, \"(x+y)\")";
			Assert::IsTrue(queryValidator.isAssignPatternRegex(str));

			str = "a#(v1e	, _\"(x+y)\"_)";
			Assert::IsTrue(queryValidator.isAssignPatternRegex(str));

		}

		TEST_METHOD(testInValidAssignPatternRegex) {
			QueryValidator queryValidator;
			string str;

			str = "a(_\"x\"_, \"x+y\")";
			Assert::IsFalse(queryValidator.isAssignPatternRegex(str));

			str = "a(_\"x%\"_, \"x+y\")";
			Assert::IsFalse(queryValidator.isAssignPatternRegex(str));

			str = "a(_\"x%\"_, \".,\")";
			Assert::IsFalse(queryValidator.isAssignPatternRegex(str));
	
		}
		
		TEST_METHOD(testValidSplit) {
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
		TEST_METHOD(testValidDeclarationRegex) {
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
		TEST_METHOD(testInvalidDeclarationRegex) {
			QueryValidator queryValidator;
			string str;

			str = "while ,v;";
			Assert::IsFalse(queryValidator.isValidDeclarationRegex(str));


		}
		TEST_METHOD(testSuchThatRegex) {
			QueryValidator queryValidator;
			string str;

			str = "such that Uses(3,4)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Uses(3,s)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Parent(3,_)";
			Assert::IsTrue(queryValidator.isValidSuchThatRegex(str));

			str = "such that Parent* (3,_)";
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

			str = "such that Follows(_,_) Modifies(2,4)";
			Assert::IsFalse(queryValidator.isValidSuchThatRegex(str));

			str = "such that Follows(_,_) an Modifies(2,4)";
			Assert::IsFalse(queryValidator.isValidSuchThatRegex(str));
		}
		TEST_METHOD(testValidSuchThatRegexExtended) {
			QueryValidator queryValidator;
			string str;

			//str = "such that Uses(3,4)such that Modifies(2,4)";
			//Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			//str = "such that Modifies(\"s\",\"a\") and Follows(s,3)";
			//Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));
			//
			//str = "such that Modifies(\"s\",\"a\") and Follows(s,3) such that Uses(3,4)";
			//Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			//str = "such that Modifies(\"s\",\"a\") and Follows(s,3) such that Uses(3,4)";
			//Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			//str = "such that Modifies(\"s\",\"a\") and Follows(s,3) such that Uses(3,4) and Parent(5,7)";
			//Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			//str = "such that Modifies(\"s\",\"a\") and Follows(s,3) such that Next(8,9) such that Uses(3,4) and Parent(5,7)";
			//Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			//str = "such that Modifies(\"s\",\"a\") such that Follows(s,3) such that Next(8,9) such that Uses(3,4) and Parent(5,7)";
			//Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));

			//str = "such that Modifies(uses, \"Next\") and Follows(s,3) such that Next(6, Parent)";
			//Assert::IsTrue(queryValidator.isValidSuchThaExtendedRegex(str));
		
			str = "such that Modifies(uses, \"Next\") Follows(s,3)";
			Assert::IsFalse(queryValidator.isValidSuchThaExtendedRegex(str));
		}
		TEST_METHOD(testInValidSuchThatRegexExtended) {
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

		TEST_METHOD(testValidIfPatternRegex) {
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
		TEST_METHOD(testInValidIfPatternRegex) {
			QueryValidator queryValidator;
			string str;

			str = "ifs(@,_,_)";
			Assert::IsFalse(queryValidator.isValidIfPatternRegex(str));

			str = "ifs(_,3,4)";
			Assert::IsFalse(queryValidator.isValidIfPatternRegex(str));

			str = "f@s(_,3,4)";
			Assert::IsFalse(queryValidator.isValidIfPatternRegex(str));
		}
		TEST_METHOD(testValidIfMultipleRegex) {
			QueryValidator queryValidator;
			string str;

			str = "pattern ifs(@,_,_)";
			Assert::IsFalse(queryValidator.isValidIfPatternRegex(str));
		}
		TEST_METHOD(testValidSelectInitialRegex) {
			QueryValidator queryValidator;
			string str;

			str = "Select s";
			Assert::IsTrue(queryValidator.isValidSelectInitialRegex(str));

			str = "Select \@";
			Assert::IsFalse(queryValidator.isValidSelectInitialRegex(str));

			str = "Select <hi, b>";
			Assert::IsTrue(queryValidator.isValidSelectInitialRegex(str));

			str = "Select <hi, p.procName>";
			Assert::IsTrue(queryValidator.isValidSelectInitialRegex(str));

			str = "Select p.procName";
			Assert::IsTrue(queryValidator.isValidSelectInitialRegex(str));

			str = "Select <this>";
			Assert::IsTrue(queryValidator.isValidSelectInitialRegex(str));
		}
		TEST_METHOD(testValidWhilePattern) {
			QueryValidator queryValidator;
			string str;

			str = "w#(x,_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));

			str = "wwewew(\"x\",_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));

			str = "waaaa(\"	x	\",_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));

			str = "wddd(ddd1,_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));

			str = "w(		_		,		_)";
			Assert::IsTrue(queryValidator.isValidWhilePatternRegex(str));
		}
		TEST_METHOD(testInValidWhilePattern) {
			QueryValidator queryValidator;
			string str;

			str = "w#(__,_)";
			Assert::IsFalse(queryValidator.isValidWhilePatternRegex(str));

			str = "w#(_+_,_)";
			Assert::IsFalse(queryValidator.isValidWhilePatternRegex(str));

			str = "w#(_,2)";
			Assert::IsFalse(queryValidator.isValidWhilePatternRegex(str));
		}
		TEST_METHOD(testValidWithClause) {
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

		TEST_METHOD(testValidWithClauseExtended) {
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
		TEST_METHOD(testValidPartialPatternMatch) {
			QueryValidator queryValidator;
			string input;

			input = "and pattern a(_,_)";
			Assert::IsTrue(queryValidator.isPartialPatternRegex(input));

			input = "and          pattern a(_,_)";
			Assert::IsTrue(queryValidator.isPartialPatternRegex(input));
		}
		TEST_METHOD(testInValidPartialPatternMatch) {
			QueryValidator queryValidator;
			string input;

			input = "andpattern a(_,_)";
			Assert::IsFalse(queryValidator.isPartialPatternRegex(input));
		}
	};
}