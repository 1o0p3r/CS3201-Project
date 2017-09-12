#include "QueryEval.h"

QueryEval::QueryEval()
{


}


QueryEval::~QueryEval()
{
}

/**
s1 = some statement number, opt = 0 or 1
0-> use getFollows
1-> use getFollowedby
*/
vector<int> QueryEval::followResult(int s1, int opt)
{	
	vector<int> queryResult = vector<int>();
	
	//case: Follow("5",s)
	if (opt) {
		queryResult = Follow().getFollows(s1);  
	}
	//case: Follow(s,"5")
	else {
		queryResult = Follow().getFollowedBy(s1);
	}

	return queryResult;
}

/**
s1 = some statement number, opt = 0 or 1
0-> use getFollowsStar
1-> use getFollowedByStar
*/
vector<int> QueryEval::followStarResult(int s1, int opt) {
	vector<int> queryResult = vector<int>();
	//case: Follow*("7",s)
	if (opt) {
		queryResult = Follow().getFollowsStar(s1);
	}
	//case: Follow*(s,"7")
	else {
		queryResult = Follow().getFollowedByStar(s1);
	}
	return queryResult;
}

/**
s1 = some statement number, opt = 0 or 1
0-> use getParent
1-> use getChild
*/
vector<int> QueryEval::parentResult(int s1, int opt) {
	vector<int> queryResult = vector<int>();
	//case: Parent("7",s)
	if (opt) {
		queryResult = Parent().getChild(s1);
	}
	//case: Parent(s,"7")
	else {
		queryResult = Parent().getParent(s1);
	}
	return queryResult;
}

/**
s1 = some statement number, opt = 0 or 1
0-> use getParentStar
1-> use getChildStar
*/
vector<int> QueryEval::parenStarResult(int s1, int opt) {
	vector<int> queryResult = vector<int>();
	//case: Parent*("7",s)
	if (opt) {
		queryResult = Parent().getChildStar(s1);
	}
	//case: Parent*(s,"7")
	else {
		queryResult = Parent().getParentStar(s1);
	}
	return queryResult;
}

vector<int> QueryEval::modifiesResult(int s1, int opt) {
	vector<int> queryResult = vector<int>();

	switch (opt) {
		//Modifies("1",z)
		case ASSIGNMOD :
			queryResult = Modify().getModifies(s1);
			break;
		
		//Modifies(s1,"z")
		case VARMODBY :
			queryResult = Modify().getModifiedBy(s1);
			break;
		
		//Modifies("someProc", z)
		case PROCMOD :
			queryResult = Modify().getProcModifies(s1);
			break;

		//Modifies(procedure,"z")
		case PROCMODBY :
			queryResult = Modify().getProcModifiedBy(s1);

	}
	return queryResult;
}

vector<int> QueryEval::usesResult(int s1, int opt) {
	vector<int> queryResult = vector<int>();

	vector<int> getUses(int s);
	vector<int> getUsedBy(int varName);
	vector<int> getProcUses(int procName);
	vector<int> getProcUsedBy(int procName);

	switch (opt) {
		//uses("1",z)
	case ASSIGNMOD:
		queryResult = Use().getUses(s1);
		break;

		//uses(s1,"z")
	case VARMODBY:
		queryResult = Use().getUsedBy(s1);
		break;

		//uses("someProc", z)
	case PROCMOD:
		queryResult = Use().getProcUses(s1);
		break;

		//uses(procedure,"z")
	case PROCMODBY:
		queryResult = Use().getProcUsedBy(s1);

	}
	return queryResult;
}