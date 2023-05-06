#pragma once
#include <SDL2/SDL_stdinc.h>
#include "shared.h"
// Maths functions
__device__ double v_scal(struct v3d a, struct v3d b);
__device__ double v_norm(struct v3d a);
__device__ struct v3d v_sub(struct v3d a, struct v3d b);
__device__ struct v3d v_add(struct v3d a, struct v3d b);
__device__ struct v3d v_mul(struct v3d a, double b);
__device__ struct v3d v_normalize(struct v3d v);
__device__ struct v3d v_cross(struct v3d v1,struct v3d v2);


__device__ void set_pixel_rgb(Uint32 *pixel, Uint8 red, Uint8 green, Uint8 blue);
__device__ void get_pixel_rgb(Uint32 pixel, Uint8 *red, Uint8 *green, Uint8 *blue);



__device__ extern struct sphere spheres[MAX_SPHERES]; 
__device__ extern int sphere_count;
__device__ extern struct camera cam;

__device__ struct v3d ray_intersect_sphere(struct ray ray, struct sphere sphere);
__device__ struct v3d get_closest_intersection(struct ray ray, struct sphere** closest_sphere);