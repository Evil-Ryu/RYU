



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>




#include "ryu_render.hpp"
#include "ryu_mandelbulb.hpp"
#include "ryu_mandelbox.hpp"
#include "ryu_kifs.hpp"
#include "ryu_context.hpp"


#define WIDTH 1920
#define HEIGHT 1080
//#define WIDTH 640
//#define HEIGHT 480



using namespace ryu_math;
//using namespace ryu_mandelbulb;
//using namespace ryu_mandelbox;
using namespace ryu_menger_sponge;


static image_t *image;
static pathtracer_t *pathtracer;
static ray_marcher_t *raymarcher;
static render_t *render;



image_t *init_image() {
    image_t *img = new image_t(WIDTH, HEIGHT, 3);
    return img;
}

ray_marcher_t *init_raymarcher(rm_params_t *rm_params) {
    ray_marcher_t *rm = new ray_marcher_t(rm_params);
    return rm;
}

pathtracer_t *init_pathtracer(ray_marcher_t *rm, ryu_context *cxt) {
    if(rm == NULL) return NULL;

    pathtracer_t *pt = new pathtracer_t(rm, cxt);
    return pt;
}

render_t *init_render(image_t *image, pathtracer_t *pt) {
    if(image == NULL || pt == NULL) return NULL;

    render_t *render =  new render_t(pt, image, 32, true);
    return render;
}

template<class T>
void safe_release(T *p) {
    if(p)
        delete p;
}


int main() {
    ryu_context *context = new ryu_context(WIDTH, HEIGHT);
    init_params(context);

    image = init_image();
    raymarcher = init_raymarcher(context->raymarching_params);
    pathtracer = init_pathtracer(raymarcher, context);
    render = init_render(image, pathtracer);

    render->banzai();

    safe_release(render);
    safe_release(pathtracer);
    safe_release(raymarcher);
    safe_release(image);
    return 0;
}
