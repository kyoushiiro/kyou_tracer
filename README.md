# kyou_tracer
Alfred Lam

aylam@ucsc.edu

10/17/19

To compile and run:

c++ rt.cpp -o rt

./rt

My first ray tracer -

Disclaimer - much of the code is taken from Ray Tracing in One Weekend by Peter Shirley, with my own additions of comments and changes of variable names/code structure. Some excerpts also taken from ScratchaPixel guides. 

Implements - 
reflective metal material
refractive glass material
diffuse matte material
an attempt at a box that doesn't quite work out

My attempt at the cube object wasn't quite complete, and I couldn't figure out whether it was due to the normals, the intersection calculations, or another factor - in the future I'd like to edit the cube code to make it work properly, and manually figure out the math behind intersecting rays and cubes.
