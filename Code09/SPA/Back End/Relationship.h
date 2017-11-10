//This class holds a particular relationship e.g. Follows with its permitted arg1 and arg2
#pragma once
#include <string>
#include <vector>

using namespace std;
class Relationship
{
private:
	int numArgs;
	vector<string> argument1;
	vector<string> argument2;
public:
	Relationship(vector<string>, vector<string>);
	Relationship();
	vector<string> getArg1();
	vector<string> getArg2();
};

