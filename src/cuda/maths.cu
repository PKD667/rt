#include "../../include/cudart.h"

__device__ double v_scal(struct v3d a, struct v3d b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

__device__ double v_norm(struct v3d a) {
    return sqrt(v_scal(a, a));
}



__device__ struct v3d v_sub(struct v3d a, struct v3d b) {
    return (struct v3d) {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
}
__device__ struct v3d v_add(struct v3d a, struct v3d b) {
    return (struct v3d) {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
}
__device__ struct v3d v_mul(struct v3d a, double b) {
    return (struct v3d) {
        .x = a.x * b,
        .y = a.y * b,
        .z = a.z * b
    };
}

__device__ struct v3d v_normalize(struct v3d v) {
    float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return (struct v3d) {
        .x = v.x / magnitude,
        .y = v.y / magnitude,
        .z = v.z / magnitude
    };
}

struct v3d v_cross(struct v3d v1,struct v3d v2) {
    struct v3d result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}