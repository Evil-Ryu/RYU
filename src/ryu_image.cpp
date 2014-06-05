/*
 * =====================================================================================
 *
 *       Filename:  ryu_image.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/14 23:42:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include "ryu_image.hpp"

image_t::image_t(int w, int h, int epp) {
    this->width = w;
    this->height = h;
    this->epp = epp;

    data = (void *)malloc(sizeof(double) * epp * width * height);
}

image_t::~image_t() {
    if(data) {
        free(data);
    }
}

void image_t::post_effect() {
    if(!data) {
        printf("Error:No image allocated.\n");
        return;
    }
    vec3 *image = (vec3 *)data;
    // post
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            vec3 &col = image[i * width + j];
            double qy = (double)i / (double)height;
            double qx = (double)j / (double)width;

            col = pow(clamp(col,0.0,1.0),vec3(0.45, 0.45, 0.45));  // gama
            col = col*0.6 + col*col*1.2 - col*col*col*0.8; // contrast
            double satu = dot(col, vec3(0.33, 0.33, 0.33));
            col = mix(col, vec3(satu, satu, satu), -0.5);  // satuation
            col =col * (0.5+0.5*pow(16.0*qx*qy*(1.0-qx)*(1.0-qy),0.7));  // vigneting
            col = clamp(col, 0.0, 1.0) * 255.0;
        }
    }
}

