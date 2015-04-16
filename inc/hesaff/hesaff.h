/*
 * hesaff.h
 *
 *  Created on: 2015-4-16
 *      Author: wangjz
 */

#ifndef HESAFF_H_
#define HESAFF_H_

#include <cv.h>
#include "hesaff/pyramid.h"
#include "hesaff/helpers.h"
#include "hesaff/affine.h"
#include "hesaff/siftdesc.h"
#include "hesaff/hesaff.h"

using namespace std;

struct HessianAffineParams
{
   float threshold;
   int   max_iter;
   float desc_factor;
   int   patch_size;
   bool  verbose;
   HessianAffineParams()
      {
         threshold = 16.0f/3.0f;
         max_iter = 16;
         desc_factor = 3.0f*sqrt(3.0f);
         patch_size = 41;
         verbose = false;
      }
};

class AffineHessianCallback
{
public:
    virtual void onAffineHessianFound(const float * desc) = 0;
};

class AffineHessianDetector : public HessianDetector, AffineShape, HessianKeypointCallback, AffineShapeCallback
{
   const Mat image;
   SIFTDescriptor sift;
   AffineHessianCallback* callbackHandler;
public:
   AffineHessianDetector(const Mat &image, const PyramidParams &par, const AffineShapeParams &ap, const SIFTDescriptorParams &sp);

   void onHessianKeypointDetected(const Mat &blur, float x, float y, float s, float pixelDistance, int type, float response);

   void onAffineShapeFound(
      const Mat &blur, float x, float y, float s, float pixelDistance,
      float a11, float a12,
      float a21, float a22,
      int type, float response, int iters);

   void setKeypointHandler(AffineHessianCallback* callback);

};


#endif /* HESAFF_H_ */
