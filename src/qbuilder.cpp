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

extern "C"
{
#include <vl/sift.h>
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
}

QBuilder::~QBuilder()
{
    if(this->selfInitial)
    {
    }
}

vector<bitset<256>> QBuilder::getDescriptor(const string path)
{
    Mat imageGray = imread(path, 0);
    vector<bitset<256>> ret;

    float *image = new float[imageGray.rows*imageGray.cols];
    for (int i = 0; i < imageGray.rows; ++i)
    {
        for (int j = 0; j < imageGray.cols; ++j)
        {
            image[j + imageGray.cols*i] = imageGray.at<unsigned char>(i, j);
        }
    }
    vl_size height = imageGray.rows;
    vl_size width = imageGray.cols;

    VlSiftFilt *siftDet = vl_sift_new(width, height, -1, 3, 0);

    // begin sift
    if (vl_sift_process_first_octave(siftDet, image) != VL_ERR_EOF)
    {
        while (1)
        {
            vl_sift_detect(siftDet);

            VlSiftKeypoint *pkeypoint = siftDet->keys;

            for (int i = 0; i < siftDet->nkeys; i ++)
            {
                VlSiftKeypoint tempkeypoint = *pkeypoint;
                pkeypoint++;

//                area[idx_point][0] = tempkeypoint.x;
//                area[idx_point][1] = tempkeypoint.y;
//                area[idx_point][2] = tempkeypoint.sigma/2;
                double angles[4];

                // The function computes the orientation(s) of the keypoint k.
                // The function returns the number of orientations found (up to four).
                // The orientations themselves are written to the vector angles.
                int angleCount = vl_sift_calc_keypoint_orientations(siftDet, angles, &tempkeypoint);

//                area[idx_point][3] = angleCount;
//                idx_point ++;

                for (int j = 0; j < angleCount; ++ j)
                {
                    //printf("%d: %f\n", j, angles[j]);

                    float *descriptors = new float[128];
                    vl_sift_calc_keypoint_descriptor(siftDet, descriptors, &tempkeypoint, angles[j]);

                    ret.push_back(this->p_qlizer(descriptors));

                    delete []descriptors;
                }

            }

            // vl_sift_process_next_octave:
            // The function computes the next octave of the Gaussian scale space.
            // Notice that this clears the record of any feature detected in the previous octave.
            if (vl_sift_process_next_octave(siftDet) == VL_ERR_EOF)
            {
                break;
            }

        }
    }
    vl_sift_delete(siftDet);

    delete []image;

    return ret;
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
