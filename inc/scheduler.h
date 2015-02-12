
#ifndef WJZ_SCHEDULER_H
#define WJZ_SCHEDULER_H

#include "qbuilder.h"
#include "finder.h"
#include <string>
#include <bitset>
#include <vector>

using namespace std;

class Scheduler
{
public:
	//Scheduler();
	~Scheduler();
	Scheduler(QBuilder& qb, Finder& fd, const string path = "", const string filter = ".jpg\\|.png\\|.tiff");
	int buildDatabase();
	int search(const string filePath);
private:
	QBuilder *qb;
	Finder *fd;
	bool selfInitial;
	string path;
	string filter;
	vector<bitset<256>> p_computeScalar(const string path);
};

#endif
