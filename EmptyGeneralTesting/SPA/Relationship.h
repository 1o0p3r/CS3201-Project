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
	Relationship(int, vector<string>, vector<string>);
	int getNumArgs();
	vector<string> getArg1();
	vector<string> getArg2();
};

