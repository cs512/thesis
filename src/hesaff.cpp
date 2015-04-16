/*
 * Copyright (C) 2008-12 Michal Perdoch
 * All rights reserved.
 *
 * This file is part of the HessianAffine detector and is made available under
 * the terms of the BSD license (see the COPYING file).
 * 
 */

#include <iostream>
#include <fstream>

#include <cv.h>
#include "hesaff/pyramid.h"
#include "hesaff/helpers.h"
#include "hesaff/affine.h"
#include "hesaff/siftdesc.h"
#include "hesaff/hesaff.h"

using namespace std;
using namespace cv;


AffineHessianDetector::AffineHessianDetector(const Mat &image, const PyramidParams &par, const AffineShapeParams &ap, const SIFTDescriptorParams &sp) :
  HessianDetector(par),
  AffineShape(ap),
  image(image),
  sift(sp)
  {
     this->setHessianKeypointCallback(this);
     this->setAffineShapeCallback(this);
     this->callbackHandler = NULL;
  }

void AffineHessianDetector::onHessianKeypointDetected(const Mat &blur, float x, float y, float s, float pixelDistance, int type, float response)
  {
     findAffineShape(blur, x, y, s, pixelDistance, type, response);
  }

void AffineHessianDetector::onAffineShapeFound(
    const Mat &blur, float x, float y, float s, float pixelDistance,
    float a11, float a12,
    float a21, float a22,
    int type, float response, int iters)
{
    // convert shape into a up is up frame
    rectifyAffineTransformationUpIsUp(a11, a12, a21, a22);

    // now sample the patch
    if (!normalizeAffine(image, x, y, s, a11, a12, a21, a22))
    {
        // compute SIFT
        sift.computeSiftDescriptor(this->patch);
        // store the keypoint
        float desc[128] = {0};
        for (int i=0; i<128; i++)
            desc[i] = sift.vec[i];
        if(this->callbackHandler != NULL)
        {
            this->callbackHandler->onAffineHessianFound(desc);
        }
    }
}

void AffineHessianDetector::setKeypointHandler(AffineHessianCallback* callback)
{
    this->callbackHandler = callback;
}

//void _main_(int argc, char **argv)
//{
//   if (argc>1)
//   {
//      Mat tmp = imread(argv[1]);
//      Mat image(tmp.rows, tmp.cols, CV_32FC1, Scalar(0));
//
//      float *out = image.ptr<float>(0);
//      unsigned char *in  = tmp.ptr<unsigned char>(0);
//
//      for (size_t i=tmp.rows*tmp.cols; i > 0; i--)
//      {
//         *out = (float(in[0]) + in[1] + in[2])/3.0f;
//         out++;
//         in+=3;
//      }
//
//      HessianAffineParams par;
//      double t1 = 0;
//      {
//         // copy params
//         PyramidParams p;
//         p.threshold = par.threshold;
//
//         AffineShapeParams ap;
//         ap.maxIterations = par.max_iter;
//         ap.patchSize = par.patch_size;
//         ap.mrSize = par.desc_factor;
//
//         SIFTDescriptorParams sp;
//         sp.patchSize = par.patch_size;
//
//         AffineHessianDetector detector(image, p, ap, sp);
//         t1 = getTime();
//         detector.detectPyramidKeypoints(image);
////         cout << "Detected " << g_numberOfPoints << " keypoints and " << g_numberOfAffinePoints << " affine shapes in " << getTime()-t1 << " sec." << endl;
//
//         char suffix[] = ".hesaff.sift";
//         int len = strlen(argv[1])+strlen(suffix)+1;
//         char buf[len];
//         snprintf(buf, len, "%s%s", argv[1], suffix); buf[len-1]=0;
//         ofstream out(buf);
////         detector.exportKeypoints(out);
//      }
//   } else {
//      printf("\nUsage: hesaff image_name.ppm\nDetects Hessian Affine points and describes them using SIFT descriptor.\nThe detector assumes that the vertical orientation is preserved.\n\n");
//   }
//}
