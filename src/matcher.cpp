/*
 * matcher.cpp
 *
 *  Created on: 2015-2-11
 *      Author: wangjz
 */

#include "matcher.h"
#include <iostream>

using namespace std;

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


bool Matcher::operator()(const char *a, const char *b)
{
    //return true;
    int count = 0;
    for(auto i = 0; i < 224/8; ++i)
    {
        auto pos = i;
        count += __builtin_popcount(a[pos] ^ b[pos]);
        if(count > this->maxDistance)
        {
            return false;
        }

    }
    return true;
}
