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
#include "SQLiteCpp/SQLiteCpp.h"

using namespace std;
using namespace SQLite;

Finder::Finder()
{
	this->selfInitial = true;
	this->isMatch = new Matcher(24);
	this->db = NULL;
}

Finder::Finder(Matcher& matcher)
{
	this->isMatch = &matcher;
	this->selfInitial = false;
	this->db = NULL;
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
		this->db = new Database(path, SQLITE_OPEN_READWRITE);
		this->db->exec(this->buildString);
	}
	catch(std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
	return 0;
}

int Finder::addIndex(vector<bitset<256>> scalars, const string filePath)
{
	if(this->db == NULL)
		return -1;

	Statement query(*(this->db), "INSERT INTO files VALUES (NULL, ?)");
	query.bind(1, filePath);
	query.exec();

	Statement getId(*(this->db), "SELECT last_insert_rowid() newid;");
	getId.exec();
	int fileId = getId.getColumn(0);
	//query.Statement(*(this->db), "");

	for(auto it = scalars.begin(); it != scalars.end(); ++it)
	{
		/* |  255~224  | 223~0 |
		 * | code word | rest  |
		 */
		int codeWord = 0;
		for(auto i = 255; i >= 224; --i)
		{
			codeWord <<= 1;
			codeWord += (*it)[i];
		}
		char feature[29] = {0};
		/* | 223~0 |
		 * | 28~0  |
		 */
		for(auto i = 223; i >= 0; --i)
		{
			auto pos = i / 8;
			feature[pos] <<= 1;
			feature[pos] += (*it)[i];
		}

		try
		{
			Statement insert(*(this->db), "INSERT INTO features VALUES (NULL, ?, ?);");
			insert.bind(1, codeWord);
			insert.bind(2, feature);
			insert.exec();
		}
		catch(std::exception& e)
		{
			std::cout << "exception: " << e.what() << std::endl;
		}

		Statement query(*(this->db), "SELECT * FROM features INDEXED BY index_code_word WHERE code_word = ? AND feature = ?");
		query.bind(1, codeWord);
		query.bind(2, feature);
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
	return 0;
}

vector<string> Finder::find(bitset<256> scalar)
{
	return vector<string>();
}

