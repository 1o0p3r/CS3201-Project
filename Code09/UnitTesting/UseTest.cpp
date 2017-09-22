#include "stdafx.h"
#include "CppUnitTest.h"
#include "Use.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(UseTest)
	{
	public:

		TEST_METHOD(setUsesCorrectly_wParent) {

			Use use;
			vector<int> parentStar;
			parentStar.push_back(1);

			use.setUses(2, 2, parentStar);
			Assert::AreEqual(2, use.getUses(1)[0]); //parent 1 uses var 2
			Assert::AreEqual(2, use.getUses(2)[0]); //stmt 2 uses var 2
			Assert::AreEqual(1, use.getUsedBy(2)[0]); //var 2 used by parent 1
			Assert::AreEqual(2, use.getUsedBy(2)[1]); //var 2 used by stmt 2

			use.setProcUses(1, 2);
			Assert::AreEqual(use.getProcUses(1)[0], 2);
			Assert::AreEqual(use.getProcUsedBy(2)[0], 1);

			use.setProcUses(1, 1);
			Assert::IsTrue(use.getProcUses(1) == vector<int>{1, 2});
			Assert::AreEqual(use.getProcUsedBy(1)[0], 1);


			Assert::AreEqual(2, use.getUses(1)[0]);
			Assert::AreEqual(2, use.getUses(2)[0]);
		}

		TEST_METHOD(parentUsesRemovesDuplicateVar) { //values are significant in parent set

			Use use;
			vector<int> parentStar;
			parentStar.push_back(1);

			use.setUses(2, 2, parentStar); //parent 1 adds var 2
			use.setUses(3, 2, parentStar); //parent 1 adds var 2
			Assert::AreEqual(2, use.getUses(1)[0]); //parent 1 modifies var 2
			Assert::IsTrue(use.getUses(1).size() == 1); //no duplicate in parent
			Assert::IsTrue(use.getUsedBy(2).size() == 3); //var modified by 3 stmts
		}

		TEST_METHOD(procUsesTablesAddCorrectly) { //values are significant in proc set

			Use use;
			use.setProcUses(1, 2);
			use.setProcUses(2, 2);

			Assert::AreEqual(2, use.getProcUses(1)[0]); //proc 1 modifies var 2
			Assert::AreEqual(2, use.getProcUses(2)[0]); //proc 2 modifies var 2
			Assert::IsTrue(use.getProcUses(1).size() == 1);
			Assert::IsTrue(use.getProcUses(2).size() == 1);
		}

		TEST_METHOD(checkUsesSizeAndBoundaryAreCorrect) {

			Use use;
			Assert::AreEqual(use.getUses(0) == vector<int>(), true);
			Assert::IsTrue(use.getUses(0).size() == 0);

			use.setProcUses(1, 2);
			Assert::IsTrue(use.getProcUses(1).size() == 1);
			use.setProcUses(1, 3);
			Assert::IsTrue(use.getProcUses(1).size() == 2);
			use.setProcUses(1, 2);
			Assert::IsTrue(use.getProcUses(1).size() == 2); //should be able to add twice, but not duplicated
			Assert::IsTrue(use.getProcUses(1) == vector<int>{2, 3});
			Assert::IsTrue(use.getProcUses(2) == vector<int>(), false); //should not exist
		}
	};
}