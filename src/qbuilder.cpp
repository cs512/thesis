#include "qbuilder.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <string>
#include <algorithm>
#include <bitset>
#include <vector>

using namespace std;
using namespace cv;

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
	Mat imageGray = imread(path, 0);
	Mat descriptors;
	vector<KeyPoint> keypoints;
	(*(this->detector))(imageGray, noArray(), keypoints, descriptors);
	return this->p_qlizer(descriptors);
}

vector<bitset<256>> QBuilder::p_qlizer(Mat descriptors)
{
	vector<bitset<256>> ret;
	cout<<descriptors.rows<<endl;
	for(auto i = 0; i < descriptors.rows; ++i)
	{
		auto rowPtr = &(descriptors.at<float>(i));
		float *sortedRow = new float[128];
		memcpy(sortedRow, rowPtr, 128*sizeof(float));
		sort(sortedRow, sortedRow+127);
		auto f1 = sortedRow[64] + sortedRow[65];
		f1 /= 2.0f;
		auto f2 = sortedRow[32] + sortedRow[33];
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
		ret.push_back(rowScalar);
	}

	return ret;
}
