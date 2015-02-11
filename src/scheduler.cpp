#include "qbuilder.h"
#include "scheduler.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <string>
#include <bitset>
#include <vector>
#include <cstdio>

using namespace std;

Scheduler::Scheduler()
{
	this->qb = new QBuilder();
	this->selfInitial = true;
	this->path = "~";
	this->filter = ".jpg\\|.png\\|.tiff";
}

Scheduler::Scheduler(QBuilder& qb, const string path, const string filter)
{
	this->qb = &qb;
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

int Scheduler::run()
{
	string cmd = "find " + path + " -type f";
	cmd += " | grep \"" + filter + "\"";
	cout<<endl <<cmd<<endl;
	FILE *fileList = popen(cmd.c_str(), "r");
	string filePath;
	char temp[2048];
	while(fscanf(fileList, "%s\n", temp) != -1)
	{
		filePath = String(temp);
		cout<<filePath<<endl;
	}
	return 0;
}

int Scheduler::p_runTask(const string path)
{

	return 0;
}
