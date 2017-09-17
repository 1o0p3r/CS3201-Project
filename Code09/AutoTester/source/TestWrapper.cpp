#include "TestWrapper.h"
#include "Parser.h"
#include "PKB.h"
#include "QueryValidator.h"
#include "QueryEval.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
	if (wrapper == 0) wrapper = new TestWrapper;
	return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
	// create any objects here as instance variables of this class
	// as well as any initialization required for your spa program
	PKB pkb;
	QueryValidator q;
	QueryEval qe;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
	// ...rest of your code...
	/*
	Simple Code 1

	0 procedure Main {
	1 x = 5;
	2 while x {
	3 	y = 2;
	4 	x = x*y+1;}}
	*/
	PKB pkb;

	//line 1
	pkb.setStatementType(1, "assign");
	pkb.setModifies(1, "x");
	pkb.setProcModifies("Main", "x");
	pkb.addConstant("5");
	pkb.addPattern(1, "x", "5");
	//line 2
	pkb.setFollows(1, 2);
	pkb.setStatementType(2, "while");
	pkb.setUses(2, "x");
	pkb.setProcUses("Main", "x");
	//line 3
	pkb.setParent(2, 3);
	pkb.setStatementType(3, "assign");
	pkb.setModifies(3, "y");
	pkb.setProcModifies("Main", "y");
	pkb.addConstant("2");
	pkb.addPattern(3, "y", "2");
	//line 4
	pkb.setFollows(3, 4);
	pkb.setParent(2, 4);
	pkb.setStatementType(4, "assign");
	pkb.setModifies(4, "x");
	pkb.setProcModifies("Main", "x");
	pkb.setUses(4, "x");
	pkb.setProcUses("Main", "x");
	pkb.setUses(4, "y");
	pkb.setProcUses("Main", "y");
	pkb.addConstant("1");
	pkb.addPattern(4, "x", "x*y+1");
	qe.setPKB(pkb);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
	// call your evaluator to evaluate the query here
	  // ...code to evaluate query...
	q.parseInput(query);
	QueryStatement qs = q.getQueryStatement();
	qe.setQueryStatement(qs);
	vector<string> ans = qe.runQueryEval();
	results.push_back(ans[0]);
	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
}
