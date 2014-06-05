/*
 * =====================================================================================
 *
 *       Filename:  ryu_mandelbox.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/28/14 20:47:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RYU_MANDELBOX_H
#define RYU_MANDELBOX_H

#include "ryu_algebra.hpp"
#include "ryu_context.hpp"

namespace ryu_mandelbox {
    void sphere_fold(vec3 &z, double &dz);
    void box_fold(vec3 &z, double &dz);
    vec4 mandelbox(vec3 z);
    vec4 f(vec3 p);
    void init_params(ryu_context *cxt);
}


#endif
