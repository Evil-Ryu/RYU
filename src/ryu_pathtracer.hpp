/*
 * =====================================================================================
 *
 *       Filename:  ryu_pathtracer.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/14 23:49:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RYU_PATHTRACER_H
#define RYU_PATHTRACER_H

#include "ryu_algebra.hpp"
#include "ryu_raymarcher.hpp"
#include "ryu_context.hpp"
using namespace ryu_math;


struct pathtracer_t {
    double aperture;
    double fov;
    double focus_dist;
    double blur;
    int depth ;
    int path_num;
    vec3 target;
    vec3 ray_origin;
    vec3 up;

    short unsigned int x16v[3];  // for erand48

    int image_width, image_height;

    ray_marcher_t *raymarcher;

    vec3 (*lighting_func)(vec3, vec3, vec3);

    pathtracer_t(ray_marcher_t *raymarcher,
                 ryu_context *cxt);

    ~pathtracer_t();


    void fresnel(vec3 half, vec3 view, vec3 normal, double n, double &kr, double &kt);
    void metallic_fresnel(vec3 half, vec3 view, vec3 normal, double n, double k,
                          double &kr, double &kt);

    vec3 cook_torrance(vec3 normal, vec3 view, vec3 light, vec3 lightcol);

    double phong(vec3 normal, vec3 incident, vec3 light, double power);

    vec3 lighting(vec3 p, vec3 n, vec3 rd);
    //
    // sky light is a dome, we could sample the sky done with a 
    // cosine ditribution based sampling, which sends more samples in
    // the direction of the normal and less to the sides proportionally to 
    // the cosine term, therefore achieving the same effect while casting
    // far less rays.
    //
    // In this following function, there is 80% diffuse and 20% glossy
    // the glossiness cone angle of 0.9 radians
    //
    vec3 brdf_ray(vec3 n, vec3 rd);

    vec3 render(vec3 ro, vec3 rd, vec3 uv, int depth);
    vec3 trace(int x, int y);

    vec3 background(vec3 uv, vec3 ro, vec3 rd); 

};



#endif
