#ifndef HITTABLELISTH
#define HITTABLELISTH

#include "hittable.h"

class hittable_list: public hittable {
public:
    hittable_list() {}
    hittable_list(hittable **l, int n) { list = l; list_size = n; }
    virtual bool hit(
        const ray& r, float tmin, float tmax, hit_record& rec) const;
    hittable **list;
    int list_size;
};

// Return if a ray hit anything within the hittable object list.
bool hittable_list::hit(const ray& r, float t_min, float t_max,
                        hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    // Loop through every hittable object.
    for(int i = 0; i < list_size; i++) {
        // Only record the object if it is the closest to the camera.
        if(list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    // Return if we hit something, keep record in rec.
    return hit_anything;
}


#endif
