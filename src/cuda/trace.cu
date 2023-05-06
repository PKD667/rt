#include "SDL2/SDL_stdinc.h"
#include "../../include/cudart.h"
#include "../../include/cuda_trace_functions.hpp"



__device__ struct sphere spheres[MAX_SPHERES]; 
__device__ int sphere_count;
__device__ struct camera cam;

__device__ void prandint(int* state) {
    *state = *state * 1103515245 + 12345;
    *state = (*state / 65536) % 32768;

}



__device__ struct v3d get_normal(struct v3d intersection_point, struct sphere* sphere) {
    struct v3d normal = v_sub(intersection_point, sphere->center);
    normal = v_normalize(normal);
    return normal;
}

__device__ Uint32 trace(struct ray ray) {
    
    struct sphere* closest_sphere = NULL;
    struct v3d closest_intersection = get_closest_intersection(ray, &closest_sphere);

    if (closest_sphere == NULL) {
        return 0x000000;
    } else if (closest_sphere->material.light_source) {
        return closest_sphere->material.color;
    }

    float color_intensity = 0.0f;

    struct v3d normal = get_normal(closest_intersection, closest_sphere);

    struct v3d light_position = (struct v3d) {0, 0, 0};
    for (int i = 0; i < sphere_count; i++) {
        if (spheres[i].material.light_source) {
            light_position = spheres[i].center;
        }

        // calculate diffuse lighting
        struct v3d light_direction = v_sub(light_position, closest_intersection);
        light_direction = v_normalize(light_direction);

        float diffuse = v_scal(normal, light_direction);
        if (diffuse > 0) {
            color_intensity += diffuse;
        }

        // calculate specular lighting

        struct v3d reflection = v_sub(v_mul(normal, 2 * v_scal(normal, light_direction)), light_direction);
        reflection = v_normalize(reflection);

        struct v3d view_direction = v_sub(cam.position, closest_intersection);
        view_direction = v_normalize(view_direction);

        float specular = pow(v_scal(reflection, view_direction), closest_sphere->material.gloss);
        if (specular > 0) {
            color_intensity += specular;
        }

        // calculate shadows



    }



    if (color_intensity > 1.0f) {
        color_intensity = 1.0f;
    }

    Uint8 r,g,b;
    get_pixel_rgb(closest_sphere->material.color, &r, &g,&b);

    r = r * color_intensity;
    g = g * color_intensity;
    b = b * color_intensity;

    Uint32 final_color;
    set_pixel_rgb(&final_color, r, g, b);
    return final_color;
}

__global__ void cuda_kernel(Uint32* pixels,int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) {
        return;
    }

    float px = (2.0f * ((x + 0.5f) / (float)width) - 1.0f) * tanf(FOV * 0.5f) * ASPECT_RATIO;
    float py = (1.0f - 2.0f * ((y + 0.5f) / (float)height)) * tanf(FOV * 0.5f);

    struct v3d ray_direction = v_normalize(v_add(v_add(v_mul(cam.right, px), v_mul(cam.up, py)), cam.forward));

    // Set the pixel at (x, y) based on the ray tracing result
    pixels[y * width + x] = trace((struct ray) {
        .origin = cam.position,
        .direction = ray_direction
    });
}

void launch_tracer(Uint32 pixels[SCREEN_HEIGHT][SCREEN_WIDTH], struct camera l_cam,struct sphere* l_spheres, int l_sphere_count) {

    // Copy the camera and sphere data to the GPU
    cudaMemcpyToSymbol(cam, &l_cam, sizeof(struct camera));
    cudaMemcpyToSymbol(spheres, l_spheres, sizeof(struct sphere) * l_sphere_count);
    cudaMemcpyToSymbol(sphere_count, &l_sphere_count, sizeof(int));
    
    // Allocate memory on the GPU for the pixel buffer
    Uint32* d_pixels;
    size_t bufferSize = sizeof(Uint32) * SCREEN_HEIGHT * SCREEN_WIDTH;
    cudaMalloc(&d_pixels, bufferSize);

    // Define the block and grid dimensions
    dim3 blockDim(16, 16);
    dim3 gridDim((SCREEN_WIDTH + blockDim.x - 1) / blockDim.x, (SCREEN_HEIGHT + blockDim.y - 1) / blockDim.y);

    // Launch the CUDA kernel
    cuda_kernel<<<gridDim, blockDim>>>(d_pixels,SCREEN_WIDTH, SCREEN_HEIGHT);

    // Copy the pixel data from the GPU to the host memory
    cudaMemcpy(pixels, d_pixels, bufferSize, cudaMemcpyDeviceToHost);

    // Free the allocated memory on the GPU
    cudaFree(d_pixels);

    // Make sure the kernel execution is complete before returning
    cudaDeviceSynchronize();
}
