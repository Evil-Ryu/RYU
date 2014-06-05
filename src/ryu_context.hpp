/*
 * =====================================================================================
 *
 *       Filename:  ryu_context.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/01/14 21:45:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RYU_CONTEXT_H
#define RYU_CONTEXT_H

#include "ryu_shader_params.hpp"
#include "ryu_algebra.hpp"


struct ryu_context {
    rm_params_t *raymarching_params;
    pt_params_t *pathtracing_params;
    int framebuffer_width;
    int framebuffer_height;
    bool gpu_render;
    bool multithreaded;
    bool tile_based_render;



    ryu_context(int width, int height);
    ~ryu_context();
};


#endif
