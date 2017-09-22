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

		TEST_METHOD(parentRemovesDuplicateVar) { //values are significant in parent set

			Modify modify;
			vector<int> parentStar;
			parentStar.push_back(1);

			modify.setModifies(2, 2, parentStar); //parent 1 adds var 2
			modify.setModifies(3, 2, parentStar); //parent 1 adds var 2
			Assert::AreEqual(2, modify.getModifies(1)[0]); //parent 1 modifies var 2
			Assert::IsTrue(modify.getModifies(1).size() == 1); //no duplicate in parent
			Assert::IsTrue(modify.getModifiedBy(2).size() == 3); //var modified by 3 stmts
		}

		TEST_METHOD(procTablesAddCorrectly) { //values are significant in proc set

			Modify modify;

			modify.setProcModifies(1, 2);
			modify.setProcModifies(2, 2);
			Assert::AreEqual(2, modify.getProcModifies(1)[0]); //proc 1 modifies var 2
			Assert::AreEqual(2, modify.getProcModifies(2)[0]); //proc 2 modifies var 2
			Assert::IsTrue(modify.getProcModifies(1).size() == 1); 
			Assert::IsTrue(modify.getProcModifies(2).size() == 1);
		}

		TEST_METHOD(checkSizeAndBoundaryAreCorrect) { 

			Modify modify;
			Assert::AreEqual(modify.getModifies(0) == vector<int>(), true); 
			Assert::IsTrue(modify.getModifies(0).size() == 0);
		}

	};
}