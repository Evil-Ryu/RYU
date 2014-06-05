/*
 * =====================================================================================
 *
 *       Filename:  ryu_render.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/24/14 15:20:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RYU_RENDER_H
#define RYU_RENDER_H


#include "ryu_image.hpp"
#include "ryu_pathtracer.hpp"


struct render_t {
    image_t *image;
    pathtracer_t *tracer;
    int tilesize;
    bool multi_thread;
    int width, height;


    render_t(pathtracer_t *tracer, image_t *img, int tilesize, bool multi_thread);
    ~render_t();

    void tile_based_renderer();
    void brute_force_renderer();
    void multi_threaded_rendering();
    void banzai(); // the main rendering process

    friend void *::tile_based_renderer_multithread(void *args);


    void save_framebuffer_to_ppm(const char *filename) ;
};




#endif
