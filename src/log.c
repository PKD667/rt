#include "../include/main.h"
#include <stdio.h>


void log_frame_information(struct camera cam, struct sphere spheres[],int frame_number) {
    FILE *logfile = NULL;
    char filename[64];
    snprintf(filename, sizeof(filename), "logs/frame_%04d.log", frame_number);

    logfile = fopen(filename, "w");
    if (logfile == NULL) {
        printf("Failed to open log file: %s\n", filename);
        return;
    }

    // Log camera information
    fprintf(logfile, "Camera:\n");
    fprintf(logfile, "  Position: (%f, %f, %f)\n", cam.position.x, cam.position.y, cam.position.z);
    fprintf(logfile, "  Direction: (%f, %f, %f)\n", cam.forward.x, cam.forward.y, cam.forward.z);
    fprintf(logfile, "\n");

    // Log sphere information
    for (int i = 0; i < 2; i++) {
        struct sphere sphere = spheres[i];
        fprintf(logfile, "Sphere %d:\n", i + 1);
        fprintf(logfile, "  Center: (%f, %f, %f)\n", sphere.center.x, sphere.center.y, sphere.center.z);
        fprintf(logfile, "  Radius: %f\n", sphere.radius);
        fprintf(logfile, "  Material Color: 0x%08X\n", sphere.material.color);
        fprintf(logfile, "\n");
    }

    fclose(logfile);
}
