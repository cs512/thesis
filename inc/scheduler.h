
#ifndef WJZ_SCHEDULER_H
#define WJZ_SCHEDULER_H

#include "qbuilder.h"
#include <string>
#include <bitset>
#include <vector>

using namespace std;

class Scheduler
{
public:
	Scheduler();
	~Scheduler();
	Scheduler(QBuilder& qb, const string path = "", const string filter = ".jpg\\|.png\\|.tiff");
	int buildDatabase();
private:
	QBuilder *qb;
	bool selfInitial;
	string path;
	string filter;
	vector<bitset<256>> p_computeScalar(const string path);
};

#endif
