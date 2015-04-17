/*
 * ASiftDetector.h
 *
 *  Created on: 2015-4-17
 *      Author: Matt Sheckells
 *      URL:    http://www.mattsheckells.com/opencv-asift-c-implementation/
 */

#ifndef ASIFTDETECTOR_H_
#define ASIFTDETECTOR_H_

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;

class ASiftDetector
{
public:
  ASiftDetector();

  void detectAndCompute(const Mat& img, std::vector< KeyPoint >& keypoints, Mat& descriptors);

private:
  void affineSkew(double tilt, double phi, Mat& img, Mat& mask, Mat& Ai);
};

#endif /* ASIFTDETECTOR_H_ */
