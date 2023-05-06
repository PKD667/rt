#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

#include "../include/cutils.h"
#include "../include/main.h"

int load_spheres(const char *filename, struct sphere *spheress);

void load_assets() {
    l_sphere_count = load_spheres("assets/spheres.json", l_spheres);
    dbg(1,"Loaded %d l_spheres",l_sphere_count);
    for (int i;i < l_sphere_count; i++) {
        dbg(2,"Sphere %d:",i);
        dbg(2,"   x: %f y: %f z: %f",l_spheres[i].center.x,l_spheres[i].center.y,l_spheres[i].center.z);
        dbg(2,"   r: %f",l_spheres[i].radius);
        dbg(2,"   color: %x",l_spheres[i].material.color);
        dbg(2,"   gloss: %f",l_spheres[i].material.gloss);
        dbg(2,"   light_source: %d",l_spheres[i].material.light_source);
    }
    //load_triangles("assets/triangles.json", triangles);
}

int load_spheres(const char *filename, struct sphere *spheress) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        msg(ERROR,"Error opening incident, file");
        return -1;
    }
    dbg(3,"File opened");

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length + 1);
    if (!data) {
        msg(ERROR,"Error allocating memory for file data");
        fclose(file);
        return -1;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *json_spheres = cJSON_Parse(data);
    if (!json_spheres) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(data);
        return -1;
    }
    free(data);
    dbg(3,"Json parsed");

    int num_spheres = cJSON_GetArraySize(json_spheres);
    if (num_spheres > MAX_SPHERES) {
        num_spheres = MAX_SPHERES;
    }

    for (int i = 0; i < num_spheres; i++) {
        cJSON *json_sphere = cJSON_GetArrayItem(json_spheres, i);
        cJSON *json_center = cJSON_GetObjectItem(json_sphere, "center");
        cJSON *json_radius = cJSON_GetObjectItem(json_sphere, "radius");
        cJSON *json_material = cJSON_GetObjectItem(json_sphere, "material");
        cJSON *json_color = cJSON_GetObjectItem(json_material, "color");
        cJSON *json_gloss = cJSON_GetObjectItem(json_material, "gloss");
        cJSON *json_light_source = cJSON_GetObjectItem(json_material, "light_source");

        if (json_center && cJSON_IsObject(json_center) &&
            json_radius && cJSON_IsNumber(json_radius) &&
            json_color && cJSON_IsString(json_color) &&
            json_gloss && cJSON_IsNumber(json_gloss) &&
            json_light_source && cJSON_IsBool(json_light_source)) {
            l_spheres[i].center.x = cJSON_GetObjectItem(json_center, "x")->valuedouble;
            l_spheres[i].center.y = cJSON_GetObjectItem(json_center, "y")->valuedouble;
            l_spheres[i].center.z = cJSON_GetObjectItem(json_center, "z")->valuedouble;
            l_spheres[i].radius = json_radius->valuedouble;
            sscanf(json_color->valuestring, "0x%x", &l_spheres[i].material.color);
            l_spheres[i].material.gloss = json_gloss->valuedouble;
            l_spheres[i].material.light_source = json_light_source->valueint;
        } else {
            fprintf(stderr, "Error: Invalid sphere data at index %d\n", i);
        }
    }

    cJSON_Delete(json_spheres);
    dbg(3,"Returning with %d l_spheres",num_spheres);
    return num_spheres;
}

