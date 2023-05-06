#include "../../include/cuda_trace_functions.hpp"

__device__ struct v3d ray_intersect_sphere(struct ray ray, struct sphere sphere) {

    struct v3d oc = v_sub(ray.origin, sphere.center);
    float a = v_scal(ray.direction, ray.direction);
    float b = 2.0f * v_scal(oc, ray.direction);
    float c = v_scal(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4.0f * a * c;

    struct v3d no_intersection = {INFINITY, INFINITY, INFINITY};

    if (discriminant < 0) {
        return no_intersection;
    } else {
        // Use the smaller positive solution, if available
        float t0 = (-b - sqrt(discriminant)) / (2.0f * a);
        float t1 = (-b + sqrt(discriminant)) / (2.0f * a);
        
        float t;
        if (t0 > 0) {
            t = t0;
        } else if (t1 > 0) {
            t = t1;
        } else {
            return no_intersection;
        }

        struct v3d intersection_point = v_add(ray.origin, v_mul(ray.direction, t));
        return intersection_point;
    }
}

__device__ struct v3d get_closest_intersection(struct ray ray, struct sphere** closest_sphere) {
    if (closest_sphere && *closest_sphere) {
        *closest_sphere = NULL;
    }
    struct v3d closest_intersection = {INFINITY, INFINITY, INFINITY};
    float closest_distance = INFINITY;

    for (int i = 0; i < sphere_count; i++) {
        //dbg(5,"Checking collision with sphere at {%f,%f,%f}",spheres[i].center.x,spheres[i].center.y,spheres[i].center.z);
        struct v3d intersection_point = ray_intersect_sphere((struct ray) ray, spheres[i]);
        
        if (intersection_point.x != INFINITY) {
            //dbg(5,"Collision with sphere %d",i);
            float distance = v_norm(v_sub(intersection_point, ray.origin));
            
            if (distance < closest_distance) {
                if (closest_sphere) *closest_sphere = &spheres[i];
                closest_intersection = intersection_point;
                closest_distance = distance;
            }
        }
    }

    return closest_intersection;
} 