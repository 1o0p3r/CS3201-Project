#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryAnalyzerTest) {
public:
	TEST_METHOD(solveParent) {
		PKB pkb;
		QueryAnalyzer qa;
		pkb.setParent(1, 3);
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);

		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		
		//synSyn
		QueryElement a( "b", "synonym" ,"while", "a", "synonym","assign", "Parent");
		qa.setPKB(pkb);
		result = qa.solveParent(a);
		hardcode = { {"1","1", "b"}, {"2","3","a" } }; // to remove when pkb done, bug at line 948
		Assert::AreEqual(result[0][0] , hardcode[0][0]);
		Assert::AreEqual(result.size(), hardcode.size());
		Assert::AreEqual(result[0].size(), hardcode[0].size());

		//intint
		QueryElement intInt("1", "number", "while", "3", "number", "assign", "Parent");
		bool hasParent = qa.isParent("1","3");
		Assert::IsTrue(hasParent); 
		
		//intSyn
		QueryElement intSyn("a", "synonym", "while", "3", "number", "assign", "Parent");
		qa.setPKB(pkb);
		result = qa.solveParent(intSyn);
		hardcode = { {"1", "a" } }; 
		Assert::AreEqual(result[0][0], hardcode[0][0]);
		Assert::AreEqual(result.size(), hardcode.size());
		Assert::AreEqual(result[0].size(), hardcode[0].size()); 
		
		//intWild
		QueryElement intWild("1", "number", "while", "_", "wildcard", "assign", "Parent");
		qa.setPKB(pkb);
		hasParent = qa.hasChildOfArg1("1");
		Assert::IsTrue(hasParent); 

		QueryElement synInt("a", "synonym", "while", "2", "number", "assign", "Parent");
		qa.setPKB(pkb);
		result = qa.solveParent(synInt);
		hardcode = { { "1", "a" } };
		Assert::AreEqual(result[0][0], hardcode[0][0]);
		Assert::AreEqual(result.size(), hardcode.size());
		Assert::AreEqual(result[0].size(), hardcode[0].size());

		QueryElement synWild("a", "synonym", "while", "_", "wildcard", "assign", "Parent");
		qa.setPKB(pkb);
		result = qa.solveParent(synInt);
		hardcode = { { "1", "a" } };
		Assert::AreEqual(result[0][0], hardcode[0][0]);
		Assert::AreEqual(result.size(), hardcode.size());
		Assert::AreEqual(result[0].size(), hardcode[0].size());

		QueryElement wildint("_", "wildcard", "while", "3", "number", "assign", "Parent");
		qa.setPKB(pkb);
		hasParent = qa.hasParentForArg2("3");
		Assert::IsTrue(hasParent);

		QueryElement wildSyn("_", "wildcard", "while", "b", "synonym", "assign", "Parent");
		qa.setPKB(pkb);
		result = qa.solveParent(wildSyn);
		hardcode = { { "2","3", "b" } };
		Assert::AreEqual(result[0][0], hardcode[0][0]);
		Assert::AreEqual(result.size(), hardcode.size());
		Assert::AreEqual(result[0].size(), hardcode[0].size());

		QueryElement wildWild("_", "wildcard", "while", "_", "wildcard", "assign", "Parent");
		qa.setPKB(pkb);
		hasParent = qa.hasParentStmts();
		Assert::IsTrue(hasParent);
	}	
	
	
	};
}