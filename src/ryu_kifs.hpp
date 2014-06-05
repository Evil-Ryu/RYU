/*
 * =====================================================================================
 *
 *       Filename:  ryu_kifs.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/31/14 16:45:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liming
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RYU_KIFS_H
#define RYU_KIFS_H


#include "ryu_algebra.hpp"
#include "ryu_context.hpp"


namespace ryu_menger_sponge {
    vec4 menger_sponge(vec3 p);
    vec4 f(vec3 p);
    void init_params(ryu_context *cxt);
}




#endif
