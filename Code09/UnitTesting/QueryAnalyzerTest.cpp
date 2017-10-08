#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryAnalyzer.h"
#include "Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryAnalyzerTest) {
public:
	TEST_METHOD(solveParent) {
		PKB pkb;
		QueryAnalyzer qa;
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
	TEST_METHOD(solveUses) {
		PKB pkb;
		QueryAnalyzer qa;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setUses(1, "x");
		pkb.setUses(2, "y");
		pkb.setUses(2, "c");
		pkb.setUses(1, "y");
		pkb.setUses(1, "c");
		
		pkb.setUses(3, "c");
		pkb.setUses(3, "x");
		

		vector<vector<string>> result;
		vector<vector<string>> hardcode;
		bool hasUses;
		/*
		integer, literalstring = 0
			integer, synonym = 1
			integer, wildcard = 2
			synonym, literalstring = 3
			synonym, synonym = 4
			synonym, wildcard = 5
			wildcard, literalstring = 6
			wildcard, synonym = 7
			wildcard, wildcard = 8
			*/
		
		QueryElement intString("1", "number", "while", "x", "variable", "assign", "Uses");
		qa.setPKB(pkb);
		hasUses = qa.validateUses("1", "x",0); //0 = intString
		Assert::IsTrue(hasUses);

		QueryElement intSyn("2", "number", "while", "k", "synonym", "assign", "Uses");
		qa.setPKB(pkb);
		result = qa.solveUsesStmt(intSyn);
		hardcode = { {"y","c", "k" } };
		for (int j = 0; j < result[0].size(); j++) {
			Assert::AreEqual(result[0][j], hardcode[0][j]);
		}

		QueryElement intWild("1", "number", "while", "_", "wildcard", "assign", "Uses");
		qa.setPKB(pkb);
		hasUses = qa.validateUses("1", "wildcard", 3); //3 = intWild
		Assert::IsTrue(hasUses);
		
		//synsyn, getUsedBy(Var) use this order eg (1,c) (2,c) etc
		QueryElement synSyn("a", "synonym", "while", "b", "synonym", "assign", "Uses");
		qa.setPKB(pkb);
		result = qa.solveUsesStmt(synSyn);
		hardcode = { { "1","2","3","1","3","1","2", "a" },{ "c","c","c","x","x","y","y","b" } }; // to remove when pkb done, bug at line 948
		for (int i = 0; i < result.size(); i++) {
			for (int j = 0; j < result[i].size(); j++) {
				Assert::AreEqual(result[i][j], hardcode[i][j]);
			}
		}
		Assert::AreEqual(result.size(), hardcode.size());
		Assert::AreEqual(result[0].size(), hardcode[0].size());
		
		QueryElement synString("a", "synonym", "while", "c", "variable", "assign", "Uses");
		qa.setPKB(pkb);
		result = qa.solveUsesStmt(synString);
		hardcode = { { "1","2","3", "a" } };
		for (int j = 0; j < result[0].size(); j++) {
			Assert::AreEqual(result[0][j], hardcode[0][j]);
		}

		QueryElement synWild("a", "synonym", "while", "_", "wildcard", "assign", "Uses");
		qa.setPKB(pkb);
		result = qa.solveUsesStmt(synWild);
		hardcode = { { "1","2","3", "a" } };
		for (int j = 0; j < result[0].size(); j++) {
			Assert::AreEqual(result[0][j], hardcode[0][j]);
		}

		QueryElement wildString("_", "wildcard", "while", "x", "string", "assign", "Uses");
		qa.setPKB(pkb);
		hasUses = qa.validateUses("wildcard", "x", 6); //6 = wildString
		Assert::IsTrue(hasUses);

		QueryElement wildSyn("_", "wildcard", "while", "a", "synonym", "assign", "Uses");
		qa.setPKB(pkb);
		result = qa.solveUsesStmt(wildSyn);
		hardcode = { { "y","c","x", "a" } };
		for (int j = 0; j < result[0].size(); j++) {
			Assert::AreEqual(result[0][j], hardcode[0][j]);
		}

		QueryElement wildWild("_", "wildcard", "while", "_", "wildcard", "assign", "Uses");
		qa.setPKB(pkb);
		hasUses = qa.validateUses("wildcard", "wildcard", 8); //8 = wildwild
		Assert::IsTrue(hasUses);
	}
	TEST_METHOD(solvePattern) {
		PKB pkb;
		QueryAnalyzer qa;
		/* while x {
			z = 8+y*c-3;
			s = 4*c-x;
		   }
		*/

		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setUses(1, "x");
		pkb.setUses(2, "y");
		pkb.setUses(2, "c");
		pkb.setUses(1, "y");
		pkb.setUses(1, "c");
		pkb.setUses(3, "c");
		pkb.setUses(3, "x");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);
		pkb.setModifies(3, "x");
		pkb.setModifies(2, "z");
		pkb.addPattern(3, "x", "4*c-x");
		pkb.addPattern(2,"z",Util::insertBrackets(" 8+ y*c - 3"));
		vector<vector<string>> clauseResult;
		vector<string> hardcode;
		vector<vector<vector<string>>> mergedResult;
		QueryStatement qs;
		vector<string> answer;
		//* implies selected
		QueryElement synSyn("a", "synonym", "while", "b", "synonym", "assign", "Uses");
		// { { "1", "2", "3", "1", "3", "1", "2", "a" }, { "c","c","c","x","x","y","y","b" } }
		QueryElement a("d", "synonym", "while", "a", "synonym", "assign", "Parent");
		// { { "1","1", "a" },{ "2","3","d" } };
		QueryElement sel("assign", "a", "synonym");
		
		QueryElement pat1("v", Util::insertBrackets("4*c-x"), "assign", "a", "synonym", "exact");
		// pattern a(v,"4*c-x")
		//	QueryElement(string arg1, string arg2, string patternEntity, 
		//		string patternSynonym, string patternArg1Type, string patternArg2Type); //Pattern

		qa.setPKB(pkb);

		qs.addSuchThatQuery(synSyn);
		qs.addSuchThatQuery(a);
		qs.addSelectQuery(sel);
		qs.addPatternQuery(pat1);
		qa.setQS(qs);
		answer = qa.runQueryEval();
	/*	qa.findQueryElements();
		mergedResult = qa.solveSTClause();
		qa.solvePatternClause();
		answer = qa.analyzeClauseResults(); */
		
		hardcode = { "3" };
		Assert::AreEqual(hardcode.size(), answer.size());
		Assert::AreEqual(hardcode[0], answer[0]);
	}
	TEST_METHOD(solveClauses) {
		PKB pkb;
		QueryAnalyzer qa;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setUses(1, "x");
		pkb.setUses(2, "y");
		pkb.setUses(2, "c");
		pkb.setUses(1, "y");
		pkb.setUses(1, "c");
		pkb.setUses(3, "c");
		pkb.setUses(3, "x");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);
		vector<vector<string>> clauseResult;
		vector<vector<vector<string>>> hardcode;
		vector<vector<vector<string>>> mergedResult;
		QueryStatement qs;

		qa.setPKB(pkb);


		//* implies selected
		QueryElement synSyn("a", "synonym", "while", "b", "synonym", "assign", "Uses");
		// { { "1", "2", "3", "1", "3", "1", "2", "a" }, { "c","c","c","x","x","y","y","b" } }
		QueryElement a("a", "synonym", "while", "d", "synonym", "assign", "Parent");
		// { { "1","1", "a" },{ "2","3","d" } };
		/*
		qs.addSuchThatQuery(synSyn);
		qs.addSuchThatQuery(a);
		qa.setQS(qs);
		qa.findQueryElements();
		mergedResult = qa.solveSTClause();
		// a, b, c
		hardcode = { { { "1", "1", "1", "1", "1", "1" },{ "c", "c", "x", "x", "y", "y" },
						{"2", "3", "2", "3", "2", "3"} } };
		for (int i = 0; i < mergedResult.size(); i++) {
			for (int j = 0; j < mergedResult.size(); j++) {
				for (int k = 0; k < mergedResult.size(); k++) {
					Assert::AreEqual(mergedResult[i][j][k], hardcode[i][j][k]);
				}
			}
		}
		Assert::AreEqual(hardcode.size(), mergedResult.size());
		*/
		//QueryElement synSyn("a", "synonym", "while", "b", "synonym", "assign", "Uses");
		// { { "1", "2", "3", "1", "3", "1", "2", "a" }, { "c","c","c","x","x","y","y","b" } }
		QueryElement b("d", "synonym", "while", "a", "synonym", "assign", "Parent");
		// { { "1","1", "a" },{ "2","3","d" } };

		qa = QueryAnalyzer();
		qa.setPKB(pkb);
		qs.addSuchThatQuery(synSyn);
		qs.addSuchThatQuery(b);
		qa.setQS(qs);
		qa.findQueryElements();
		mergedResult = qa.solveSTClause();

		hardcode = { {{ "2","3","3","2" }, { "c", "c", "x", "y" }, { "1","1","1","1" } }};
		for (int i = 0; i < mergedResult.size(); i++) {
			for (int j = 0; j < mergedResult.size(); j++) {
				for (int k = 0; k < mergedResult.size(); k++) {
					Assert::AreEqual(mergedResult[i][j][k], hardcode[i][j][k]);
				}
			}
		}

	}
	TEST_METHOD(solveSelectUses) {
		PKB pkb;
		QueryAnalyzer qa;
		pkb.setStatementType(1, "while");
		pkb.setStatementType(2, "assign");
		pkb.setStatementType(3, "assign");
		pkb.setUses(1, "x");
		pkb.setUses(2, "y");
		pkb.setUses(2, "c");
		pkb.setUses(1, "y");
		pkb.setUses(1, "c");
		pkb.setUses(3, "c");
		pkb.setUses(3, "x");
		pkb.setParent(1, 3);
		pkb.setParent(1, 2);

		vector<vector<string>> clauseResult;
		vector<string> hardcode;
		vector<vector<vector<string>>> mergedResult;
		QueryStatement qs;
		vector<string> answer;
		//* implies selected
		QueryElement synSyn("a", "synonym", "while", "b", "synonym", "assign", "Uses");
		// { { "1", "2", "3", "1", "3", "1", "2", "a" }, { "c","c","c","x","x","y","y","b" } }
		QueryElement a("a", "synonym", "while", "d", "synonym", "assign", "Parent");
		// { { "1","1", "a" },{ "2","3","d" } };
		QueryElement sel("while", "a", "synonym");

		qa.setPKB(pkb);

		qs.addSuchThatQuery(synSyn);
		qs.addSuchThatQuery(a);
		qs.addSelectQuery(sel);
		qa.setQS(qs);
		qa.findQueryElements();
		mergedResult = qa.solveSTClause();
		answer = qa.analyzeClauseResults();
		hardcode = { "1" };
		Assert::AreEqual(hardcode[0], answer[0]);


	}

	};
}