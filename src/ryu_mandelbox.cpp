/*
 * =====================================================================================
 *
 *       Filename:  ryu_mandelbox.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/28/14 20:46:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "ryu_mandelbox.hpp"
#include "ryu_raymarcher.hpp"
#include <cassert>

namespace ryu_mandelbox {
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
        rm_params->dist_eps             = 0.003;
        rm_params->shadow_march_steps   = 128;
        rm_params->shadow_occlusion_eps = 1e-5;
        rm_params->normal_eps           = 1e-6;


        pt_params_t *pt_params = cxt->pathtracing_params;
        pt_params->aperture             = 0.6;
        pt_params->fov                  = 3.5;
        pt_params->focus_dist           = 1.2;
        pt_params->blur                 = 0.00115;
        pt_params->depth                = 5;
        pt_params->path_num             = 1; //------------
        pt_params->target               = vec3(0.0, 0.0, 0.0);
        pt_params->ray_origin           = vec3(0.0, 0.0, 3.9);
        pt_params->up                   = vec3(0.0, 1.0, 0.0);

    }





    // parameters for this fractal
    // change these parameters as needed
    double fixed_radius2 = 1.9;
    double min_radius2 = 0.1;
    double scale = -2.6;
    double folding_limit = 1.0;
    int iterations = 40;
    vec3 mtl = vec3(1.0, 1.0, 2.0);


    void sphere_fold(vec3 &z, double &dz) {
        double r2 = dot(z, z);
        if(r2 < min_radius2) {
            double temp = (fixed_radius2 / min_radius2);
            z = z * temp;
            dz = dz * temp;
        }else if(r2 < fixed_radius2) {
            double temp = (fixed_radius2 / r2);
            z = z * temp;
            dz = dz * temp;
        }
    }

    void box_fold(vec3 &z, double &dz) {
        z = clamp(z, -folding_limit, folding_limit) * 2.0 - z;
    }

    vec4 mandelbox(vec3 z) {
        vec3 offset = z;
        double dr = 1.0;
        for(int n = 0; n < iterations; ++n) {
            box_fold(z, dr);
            sphere_fold(z, dr);

            z = scale * z + offset;
            dr = dr * abs(scale) + 1.0; // scalar running derivative
        }
        double r = length(z);
        return vec4(r / abs(dr), mtl.x, mtl.y, mtl.z);
    }

    vec4 f(vec3 p){ 
#define OBJ_NUM 1
        vec4 objs[OBJ_NUM];

        //ryu_raymarcher_toolbox::ry(p, -0.625);
        objs[0] = mandelbox(p);
        vec4 res = vec4(1e10, 0.0, 0.0, 0.0);
        for(int i = 0; i < OBJ_NUM; ++i) {
            if(res.x > objs[i].x) 
                res = objs[i];
        }
        return res;
    } 


}
