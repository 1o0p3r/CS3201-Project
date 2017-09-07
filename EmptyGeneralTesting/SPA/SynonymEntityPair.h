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
	~SynonymEntityPair();
	vector<string> getSynonymList();
	string getEntity();
};