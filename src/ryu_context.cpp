/*
 * =====================================================================================
 *
 *       Filename:  ryu_context.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/01/14 22:01:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ryu_context.hpp"
ryu_context::ryu_context(int width, int height) :
        framebuffer_width(width),
        framebuffer_height(height),
        gpu_render(false),
        multithreaded(true),
        tile_based_render(true) {
            raymarching_params = new rm_params_t;
            pathtracing_params = new pt_params_t;
        }

ryu_context::~ryu_context() {
    if(raymarching_params) delete raymarching_params;
    if(pathtracing_params) delete pathtracing_params;
}


