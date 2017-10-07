#include "stdafx.h"
#include "CppUnitTest.h"
#include "Modify.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ModifyTest)
	{
	public:

		TEST_METHOD(setModifiesCorrectly_wParent) { //set modifies for stmt and parent correctly

			Modify modify;
			vector<int> parentStar;
			parentStar.push_back(1);

			modify.setModifies(2, 2 , parentStar);
			Assert::AreEqual(2, modify.getModifies(1)[0]); //parent 1 modifies var 2
			Assert::AreEqual(2, modify.getModifies(2)[0]); //stmt 2 modifies var 2
			Assert::AreEqual(1, modify.getModifiedBy(2)[0]); //var 2 modified by parent 1
			Assert::AreEqual(2, modify.getModifiedBy(2)[1]); //var 2 modified by stmt 2
			
			modify.setProcModifies(1, 2);
			Assert::AreEqual(modify.getProcModifies(1)[0], 2);
			Assert::AreEqual(modify.getProcModifiedBy(2)[0], 1);
			
			modify.setProcModifies(1, 1);
			Assert::IsTrue(modify.getProcModifies(1) == vector<int>{1, 2});
			Assert::AreEqual(modify.getProcModifiedBy(1)[0], 1);
		}

		TEST_METHOD(parentModifiesRemovesDuplicateVar) { //values are significant in parent set

			Modify modify;
			vector<int> parentStar;
			parentStar.push_back(1);

			modify.setModifies(2, 2, parentStar); //parent 1 adds var 2
			modify.setModifies(3, 2, parentStar); //parent 1 adds var 2
			Assert::AreEqual(2, modify.getModifies(1)[0]); //parent 1 modifies var 2
			Assert::IsTrue(modify.getModifies(1).size() == 1); //no duplicate in parent
			Assert::IsTrue(modify.getModifiedBy(2).size() == 3); //var modified by 3 stmts
		}

		TEST_METHOD(procModifiesTablesAddCorrectly) { //values are significant in proc set

			Modify modify;

			modify.setProcModifies(1, 2);
			modify.setProcModifies(2, 2);
			Assert::AreEqual(2, modify.getProcModifies(1)[0]); //proc 1 modifies var 2
			Assert::AreEqual(2, modify.getProcModifies(2)[0]); //proc 2 modifies var 2
			Assert::IsTrue(modify.getProcModifies(1).size() == 1); 
			Assert::IsTrue(modify.getProcModifies(2).size() == 1);
		}

		TEST_METHOD(procModifiesTablesAddCorrectly2) { //values are significant in proc set

			Modify modify;
			vector<int> proc1CalledBy = {};
			vector<int> proc1Calls = { 2 };
			vector<int> proc2CalledBy = { 1 };
			vector<int> proc2Calls = {};
			vector<int> proc1CalledByStmt = {};
			vector<int> proc2CalledByStmt = {2};

			modify.setProcModifies(1, 1, proc1CalledBy, proc1Calls, proc1CalledByStmt);
			modify.setProcModifies(1, 2, proc1CalledBy, proc1Calls, proc1CalledByStmt);
			modify.setProcModifies(2, 3, proc2CalledBy, proc2Calls, proc2CalledByStmt);
			modify.setProcModifies(2, 4, proc2CalledBy, proc2Calls, proc2CalledByStmt);
			Assert::IsTrue(modify.getProcModifies(1) == vector<int>{1, 2, 3, 4});
		}

		TEST_METHOD(setModifiesWMultipleProcsCorrectly) { //values are significant in proc set

			Modify modify;
			vector<int> proc1CalledBy = {};
			vector<int> proc1Calls = { 2 };
			vector<int> proc2CalledBy = { 1 };
			vector<int> proc2Calls = {};
			vector<int> proc1CalledByStmt = {};
			vector<int> proc2CalledByStmt = { 2 };

			modify.setModifies(1, 1);
			modify.setModifies(2, 2);
			modify.setModifies(3, 3);
			modify.setModifies(4, 4);


			modify.setProcModifies(1, 1, proc1CalledBy, proc1Calls, proc1CalledByStmt);
			modify.setProcModifies(1, 2, proc1CalledBy, proc1Calls, proc1CalledByStmt);
			modify.setProcModifies(2, 3, proc2CalledBy, proc2Calls, proc2CalledByStmt);
			modify.setProcModifies(2, 4, proc2CalledBy, proc2Calls, proc2CalledByStmt);
			Assert::IsTrue(modify.getModifies(2) == vector<int>{2, 3, 4});
		}

		TEST_METHOD(checkModifiesSizeAndBoundaryAreCorrect) { 

			Modify modify;
			Assert::AreEqual(modify.getModifies(0) == vector<int>(), true); 
			Assert::IsTrue(modify.getModifies(0).size() == 0);
		}

	};
}