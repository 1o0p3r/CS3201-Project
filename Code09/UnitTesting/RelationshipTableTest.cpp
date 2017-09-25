#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidator.h"
#include "RelationshipTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(relationshipTableTest)
	{
	public:
		TEST_METHOD(isValidArgRel) {
			RelationshipTable relationshipTable;

			string rel;
			string type;
			int idx;
		
			rel = "Follows";
			type = "stmt";
			idx = 1;
			Assert::IsTrue(relationshipTable.isValidArg(rel, type, idx));
			
			rel = "Follows*";
			type = "stmt";
			idx = 1;
			Assert::IsTrue(relationshipTable.isValidArg(rel, type, idx));
			
			rel = "Rubbish";
			type = "stmt";
			idx = 1;
			Assert::IsFalse(relationshipTable.isValidArg(rel, type, idx));
			
			rel = "Follows*";
			type = "stmtlist";
			idx = 1;
			Assert::IsFalse(relationshipTable.isValidArg(rel, type, idx));
			
			rel = "Uses";
			type = "while";
			idx = 1;
			Assert::IsTrue(relationshipTable.isValidArg(rel, type, idx));
			
			rel = "Uses";
			type = "constant";
			idx = 1;
			Assert::IsFalse(relationshipTable.isValidArg(rel, type, idx));
			
			rel = "Uses";
			type = "wildcard";
			idx = 1;
			Assert::IsFalse(relationshipTable.isValidArg(rel, type, idx));

			rel = "Uses";
			type = "wildcard";
			idx = 2;
			Assert::IsTrue(relationshipTable.isValidArg(rel, type, idx));

			rel = "Modifies";
			type = "wildcard";
			idx = 1;
			Assert::IsFalse(relationshipTable.isValidArg(rel, type, idx));

			rel = "Parent";
			type = "procedure";
			idx = 1;
			Assert::IsFalse(relationshipTable.isValidArg(rel, type, idx));
			
		}
	};
}