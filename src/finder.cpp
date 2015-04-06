/*
 * finder.cpp
 *
 *  Created on: 2015-2-11
 *      Author: wangjz
 */
#include "finder.h"
#include "matcher.h"
#include <vector>
#include <string>
#include <bitset>
#include <iostream>
#include <map>
#include "SQLiteCpp/SQLiteCpp.h"
#include "debug.h"

using namespace std;
using namespace SQLite;

Finder::Finder()
{
    this->selfInitial = true;
    this->isMatch = new Matcher(24);
    this->maxCodeWordDistance = 0;
    this->xorTable.push_back(0);
    this->db = NULL;
}

Finder::Finder(Matcher& matcher,int maxCodeWordDistance)
{
    this->isMatch = &matcher;
    this->selfInitial = false;
    this->db = NULL;
    if(maxCodeWordDistance > 5)
        this->maxCodeWordDistance = 5;
    else
        this->maxCodeWordDistance = maxCodeWordDistance;
    for(auto i = 0; i <= this->maxCodeWordDistance; ++i)
        this->buildTable(i, 0, 0);
}

int Finder::buildTable(int index, int begin, int number)
{
    if(index == 0)
    {
        this->xorTable.push_back(number);
        return 0;
    }
    else if(index < 0)
    {
        return 0;
    }
    if(begin > 31)
        return 0;
    for(auto i = begin; i < 32; ++i)
    {
        this->buildTable(index - 1, i + 1, number | (1<<i));
    }
    return 0;
}

Finder::~Finder()
{
    if(this->selfInitial)
    {
        delete (this->isMatch);
    }
    if(this->db != NULL)
        delete (this->db);
}

void Finder::playGround()
{
    try
    {
        this->db = new Database("./test.db3", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
        this->db->exec(this->buildString);
        Statement insert(*(this->db), "insert into features values (NULL, 12, \"1231231231231231231231231232\");");
        insert.exec();
    }
    catch(std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
    delete this->db;
    this->db = NULL;
    return;
}

int Finder::newDatabase(const string path)
{
    try
    {
        if(this->db != NULL)
            delete (this->db);
        this->db = new Database(path, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
        this->db->exec(this->buildString);
    }
    catch(std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
    return 0;
}

int Finder::loadDatabase(const string path)
{
    try
    {
        if(this->db != NULL)
            delete (this->db);
        this->db = new Database(path, SQLITE_OPEN_READONLY);
    }
    catch(std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
    return 0;
}

int Finder::getCodeword(bitset<256> scalar)
{
    /* |    0~31   | 32~255 |
    *  | code word |  rest  |
    */
    int codeWord = 0;
    for(auto i = 0; i < 32; ++i)
    {
#ifdef DEBUG
        cout<<scalar[i];
#endif
        codeWord <<= 1;
        codeWord += scalar[i];
    }
#ifdef DEBUG
        cout<<"CWCW"<<endl;
        cout<<"codeWord:"<<codeWord<<endl;
#endif
    return codeWord;
}

int Finder::getFeature(bitset<256> scalar, char* fet)
{
    //char feature[29] = {0};
    /* | 32~255 |
    *  | 0~28  |
    */
    for(auto i = 32; i < 256; ++i)
    {
        auto pos = (i-32) / 8;
        fet[pos] <<= 1;
        fet[pos] += scalar[i];
    }
    return 0;
}

#ifdef DEBUG
#include<fstream>
#endif

int Finder::addIndex(vector<bitset<256>> scalars, const string filePath)
{
    if(this->db == NULL)
        return -1;
#ifdef DEBUG
    ofstream fo(filePath+".txt");
#endif
    Statement query(*(this->db), "INSERT INTO files VALUES (NULL, ?)");
    query.bind(1, filePath);
    query.exec();

    Statement getId(*(this->db), "SELECT last_insert_rowid() newid;");
    getId.executeStep();
    int fileId = getId.getColumn(0);
    //query.Statement(*(this->db), "");
    Transaction transaction(*(this->db)); // for performance reason
    for(auto it = scalars.begin(); it != scalars.end(); ++it)
    {
        auto codeWord = this->getCodeword(*it);
#ifdef DEBUG
        fo << codeWord << endl;
#endif
        char feature[29] = {0};
        this->getFeature(*it, feature);

        try
        {
            Statement insert(*(this->db), "INSERT INTO features VALUES (NULL, ?, ?);");
            insert.bind(1, codeWord);
            insert.bind(2, (void *)feature, 28);
            insert.exec();
        }
        catch(std::exception& e)
        {
            std::cout << "exception: " << e.what() << std::endl;
        }
        Statement query(*(this->db), "SELECT * FROM features INDEXED BY index_code_word WHERE code_word = ? AND feature = ?");
        query.bind(1, codeWord);
        query.bind(2, (void *)feature, 28);
        int featureId = -1;
        if(query.executeStep())
        {
            featureId = query.getColumn(0);
        }
        Statement insert(*(this->db), "INSERT INTO features_files VALUES (NULL, ?, ?)");
        insert.bind(1, featureId);
        insert.bind(2, fileId);
        insert.exec();
    }
    transaction.commit();
#ifdef DEBUG
    fo.close();
#endif
    return 0;
}

vector<int> Finder::buildCodeWord(int codeWord)
{
    vector<int> res;
    for(auto it = this->xorTable.begin(); it != this->xorTable.end(); ++it)
    {
        res.push_back((*it) ^ codeWord);
    }
    return res;
}

map<string, int> Finder::find(vector<bitset<256>> scalars)
{
    vector<int> res;

    for(auto it = scalars.begin(); it != scalars.end(); ++it)
    {
        auto codeWord = this->getCodeword(*it);
        char feature[29] = {0};
        this->getFeature(*it, feature);

        try
        {
        	for(auto itemxb = this->xorTable.begin(); itemxb != this->xorTable.end(); ++itemxb)
        	{
        		Statement query(*(this->db), "SELECT * FROM features WHERE code_word = ?;");
				query.bind(1, codeWord ^ (*itemxb));
				while(query.executeStep())
				{
					if((*(this->isMatch))(feature, query.getColumn(2)))
					{
						auto t = query.getColumn(0);
						res.push_back(t);
					}
				}
        	}
        }
        catch(std::exception& e)
        {
            std::cout << "exception: " << e.what() << std::endl;
        }
    }
    map<string, int> ret;
    for(auto it = res.begin(); it != res.end(); ++it)
    {
        Statement query(*(this->db), "SELECT files.path FROM features_files, files WHERE features_files.feature_id = ? and features_files.file_id = files.id;");
        query.bind(1, (*it));
        int count = 0;
        while(query.executeStep())
        {
        	//cout<<count++<<endl;
        	count++;
            string path = query.getColumn(0);
            if(ret.find(path) != ret.end())
            {
                //finded
                ret[path] += 1;
            }
            else
            {
                //not find
                ret[path] = 1;
            }
        }
    }
    return ret;
}

