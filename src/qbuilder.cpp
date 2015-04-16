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

using namespace std;
using namespace cv;

bool comp(float a,float b)
{
    return a>b;
}

QBuilder::QBuilder()
{
    this->selfInitial = true;
//    initModule_nonfree();
//    this->detector = Algorithm::create<Feature2D>("Feature2D.SIFT");
//    this->detector = new AffineHessianDetector;
}

QBuilder::~QBuilder()
{
    if(this->selfInitial)
    {
//        delete this->detector;
//        this->detector.release();
    }
}

QBuilder::QBuilder(Ptr<Feature2D> detector)
{
//    this->detector = detector;
    this->selfInitial = false;
}

//vector<bitset<256>> QBuilder::getDescriptor(const string path)
//{
//    Mat imageGray = imread(path, 0);
//    Mat descriptors;
//    vector<KeyPoint> keypoints;
//    (*(this->detector))(imageGray, noArray(), keypoints, descriptors);
//#ifdef DEBUG
//    ofstream fo(path+".sift");
//    for(auto i = 0; i < descriptors.rows; ++i)
//    {
//        for(auto j = 0; j < descriptors.cols; ++j)
//        {
//            fo << descriptors.at<float>(i, j) << ' ';
//        }
//        fo << endl;
//    }
//    fo.close();
//#endif
//    return this->p_qlizer(descriptors);
//}

//vector<bitset<256>> QBuilder::p_qlizer(Mat &descriptors)
//{
//    vector<bitset<256>> ret;
//    for(auto i = 0; i < descriptors.rows; ++i)
//    {
//        //auto rowPtr = (float *)&(descriptors.row(i).col(0).data[0]);
//        auto rowPtr = &(descriptors.at<float>(i, 0));
//        float sortedRow[128] = {0};
//        memcpy(sortedRow, rowPtr, 128*sizeof(float));
//        sort(sortedRow, sortedRow+128, comp);
//        auto f1 = sortedRow[64] + sortedRow[63];
//        f1 /= 2.0f;
//        auto f2 = sortedRow[32] + sortedRow[31];
//        f2 /= 2.0f;
//
//        bitset<256> rowScalar;
//        for(auto j = 0; j < 128; ++j)
//        {
//            if(rowPtr[j] < f1)
//            {
//                rowScalar[j] = 0;
//                rowScalar[j+128] = 0;
//            }
//            else if(rowPtr[j] < f2)
//            {
//                rowScalar[j] = 1;
//                rowScalar[j+128] = 0;
//            }
//            else
//            {
//                rowScalar[j] = 1;
//                rowScalar[j+128] = 1;
//            }
//        }
//        ret.push_back(rowScalar);
//    }
//
//    return ret;
//}

vector<bitset<256>> QBuilder::getDescriptor(const string path)
{
//    Mat imageGray = imread(path, 0);
//    Mat image(imageGray, CV_32FC1);
    Mat tmp = imread(path);
    Mat imageGray(tmp.rows, tmp.cols, CV_32FC1, Scalar(0));

    float *out = imageGray.ptr<float>(0);
    unsigned char *in  = tmp.ptr<unsigned char>(0);

    for (size_t i=tmp.rows*tmp.cols; i > 0; i--)
    {
        *out = (float(in[0]) + in[1] + in[2])/3.0f;
        out++;
        in+=3;
    }

    this->ret = vector<bitset<256>>();

    PyramidParams p;
    p.threshold = par.threshold;

    AffineShapeParams ap;
    ap.maxIterations = par.max_iter;
    ap.patchSize = par.patch_size;
    ap.mrSize = par.desc_factor;

    SIFTDescriptorParams sp;
    sp.patchSize = par.patch_size;

    AffineHessianDetector detector(imageGray, p, ap, sp);
    detector.setKeypointHandler(this);
    detector.detectPyramidKeypoints(imageGray);
    return this->ret;
}

bitset<256> QBuilder::p_qlizer(const float *descriptors)
{
        //auto rowPtr = (float *)&(descriptors.row(i).col(0).data[0]);
    auto rowPtr = descriptors;
    float sortedRow[128] = {0};
    memcpy(sortedRow, rowPtr, 128*sizeof(float));
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

void QBuilder::onAffineHessianFound(const float * desc)
{
    this->ret.push_back(this->p_qlizer(desc));
}
