#ifndef CUBEH
#define CUBEH

#include <cmath>

#include "hittable.h"

// Cube class - create an axis-aligned cube.

class cube : public hittable {
public:
    cube() {}
    cube(vec3 cen, float s, material *m) : center(cen), size(s), mat_ptr(m)  {
        float halfSize = s/2.0;
        bounds[0] = cen - vec3(halfSize, halfSize, halfSize);
        bounds[1] = cen + vec3(halfSize, halfSize, halfSize);
    };
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    vec3 center;
    float size;
    vec3 bounds[2];

    // Reference to the cube's material.
    material *mat_ptr;
};

// Returns whether or not a ray r intersects this cube.
bool cube::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 orig = r.origin();
    vec3 dir = r.direction();
    vec3 invDir(1/dir.x(), 1/dir.y(), 1/dir.z()); 
    int sign[3];
    sign[0] = (invDir.x() < 0);
    sign[1] = (invDir.y() < 0);
    sign[2] = (invDir.z() < 0);

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[sign[0]].x() - orig.x()) * invDir.x();
    tmax = (bounds[1-sign[0]].x() - orig.x()) * invDir.x();
    tymin = (bounds[sign[1]].y() - orig.y()) * invDir.y();
    tymax = (bounds[1-sign[1]].y() - orig.y()) * invDir.y();

    if((tmin > tymax) || (tymin > tmax))
        return false;
    if(tymin > tmin) 
        tmin = tymin;
    if(tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[sign[2]].z() - orig.z()) * invDir.z();
    tzmax = (bounds[1-sign[2]].z() - orig.z()) * invDir.z();

    if((tmin > tzmax) || tzmin > tmax)
        return false;
    if(tzmin > tmin)
        tmin = tzmin;
    if(tzmax < tmax)
        tmax = tzmax;

    rec.t = tmin;
    if(rec.t < 0) {
        rec.t = tmax;
        if(rec.t < 0) return false;
    }
    rec.p = r.point_at_parameter(rec.t);

    vec3 difference = rec.p - center;
    vec3 norm;
    if(std::abs(difference.x()) > std::abs(difference.y())) {
        if(std::abs(difference.x()) > std::abs(difference.z())) {
            if(difference.x() > 0) {
                norm = vec3(1,0,0);
            } 
            else {
                norm = vec3(-1,0,0);
            }
        }
        else {
            if(difference.z() > 0) {
                norm = vec3(0,0,1);
            }
            else {
                norm = vec3(0,0,-1);
            }
        }
    }
    else {
        if(std::abs(difference.y()) > std::abs(difference.z())) {
            if(difference.y() > 0) {
                norm = vec3(0,1,0);
            }
            else {
                norm = vec3(0,-1,0);
            }
        }
        else {
            if(difference.z() > 0) {
                norm = vec3(0,0,1);
            }
            else {
                norm = vec3(0,0,-1);
            }
        }
    }
    rec.normal = norm;
    rec.mat_ptr = mat_ptr;

    return true;
}


#endif
