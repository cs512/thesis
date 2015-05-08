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

static bool comp(float a,float b)
{
    return a>b;
}

static unsigned char build3_3Bins(const float *desc)
{
    int arr[9] = {0};
    int maxx = 0;
    for(auto i = 0; i < 8; ++i)
    {
        arr[i] = desc[i] + desc[i + 8] + desc[i + 16]
               + desc[i + 32] + desc[i + 40] + desc[i + 48]
               + desc[i + 96] + desc[i + 104] + desc[i + 112];
        maxx = (maxx > arr[i]) ? maxx : arr[i];
    }
    float tot = 0, count = 0;
    for(auto i = 0; i < 8; ++i)
    {
        if(arr[i] == maxx)
        {
            count += 1.0;
            tot += arr[i];
        }
    }
    tot /= count * 2;
    unsigned char ret = tot;
    ret &= 0b11;
    return ret;
}

static unsigned char build2_2Bins(const float *desc)
{
    int arr[9] = {0};
    int maxx = 0;
    for(auto i = 0; i < 8; ++i)
    {
        arr[i] = desc[i] + desc[i + 8] + desc[i + 32] + desc[i + 40];
        maxx = (maxx > arr[i]) ? maxx : arr[i];
    }
    float tot = 0, count = 0;
    for(auto i = 0; i < 8; ++i)
    {
        if(arr[i] == maxx)
        {
            count += 1.0;
            tot += arr[i];
        }
    }
    tot /= count * 2;
    unsigned char ret = tot;
    ret &= 0b11;
    return ret;
}

static unsigned char build1_1Bins(const float *desc)
{
    float tot = 0, count = 0;
    int maxx = 0;
    int arr[9] = {0};
    for(auto i = 0; i < 8; ++i)
    {
        arr[i] = desc[i];
        maxx = (maxx > arr[i]) ? maxx : arr[i];
    }
    for(auto i = 0; i < 8; ++i)
    {
        if(arr[i] == maxx)
        {
            count += 1.0;
            tot += arr[i];
        }
    }
    tot /= count * 2;
    unsigned char ret = tot;
    ret &= 0b11;
    return ret;
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
    float sortedRow[128] = {0};
    auto rowPtr = sortedRow;
    for(auto i = 0; i < 128; ++i)
    {
        sortedRow[i] = descriptors[i];
    }
    //auto rowPtr = (float *)&(descriptors.row(i).col(0).data[0]);
    unsigned char grayTable[] = {0, 1, 3, 2, 6, 7, 5, 4};
    bitset<256> rowScalar;
    for(auto x = 0; x < 2; ++x)
        for(auto y = 0; y < 2; ++y)
        {
            unsigned char ret = build3_3Bins(rowPtr + x * 32 + y * 8);
            int pos = x * 2 + y;
            rowScalar[pos] = (grayTable[ret & 0b111] >> 2) & 1;
            rowScalar[pos + 1] = (grayTable[ret & 0b111] >> 1) & 1;
            //rowScalar[pos + 2] = grayTable[ret & 0b111] & 1;
        }
    for(auto x = 0; x < 3; ++x)
        for(auto y = 0; y < 3; ++y)
        {
            unsigned char ret = build2_2Bins(rowPtr + x * 32 + y * 8);
            int pos = (x * 3 + y) * 2 + 8;
            rowScalar[pos] = (grayTable[ret & 0b111]) & 1;
            rowScalar[pos + 1] = (grayTable[ret & 0b111] >> 1) & 1;
            //rowScalar[pos + 2] = grayTable[ret & 0b111] & 1;
        }
    for(auto x = 0; x < 4; ++x)
        for(auto y = 0; y < 4; ++y)
        {
            unsigned char ret = build1_1Bins(rowPtr + x * 32 + y * 8);
            int pos = (x * 4 + y) * 2 + 32;
            rowScalar[pos] = (grayTable[ret & 0b111]) & 1;
            rowScalar[pos + 1] = (grayTable[ret & 0b111] >> 1) & 1;
            //rowScalar[pos + 2] = grayTable[ret & 0b111] & 1;
        }
    return rowScalar;
}
