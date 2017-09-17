#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	TEST_CLASS(SPATest) {
public:

	TEST_METHOD(IntegrationTesting) {
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
	}
	};
}