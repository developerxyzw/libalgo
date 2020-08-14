# libalgo - Algorithm library

A library for using in competitive programming and so on.

Supported technologies:
* Geometry

Supported geometry-related things:
* Geometry primitives: points, lines, polygons, circles, triangles
* Distance point-point, point-line and point-segment
* Is_point_in_polygon O(n) version
* In_point_in_polygon O(log n) version with precompute in O(n)

System reqs:
* C++11-compliant compiler required (Visual Studio 2012+, gcc 4.8.1+, clang 3.3+)
* CMake ???

## How to build

```
cmake
make  // Or your make tool, e.g. ninja, nmake, mingw32-make, jom
make install
```

## How to use

1. Add `-lalgo` to linker args (link with `libalgo.a` OR `algo.lib`, depending on platform and compiler)
1. Add library path to linker (e.g. `-L/home/dev/lib/`)
1. Add include path to compiler (e.g. `-I/home/dev/include/`)
1. Include `algo.h` in your project and use functions from it
