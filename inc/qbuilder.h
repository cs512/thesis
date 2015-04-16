#ifndef WJZ_QBUILDER_H
#define WJZ_QBUILDER_H
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <string>
#include <bitset>
#include <vector>

#include "hesaff/hesaff.h"
#include "hesaff/affine.h"

using namespace std;
using namespace cv;

class QBuilder : AffineHessianCallback
{
public:
    QBuilder();
    ~QBuilder();
    QBuilder(Ptr<Feature2D> detector);
    vector<bitset<256>> getDescriptor(const string path);
    void onAffineHessianFound(const float * desc);

private:
    bitset<256> p_qlizer(const float * descriptors);
    bool selfInitial;
    HessianAffineParams par;
    vector<bitset<256>> ret;
//    Ptr<Feature2D> detector;
//    AffineHessianDetector* detector;
};

#endif
