# libalgo - Algorithm library

A library for using in competitive programming and so on.

Supported technologies:
* Geometry (gmath.h, gmath.cpp)
* DSU (dsu.h, dsu.cpp)
* QuickSort (qsort.h, qsort.cpp)

Supported geometry-related things:
* Geometry primitives: points, lines, polygons, circles, triangles, NEW: rays
* Distance all between point, line, ray and segment
* Is_point_in_polygon O(n) version and O(log n) version with precompute in O(n)
* You can use it to solve task 16 (Not now, but after Jan 1, 2021, 4:20 MSK or I will get a RJ. Please don't frame me)

Supported DSU:
* Optimization: none or any combination of way-shrink optimization and rank optimization (supported rank optimizations: count, depth)
* Dynamic size. You can resize, shrink and do other things with DSU
* Currently, no copy constructors or other good (or bad) things

Quicksort:
* Optimization: If array is sorted, no need to sort it again
* Simple partition

System reqs:
* Fully C++14-compliant compiler required (Visual Studio 2017+, gcc 5+, clang 3.4+)
