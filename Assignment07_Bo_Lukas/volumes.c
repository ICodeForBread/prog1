// Author: Boyue, Lukas
#include "base.h"

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

typedef enum typetag_e {
    CYLINDER,
    CUBOID,
    SPHERE,
} TypeTag;

/*
typetag is basicly an id.
its used to determin what shape the body has
and gets used in the volume function. the switch uses it to find out what formular it has use
*/

typedef struct cylinder_s {
    double r, h;
} Cylinder;

typedef struct cuboid_s {
    double a, b, c;
} Cuboid;

typedef struct sphere_s {
    double r;
} Sphere;

typedef struct body_s {
    TypeTag tag;
    union {
        Cylinder cyl;
        Cuboid cub;
        Sphere sph;
    };
} Body;

Body make_cylinder(double r, double h) {
    Cylinder cyl = {r, h};
    Body body = {.tag = CYLINDER, .cyl = cyl};
    return body;
}

Body make_cuboid(double a, double b, double c) {
    Cuboid cub = {a, b, c};
    Body body = {.tag = CUBOID, .cub = cub};
    return body;
}

Body make_sphere(double r) {
    Sphere sph = {r};
    Body body = {.tag = SPHERE, .sph = sph};
    return body;
}

double volume(Body body) {
    switch(body.tag) {
        case CYLINDER:
            return M_PI * body.cyl.r * body.cyl.r * body.cyl.h;
        case CUBOID:
            return body.cub.a * body.cub.b * body.cub.c;
        case SPHERE:
            return 4 / 3.0 * M_PI * body.sph.r * body.sph.r * body.sph.r;
    }
    return -1;
}

void volume_test(void) {
    test_within_d(volume(make_sphere(2)), 4 /3.0 * M_PI * 2 * 2 * 2, 1e-6);
    test_within_d(volume(make_cuboid(2, 3, 4)), 2 * 3 * 4, 1e-6);
    test_within_d(volume(make_cylinder(3, 4)), 4 * M_PI * 3 * 3 , 1e-6);
}

int main(void) {
    volume_test();
    return 0;
}
