/*
 * =====================================================================================
 *
 *       Filename:  ryu_pathtracer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/14 23:51:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */


#include "ryu_pathtracer.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace ryu_math;

pathtracer_t::pathtracer_t(ray_marcher_t *raymarcher,
                           ryu_context *cxt) {

    pt_params_t *params = cxt->pathtracing_params;

    this->aperture     = params->aperture;
    this->fov          = params->fov;
    this->focus_dist   = params->focus_dist;
    this->blur         = params->blur;
    this->depth        = params->depth;
    this->path_num     = params->path_num;
    this->target       = params->target;
    this->ray_origin   = params->ray_origin;
    this->up           = params->up;
    this->image_height = cxt->framebuffer_height;
    this->image_width  = cxt->framebuffer_width;
    this->raymarcher   = raymarcher;

    this->x16v[0] = 0;
    this->x16v[1] = 0;
    this->x16v[2] = 0;
}

pathtracer_t::~pathtracer_t() {}

static vec3 g_sundir = normalize(vec3(-3.5, 8.0, 2.8)); 
static vec3 g_suncol= vec3(1.64, 1.27, 0.99); 
//static vec3 g_suncol= vec3(1.37, 0.99, 0.79);
static vec3 g_skycol = vec3(0.6, 0.5, 1.0); 

// n is the refraction index, for example, water's n is 1.333
// output value kr, kt are reflectance and transmittance.
void pathtracer_t::fresnel(vec3 half, vec3 view, vec3 normal, double n, double &kr, double &kt) {
    double fac = pow(1.0 - dot(view, half), 5.0);
    kr = ((n-1.0)*(n-1.0) + 4.0*n*fac) / ((n+1.0)*(n+1.0));
    kt = 1.0 - kr;
}

// the refraction indices is a complex number n+ki
// this is an approximation presented in the paper:http://sirkan.iit.bme.hu/~szirmay/fresnel.pdf
void pathtracer_t::metallic_fresnel(vec3 half, vec3 view, vec3 normal, double n, double k,
                                    double &kr, double &kt) {
    double fac = pow(1.0 - dot(view, half), 5.0);
    kr = ((n-1.0)*(n-1.0) + 4.0*n*fac + k*k) / ((n+1.0)*(n+1.0) + k*k);
    kt = 1.0 - kr;
}


//returns a coefficient of specular reflection
vec3 pathtracer_t::cook_torrance(vec3 normal, vec3 view, vec3 light, vec3 lightcol) {

    double refraction_index = 1.5;
    double roughness = 0.2;
    double gauss_const = 100.0;

    vec3 half = normalize(view + light);

    double ndotv = dot(normal, view);
    double ndoth = dot(normal, half);
    double ndotl = dot(normal, light);
    double vdoth = dot(view, half);

    // fresnel
    double refl_ratio, refr_ratio;
    fresnel(half, view, normal, refraction_index, refl_ratio, refr_ratio);
    //metallic_fresnel(half, view, normal, refraction_index, 5.0, refl_ratio, refr_ratio);

    // steps below should be calculated for each light then summed up, we here assume only one light exist.

    // microfacet distribution
    double alpha = acos(ndoth);
    double d = gauss_const * exp(-(alpha*alpha) / (roughness*roughness));

    // geometric attenuation factor
    double g = min(1.0, min(2.0*ndoth*ndotv/vdoth, 2.0*ndoth*ndotl/vdoth));

    double fspec = (refl_ratio*d*g)/(PI*ndotv*ndotl);

    double fac = pow(1.0 - ndotl, 5.0);
    double fdiff = ((refraction_index-1.0)*(refraction_index-1.0) + 4.0*refraction_index*fac) / ((refraction_index+1.0)*(refraction_index+1.0));

    fdiff = (1.0 - fdiff / (2.0 * PI));

    //return  (fspec + fdiff) * lightcol * ndotl;
    return  lightcol * ndotl + fspec;
}

double pathtracer_t::phong(vec3 normal, vec3 incident, vec3 light, double power) {
    double spe = max(0.0, pow(clamp(dot(light, reflect(incident, normal)), 0.0, 1.0), power)); 
    return spe;
}

vec3 pathtracer_t::lighting(vec3 p, vec3 n, vec3 rd) {
    vec3 dcol = vec3(0.0, 0.0, 0.0);
    vec3 light_ray;
    vec3 point;

    // sample sun
    {
        point = 1000.0 * g_sundir + 50.0 * sample_on_random_unit_disk(n, erand48(x16v), erand48(x16v), uniform_sample_disk);
        light_ray = normalize(point - p);
        //double cosine = max(0.0, dot(light_ray, n));
        //double spe = phong(n, rd, g_sundir, 16.0);
        //double spe = cook_torrance(n, vec3(-rd.x, -rd.y, -rd.z), g_sundir);
        //dcol = dcol + 4.5 * cosine * g_suncol * raymarcher->shadow(p, light_ray) +  spe;
        dcol = dcol + 6.5 * cook_torrance(n, vec3(-rd.x, -rd.y, -rd.z), light_ray, g_suncol) * raymarcher->shadow(p, light_ray);
    }

    // sample sky
    {
        point = 1000.0 * sample_on_random_unit_hemisphere(n, erand48(x16v), erand48(x16v), cosine_sample_hemisphere); 
        light_ray = normalize(point - p); 
        //dcol = dcol + 0.6 * g_skycol * raymarcher->shadow(p, light_ray);
        dcol = dcol + 1.5 * cook_torrance(n, vec3(-rd.x, -rd.y, -rd.z), light_ray, g_skycol) * raymarcher->shadow(p, light_ray);
    }
    return dcol;
}

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
vec3 pathtracer_t::brdf_ray(vec3 n, vec3 rd) {
    if(erand48(x16v) < 0.4) {
        return sample_on_random_unit_hemisphere(n, erand48(x16v), erand48(x16v), cosine_sample_hemisphere); 
    }else {
        //return coneDirection(reflect(rd, n), 0.9);
        return reflect(rd, n);
    }
}

vec3 pathtracer_t::background(vec3 uv, vec3 ro, vec3 rd) {
    vec3 bg = exp(-length(vec3(uv.x, uv.z, 0.0)))*vec3(0.6, 0.9, 1.5);
    //double halo = clamp(dot(normalize(vec3(-ro.x, -ro.y, -ro.z)), rd), 0.0, 1.0); 
    //vec3 col=bg*vec3(2.0,1.0,1.0)*pow(halo,17.0); 


    //vec3 bg = exp(uv.y - 1.0) * vec3(0.5, 0.5, 0.5);
    vec3 col = bg;

    return col;
}

vec3 pathtracer_t::render(vec3 ro, vec3 rd, vec3 uv, int depth) {
    if(depth == 0) return vec3(0.0, 0.0, 0.0);

    double normal_back_step = 1.0;

    vec3 bg = background(uv, ro, rd);

    vec3 col = bg;

    vec4 res = raymarcher->intersect(ro, rd);

    if(res.x > 0.0){
        vec3 scol = vec3(res.y, res.z, res.w);
        vec3 p = ro + rd * res.x;
        vec3 n = raymarcher->normal(p);
        //vec3 n=normal(p - normal_back_step * var_eps * rd);

        vec3 dcol = lighting(p, n, rd);

        rd = brdf_ray(n, rd);
        vec3 icol = render(ro, rd, uv, depth-1);

        col = (dcol + icol) * 0.2 * scol;;
        col = mix(col, bg, 1.0-exp(-0.0001*res.x*res.x));
    }

    return col;
}

vec3 pathtracer_t::trace(int x, int y) {
    vec3 col = vec3(0.0, 0.0, 0.0);


    // 256 x antialiasing
    for(int i = 0; i < path_num; ++i) {

        vec3 q=(vec3(x, y) + vec3(erand48(x16v), erand48(x16v)))/vec3(image_width, image_height, 1.0); 
        //vec3 q=(vec3(x, y) )/vec3(image_width, image_height, 1.0); 
        vec3 p = q * 2.0 - 1.0; 
        p.x*=double(image_width)/double(image_height);

        vec3 ta = target;
        vec3 ro = ray_origin;

        vec3 cf = normalize(ta-ro); 
        vec3 cs = normalize(cross(cf,up));
        vec3 cu = normalize(cross(cs,cf)); 


        // create ray with depth of field
        vec3 er = normalize(vec3(p.x, p.y, fov));
        vec3 rd = normalize(er.x*cs + er.y*cu + er.z*cf);  

        vec3 go = blur * vec3(erand48(x16v), erand48(x16v), 0.0);
        vec3 gd = normalize(er * focus_dist - go);
        ro = ro + go.x*cs + go.y*cu;
        rd = rd + gd.x*cs + gd.y*cu;

        col = col + render(ro, normalize(rd), p, depth);
    }
    col = col / double(path_num);
    return col;
}


