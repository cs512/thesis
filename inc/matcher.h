/*
 * matcher.h
 *
 *  Created on: 2015-2-11
 *      Author: wangjz
 */

#ifndef WJZ_MATCHER_H
#define WJZ_MATCHER_H

#include <bitset>

using namespace std;

class Matcher
{
public:
	Matcher(int maxDistance);
	Matcher();
	~Matcher();
	bool operator()(bitset<224>, bitset<224>);
private:
	int maxDistance;
};

#endif /* MATCHER_H_ */
