
#ifndef WJZ_SCHEDULER_H
#define WJZ_SCHEDULER_H

#include "qbuilder.h"
#include <string>

using namespace std;

class Scheduler
{
public:
	Scheduler();
	~Scheduler();
	Scheduler(QBuilder& qb, const string path = "", const string filter = ".jpg\\|.png\\|.tiff");
	int run();
private:
	QBuilder *qb;
	bool selfInitial;
	string path;
	string filter;
	int p_runTask(const string path);
};

#endif
