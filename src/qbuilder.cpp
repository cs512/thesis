#include "qbuilder.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <string>
#include <algorithm>
#include <bitset>
#include <vector>
#include "debug.h"
#include <fstream>

extern "C" {
    #include "opensift/sift.h"
    #include "opensift/imgfeatures.h"
}

using namespace std;
using namespace cv;

bool comp(float a,float b)
{
    return a>b;
}

QBuilder::QBuilder()
{
    this->selfInitial = true;
    initModule_nonfree();
    this->detector = Algorithm::create<Feature2D>("Feature2D.SIFT");
}

QBuilder::~QBuilder()
{
    if(this->selfInitial)
    {
        this->detector.release();
    }
}

QBuilder::QBuilder(Ptr<Feature2D> detector)
{
    this->detector = detector;
    this->selfInitial = false;
}

vector<bitset<256>> QBuilder::getDescriptor(const string path)
{
    //Mat imageGray = imread(path, 0);
    IplImage* img = NULL;
    img = cvLoadImage( path.c_str(), 1 );
    feature* features = NULL;
    int n = sift_features(img, &features);
    cvReleaseImage(&img);
    img = NULL;
    vector<bitset<256>> descriptors;
    for(auto i = 0; i < n; ++ i)
    {
        descriptors.push_back(this->p_qlizer(features[i].descr));
    }
    free(features);
#ifdef DEBUG
    ofstream fo(path+".sift");
    for(auto i = 0; i < descriptors.rows; ++i)
    {
        for(auto j = 0; j < descriptors.cols; ++j)
        {
            fo << descriptors.at<float>(i, j) << ' ';
        }
        fo << endl;
    }
    fo.close();
#endif
    return descriptors;
}

bitset<256> QBuilder::p_qlizer(const double *descriptors)
{
    auto rowPtr = descriptors;
    float sortedRow[128] = {0};
    for(auto i = 0; i < 128; ++i)
    {
        sortedRow[i] = descriptors[i];
    }
//    memcpy(sortedRow, rowPtr, 128*sizeof(float));
    sort(sortedRow, sortedRow+128, comp);
    auto f1 = sortedRow[64] + sortedRow[63];
    f1 /= 2.0f;
    auto f2 = sortedRow[32] + sortedRow[31];
    f2 /= 2.0f;

    bitset<256> rowScalar;
    for(auto j = 0; j < 128; ++j)
    {
        if(rowPtr[j] < f1)
        {
            rowScalar[j] = 0;
            rowScalar[j+128] = 0;
        }
        else if(rowPtr[j] < f2)
        {
            rowScalar[j] = 1;
            rowScalar[j+128] = 0;
        }
        else
        {
            rowScalar[j] = 1;
            rowScalar[j+128] = 1;
        }
    }
    return rowScalar;
}
