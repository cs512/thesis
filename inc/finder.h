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
#include <map>
#include "matcher.h"
#include "SQLiteCpp/Database.h"

using namespace std;
using namespace SQLite;

class Finder
{
public:
    Finder();
    Finder(Matcher&, int maxCodeWordDistance);
    ~Finder();
    int newDatabase(const string path);
    int loadDatabase(const string path);
    int addIndex(vector<bitset<256>> scalars, const string filePath);
    map<string, int> find(vector<bitset<256>> scalars);
    void playGround();
private:
    vector<int> buildCodeWord(int);
    int buildTable(int, int, int);
    Matcher *isMatch;
    bool selfInitial;
    Database *db;
    int maxCodeWordDistance;
    vector<int> xorTable;
    const string buildString =
            "PRAGMA foreign_keys = ON;"\
            "create table features"\
            "("\
            "id integer primary key autoincrement, "\
            "code_word integer, "\
            "feature blob,"\
            "CONSTRAINT uc_feature UNIQUE (code_word,feature)"\
            ");"\
            "create index index_code_word on features(code_word);"\
            ""\
            "create table files"\
            "("\
            "id integer primary key autoincrement,"\
            "path varchar(1024)"\
            ");"\
            ""\
            "create table features_files"\
            "("\
            "id integer primary key autoincrement,"\
            "feature_id integer,"\
            "file_id integer,"\
            "foreign key(feature_id) references features(id),"\
            "foreign key(file_id) references files(id)"\
            "); "\
            "create index index_feature_id on features_files(feature_id);";
};


#endif
