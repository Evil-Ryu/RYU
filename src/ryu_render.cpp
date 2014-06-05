/*
 * =====================================================================================
 *
 *       Filename:  ryu_render.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/24/14 15:20:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ryu_render.hpp"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>


render_t::render_t(pathtracer_t *tracer, image_t *img, int tilesize = 32, bool multi_thread = true) {
    this->tracer = tracer;
    this->image = img;
    this->width = img->width;
    this->height = img->height;
    this->tilesize = tilesize;
    this->multi_thread = multi_thread;
}

render_t::~render_t() {

}

// only update r times
// bar width w
// process has done x out of n rounds
static void progress_bar(const int x, const int n, const int r, const int w){
    static int prev_x = 0;  // avoid backsteping when runs multithreaded
    if(prev_x > x) return;
    prev_x = x;

    if(x%(n/r)!=0)return;
    double ratio=x/(double)n;
    int c=ratio*w;
    printf("%3d%% [",(int)(ratio*100));
    for(int x=0;x<c;++x)printf("=");
    for(int x=c;x<w;++x)printf(" ");
    // ANSI Control codes to go back to the previous
    // line(\033[F) and clear it(\033[J)
    printf("]\n\033[F\033[J");
}

void render_t::tile_based_renderer(){
    vec3 *img = (vec3 *)(image->data);
     // tile renderer
    const int x_tiles = width / tilesize;
    const int y_tiles = height / tilesize;
    const int tiles = x_tiles * y_tiles;

    for(int tile = 0; tile < tiles; ++tile) {
        const int ia = tilesize * (tile % x_tiles);
        const int ja = tilesize * (tile / x_tiles);

        for(int y = 0; y < tilesize; ++y) {
            for(int x = 0; x < tilesize; ++x) {
                img[width * (ja + y) + (ia + x)]=tracer->trace(ia + x, ja + y);
            }
        }
        progress_bar(tile, tiles, 50, 50);
    }
}

void render_t::brute_force_renderer() {
    vec3 *img = (vec3 *)(image->data);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;++x){
            img[y*width + x]=tracer->trace(x, y);
        }
        progress_bar(y, height, 50, 50);
    }
}

void render_t::save_framebuffer_to_ppm(const char *filename) {
    vec3 *img = (vec3 *)(image->data);
    FILE *f=fopen(filename, "w");
    fprintf(f,"P3\n%d %d\n%d\n",width,height,255);
    for(int y = height - 1; y >= 0; --y) {
        for(int x = 0; x < width; ++x) {
            fprintf(f,"%d %d %d ",int(img[y*width+x].x),
                    int(img[y*width+x].y),
                    int(img[y*width+x].z));
        }
    }
    fclose(f); 
}

struct args_t {
    unsigned int tile_id;
    sem_t mutex;
    vec3 *image;
    int width;
    int height;
    int tilesize;
    pathtracer_t *tracer;
};

void *tile_based_renderer_multithread(void *args) {
    args_t *p = (args_t *)args;
    int width = p->width; 
    int height = p->height; 
    vec3 *img = p->image; 
    int tilesize = p->tilesize; 
    pathtracer_t *tracer = p->tracer;

    sem_wait(&p->mutex);

    // tile renderer
    const int x_tiles = width / tilesize;
    const int y_tiles = height / tilesize;
    const int tiles = x_tiles * y_tiles;

    for(;;) {
        const int tile = __sync_fetch_and_add(&p->tile_id, 1);
        if(tile >= tiles) break;
        //printf("%d\n", p->tile_id);

        const int ia = tilesize * (tile % x_tiles);
        const int ja = tilesize * (tile / x_tiles);

        for(int y = 0; y < tilesize; ++y) {
            for(int x = 0; x < tilesize; ++x) {
                img[width * (ja + y) + (ia + x)]=tracer->trace(ia + x, ja + y);
            }
        }
        progress_bar(tile, tiles, 50, 50);
    }

    sem_post(&p->mutex);
    pthread_exit(NULL);
}

void render_t::multi_threaded_rendering() {
    int num_cores = sysconf(_SC_NPROCESSORS_CONF);

    args_t *args = new args_t;
    sem_init(&args->mutex, 0, num_cores);
    args->tile_id = 0;
    args->width = image->width;
    args->height = image->height;
    args->image = (vec3 *)(image->data);
    args->tilesize = tilesize;
    args->tracer = tracer;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_t th[256];
    for(int i = 0; i < num_cores; ++i) {
        int rc = pthread_create(&th[i], &attr, tile_based_renderer_multithread, (void *)args);
        if(rc) {
            printf("Error:unable to create thread:%d\n", rc);
            exit(-1);
        }
    }

    pthread_attr_destroy(&attr);
    //for(int i = 0; i < num_cores; ++i) {
    //pthread_join(th[i], NULL);
    //}

    void *status;
    for(int i=0; i < num_cores; i++ ){
        int rc = pthread_join(th[i], &status);
        if (rc){
            printf("Error:unable to join %d\n", rc);
            exit(-1);
        }
        printf("Main:completed thread id %d\n", i);
        //printf("exiting with status: %d\n", *((int *)status));
    }

    image->post_effect();
    save_framebuffer_to_ppm("image.ppm");

    pthread_exit(NULL);
}

void render_t::banzai() {
    if(multi_thread) {
        multi_threaded_rendering();
    } else {
        tile_based_renderer();
    }
}

