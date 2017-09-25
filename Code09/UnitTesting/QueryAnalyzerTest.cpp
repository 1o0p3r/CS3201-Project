#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryAnalyzerTest) {
public:
	TEST_METHOD(addConstant) {
		PKB pkb;
		QueryAnalyzer qa;
		pkb.setParent(1, 3);
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setParent(1, 3);

		vector<vector<string>> result;
		vector<vector<string>> hardcode;
	
		QueryElement a( "b", "synonym" , "a", "synonym", "Parent");
		qa.setPKB(pkb);
		result = qa.solveParent(a);
		hardcode = { {"1", "b"}, {"3","a" } };
		Assert::AreEqual(result[0][0] , hardcode[0][0]);
		Assert::AreEqual(result.size(), hardcode.size());
		Assert::AreEqual(result[0].size(), hardcode[0].size());

	}
	
	};
}