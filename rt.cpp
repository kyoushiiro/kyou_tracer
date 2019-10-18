#include "vec3.h"
#include "material.h"
#include "hittable_list.h"
#include "sphere.h"
#include "cube.h"
#include "camera.h"
#include "random.h"

#include <float.h>
#include <fstream>
#include <iostream>

const vec3 teal(0.529, 0.808, 0.922);

// Returns the color of the pixel that the ray hits, or whatever color the background is.
vec3 color(const ray& r, hittable *world, int depth) {
    hit_record rec;

    // Prevent recursions of more than 5.
    // Ignore intersections very close to 0 (ie. t=0.000001)
    if(world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        if(depth < 5 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    int nx = 800;
    int ny = 400;
    int ns = 100;

    std::ofstream ofs("./rt.ppm", std::ios::out | std::ios::binary);
    ofs << "P3\n" << nx << " " << ny << "\n255\n";

    // Create list of hittable objects
    hittable *list[6];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(vec3(0,-100.5, -1), 100, new lambertian(vec3(0.88, 0.9, 0.85)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.32));
    list[3] = new sphere(vec3(-1,0,-0.6), 0.5, new glass(1.5));
    list[4] = new sphere(vec3(-1.4,0.95,-1.75), 0.5, new metal(teal, 0.05));
    list[5] = new cube(vec3(1.8,2.2,-3.21), 1.0, new lambertian(vec3(0.8, 0.2, 0.5)));
    hittable *world = new hittable_list(list, 6);
    camera cam;

    for(int j = ny-1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for(int s = 0; s < ns; s++) {
                // Create ray from camera to pixel position
                // Use random_double() to blend surrounding pixels for anti-aliasing.
                float u = float(i + random_double()) / float(nx);
                float v = float(j + random_double()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }

            // Get the average color of all the sampled pixels
            col /= float(ns);
            // Gamma correction (transform the 0-1 values before storing as a byte)
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            // Convert from 0-1 values to RGB values.
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            // Output to ppm file.
            ofs << ir << " " << ig << " " << ib << "\n";
        }
        //std::cout << j << "\n";
    }
    ofs.close();
}


