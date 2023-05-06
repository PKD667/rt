#pragma once
#include <SDL2/SDL_stdinc.h>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define FOV (40.0f * M_PI / 180.0f) // Field of view (in radians)
#define ASPECT_RATIO (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT
#define MAX_SPHERES 100

struct v3d {
  double x, y, z;
};

#define V_NULL (struct v3d){0,0,0}

struct material {
    Uint32 color;
    float gloss;

    bool light_source;
};

struct sphere {
    struct v3d center;
    double radius;
    struct material material;
};

struct triangle {
    struct v3d a;
    struct v3d b;
    struct v3d c;
};

struct ray {
    struct v3d origin;
    struct v3d direction;
};

struct camera {
    struct v3d position;
    struct v3d forward;
    struct v3d up;
    struct v3d right;
    float yaw;
    float pitch;
};
