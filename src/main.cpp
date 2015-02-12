#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "main.h"
#include "qbuilder.h"
#include "scheduler.h"
#include "finder.h"

using namespace std;
using namespace cv;

int testOfQBuilder()
{
	QBuilder qb;
	//cout<<qb.getDescriptor("/home/wangjz/thesisProgect/dataset/testPic.jpg")[0];
	qb.getDescriptor("/home/wangjz/thesisProgect/dataset/testPic.jpg");
	return 0;
}

int testOfScheduler()
{
	QBuilder qb;
	Finder fd;
	fd.newDatabase("./test.db3");
	Scheduler sch = Scheduler(qb, fd, ".", "jpg");
	sch.buildDatabase();
	sch.search("/home/wangjz/thesisProgect/dataset/testPic.jpg");
	return 0;
}

int testOfFinder()
{
	Finder fd;
	fd.newDatabase("./test.db3");
	return 0;
}

int playWithFinder()
{
	Finder fd;
	fd.playGround();
	return 0;
}

int main()
{
    //testOfQBuilder();
    testOfScheduler();
    //playWithFinder();
//	testOfFinder();
    return 0;
}
