#include "AnalyzerFactory.h"

enum clauseRelation
{
	undefinedClause, withClause
};



AnalyzerFactory::AnalyzerFactory()
{
	initClauseTypeMap();
}

tuple<bool, vector<vector<string>>> AnalyzerFactory::getResults(QueryElement clause, string clauseRel)
{
	tuple<bool, vector<vector<string>>> result;
	switch(clauseTypeMap[clauseRel])
	{
	case withClause:
		result = tuple<bool, vector<vector<string>>>();
		break;
	}
	return result;
}

void AnalyzerFactory::initClauseTypeMap()
{
	clauseTypeMap["with"] = withClause;
}



