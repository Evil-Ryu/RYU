/*
 * =====================================================================================
 *
 *       Filename:  math.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/14 21:08:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef RYU_MATH_H
#define RYU_MATH_H

#include <math.h>
#define PI 3.14159263538979


namespace ryu_math { 

    double clamp(double x, double a, double b);
    double max(double a, double b);
    double min(double a, double b);
    double abs(double a);
    void swap(double &a, double &b);
    float hash();
    float sfrand( int *seed );
}

using namespace ryu_math;

struct vec3 {       
    double x, y, z;       
    vec3(double r=0, double g=0, double b=0){ x=r; y=g; z=b; } 
    vec3 operator+=(const vec3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    vec3 operator-=(const vec3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    vec3 operator*=(const vec3 &v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    vec3 operator/=(const vec3 &v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
    friend vec3 operator+(const vec3 &a, const vec3 &b) {return vec3(a.x+b.x, a.y+b.y, a.z+b.z); } 
    friend vec3 operator+(double a, const vec3 &b) {return vec3(a+b.x, a+b.y, a+b.z); } 
    friend vec3 operator+(const vec3 &a, double b) {return vec3(a.x+b, a.y+b, a.z+b); } 
    friend vec3 operator-(const vec3 &a, const vec3 &b) {return vec3(a.x-b.x, a.y-b.y, a.z-b.z); } 
    friend vec3 operator-(double a, const vec3 &b) {return vec3(a-b.x, a-b.y, a-b.z); } 
    friend vec3 operator-(const vec3 &a, double b) {return vec3(a.x-b, a.y-b, a.z-b); } 
    friend vec3 operator*(const vec3 &a, const vec3 &b) {return vec3(a.x*b.x, a.y*b.y, a.z*b.z); } 
    friend vec3 operator*(double a, const vec3 &b) {return vec3(a*b.x, a*b.y, a*b.z); } 
    friend vec3 operator*(const vec3 &a, double b) {return vec3(a.x*b, a.y*b, a.z*b); } 
    friend vec3 operator/(const vec3 &a, const vec3 &b) {return vec3(a.x/b.x, a.y/b.y, a.z/b.z); } 
    friend vec3 operator/(double a, const vec3 &b) {return vec3(a/b.x, a/b.y, a/b.z); } 
    friend vec3 operator/(const vec3 &a, double b) {return vec3(a.x/b, a.y/b, a.z/b); } 
    friend bool operator==(const vec3 &a, const vec3 &b) {return (a.x==b.x) && (a.y == b.y) && (a.z == b.z); } 
    friend bool operator!=(const vec3 &a, const vec3 &b) {return (a.x!=b.x) || (a.y != b.y) || (a.z != b.z); } 
    friend vec3 cross(const vec3 &a, const vec3 &b){return vec3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);} 
    friend vec3 normalize(const vec3 &b) { 
        double len = sqrt(b.x*b.x + b.y*b.y + b.z*b.z);
        if(len > 0.0)
            return b * (1.0/len);
        else  return vec3(0.0, 0.0, 0.0);
    }
    friend double dot(const vec3 &a, const vec3 &b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
    friend vec3 pow(const vec3 &a, const vec3 &b) {
        return vec3(pow(a.x, b.x), pow(a.y, b.y), pow(a.z, b.z));
    }
    friend vec3 mix(const vec3 &x, const vec3 &y, double a) {
        return x*(1.0 - a) + y*a; 
    }
    friend vec3 sin(const vec3 &b) { return vec3(sin(b.x), sin(b.y), sin(b.z));}
    friend vec3 cos(const vec3 &b) { return vec3(cos(b.x), cos(b.y), cos(b.z));}
    friend vec3 modf(const vec3 &b) { double i; return vec3(modf(b.x, &i), modf(b.y, &i), modf(b.z, &i)); }
    friend double length(const vec3 &b) { return sqrt(b.x*b.x + b.y*b.y + b.z*b.z); } 
    friend vec3 reflect(const vec3 &a, const vec3 &n) {
        return normalize(a - n*dot(a,n)*2.0);
    }
    friend vec3 clamp(const vec3 &v, double a, double b) {
        return vec3(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b));
    }
    friend vec3 abs(const vec3 &v) {
        return vec3(abs(v.x), abs(v.y), abs(v.z));
    }
}; 

struct vec4 {
    double x, y, z, w;
    vec4(double r=0, double g=0, double b=0, double a=0){ x=r; y=g; z=b; w=a;}
};


vec3 uniform_sample_hemisphere(double u1, double u2);
vec3 uniform_sample_disk(double u1, double u2);
vec3 cosine_sample_hemisphere(double u1, double u2);
vec3 sample_on_random_unit_disk(vec3 disk_normal, double u1, double u2, vec3 (* sample_func)(double, double));
vec3 sample_on_random_unit_hemisphere(vec3 normal, double u1, double u2, vec3 (* sample_func)(double, double));

#endif
