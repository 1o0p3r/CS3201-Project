#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(SelectTupleTest) {
public:

	TEST_METHOD(SelectTuple) {
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		vector<string> answerkey;
		vector<string> hardcodedAnswer;

		string filename = "..\\..\\Tests09\\Sample-Source-1.txt";
		Parse(filename, pkb);

		vector<string> test2;
		vector<int> test3;
		vector<vector<string>> result;
		vector<vector<string>> hardcode;


		// assign a; procedure p; constant c; stmt s; prog_line n; variable v; Select < p.procName, c.value, s, v, v.varName > with n=1

		//same table
		QueryElement synSynUses("a", "synonym", "assign", "c", "synonym", "variable", "Uses");
		QueryElement synSynTuple("assign,stmt,variable", "a,b,c", "tuple", "synonym,synonym,synonym");
		QueryElement synSynFol("a", "synonym", "assign", "b", "synonym", "stmt", "Follows*"); //FollowsStar
		qs = QueryStatement();
		qs.addSuchThatQuery(synSynFol);
		qs.addSuchThatQuery(synSynUses);
		qs.addSelectQuery(synSynTuple);
		qa.setQS(qs);
		qa.setPKB(pkb);
		answerkey = qa.runQueryEval();
		hardcodedAnswer = { "5,6,l0l","5,6,li3m","5,6,x","5,6,y"};
		Assert::IsTrue(!answerkey.empty());
		for (int i = 0; i < hardcodedAnswer.size(); i++)
				Assert::AreEqual(hardcodedAnswer[i], answerkey[i]);

	}
	TEST_METHOD(SelectTupleDiffTable)
	{
		PKB pkb;
		QueryAnalyzer qa;
		QueryStatement qs;
		tuple<bool, vector<vector<string>>> clauseResult;
		vector<string> answerkey;
		vector<string> hardcodedAnswer;

		string filename = "..\\..\\Tests09\\Sample-Source-1.txt";
		Parse(filename, pkb);

		vector<string> test2;
		vector<int> test3;
		vector<vector<string>> result;
		vector<vector<string>> hardcode;

		QueryElement synSynUses("a", "synonym", "assign", "b", "synonym", "variable", "Uses");
		QueryElement synSynTuple("assign,variable,while,assign", "c,b,d,a", "tuple", "synonym,synonym,synonym,synonym");
		QueryElement synSynFol("c", "synonym", "assign", "d", "synonym", "while", "Follows*"); //FollowsStar
		qs = QueryStatement();
		qs.addSuchThatQuery(synSynFol);
		qs.addSuchThatQuery(synSynUses);
		qs.addSelectQuery(synSynTuple);
		qa.setQS(qs);
		qa.setPKB(pkb);
		answerkey = qa.runQueryEval();
		hardcodedAnswer = { "1,ggg,2,14","1,l0l,2,3","1,l0l,2,5","1,li3m,2,5","1,pop,2,9","1,x,2,3","1,x,2,5","1,y,2,5"};
		Assert::IsTrue(!answerkey.empty());
		for (int i = 0; i < hardcodedAnswer.size(); i++)
				Assert::AreEqual(hardcodedAnswer[i], answerkey[i]);

	}

	};
}