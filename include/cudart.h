#include "shared.h"

// cuda_functions.h
#ifndef CUDA_FUNCTIONS_H
#define CUDA_FUNCTIONS_H

#ifdef __cplusplus 
extern "C" {
#endif

void launch_tracer(Uint32 pixels[SCREEN_HEIGHT][SCREEN_WIDTH], struct camera l_cam,struct sphere* l_spheres, int l_sphere_count);



#ifdef __cplusplus
}
#endif

#endif
