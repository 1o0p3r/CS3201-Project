#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(PatternAnalyzerTest) {
public:
	TEST_METHOD(PatternIf) {
		
		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-Pattern.txt";
		Assert::IsTrue(Parse(filename, pkb));


		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		vector<vector<string>> pkbHardCode;
		vector<vector<string>> hardCodeResult;

		//"if ifs; Select ifs pattern ifs("a", _)"
		QueryElement ifPat("a", "_", "_", "if", "ifs", "variable", "", "", "", "pattern");
		qa.setPKB(pkb);
		qs.addNormalQueryElement(ifPat);
		qa.setQS(qs);
		qa.findQueryElements();
		qa.solvePatternClause(ifPat);
		auto answer = qa.getMergedQueryTable();
		hardCodeResult = { {"6","18","ifs"} };
		for ( int k =0; k<answer.size();k++)
			for( int i =0; i < answer[k].size();i++)
				for(int j=0; j< answer[k][i].size(); j++)
					Assert::AreEqual(answer[0][i][j], hardCodeResult[i][j]);

	}
	TEST_METHOD(PatternWhile) {

		PKB pkb;
		string filename = "..\\..\\Tests09\\Sample-Source-Pattern.txt";
		Assert::IsTrue(Parse(filename, pkb));


		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		vector<vector<string>> pkbHardCode;
		vector<vector<string>> hardCodeResult;

		//"if ifs; Select ifs pattern ifs("a", _)"
		QueryElement whilePat("b", "_", "_", "while", "whiles", "variable", "", "", "", "pattern");
		qa.setPKB(pkb);
		qs.addNormalQueryElement(whilePat);
		qa.setQS(qs);
		qa.findQueryElements();
		qa.solvePatternClause(whilePat);
		auto answer = qa.getMergedQueryTable();
		hardCodeResult = { { "20","24","whiles" } };
		for (int k = 0; k<answer.size(); k++)
			for (int i = 0; i < answer[k].size(); i++)
				for (int j = 0; j< answer[k][i].size(); j++)
					Assert::AreEqual(answer[0][i][j], hardCodeResult[i][j]);

	}
		//line ends
	};
}