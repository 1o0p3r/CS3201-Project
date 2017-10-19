#include "TestWrapper.h"
#include "Parser.h"
#include "PKB.h"
#include "QueryValidator.h"
#include "QueryAnalyzer.h"
#include "Util.h"

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
	QueryAnalyzer analyzer;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	if (!Parse(filename, pkb)) {
		cout << "Parsing failed." << "\n";
		exit(-1);
	};
	analyzer.setPKB(pkb);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
	// call your evaluator to evaluate the query here
	  // ...code to evaluate query...
	QueryValidator validator;
	if (validator.parseInput(query)) {
		QueryStatement statement = validator.getQueryStatement();
		analyzer.setQS(statement);
		vector<string> ans = analyzer.runQueryEval();
		for each (string s in ans) {
			results.push_back(s);
		}
	}
	// AutoTester.exe Sample-Source-3.txt Sample-Queries-3.txt out.xml
	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
}
