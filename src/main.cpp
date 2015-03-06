#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "main.h"
#include "qbuilder.h"
#include "scheduler.h"
#include "finder.h"
#include <getopt.h>
#include <cstring>

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
    Matcher mt;
    Finder fd(mt, 3);
    fd.newDatabase("./test.db3");
    Scheduler sch = Scheduler(qb, fd, ".", "jpg");
    sch.buildDatabase();
//  sch.search("/home/wangjz/thesisProgect/dataset/testPic.jpg");
    auto res = sch.search("/home/wangjz/thesisProgect/dataset/testPic.jpg");
//  cout<<res<<endl;
    return 0;
}

int testOfFinder()
{
    Matcher mt;
    Finder fd(mt, 3);
    //fd.newDatabase("./test.db3");
    return 0;
}

int playWithFinder()
{
    Finder fd;
    fd.playGround();
    return 0;
}

int createDatabase(const string sqlPath, const string dbPath)
{
    QBuilder qb;
    Matcher mt;
    Finder fd(mt, 1);
    fd.newDatabase(sqlPath);
    Scheduler sch(qb, fd, dbPath,".jpg\\|.png\\|.tiff");
    sch.buildDatabase();
    return 0;
}

int searchDatabase(const string sqlPath, const string filePath)
{
	QBuilder qb;
	Matcher mt;
	Finder fd(mt, 3);
	fd.loadDatabase(sqlPath);
	Scheduler sch(qb, fd, "","");
	auto res = sch.search(filePath);
	for(auto it = res.begin(); it != res.end(); ++it)
	{
		cout<<(*it).first<<":"<<(*it).second<<endl;
	}
	return 0;
}

int main(int argc, char *argv[])
{

//    testOfQBuilder();
//    testOfScheduler();
//    playWithFinder();
//	  testOfFinder();
    int oc;
    char *opt_arg;
    const char *short_options = "f:d:s:";
    const struct option long_options[] = {
        {  	"func",	1,  NULL,   'f' },
        {  	"dir",	1,  NULL,   'd' },
        {  	"sql",	1,  NULL,   's' },
        {	"pic",	1,	NULL,	'p' },
        {	NULL,	0,  NULL,   0   }
    };
    int mode = 0;
    string dbSqlPath = "";
    string dbDir = "";
    string picPath = "";
    while((oc = getopt_long(argc, argv, short_options, long_options, NULL)) != -1)
    {
        switch(oc)
        {
            case 'f':
                //func
                opt_arg = optarg;
                if(strcmp("create", opt_arg) == 0)
                {
                    mode = CREATE_MODE;
                }
                else if(strcmp("find", opt_arg) == 0)
                {
                    mode = SEARCH_MODE;
                }
                else
                {
                    cout<<"function "<<opt_arg<<" not supported."<<endl;
                    return -1;
                }
                break;
            case 'd':
                //dir
                opt_arg = optarg;
                dbDir.clear();
                dbDir.append(opt_arg);
                break;
			case 's':
                //dbfile
                opt_arg = optarg;
                dbSqlPath.clear();
                dbSqlPath.append(opt_arg);
                break;
			case 'p':
				opt_arg = optarg;
				picPath.clear();
				picPath.append(opt_arg);
				break;
            case '?':
                cout<<"help information"<<endl;
                return -1;
                break;
        }
    }
    if(mode == CREATE_MODE)
    {
    	createDatabase(dbSqlPath, dbDir);
    }
    else if(mode == SEARCH_MODE)
    {
    	searchDatabase(dbSqlPath, picPath);
    }
    return 0;
}
