#pragma once

#include <SDL2/SDL.h>
#include "stdbool.h"

#include "shared.h"

extern struct camera l_cam;

extern struct sphere l_spheres[MAX_SPHERES];
extern int l_sphere_count;

// intersection functions*
struct v3d get_closest_intersection(struct ray ray, struct sphere** closest_sphere);
struct v3d get_normal(struct v3d intersection_point, struct sphere* sphere);
struct v3d ray_intersect_sphere(struct ray ray, struct sphere sphere);

// shading functions
Uint32 shade(struct v3d intersection_point, struct v3d normal,struct v3d incident,struct material material);

// trace a ray and return the color
Uint32 trace(struct ray ray);

// log stuff
void log_frame_information(struct camera cam, struct sphere spheres[],int frame_number);

// asset loader
void load_assets();

