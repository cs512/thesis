#include "qbuilder.h"
#include "scheduler.h"
#include "finder.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <string>
#include <bitset>
#include <vector>
#include <cstdio>

using namespace std;
/*
Scheduler::Scheduler()
{
    this->qb = new QBuilder();
    this->selfInitial = true;
    this->path = "~";
    this->filter = ".jpg\\|.png\\|.tiff";
}
*/

Scheduler::Scheduler(QBuilder& qb, Finder& fd, const string path, const string filter)
{
    this->qb = &qb;
    this->fd = &fd;
    this->selfInitial = false;
    this->path = path;
    this->filter = filter;
}

Scheduler::~Scheduler()
{
    if(this->selfInitial)
    {
        delete this->qb;
    }
}

int Scheduler::buildDatabase()
{
    string cmd = "find " + path + " -type f";
    cmd += " | grep \"" + filter + "\"";
    //cout<<endl <<cmd<<endl;
    FILE *fileList = popen(cmd.c_str(), "r");
    string filePath;
    char temp[2048];
    while(fscanf(fileList, "%s\n", temp) != -1)
    {
        filePath = String(temp);
        auto res = this->p_computeScalar(filePath);
        //cout<<filePath<<endl;
        this->fd->addIndex(res, filePath);
    }
    return 0;
}

map<string, int> Scheduler::search(const string filePath)
{
    return this->fd->find(this->p_computeScalar(filePath));
    //cout<<res.size()<<endl;
}

vector<bitset<256>> Scheduler::p_computeScalar(const string path)
{
    return this->qb->getDescriptor(path);
}
