/*
 * =====================================================================================
 *
 *       Filename:  ryu_kifs.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/31/14 16:47:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ryu_kifs.hpp"
#include "ryu_raymarcher.hpp"
#include <cassert>
#include <cstdio>

namespace ryu_menger_sponge {
    using namespace ryu_math;

    void init_params(ryu_context *cxt) {
        assert(cxt != NULL);
        assert(cxt->raymarching_params != NULL);
        assert(cxt->pathtracing_params != NULL);

        rm_params_t *rm_params = cxt->raymarching_params;
        rm_params->f                    = f;
        rm_params->max_march_steps      = 256;
        rm_params->fudge                = 0.5;
        rm_params->clarity_power        = 0.5;
        rm_params->dither               = 1.0;
        rm_params->dist_eps             = 1e-3;
        rm_params->shadow_march_steps   = 128;
        rm_params->shadow_occlusion_eps = 1e-5;
        rm_params->normal_eps           = 1e-6;


        pt_params_t *pt_params = cxt->pathtracing_params;
        pt_params->aperture             = 0.6;
        pt_params->fov                  = 5.5;
        pt_params->focus_dist           = sqrt(1.75*1.75+1.85*1.85+1.4*1.4 )* 0.6;
        pt_params->blur                 = 0.02115;
        pt_params->depth                = 5;
        pt_params->path_num             = 256;
        pt_params->target               = vec3(0.0, 0.5, 0.0);
        pt_params->ray_origin           = vec3(3.5, 3.7, 2.8) * 0.5;
        pt_params->up                   = vec3(0.0, 1.0, 0.0);

    }




    int iterations = 7;
    double scale = 3.0;
    double cx = 1.0, cy = 1.1, cz = 1.2;  
    vec3 mtl = vec3(1.2, 0.89, 0.69);

    vec4 menger_sponge(vec3 p) { 
        for(int i = 0; i < iterations; ++i){
            p = abs(p); 

            if(p.x - p.y < 0.0) { p = vec3(p.y, p.x, p.z); } 
            if(p.x - p.z < 0.0) { p = vec3(p.z, p.y, p.x); } 
            if(p.y - p.z < 0.0) { p = vec3(p.x, p.z, p.y); } 

            ryu_raymarcher_toolbox::ry(p, 0.05);
            p.x = scale * p.x - cx * (scale - 1.0); 
            p.y = scale * p.y - cy * (scale - 1.0); 
            p.z = scale * p.z; 

            if(p.z > 0.5 * cz * (scale - 1.0)) { p.z -= (scale - 1.0); } 
        } 
        return vec4((length(p) - 0.0) * pow(scale, double(-iterations)), mtl.x, mtl.y, mtl.z);
    } 


    vec4 f(vec3 p){ 
        ryu_raymarcher_toolbox::ry(p, -0.2);
#define OBJ_NUM 1
        vec4 objs[OBJ_NUM];

        objs[0] = menger_sponge(p);

        vec4 res = vec4(1e10, 0.0, 0.0, 0.0);
        for(int i = 0; i < OBJ_NUM; ++i) {
            if(res.x > objs[i].x) 
                res = objs[i];
        }
        return res;
    } 


}
