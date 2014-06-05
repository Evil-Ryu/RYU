/*
 * =====================================================================================
 *
 *       Filename:  ryu_raymarcher.h
 *
 *    Description:  a raymarcher framework as well as toolbox
 *
 *        Version:  1.0
 *        Created:  05/23/14 21:22:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RYU_RAYMARCHER_H
#define RYU_RAYMARCHER_H

#include "ryu_algebra.hpp"
#include "ryu_shader_params.hpp"
#include <cmath>

using namespace ryu_math;

struct ray_marcher_t {

    vec4 (*f)(vec3);  // implement this outside
    double var_eps; // ray should be considered intersected if its dist to object below this threshold.
    int max_march_steps;
    double fudge;  // lower this if missing details, [0,1]
    double clarity_power;
    double dither;  // can be used to remove banding, [0, 1]
    double dist_eps;
    double shadow_march_steps;
    double normal_eps;
    double shadow_occlusion_eps;

    ray_marcher_t(rm_params_t *params);

    ~ray_marcher_t();

    double softshadow(vec3 ro, vec3 rd, double k );

    double shadow(vec3 ro, vec3 rd);

    vec3 normal(vec3 p);

    //  comparing to the previous simpler one, this version gives more control, but the effect is the same.
    vec4 intersect(vec3 ro, vec3 rd);



    
};


namespace ryu_raymarcher_toolbox {

    //--------------tool box-----------------------------------------------------

    void ry(vec3 &p, double a);
    void rx(vec3 &p, double a);

    /* the scalar distance estimator------------------------------------------------*/

    vec4 sphere(vec3 p, double r, vec3 mtl);
    

    vec4 plane(vec3 p, double y, vec3 mtl);

    // t.x is the outter radius, t.y is the inner radius
    vec4 sd_torus(vec3 p, vec3 t, vec3 mtl);
}

#endif
