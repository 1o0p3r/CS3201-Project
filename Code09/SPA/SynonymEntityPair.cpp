#include "SynonymEntityPair.h"
#include <string>
#include <vector>

using namespace std;
SynonymEntityPair::SynonymEntityPair(string entityIn, vector<string>synonymIn)
{
	synonymList = synonymIn;
	entity = entityIn;
}

vector<string> SynonymEntityPair::getSynonymList() {
	return synonymList;
}

string SynonymEntityPair::getEntity() {
	return entity;
}
