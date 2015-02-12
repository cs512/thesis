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
    int count = 0;
    for(auto i = 0; i < 224; ++i)
    {
        auto pos = i >> 3;
        count += ((a[pos] ^ b[pos]) >> (i & 7)) & 1;
        if(count > this->maxDistance)
        {
            cout<<"a:";
            for(auto j = 0; j < 28; ++j)
            {
                cout<<int(a[j])<<',';
            }
            cout<<endl;
            cout<<"b:";
            for(auto j = 0; j < 28; ++j)
            {
                cout<<int(b[j])<<',';
            }
            cout<<endl;
            return false;
        }

    }
    return true;
}
