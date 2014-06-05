/*
 * =====================================================================================
 *
 *       Filename:  ryu_mandelbulb.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/24/14 16:12:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ryu_mandelbulb.hpp"
#include <math.h>
#include <stdio.h>
#include <cassert>

namespace ryu_mandelbulb {

    using namespace ryu_math;

    void init_params(ryu_context *cxt) {
        assert(cxt != NULL);
        assert(cxt->raymarching_params != NULL);
        assert(cxt->pathtracing_params != NULL);

        rm_params_t *rm_params = cxt->raymarching_params;
        rm_params->f                    = f;
        rm_params->max_march_steps      = 256;
        rm_params->fudge                = 1.0;
        rm_params->clarity_power        = 0.5;
        rm_params->dither               = 1.0;
        rm_params->dist_eps             = 1e-6;
        rm_params->shadow_march_steps   = 128;
        rm_params->shadow_occlusion_eps = 1e-5;
        rm_params->normal_eps           = 1e-6;


        pt_params_t *pt_params = cxt->pathtracing_params;
        pt_params->aperture             = 0.6;
        pt_params->fov                  = 3.5;
        pt_params->focus_dist           = 1.2;
        pt_params->blur                 = 0.00115;
        pt_params->depth                = 5;
        pt_params->path_num             = 2;
        pt_params->target               = vec3(0.0, 0.0, 0.0);
        pt_params->ray_origin           = vec3(0.0, 2.0, 5.9);
        pt_params->up                   = vec3(0.0, 1.0, 0.0);

    }





    //------------------globals---------------------------
    static const double POWER = 8.0;
    static const int ITER = 24;
    //static const vec3 mtl = vec3(0.6, 0.8, 0.6);
    static const vec3 mtl = vec3(1.0, 1.1, 1.0);
    static const double BAILOUT = 4.0;


    //-------------------functions-------------------------
    vec3 bulb_power(double r, double power, vec3 z, vec3 p) {
        double theta = acos(z.y / r) * power;
        double phi = atan(z.z / z.x) * power;
        r = pow(r, power);
        return vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi)) * r + p;
    }

    /*  this one could produce similar result when power = 2 with image of mandelbrot set */
    vec3 bulb_power_positive(double r, double power, vec3 z, vec3 p) {
        double theta = atan(z.y / z.x) * power;
        double phi = asin(z.z / r) * power;
        r = pow(r, power);
        return vec3(cos(theta)*cos(phi), sin(theta)*cos(phi), sin(phi)) * r + p;
    }


    /* the scalar distance estimator------------------------------------------------*/
    /* the normal is calculated by central difference */
    vec4 mandelbulb(vec3 p) {
        p = vec3(p.x, p.z, p.y);
        vec3 z = p;
        double power = POWER;
        double r, theta, phi;
        double dr = 1.0;
        double t0 = 1.0;
        for(int i = 0; i < ITER; ++i) {
            r = length(z);
            if(r > BAILOUT) break;
            dr = pow(r, power - 1.0) * dr * power + 1.0; 
            z = bulb_power_positive(r, power, z, p);
            //z = bulb_power(r, power, z, p);
            t0 = min(t0, length(z));  // orbit trap, used to mimic ao
        }
        //t0 = pow(clamp(t0, 0.0, 1.0), 5.55);
        //vec3 mtl = 0.5 + 0.5 * sin(3.0 + 4.2 * t0 + vec3(0.0, 0.5, 1.0));
        //vec3 mtl = vec3(t0, t0, t0);
        return vec4(0.5 * log(r) * r / dr, mtl.x, mtl.y, mtl.z);
    }



    vec4 f(vec3 p){ 
        /*ry(p, stime);*/
#define OBJ_NUM 1
        vec4 objs[OBJ_NUM];

        objs[0] = mandelbulb(p);

        vec4 res = vec4(1e10, 0.0, 0.0, 0.0);
        for(int i = 0; i < OBJ_NUM; ++i) {
            if(res.x > objs[i].x) 
                res = objs[i];
        }
        return res;
    } 


}


