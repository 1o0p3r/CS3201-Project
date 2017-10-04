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

	TEST_METHOD(IntegrationTesting) {
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-3.txt";
		Assert::IsTrue(Parse(filename, pkb));

		QueryValidator validator;
		QueryStatement statement;
		QueryAnalyzer analyzer;
		analyzer.setPKB(pkb);
		vector<string> answer;
		vector<string> queries = {
		/*	"stmt s; Select s such that Follows(s,3)",  
			"stmt s; Select s such that Follows(1,s)",  
			"stmt s; Select s such that Follows*(s,2)", 
			"variable v; assign a; Select v such that Modifies(a,v)", 
			"variable v; Select v such that Modifies(2,v)", 
			"variable v; Select v such that Modifies(3,v)", 
			"variable v; Select v such that Uses(3,v)",     
			"variable v; Select v such that Uses(4,v",                 
			"variable v; assign a; Select a such that Uses(a,\"y\")",  
			"assign a; Select a such that Parent(a,3)",      
			"stmt a; Select a such that Parent(a,3)",
			"stmt a; Select a such that Parent*(a,4)", */
			"stmt s; assign a; Select a such that Parent*(s,4）pattern a(_, \"x*y+ 1\")",
		/*	"stmt s; assign a; Select a such that Follows(s,4）pattern a(_, _\"y+1\")",
			"stmt s; assign a; Select a such that Modifies(a,_）pattern a(\"x\", _)" */
		};
		vector<vector<string>> expected = {
		/*	{},
			{"2"},
			{"1"},
			{"x", "y"},
			{"x", "y"},
			{"y"},
			{},
			{}, 
			{"4"}, 
			{}, 
			{"2"},
			{"2"}, */
			{"4"},
		/*	{},
			{"1", "4"} */
		};

		for (int i = 0; i < queries.size(); i++) {
			string testNo = "In test ";
			testNo.append(to_string(i + 1));
			Logger::WriteMessage(testNo.c_str());
			validator = QueryValidator(); //re-init validator.
			if (validator.parseInput(queries[i])) {
				statement = validator.getQueryStatement();
				analyzer.setQS(statement);
				answer = analyzer.runQueryEval();
			} else {
				Logger::WriteMessage("Invalid Query");
				answer = {};
			}
			Assert::AreEqual(answer.size(), expected[i].size());
			for (int j = 0; j < answer.size(); j++) {
				Assert::AreEqual(answer[j], expected[i][j]);
			}
		}
	}
	};
}

