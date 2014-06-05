/*
 * =====================================================================================
 *
 *       Filename:  ryu_image.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/14 23:34:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  image processing, like post effects
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RYU_IMAGE_H
#define RYU_IMAGE_H

#include <math.h>
#include "ryu_algebra.hpp"
using namespace ryu_math;


struct image_t {

    int width;
    int height;
    int bpp;
    int epp;  // elements per pixel
    void *data;
    
    image_t(int w, int h, int epp);
    ~image_t();

    void post_effect();
};




#endif
