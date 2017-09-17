// QueryPreprocessor.cpp : Defines the entry point for the console application.
//

#include "QueryValidator.h"
#include <string>
#include "QueryPreprocessor.h"

using namespace std;
QueryPreprocessor::QueryPreprocessor()
{
	QueryValidator queryValidator = QueryValidator();
	queryValidator.startParsing(str);
}
