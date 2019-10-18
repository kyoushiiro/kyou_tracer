#ifndef RANDOMH
#define RANDOMH

#include <cstdlib>

// Random class created for possible better random
// number generating algorithms in future. For now
// just use prebuilt one.

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

// Generates a random point within a unit sphere with length <= 1
vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

// Chistophe Schlick's approximation of reflectivity in glass.
float schlick(float cosine, float ref_idx) {
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine), 5);
}


#endif
