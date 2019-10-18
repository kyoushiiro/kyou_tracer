#ifndef MATERIALH
#define MATERIALH

#include "hittable.h"
#include "random.h"
#include "ray.h"

// An abstract material class that encapsulates behavior
// of materials and allows for easy addition of new materials.

class material {
public:
    // When a ray hits this object, produce a scattered ray or 
    // announce that this material absorbed the ray.
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, vec3& attenuation,
        ray& scattered) const = 0;
};

/* MATERIALS ======================================================= */

// Classic diffuse material, matte appearance.
class lambertian : public material {
public:
    lambertian(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         vec3& attenuation, ray& scattered) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target-rec.p);
        attenuation = albedo;
        return true;
    }
    vec3 albedo;
};

// Reflective metal material, fuzz indiciates how clear the reflection is.
class metal : public material {
public:
    metal(const vec3& a, float f) : albedo(a) {
        if(f < 1) fuzz = f; else fuzz = 1;
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vec3 albedo;
    float fuzz;
};

// Refractive material, always refracts when possible.
class glass : public material {
public:
    glass(float ri) : ref_idx(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         vec3& attenuation, ray& scattered) const {

        // To be completely honest there's a lot of math here in calculating
        // the refraction that I don't completely understand. I'm just going
        // along with the formulas and tutorial with my best guess as to what
        // it's doing mathematically :)
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;

        float reflect_prob;
        float cosine;

        // Check direction of normal and ray.
        if(dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal)
                    / r_in.direction().length();
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal)
                    / r_in.direction().length();
        }

        // Determine the probability of reflection vs refraction.
        if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        }
        else {
            reflect_prob = 1.0;
        }

        // If refraction, return the refracted ray, else return reflected ray.
        if(random_double() < reflect_prob) {
            scattered = ray(rec.p, reflected);
        }
        else {
            scattered = ray(rec.p, refracted);
        }

        return true;
    }
    // Refractive index
    float ref_idx;
};

/* END OF MATERIALS ================================================ */


#endif
