/*
 * =====================================================================================
 *
 *       Filename:  ryu_algebra.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/14 23:11:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ryu_algebra.hpp"

namespace ryu_math { 

    float seed = 0.;  // for hash

    double clamp(double x, double a, double b) {
        return x < a ? a : (x > b ? b : x);
    }
    double max(double a, double b) {
        return a > b ? a : b;
    }
    double min(double a, double b) {
        return a < b ? a : b;
    }
    double abs(double a) {
        return a > 0.0 ? a : -a;
    }
    void swap(double &a, double &b) {
        double tmp = a;
        a = b;
        b = tmp;
    }


    float hash() { double i; return modf(sin(seed++)*43758.5453123, &i); }

    float sfrand( int *seed )
    {
        float res;

        seed[0] *= 16807;

        *((unsigned int *) &res) = ( ((unsigned int)seed[0])>>9 ) | 0x40000000;

        return( res-3.0f );
    }
}

vec3 uniform_sample_hemisphere(double u1, double u2) {
    float r = sqrt(1.0 - u1 * u1);
    float phi = 2 * PI * u2;
    return vec3(r*cos(phi), r*sin(phi), u1);
}

vec3 uniform_sample_disk(double u1, double u2) {
    double phi,r;
    double a = 2*u1 - 1;
    double b = 2*u2 - 1;
    b = (b == 0.0) ? b + 0.1 : b;
    if (a*a > b*b) { // use squares instead of absolute values
        r = a;
        phi = (PI/4)*(b/a);
    } else {
        r = b;
        phi = (PI/2)-(PI/4)*(a/b);
    }
    return vec3(r*cos(phi), r*sin(phi), 1.0 );
}

vec3 cosine_sample_hemisphere(double u1, double u2) {
    vec3 ret;
    ret = uniform_sample_disk(u1, u2); //could use simpler one here.
    ret.z = sqrt(max(0.0, 1.0 - ret.x*ret.x - ret.y*ret.y));
    return ret;
}

vec3 sample_on_random_unit_disk(vec3 disk_normal, double u1, double u2, vec3 (* sample_func)(double, double)) {
    vec3 point = sample_func(u1, u2);

    vec3 cs = cross(disk_normal,vec3(0.0,1.0,0.0));
    if(cs.x + cs.y + cs.z != 0.0) {
        cs = normalize(cs);
        vec3 cu = normalize(cross(cs,disk_normal)); 
        point = normalize(point.x*cs + point.y*cu + point.z*disk_normal);  
        //printf("%f, %f, %f, %f, %f, %f\n", cs.x, cs.y, cs.z, disk_normal.x, disk_normal.y, disk_normal.z);
    }
    //printf("%f, %f, %f\n", disk_normal.x, disk_normal.y, disk_normal.z);
    return point; 
}

vec3 sample_on_random_unit_hemisphere(vec3 normal, double u1, double u2, vec3 (* sample_func)(double, double)){
    vec3 point = sample_func(u1, u2);

    vec3 cs = cross(normal, vec3(0.0,1.0,0.0));
    if(cs.x + cs.y + cs.z != 0.0) {
        cs = normalize(cs);
        vec3 cu = normalize(cross(cs, normal)); 
        point = normalize(point.x*cs + point.y*cu + point.z*normal);  
    }
    return point; 
}

