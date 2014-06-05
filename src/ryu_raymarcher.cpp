/*
 * =====================================================================================
 *
 *       Filename:  ryu_raymarcher.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/14 23:17:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ryu_raymarcher.hpp"

ray_marcher_t::ray_marcher_t(rm_params_t *params) {

    this->f = params->f;
    this->max_march_steps = params->max_march_steps;
    this->fudge = params->fudge;
    this->clarity_power = params->clarity_power;
    this->dither = params->dither;
    this->dist_eps = params->dist_eps;
    this->shadow_march_steps = params->shadow_march_steps;
    this->shadow_occlusion_eps = params->shadow_occlusion_eps;
    this->normal_eps = params->normal_eps;

    this->var_eps = 0.0;
}

ray_marcher_t::~ray_marcher_t() {

}

double ray_marcher_t::softshadow(vec3 ro, vec3 rd, double k ){ 
    double akuma=1.0,h=0.0; 
    double t = 0.01;
    for(int i=0; i < shadow_march_steps; ++i){ 
        h=f(ro+rd*t).x; 
        if(h<shadow_occlusion_eps)return 0.02; 
        akuma=min(akuma, k*h/t); 
        t = t + clamp(h,0.01,2.0); 
    } 
    return akuma; 
} 

double ray_marcher_t::shadow(vec3 ro, vec3 rd){ 
    double t = 0.01, h;
    //for(int i = 0; i < SHADOW_RAY_STEP; ++i) {
    while(t < 20.0){
        h = f(ro + rd * t).x;
        if(h < 0.001) return 0.0;
        t += h;
    }
    return 1.0;
}

vec3 ray_marcher_t::normal(vec3 p){ 
    double eps = normal_eps;
    vec3 ex=vec3(eps, 0.0, 0.0); 
    vec3 ey=vec3(0.0, eps, 0.0); 
    vec3 ez=vec3(0.0, 0.0, eps); 
    vec3 nor= normalize(vec3(f(p+ex).x-f(p-ex).x, f(p+ey).x-f(p-ey).x, f(p+ez).x-f(p-ez).x)); 
    return nor;
} 

//  comparing to the previous simpler one, this version gives more control, but the effect is the same.
vec4 ray_marcher_t::intersect(vec3 ro, vec3 rd)
{
    double t = 0.0;
    vec4 res = vec4(-1.0, -1.0, -1.0, -1.0);
    vec4 h = vec4(1.0, 1.0, 1.0, 1.0);

    for( int i=0; i<max_march_steps; i++ )
    {
        if( h.x < var_eps || t>20.0 ) {
            res.x -= (var_eps - h.x); // moving back
            break;
        }

        h = f(ro + rd*t);
        h.x *= fudge;

        // dither
        if(i == 0) {
            h.x *= (dither * hash()) + (1.0 - dither);
        }

        t = t + h.x;  // marching
        res = vec4(t, h.y, h.z, h.w);

        var_eps = pow(t, clarity_power) * dist_eps;
    }
    if( t>20.0 ) res=vec4(-1.0, -1.0, -1.0, -1.0);
    return res;
}










namespace ryu_raymarcher_toolbox {
    using namespace ryu_math;

    //--------------tool box-----------------------------------------------------

    void ry(vec3 &p, double a){  
        double c,s;vec3 q=p;  
        c = cos(a); s = sin(a);  
        p.x = c * q.x + s * q.z;  
        p.z = -s * q.x + c * q.z; 
    }  
    void rx(vec3 &p, double a){  
        double c,s;vec3 q=p;  
        c = cos(a); s = sin(a);  
        p.y = c * q.y - s * q.z;  
        p.z = s * q.y + c * q.z; 
    }  

    /* the scalar distance estimator------------------------------------------------*/

    vec4 sphere(vec3 p, double r, vec3 mtl) {
        double len = length(p) - r;
        return vec4(len, mtl.x, mtl.y, mtl.z);
    }

    vec4 plane(vec3 p, double y, vec3 mtl) {
        double len = length(vec3(p.x, y, p.z) - p);
        return vec4(len, mtl.x, mtl.y, mtl.z);
    }

    vec4 cube(vec3 p, double size, vec3 mtl){
        double len = max(max(std::abs(p.x)-size,
                             std::abs(p.y)-size),
                         std::abs(p.z)-size);
        return vec4(len, mtl.x, mtl.y, mtl.z);
    }

    // t.x is the outter radius, t.y is the inner radius
    vec4 sd_torus(vec3 p, vec3 t, vec3 mtl) {
        double len = sqrt(p.x*p.x + p.z*p.z);
        vec3 q = vec3(len-t.x, p.y, 0.0);
        return vec4(length(q) - t.y, mtl.x, mtl.y, mtl.z);
    }

}

