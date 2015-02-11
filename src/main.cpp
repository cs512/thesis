#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "main.h"
#include "qbuilder.h"
#include "scheduler.h"

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
	Scheduler sch = Scheduler(qb, ".", "jpg");
	sch.buildDatabase();
	return 0;
}

int main()
{
    testOfQBuilder();
    testOfScheduler();
    return 0;
}
