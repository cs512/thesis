#ifndef WJZ_QBUILDER_H
#define WJZ_QBUILDER_H
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <string>
#include <bitset>
#include <vector>

using namespace std;
using namespace cv;

class QBuilder
{
public:
    QBuilder();
    ~QBuilder();
    QBuilder(Ptr<Feature2D> detector);
    vector<bitset<256>> getDescriptor(const string path);

private:
    vector<bitset<256>> p_qlizer(Mat &descriptors);
    bool selfInitial;
    Ptr<Feature2D> detector;
};

#endif
