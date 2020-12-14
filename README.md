# libalgo - Algorithm library

A library for using in competitive programming and so on.

Supported technologies:
* Geometry (gmath.h, gmath.cpp)
* DSU (dsu.h, dsu.cpp)

Supported geometry-related things:
* Geometry primitives: points, lines, polygons, circles, triangles
* Distance point-point, point-line and point-segment
* Is_point_in_polygon O(n) version
* In_point_in_polygon O(log n) version with precompute in O(n)

Supported DSU:
* Optimization: none or any combination of way-shrink optimization and rank optimization (supported rank optimizations: count, depth)
* Dynamic size. You can resize, shrink and do other things with DSU
* Currently, no copy constructors or other good (or bad) things

System reqs:
* C++11-compliant compiler required (Visual Studio 2012+, gcc 4.8.1+, clang 3.3+)
