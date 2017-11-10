//This class holds two attibute i.e. vector of synonym and its associated design entity
#pragma once
#include <string>
#include <vector>

using namespace std;
class SynonymEntityPair
{
	vector<string> synonymList;
	string entity;
public:
	SynonymEntityPair(string, vector<string>);
	vector<string> getSynonymList();
	string getEntity();
};