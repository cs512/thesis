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
	Matcher();
	~Matcher();
	bool operator()(bitset<224>, bitset<224>);
private:
};

#endif /* MATCHER_H_ */
