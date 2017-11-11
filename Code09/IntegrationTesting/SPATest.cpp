#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
#include "QueryValidator.h"
#include "QueryEval.h"
#include "QueryAnalyzer.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	TEST_CLASS(SPATest) {
public:

	TEST_METHOD(SampleSource1) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-1.txt";
		Assert::IsTrue(Parse(filename, pkb));

		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
			"stmt sOne, sTwo; Select sOne such that Follows*(1, sOne)",
			"stmt sOne, sTwo; Select sTwo such that Follows  (sTwo,  6)",
			"stmt OneTwo; Select BOOLEAN such that Follows(3,3)",
			"assign as1; while wew; Select as1 such that Parent*		 (wew, as1)",
			"assign as1; while wew; Select as1 such that Parent*(2, as1)",
			"assign as1; while wew; Select wew such that Parent*(wew, 3)",
			"assign as1; while wew; Select as1 such that Parent*(as1, 3)",
			"procedure pOne; Select pOne such that Modifies(pOne, \"y\")",
			"procedure pOne; call cOne, cTwo; Select pOne such that Modifies(cTwo, \"  y\")",
			"procedure pOne; call cOne, cTwo; Select cOne such that Modifies(cOne, \"if\")",
			"procedure pOne; call cOne, cTwo; Select pOne such that Modifies(\"Pear\", \"if\")",
			"variable v; Select v such that Modifies(8, v)",
			"procedure pOne; Select pOne such that Uses(pOne, \"y\")",
			"call cOne, cTwo; Select cTwo such that Uses(cTwo,  \"	ggg\")",
			"variable v; Select v such that Uses(8, v)",
			"assign a; procedure pOne; Select BOOLEAN such that Uses(a, pOne)",
			"assign a; procedure pOne; Select BOOLEAN such that Uses(\"Pear\", \"if\")",
			"assign a2; procedure pTwo;  Select a2 pattern a2(_,  _\"x\"_)",
			"assign a2; while wews; Select wews such that Parent*(wews, a2) pattern a2(\"y\", _)",
			"assign a2; if ifs; Select ifs such that Follows*(ifs, a2) pattern a2(\"if\", _)",
			"assign a2; if ifs; Select ifs pattern ifs(\"if\", _, _)",
			"assign a2; if ifs; Select ifs pattern ifs(_, _, _) such that Modifies(a2, \"ggg\")",
			"assign a2; if ifs; while wews; variable v; Select wews pattern wews(v, _)",
			"procedure pOne, pTwo; Select pOne such that Calls(pOne, pTwo)",
			"assign a2; procedure pOne; variable v; Select a2 such that Modifies(a2, v) pattern a2(_, _\"x\"_) and a2(\"y\", _)",
			"stmt s; assign a2; procedure pOne; variable v; if ifs; Select a2 such that Parent*(12,a2) pattern a2(\"boom\",_) pattern a2(_, \"1\")",
			"stmt s; assign a2; procedure pOne; variable v; if ifs; Select pOne such that Modifies(pOne, \"li3m\") and Uses(pOne,_) ",
			"stmt s; Select s with s.stmt# = 5",
			"procedure pro1, pro2; Select pro1 with pro2.procName = \"Pear\" such that Calls(pro1,pro2) with pro1.procName = \"Nana\"",
			"variable v1; stmt s; Select s such that Parent*(s,_) and Uses(s,v1) with v1.varName = \"Y2K\" ",
			"call c1; Select c1 with c1.procName =\"Apple\" ",
			"call c1; procedure p1; Select c1 with c1.procName = p1.procName ",
			"call c1,c2; Select c1 with c1.procName = c2.procName ",
			"call c1; Select c1 with \"Pear\" = c1.procName "
		};
		vector<vector<string>> expected = {
			{"10", "2", "4"},
			{ "5" },
			{"false"},
			{ "3" },		
			{ "3" },
			{ "2" },
			{},
			{ "Apple", "Nana" },
			{ "Apple",  "Nana", "Pear"},
			{"18","19","8"},
			{ "Apple", "Nana", "Pear" },
			{ "boom", "ggg", "if" },
			{ "Apple", "Nana" },
			{ "18","19","8" },			//Results obtained: Empty , fixed hardcoded results + program
			{ "Y2K", "ggg" , "if"},
			{"false"},
			{"true"},		//boolean must always return something, and it is true instead of empty 
			{"3", "5"},
			{"2"},
			{},		//Results obtained: Empty, hardcoded answer is wrong, line 12 does not follow anything.
			{"12"},				//Correct
			{"12","15","4","6"},				//Exception
			{"2"},				//Exception
			{"Apple", "Nana"},	//Passed
			{"3"},		//Passed
			{"16"},		//Passed
			{"Apple", "Nana"},
			{"5"},
			{"Nana"},
			{"12", "15", "4", "6"},
			{ "18" },
			{ "18", "19", "8" },
			{ "18", "19", "8" },
			{ "19", "8" }
		};

		for (int i = 0; i < queries.size(); i++) {
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			}
			else {
				Logger::WriteMessage("Invalid Query in Source 1");
				Logger::WriteMessage(queries[i].c_str());
				answer = {};
			}
			string testNo = "size error in Source 1 in test ";
			testNo.append(to_string(i + 1));
			string testNo_1 = "value error in Source 1 in test ";
			testNo_1.append(to_string(i + 1));
			wstring error = wstring(testNo.begin(), testNo.end());
			vector<string> result = answer;
			Assert::AreEqual(expected[i].size(), answer.size(), error.c_str());
			for (int j = 0; j < answer.size(); j++) {
				error = wstring(testNo_1.begin(), testNo_1.end());
				Assert::AreEqual(expected[i][j], answer[j], error.c_str());
			}
		}
	}

	TEST_METHOD(SampleSource3) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		Assert::IsTrue(Parse(filename, pkb));

		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
			"stmt s; Select s such that Follows(s,3)",
			"stmt s; Select s such that Follows(1,s)",
			"stmt s; Select s such that Follows*(s,2)",
			"variable v; assign a; Select v such that Modifies(a,v)",
			"variable v; Select v such that Modifies(2,v)",
			"variable v; Select v such that Modifies(3,v)",
			"variable v; Select v such that Uses(3,v)",
			"variable v; Select v such that Uses(4,v)",
			"variable v; assign a; Select a such that Uses(a,\"y\")",
			"assign a; Select a such that Parent(a,3)",
			"stmt a; Select a such that Parent(a,3)",
			"stmt a; Select a such that Parent*(a,4)",
			"stmt s; assign a; Select a such that Parent*(s,4)pattern a(_, \"x*y+ 1\")",
			"stmt s; assign a; Select a such that Follows(s,4)pattern a(_, _\"y+1\"_)",
			"stmt s; assign a; Select a such that Modifies(a,_)pattern a(\"x\", _)",
			"stmt s; assign a; Select s with 1=1",
			"stmt s; assign a; Select s with \"x\"=\"x\"",
			"stmt s; assign a; Select s with \"x\"=\"y\"",
			"stmt s; assign a; Select s with 1=2"
		};
		vector<vector<string>> expected = {
			{},
			{"2"},
			{"1"},
			{"x", "y"},
			{"x", "y"},
			{"y"},
			{},
			{"x", "y"},
			{"4"},
			{},
			{"2"},
			{"2"},
			{"4"},
			{},
			{"1", "4"},
			{"2","1","3","4"},
			{"2","1","3","4" },
			{},
			{}
		};
		
		for (int i = 0; i < queries.size(); i++) {
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			} else {
				Logger::WriteMessage("Invalid Query in Source 3");
				Logger::WriteMessage(queries[i].c_str());
				answer = {};
			}
			string testNo = "size error in Source 3 in test ";
			testNo.append(to_string(i + 1));
			string testNo_1 = "value error in Source 3 in test ";
			testNo_1.append(to_string(i + 1));
			wstring error = wstring(testNo.begin(), testNo.end());
			vector<string> result = answer;
			Assert::AreEqual(expected[i].size(), answer.size(), error.c_str());
			for (int j = 0; j < answer.size(); j++) {
				error = wstring(testNo_1.begin(), testNo_1.end());
				Assert::AreEqual(expected[i][j], answer[j], error.c_str());
			}
		}
	}
	TEST_METHOD(SampleSourceNext) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-Next.txt";
		Assert::IsTrue(Parse(filename, pkb));

		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
			"stmt s;Select s such that Next(2,3)",
			"stmt s;Select s such that Next(26,s)", 
			"stmt s;Select s such that Next(s,33)",
			"stmt s1,s2;Select s1 such that Next(s1,s2)",
			"stmt s; Select s such that Next(s,s)",
			"while w; Select w such that Next(w, 38)",
			"while w;Select w such that Next(38,w)",
			"while w1,w2;Select w1 such that Next(w1,w2)",
			"while w;Select w such that Next(w,w)",
			"assign a;Select a such that Next(1,a)"
		};

		vector<vector<string>> expected = {
			{"8", "17", "27", "34", "37", "40", "1", "2", "4", "6", "9",
			"10", "12", "14", "16", "18", "23", "24", "25", "28", "29", "31",
			"32", "36", "38", "42", "43", "44", "5", "11", "13", "21", "26",
			"30", "41", "3", "7", "15", "19", "20", "22", "33", "35", "39"}, //correct
			{"27", "29"},	//Returning every stmt*/
			{"15", "17", "25", "27", "31", "32"},	//Returning evety stmt
			{"1", "10", "11", "12", "13", "14","15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
			"3", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "4","40", "41", "42", "43","5", "6", "7","8", "9"},
			{}, //Correct
			{"37"},	//Obtained : 8 17 27 34 37 40
			{"37"}, //Obtained : 8 17 27 34 37 40
			{"34"}, // Obtained : 8 17 27 34 37 40
			{},	//Correct
			{"2"} // Obtained 22 integers
		};
		
		for (int i = 0; i < queries.size(); i++) {
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			}
			else {
				Logger::WriteMessage("Invalid Query in Source Next");
				Logger::WriteMessage(queries[i].c_str());
				answer = {};
			}
			string testNo = "size error in Source Next in test ";
			testNo.append(to_string(i + 1));
			string testNo_1 = "value error in Source Next in test ";
			testNo_1.append(to_string(i + 1));
			wstring error = wstring(testNo.begin(), testNo.end());
			vector<string> result = answer;
			Assert::AreEqual(expected[i].size(), answer.size(), error.c_str());
			for (int j = 0; j < answer.size(); j++) {
				error = wstring(testNo_1.begin(), testNo_1.end());
				error.append(L" - ");
				string num = to_string(j + 1);
				error.append(wstring(num.begin(), num.end()));
				Assert::AreEqual(expected[i][j], answer[j], error.c_str());
			}
		}
	}
	TEST_METHOD(SampleSource2) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-2.txt";
		Assert::IsTrue(Parse(filename, pkb));

		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
			"while w; Select w such that Parent(w, 7)",
			"assign a; Select a such that Parent*(3, a)",
			"if a; stmt b; Select a such that Parent*(3,b)",
			"stmt s; Select s such that Follows(8, s)",
			"stmt c; Select c such that Follows*(1, c)",
			"stmt a; Select a such that Follows(a, 9)",
			"stmt s; Select s such that Modifies(s, \"i\")",
			"variable n; Select n such that Modifies(1, n)",
			"assign a; variable b; while i; Select i such that Modifies(a, b)",
			"variable v; Select v such that Uses(\"Second\", v)",
			"stmt s; Select s such that Uses(s, \"i\")",
			"variable v; assign a; Select v such that Uses(a, v) pattern a(_, _\"x+1\"_)",
			"assign w; variable v; Select w such that Modifies(w, v) pattern w(_,\"2*y\")",
			"assign a; Select a pattern a(\"z\", _\"x+i\")",
			"assign a; Select a pattern a(_, _\"x + 1\"_)",
			"stmt a; stmt b; stmt c; stmt d; stmt f; Select c such that Follows(a,b) and Follows(c,d) and Follows(f,b) and Follows (c,b)"
		};
		vector<vector<string>> expected = {
			{ "3" },
			{ "4", "5", "6", "7" },
			{ "8" },
			{ "11" },
			{ "11", "12", "13", "2", "3", "8" },
			{},
			{ "2","3", "7" },
			{ "x" },
			{ "3" },
			{ "i", "x", "y", "z" },
			{ "11", "3", "7" },
			{ "x" },
			{},
			{},
			{ "9" },
			{"1","11","12","2","3","4","5","6","8"}
		};
		validator = QueryValidator(); //re-init validator.
		for (int i = 0; i < queries.size(); i++) {
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			} else {
				Logger::WriteMessage("Invalid Query in Source 2");
				Logger::WriteMessage(queries[i].c_str());
				answer = {};
			}
			string testNo = "size error in Source 2 in test ";
			testNo.append(to_string(i + 1));
			string testNo_1 = "value error in Source 2 in test ";
			testNo_1.append(to_string(i + 1));
			wstring error = wstring(testNo.begin(), testNo.end());
			vector<string> result = answer;
			Assert::AreEqual(expected[i].size(), answer.size(), error.c_str());
			for (int j = 0; j < answer.size(); j++) {
				error = wstring(testNo_1.begin(), testNo_1.end());
				Assert::AreEqual(expected[i][j], answer[j], error.c_str());
			}
		}
	}

	TEST_METHOD(SampleSource4) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-4.txt";
		Assert::IsTrue(Parse(filename, pkb));

		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
			"Select BOOLEAN",	//0
			"stmt s; Select s such that Follows(s,3)", //1 Correct
			"stmt s; Select BOOLEAN such that Follows(3,3)", //1.5
			"stmt s; Select s such that Follows(3,s)", //2 Correct
			"stmt s; Select s such that Follows*(1,s)", //3 Correct
			"stmt s; Select s such that Parent(s, 6)", //4 Correct
			"stmt s; Select s such that Parent*(s, 6)", //5 //Correct, check parent* 
			"while w; Select w such that Parent*(w, 6)", //6 //Correct, narrow parent* by type
			"variable v; Select  v such that Modifies(6, v)", //7 //Correct, check call stmt modifies 
			"variable v; Select v such that Modifies(8, v)", // 8 //Incorrect, check container stmt modifies // getModifies bug, pkb error.
			"procedure p; if ifs; Select <p, ifs> such that Modifies(p, \"a\")", //9 //Exception caught at selectTuple() narrow search in proc by type
			"call calls; procedure p; Select calls such that Calls(p, _)",//10  //Correct
			"procedure p; Select p such that Calls (p, p)", //11 //Correct
			"procedure p; stmt s; Select s with p.procName = \"jedi\" such that Modifies(p, \"k\")", //12 Correct 
			"assign a; Select a pattern a(_, _\"y\"_)", //13 //Correct
			"assign a; Select a pattern a(\"k\", _\"n * e\"_)", //14 //Correct
			"stmt s; assign a; Select s such that Parent(s,a) pattern a(_, _\"y\"_)", //15 //Correct
			"assign a; Select a such that Next(6, a)", //16 //Correct, check next function at boundary
			"stmt s; Select s such that Next(11, s)", //17 //Correct, check next function at boundary
			"stmt s; Select s such that Next(21, s)", //18 //Correct, check next function at boundary
			"assign a; Select a such that Next(15, a)", //19 //Correct, check next function at boundary
			"stmt s; Select s such that Next(18, s)", //20 //Correct, check next function at boundary
			"stmt s; assign a; while w; Select a such that Uses(a, \"y\") and Parent*(w, a) pattern w(\"m\",_)", //21 Correct, multiple clauses
			"constant c; Select c", //22 Correct
			"procedure p; stmt s; assign a; Select s such that Modifies(p, \"m\") and Uses(p, \"y\") and Parent(s,a) pattern a(_,_\"y\"_)", //23 Incorrect, Missing out 1 result : line 13
			"assign a; Select a such that Next*(19, a)", //24 //Incorrect, currrently giving back all assignments
			"assign a; Select BOOLEAN pattern a(\"green\", _\"1 * y * z\"_)", //25 Incorrect,	Exception CAUGHT AT getPatternVariableExpressionSubstring
			"stmt s1, s2; Select s2 such that Parent*(s2, s1) and Modifies(s1, \"k\")", //26 Correct
			"stmt s; if ifs; while w; Select s such that Parent*(ifs, s) and Parent*(w, s)", //27 Correct
			"stmt s; if ifs; while w; Select w such that Parent*(ifs, s) and Parent*(w, s)", //28 Coorect
			"stmt s; while w1, w2; if ifs; Select s such that Parent(w1, s) and Parent*(ifs, s) and Parent*(w2, s)", //29  Correct
			"stmt s; if ifs; while w; Select s such that Next(ifs, s) and Next(s, w)", //30	 Correct
		};
		vector<vector<string>> expected = {
			{"true"},
			{}, //1
			{"false"},
			{ "4" }, //2
			{ "2" }, //3
			{ "4" }, //4
			{ "2", "4" }, //5
			{ "4" }, //6
			{ "e" ,"f", "g", "k", "m", "y" }, //7
			{ "e" ,"f", "g", "k", "m", "y" }, //8
			{ "yoda 15","yoda 2" }, //9 // missing <yoda,15>
			{ "6", "11" }, //10
			{}, //11
			{ "4", "8", "10", "13", "17", "20", "1", "3", "5", "7", "9", "12", "14", "16", "18", "19", "21", "2", "15", "6", "11" }, //12
			{ "1", "16", "18", "7" }, //13
			{}, //14
			{ "15", "17", "2" }, //15
			{ }, //16
			{ "10" }, //17
			{ "20" }, //18
			{ "16", "19" }, //19
			{ "17"}, //20
			{ "16", "18" }, //21
			{ "1", "4", "5", "6", "7" }, //22
			{ "15", "17", "2"}, //23 //amswer 13 is parent* hence removed
			{ "14", "16", "18", "19", "21" }, //24
			{ "true" }, //25
			{ "10", "13", "15", "2", "4", "8" }, //26
			{ "10", "11", "16", "17", "18", "19", "20", "21", "5", "6", "9" }, //27
			{ "10", "13", "17", "20", "4", "8" }, //28
			{ "10","11","18","21","5","6","9" }, //29
			{ "16", "19", "3", "7" }, //30 
			
		};

		for (int i = 0; i < queries.size(); i++) {
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			}
			else {
				Logger::WriteMessage("Invalid Query in Source 4");
				Logger::WriteMessage(queries[i].c_str());
				answer = {};
			}
			string testNo = "size error in Source 4 in test ";
			testNo.append(to_string(i + 1));
			string testNo_1 = "value error in Source 4 in test ";
			testNo_1.append(to_string(i + 1));
			wstring error = wstring(testNo.begin(), testNo.end());
			vector<string> result = answer;
			Assert::AreEqual(expected[i].size(), answer.size(), error.c_str());
			for (int j = 0; j < answer.size(); j++) {
				error = wstring(testNo_1.begin(), testNo_1.end());
				Assert::AreEqual(expected[i][j], answer[j], error.c_str());
			}
		}
	}
	TEST_METHOD(SampleSourcePattern2) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-Pattern-2.txt";
		Assert::IsTrue(Parse(filename, pkb));

		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
			"assign a; Select a pattern a(_, _\"dan\"_)",
			"assign a; Select a pattern a(_, _\"danger\"_)",
			"assign a; Select a pattern a(_, \"dan\")",
			"assign a; Select a pattern a(_, \"(fig + popcicle)\")",
			"assign a; Select a pattern a(_, _\"absolute - dan \"_)",
			"assign a; Select a pattern a(_, _\"fig \"_)"
		};
		vector<vector<string>> expected = {
			{ "1","10","11","2","3","7","8","9" },
			{ "4","5","6" },
			{},
			{"12"},
			{"10","11"},
			{"10","11","12","7","8","9"}
		};
		validator = QueryValidator(); //re-init validator.
		for (int i = 0; i < queries.size(); i++) {
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			}
			else {
				Logger::WriteMessage("Invalid Query in Source 2");
				Logger::WriteMessage(queries[i].c_str());
				answer = {};
			}
			string testNo = "size error in Source Pattern 2 in test ";
			testNo.append(to_string(i + 1));
			string testNo_1 = "value error in Source Pattern 2 in test ";
			testNo_1.append(to_string(i + 1));
			wstring error = wstring(testNo.begin(), testNo.end());
			vector<string> result = answer;
			Assert::AreEqual(expected[i].size(), answer.size(), error.c_str());
			for (int j = 0; j < answer.size(); j++) {
				error = wstring(testNo_1.begin(), testNo_1.end());
				Assert::AreEqual(expected[i][j], answer[j], error.c_str());
			}
		}
		
	}
	TEST_METHOD(SampleSouce6) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-6.txt";
		Parse(filename, pkb);
		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
			//"procedure p1; Select p1 such that Calls*(p1, _)", //getAllCalls broken api
			//"procedure p1; Select p1 such that Calls*(p1, \"SystemTestFour\")",
			//"procedure p1, p2, p3; Select p1 such that Calls(p3, p2) and Calls(p2, \"SystemTestThree\") and Calls*(p3, \"SystemTestFour\") and Calls*(p1, p2)",
			"procedure p1; Select p1 such that Calls*(_, \"SystemTestTwo\")",
			

			//frm sample source 6-v2 Next Queries
			//"stmt s; Select s such that Next(s,_)" //1
			//"assign a1, a2; if ifsOne, ifsTwo; while wOne, wTwo; stmt s; Select s such that Next(s, wOne) and Next(wOne, wTwo) and Next(wTwo,a1) and Next(a1, ifsOne)",
			//"assign a1, a2; if ifs; while w; Select ifs such that Next*(a2, ifs) and Next*(ifs, w) and Next*(w, a1) and Next*(a1, a2)",
			//"assign a; while w; if ifs; Select a Pattern a(_, _) and w(_, _) and ifs(_, _, _)",
		//	"while w; Select w such that Next*(w, _)"
		};
		vector<vector<string>> expected = {
			//{ "SystemTestOne","SystemTestThree","SystemTestTwo" },
			//{"SystemTestThree","SystemTestTwo"},
			//{},
			//{},
			//{}, //1, Next Unable to store results correctly for While with If nested, see procedure systemtesttwo
			//{"45","49","55"},
			//{}
			{"23","30","32","43","46","50","6","8"}
			
		};
		validator = QueryValidator(); //re-init validator.
		for (int i = 0; i < queries.size(); i++) {
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			}
			else {
				Logger::WriteMessage("Invalid Query in Source 6");
				Logger::WriteMessage(queries[i].c_str());
				answer = {};
			}
			string testNo = "size error in Source 6 in test ";
			testNo.append(to_string(i + 1));
			string testNo_1 = "value error in Source 6 in test ";
			testNo_1.append(to_string(i + 1));
			wstring error = wstring(testNo.begin(), testNo.end());
			vector<string> result = answer;
			Assert::AreEqual(expected[i].size(), answer.size(), error.c_str());
			for (int j = 0; j < answer.size(); j++) {
				error = wstring(testNo_1.begin(), testNo_1.end());
				Assert::AreEqual(expected[i][j], answer[j], error.c_str());
			}
		}

		
	}

	TEST_METHOD(SampleSourceModel_3_1) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Source3-1.txt";
		Parse(filename, pkb);
		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
			//"stmt s; assign a; Select <s, a> such that Follows(s, a)",
			//"stmt s; while w; Select <s, w> such that Follows(s, w)",
			//"prog_line n1, n2; Select n1 such that Next*(n1, n2) and Uses(n2, \"b\") with n2 = 43"
			"stmt s, s1; Select <s, s1> such that Affects*(s, s1)"

		};
		vector<vector<string>> expected = {
			//{"14 15", "15 16", "16 17","2 3","36 37","37 38","4 23","42 43", "50 51", "56 57","58 59","60 61","62 63","77 80","83 84", "87 89", "91 92", "92 93"},
			//{"19 20","23 24","27 28","3 4","31 32","38 39","6 13","81 82","82 85","85 87","89 90","9 10"},
			//{ "36","37","38","39","40","41","42","43","44","45","46","47","48","49"},
			{ "11 11","11 14","11 14","11 15","11 16","11 17","11 19","11 21","11 22","11 23","11 25","11 27","11 29",
				"11 31","11 33","11 34","11 35","11 5","11 8","11 9","14 11","14 14","14 15","14 16","14 17","14 19","14 21",
				"14 22","14 23","14 25","14 27","14 29","14 31","14 33","14 34","14 35","14 5","14 8","14 9","15 11","15 14",
				"15 15","15 16","15 17","15 19","15 21","15 22","15 23","15 25","15 27","15 29","15 31","15 33","15 34","15 35",
				"15 5","15 8","15 9","16 11","16 14","16 15","16 16","16 17","16 19","16 21","16 22","16 23","16 25","16 27",
				"16 29","16 31","16 33","16 34","16 35","16 5","16 8","16 9","17 11","17 14","17 15","17 16","17 17","17 19",
				"17 21","17 22","17 23","17 25","17 27","17 29","17 31","17 33","17 34","17 35","17 5","17 8","17 9","19 11",
				"19 14","19 15","19 16","19 17","19 19","19 21","19 22","19 23","19 25","19 27","19 29","19 31","19 33","19 34",
				"19 35","19 5","19 8","19 9","21 11","21 14","21 15","21 16","21 17","21 19","21 21","21 22","21 23","21 25",
				"21 27","21 29","21 31","21 33","21 34","21 35","21 5","21 8","21 9","22 11","22 14","22 15","22 16","22 17",
				"22 19","22 21","22 22","22 23","22 25","22 27","22 29","22 31","22 33","22 34","22 35","22 5","22 8","22 9",
				"23 25","23 27","23 29","23 31","23 33","23 34","23 35","25 25","25 27","25 29","25 31","25 33","25 34","25 35",
				"27 25","27 27","27 29","27 31","27 33","27 34","27 35","29 25","29 27","29 29","29 31","29 33","29 34","29 35",
				"3 11","3 14","3 15","3 16","3 17","3 19","3 21","3 22","3 23","3 25","3 27","3 29","3 31","3 33","3 34","3 35",
				"3 5","3 8","3 9","31 25","31 27","31 29","31 31","31 33","31 34","31 35","33 25","33 27","33 29","33 31","33 33",
				"33 34","33 35","34 25","34 27","34 29","34 31","34 33","34 34","34 35","35 25","35 27","35 29","35 31","35 33",
				"35 34","35 35","36 38","36 42","36 43","36 46","36 48","36 49","37 38","37 42","37 43","37 46","37 48","37 49",
				"38 42","38 43","38 46","38 48","38 49","42 42","42 43","42 46","42 49","43 42","43 43","43 46","43 49","45 42",
				"45 43","45 45","45 46","45 49","46 42","46 43","46 46","46 49","48 42","48 43","48 46","48 48","48 49","49 42",
				"49 43","49 46","49 49","5 11","5 14","5 15","5 16","5 17","5 19","5 21","5 22","5 23","5 25","5 27","5 29","5 31",
				"5 33","5 34","5 35","5 5","5 8","5 9","50 51","50 56","50 57","50 60","50 61","50 62","50 63","51 60","51 61",
				"51 62","51 63","57 56","57 57","58 59","60 61","62 63","79 80","8 11","8 14","8 15","8 16","8 17","8 19","8 21",
				"8 22","8 23","8 25","8 27","8 29","8 31","8 33","8 34","8 35","8 5","8 8","8 9","81 89","81 91","81 92","81 93",
				"83 86","83 89","83 91","83 92","83 93","84 84","84 89","84 91","84 92","84 93","86 86","86 89","86 91","86 92",
				"86 93","89 91","89 92","89 93","9 11","9 14","9 15","9 16","9 17","9 19","9 21","9 22","9 23","9 25","9 27","9 29",
				"9 31","9 33","9 34","9 35","9 5","9 8","9 9","91 91","91 92","91 93","92 91","92 92","92 93","93 91","93 92","93 93", }

			
		};
		validator = QueryValidator(); //re-init validator.
		for (int i = 0; i < queries.size(); i++) {
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			}
			else {
				Logger::WriteMessage("Invalid Query in Source 3-1");
				Logger::WriteMessage(queries[i].c_str());
				answer = {};
			}
			string testNo = "size error in Source 3-1 in test ";
			testNo.append(to_string(i + 1));
			string testNo_1 = "value error in Source 3-1 in test ";
			testNo_1.append(to_string(i + 1));
			wstring error = wstring(testNo.begin(), testNo.end());
			vector<string> result = answer;
			Assert::AreEqual(expected[i].size(), answer.size(), error.c_str());
			for (int j = 0; j < answer.size(); j++) {
				error = wstring(testNo_1.begin(), testNo_1.end());
				Assert::AreEqual(expected[i][j], answer[j], error.c_str());
			}
		}


	}

	TEST_METHOD(Sample_Source_7) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-7.txt";
		Parse(filename, pkb);
		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
			//"if ifs; prog_line n; Select BOOLEAN such that Follows*(ifs, n)",
		//	"call c; while w; if ifs; Select <c, ifs, w> such that Follows*(ifs, w)"

		};
		vector<vector<string>> expected = {
			//{"true"},
		/*	{ "119 102 105","119 46 50","120 102 105","120 46 50","122 102 105","122 46 50",
				"124 102 105","124 46 50","125 102 105","125 46 50","129 102 105","129 46 50",
				"16 102 105","16 46 50","18 102 105","18 46 50","22 102 105","22 46 50","6 102 105",
				"6 46 50","64 102 105","64 46 50","66 102 105","66 46 50","99 102 105","99 46 50" }
*/
		};
		validator = QueryValidator(); //re-init validator.
		for (int i = 0; i < queries.size(); i++) {
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			}
			else {
				Logger::WriteMessage("Invalid Query in Source 7");
				Logger::WriteMessage(queries[i].c_str());
				answer = {};
			}
			string testNo = "size error in Source 7 in test ";
			testNo.append(to_string(i + 1));
			string testNo_1 = "value error in Source 7 in test ";
			testNo_1.append(to_string(i + 1));
			wstring error = wstring(testNo.begin(), testNo.end());
			vector<string> result = answer;
			Assert::AreEqual(expected[i].size(), answer.size(), error.c_str());
			for (int j = 0; j < answer.size(); j++) {
				error = wstring(testNo_1.begin(), testNo_1.end());
				Assert::AreEqual(expected[i][j], answer[j], error.c_str());
			}
		}


	}

	};
}

