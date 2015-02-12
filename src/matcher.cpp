/*
 * matcher.cpp
 *
 *  Created on: 2015-2-11
 *      Author: wangjz
 */

#include "matcher.h"


Matcher::Matcher(int maxDistance)
{
	this->maxDistance = maxDistance;
}

Matcher::Matcher()
{
	this->maxDistance = 24;
}

Matcher::~Matcher()
{

}


bool Matcher::operator()(bitset<224> a, bitset<224> b)
{
	int count = 0;
	for(auto i = 0; i < 224; ++i)
	{
		count += a[i] ^ b[i];
		if(count > this->maxDistance)
			return false;
	}
	return true;
}
