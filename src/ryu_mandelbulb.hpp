/*
 * =====================================================================================
 *
 *       Filename:  ryu_mandelbulb.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/14 21:17:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RYU_MANDELBULB_H
#define RYU_MANDELBULB_H


#include "ryu_algebra.hpp"
#include "ryu_context.hpp"

namespace ryu_mandelbulb {

    vec3 bulb_power(double r, double power, vec3 z, vec3 p);
    /*  this one could produce similar result when power = 2 with image of mandelbrot set */
    vec3 bulb_power_positive(double r, double power, vec3 z, vec3 p);
   /* the scalar distance estimator------------------------------------------------*/
    /* the normal is calculated by central difference */
    vec4 mandelbulb(vec3 p);

    vec4 f(vec3 p);
    void init_params(ryu_context *cxt);
}


#endif
