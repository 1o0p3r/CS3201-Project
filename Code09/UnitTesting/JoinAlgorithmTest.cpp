#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(JoinAlgorithmTest) {
public:
	TEST_METHOD(twoSynonymSameTable) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);
		pkb.setFollows(2, 3);
		qa.setPKB(pkb);
		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		vector<vector<vector<string>>> database;
		//intint
		
		vector<vector<string>> a{ {"1","2","3","a"}, {"7","8","9","b"} };
		vector<vector<string>> b{ {"2","3","a"}, {"8","9","b"} };
		qa.insertClauseResults(a);
		qa.restrictTableValues(a, 0, 1, 0, b);
		hardcode = { { "2","3","a" },{ "8","9","b" } };
		database = qa.getMergedQueryTable();
		for (int i = 0; i < database[0].size(); i++)
			for (int j = 0; j < database[0][i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], database[0][i][j]);
			}
	}

	TEST_METHOD(twoSynonymDiffTable) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);
		pkb.setFollows(2, 3);
		qa.setPKB(pkb);
		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		vector<vector<vector<string>>> database;
		//intint
		vector<vector<string>> a{ { "1","2","3","4","a" },{ "7","8","9","10","b" } };
		vector<vector<string>> b{ { "1","2","3","c" },{ "7","8","9","b" } };
		vector<vector<string>> c{ { "3","4","c"} , {"11","12","d" } };
		// 1 2 3 , 7 8 9 ,1 2 3 
		
		vector<vector<string>> intermediate;
		qa.insertClauseResults(a);
		qa.insertClauseResults(b);
		qa.insertClauseResults(c);
		hardcode = { { "3","a" },{ "9","b" },{"3","c"},{"11","d"} };
		database = qa.getMergedQueryTable();
		for (int i = 0; i < database[0].size(); i++)
			for (int j = 0; j < database[0][i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], database[0][i][j]);
			}
	}

	TEST_METHOD(arg1Common1ArgOnly) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);
		pkb.setFollows(2, 3);
		qa.setPKB(pkb);
		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		vector<vector<vector<string>>> database;
		//intint
		vector<vector<string>> a{ { "1","2","3","4","a" },{ "b","a","d","c","b" } };
		vector<vector<string>> b{ { "c","b","x","b" } };
		// 1 4 , b c 

		vector<vector<string>> intermediate;
		qa.insertClauseResults(a);
		qa.insertClauseResults(b);
		hardcode = { { "1","4","a" },{ "b","c","b" } };
		database = qa.getMergedQueryTable();
		for (int i = 0; i < database[0].size(); i++)
			for (int j = 0; j < database[0][i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], database[0][i][j]);
			}
	}

	TEST_METHOD(arg1Common2Args) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);
		pkb.setFollows(2, 3);
		qa.setPKB(pkb);
		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		vector<vector<vector<string>>> database;
		//intint
		vector<vector<string>> a{ { "1","2","3","4","a" },{ "b","a","d","c","b" } };
		vector<vector<string>> b{ { "c","b","a","b" },{"hello","world","how","you" } };
		// 1  2 4 ,b a c , world how hello

		vector<vector<string>> intermediate;
		qa.insertClauseResults(a);
		qa.insertClauseResults(b);
		hardcode = { { "1","2","4","a" },{ "b","a","c","b" },{"world","how","hello","you"} };
		database = qa.getMergedQueryTable();
		for (int i = 0; i < database[0].size(); i++)
			for (int j = 0; j < database[0][i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], database[0][i][j]);
			}
	}

	TEST_METHOD(arg2Common2Args) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);
		pkb.setFollows(2, 3);
		qa.setPKB(pkb);
		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		vector<vector<vector<string>>> database;
		//intint
		vector<vector<string>> a{ { "1","2","3","4","a" },{ "b","a","d","c","b" } };
		vector<vector<string>> b{ { "c","b","a","e" },{ "1","16","4","a" } };
		// 1  2 4 ,b a c , world how hello

		vector<vector<string>> intermediate;
		qa.insertClauseResults(a);
		qa.insertClauseResults(b);
		hardcode = { { "1","4","a" },{ "b","c","b" },{ "c","a","e" } };
		database = qa.getMergedQueryTable();
		for (int i = 0; i < database[0].size(); i++)
			for (int j = 0; j < database[0][i].size(); j++) {
				Assert::AreEqual(hardcode[i][j], database[0][i][j]);
			}
	}


	};
}