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
			"procedure pro1, pro2; Select pro1 with pro2.procName = \"Pear\" such that Calls(pro1,pro2) with pro1.procName = \"Nana\""
		};
		vector<vector<string>> expected = {
			{"10", "2", "4"},
			{ "5" },
			{ "FALSE" },
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
			{},
			{"TRUE"},		//boolean must always return something, and it is true instead of empty 
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
			{"Nana"}

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
			"stmt s; assign a; Select a such that Modifies(a,_)pattern a(\"x\", _)" 
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
			{"1", "4"} 
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
			{ "8", "17", "27", "34", "37", "40", "1", "2", "4", "6", "9",
			"10", "12", "14", "16", "18", "23", "24", "25", "28", "29", "31",
			"32", "36", "38", "42", "43", "5", "11", "13", "21", "26",
			"30", "41", "3", "7", "15", "19", "20", "22", "33", "35", "39" }, //Returning evety stmt
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
			"stmt s; Select s such that Follows(s,3)",
			"stmt s; Select s such that Follows(3,s)",
			"stmt s; Select s such that Follows*(1,s)",
			"stmt s; Select s such that Parent(s, 6)",
			"stmt s; Select s such that Parent*(s, 6)",
			"while w; Select w such that Parent*(w, 6)",
			"assign a; Select a pattern a(_, _\"y\"_)",
			"stmt s; assign a; Select s such that Parent(s,a) pattern a(_, _\"y\"_)",
			"assign a; Select a such that Next(6,a)"
		};
		vector<vector<string>> expected = {
			{},
			{ "4" },
			{ "2" },
			{ "4" },
			{ "2", "4" },
			{ "4" },
			{ "1", "7", "16", "18" },
			{ "2", "15", "17" },
			{ "4" }
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

	};
}

