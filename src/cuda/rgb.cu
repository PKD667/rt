#include "SDL2/SDL_stdinc.h"

__device__ void set_pixel_rgba(Uint32 *pixel, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
    *pixel = (red << 24) | (green << 16) | (blue << 8) | alpha;
}
__device__ void set_pixel_rgb(Uint32 *pixel, Uint8 red, Uint8 green, Uint8 blue) {
    set_pixel_rgba(pixel, red, green, blue, 255);
}


__device__ void get_pixel_rgb(Uint32 pixel, Uint8 *red, Uint8 *green, Uint8 *blue) {
    *red   = (pixel >> 24) & 0xFF;
    *green = (pixel >> 16) & 0xFF;
    *blue  = (pixel >> 8)  & 0xFF;
}