//This class holds two attibute i.e. vector of synonym and its associated design entity
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
