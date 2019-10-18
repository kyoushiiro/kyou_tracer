#ifndef HITTABLEH
#define HITTABLEH

#include "ray.h"

// The hittable class is an abstract class that is extended by any object
// that can be hit by raycasting.

// Let the compiler know that there is a pointer to the material class.
class material;

// Creating a hit_record struct avoids the need for many arguments in hit()
struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
};

class hittable {
public:
    virtual bool hit(
        const ray&r, float t_min, float t_max, hit_record& rec) const = 0;
};


#endif
