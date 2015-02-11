/*
 * finder.h
 *
 *  Created on: 2015-2-11
 *      Author: wangjz
 */

#ifndef WJZ_FINDER_H
#define WJZ_FINDER_H

#include <string>
#include <bitset>
#include <vector>

using namespace std;

class Finder
{
public:
	Finder();
	~Finder();
	int newDatabase(const string path);
	int loadDatabase(const string path);
	int addIndex(bitset<256> scalar, const string filePath);
	vector<const string> find(bitset<256> scalar);
};


#endif
