#ifndef WJZ_QBUILDER_H
#define WJZ_QBUILDER_H
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <string>
#include <bitset>
#include <vector>

extern "C"
{
#include <vl/sift.h>
}

using namespace std;
using namespace cv;

class QBuilder
{
public:
    QBuilder();
    ~QBuilder();
    vector<bitset<256>> getDescriptor(const string path);

private:
    bitset<256> p_qlizer(const float *descriptors);
    bool selfInitial;
};

#endif
