/*
 * =====================================================================================
 *
 *       Filename:  ryu_shader_params.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/01/14 21:33:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RYU_SHADER_PARAMS_H
#define RYU_SHADER_PARAMS_H
#include "ryu_algebra.hpp"

// params for raymarching and pathtracing
struct rm_params_t {
    int    max_march_steps;
    int    shadow_march_steps;
    double fudge;
    double clarity_power;
    double dither;
    double dist_eps;
    double shadow_occlusion_eps;
    double normal_eps;
    vec4 (*f)(vec3);
};

struct pt_params_t {
    double aperture;
    double fov;
    double focus_dist;
    double blur;
    double depth;
    double path_num;
    vec3 target;
    vec3 ray_origin;
    vec3 up;
};


#endif
