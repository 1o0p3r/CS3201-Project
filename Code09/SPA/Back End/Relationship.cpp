//This class holds a particular relationship e.g. Follows with its permitted arg1 and arg2
#include "Relationship.h"
#include "vector"

using namespace std;
Relationship::Relationship(vector<string> arg1, vector<string> arg2)
{
	argument1 = arg1;
	argument2 = arg2;
}

Relationship::Relationship() {

}

vector<string> Relationship::getArg1() {
	return argument1;
}
vector<string> Relationship::getArg2() {
	return argument2;
}