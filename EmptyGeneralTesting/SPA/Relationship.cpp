#include "Relationship.h"
#include "vector"

using namespace std;
Relationship::Relationship(int num, vector<string> arg1, vector<string> arg2)
{
	numArgs = num;
	argument1 = arg1;
	argument2 = arg2;
}

Relationship::Relationship() {

}
int Relationship::getNumArgs() {
	return numArgs;
}
vector<string> Relationship::getArg1() {
	return argument1;
}
vector<string> Relationship::getArg2() {
	return argument2;
}