#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


#include "../include/main.h"
#include "../include/cutils.h"
#include "../include/cudart.h"


struct sphere l_spheres[MAX_SPHERES]; 
int l_sphere_count;
struct camera l_cam;

int main(int argc, char* args[]) {
    l_cam = (struct camera) {
        .position = { .x = 0, .y = 0, .z = -500 },
        .forward = { .x = 0, .y = 0, .z = 1 },
        .up = { .x = 0, .y = 1, .z = 0 },
        .right = { .x = 1, .y = 0, .z = 0 }
    };
    DEBUG = 3;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    window = SDL_CreateWindow("Display Pixels", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL) {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    Uint32 pixels[SCREEN_HEIGHT][SCREEN_WIDTH];
    
    // all pixel are black
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            pixels[y][x] = 0x00000000;
        }
    }
    // load 
    load_assets();

    bool quit = false;
    SDL_Event e;

    time_t start_time = time(NULL);
    int frame_count;
    char title[64];

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        launch_tracer(pixels, l_cam, l_spheres, l_sphere_count);
        
        // chnage l_camera position with keyboard
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_UP]) {
            l_cam.position.y += 1;
        }
        if (state[SDL_SCANCODE_DOWN]) {
            l_cam.position.y -= 1;
        }
        if (state[SDL_SCANCODE_LEFT]) {
            l_cam.position.x -= 1;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            l_cam.position.x += 1;
        }
        if (state[SDL_SCANCODE_W]) {
            l_cam.position.z += 1;
        }
        if (state[SDL_SCANCODE_S]) {
            l_cam.position.z -= 1;
        }

        SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        time_t current_time = time(NULL);
        double elapsed_time = difftime(current_time, start_time);
        sprintf(title,"%f",frame_count++/elapsed_time);
        SDL_SetWindowTitle(window, title);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
