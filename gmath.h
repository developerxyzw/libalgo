// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef gMath_included
#define gMath_included

#include <iostream>
#include <vector>
#include <utility>
#include <math.h>
#include <stdint.h>

//=========================================
// Math Lib 1.2
// (c) 2019-2020 Grigoriy Goryunov [t.me/developerxyz]

//=========================================
struct gMathConst
{
	const long double PI = acos(-1);                       // PI, independent of math.h definition
	const long double PI2 = asin(1);                       // PI/2, independent of math.h definition
	const long double eps = 1e-10;                         // gMath epsilon (rounding error)
	static gMathConst *getInstance();
private:
	gMathConst() = default;
	gMathConst(const gMathConst &) = delete;
	gMathConst(gMathConst &&) = delete;
};
#define the_gMathConst gMathConst::getInstance()
#define the_PI         the_gMathConst->PI
#define the_PI2        the_gMathConst->PI2
#define the_eps        the_gMathConst->eps

long double gabs(long double val);

//=========================================
struct pt
{
	long double x, y;
	pt(long double x = 0, long double y = 0);
	pt(const pt &a, const pt &b);
	pt(const pt &a) = default;
	long double len() const;
	long double len2() const;                              // squared length
	void norm();
	pt getNorm() const;
	long double cp(const pt &w) const;                     // cross prod (area of parallelogram)
	long double dp(const pt &w) const;                     // dot prod
	long double angle(const pt &w) const;                  // absolute angle (Pt,[0,0],w)
	long double angle2(const pt &w) const;                 // relative angle (Pt,[0,0],w)
	long double relangle(const pt &w) const;               // relative angle (Pt,[0,0],w)
	pt operator+(const pt &w) const;
	pt operator-() const;                                  // negate
	pt &operator*(long double a);
	pt ortho() const;
	pt opposite() const;
	pt rot(long double angle) const;
	bool inAngle(const pt &A, const pt &O, const pt &B) const;
};

//=========================================
struct seg
{
	pt A, B;
	seg(const pt &A = pt(0, 0), const pt &B = pt(0, 0));
	bool operator^(const seg &b) const;                    // are intersecting?
	bool cont(const pt &p) const;
	pt mid() const;
};

//=========================================
struct line
{
	long double A, B, C;                                   // Ax + By + C = 0 for all (x, y) in line
	line(long double a = 0, long double b = 0, long double c = 0);
	line(const pt &q, const pt &w);                        // line by 2 points
	line(const pt &A, const pt &B, const pt &C);           // bisector of angle ABC
	line(const pt &a, const line &b);                      // perpendicular from A to B
	line(const line &b) = default;
	bool operator^(const line &b) const;                   // are intersecting?
	bool operator==(const line &b) const;
	std::pair<int, pt> operator*(const line &L2) const;    // get intersection
	pt norm() const;
	long double operator()(const pt &a) const;             // evaluate Ax + By + C if a(x, y)
	bool cont(const pt &a) const;
	pt getPoint() const;                                   // return a point in line
};

//=========================================
struct ray
{
	pt A, B;
	ray(const pt &A = pt(0, 0), const pt &B = pt(1, 0));   // Construct a ray A-->B
	bool operator^(const ray &b) const;                    // are intersecting?
	bool cont(const pt &p) const;
	line ln() const;                                       // get the line that contains ray
	bool areCodirect(const ray &b) const;                  // check if rays are codirected
};

//=========================================
struct poly
{
	std::vector<pt> pts;
	std::vector<long double> polar;                        // for cont_log, not to be edited directly
	pt zero;                                               // for cont_log, not to be edited directly
	int i_zero;                                            // for cont_log, not to be edited directly
	poly(std::vector<pt> p = std::vector<pt>(0));
	long double area() const;                              // absolute
	long double per() const;
	bool cont(const pt &a) const;
	bool cont_log(const pt &a) const;                      // optimized version of cont(pt)
	void cont_log_init();                                  // call before cont_lot(pt) and on each change!
	bool isConvex();                                       // warning! polygon is made CW after calling
	void reverseCW();                                      // make clockwise
	void reverseCCW();                                     // make counter-clockwise
};

//=========================================
struct krug
{
	pt center;
	long double rad;
	krug(const pt &O = pt(0, 0), long double R = 0);
	std::vector<pt> operator*(const krug &b) const;        // vector of point(s) of intersection
};

//=========================================
struct triangle
{
	pt A, B, C;
	triangle(const pt &a = pt(0, 0), const pt &b = pt(0, 0), const pt &c = pt(0, 0));
	enum class type
	{
		inval, acute, right, obtuse
	};       // inval <=> A, B and C are on the same line
	type getType() const;
	long double area() const;                              // absolute
	long double per() const;
	krug in() const;
	krug out() const;
	triangle fromLengths(long double a, long double b, long double c) const;
};

//=========================================
long double dist(const line &a, const pt &b);
long double dist(const pt &a, const line &b);
long double dist(const pt &a, const pt &b);
long double dist(const pt &a, const seg &b);
long double dist(const seg &a, const pt &b);
long double dist(const line &a, const line &b);
long double dist(const line &a, const seg &b);
long double dist(const seg &a, const line &b);
long double dist(const seg &a, const seg &b);
long double dist(const pt &a, const ray &b);
long double dist(const ray &a, const pt &b);
long double dist(const line &a, const ray &b);
long double dist(const ray &a, const line &b);
long double dist(const ray &a, const ray &b);
long double dist(const ray &a, const seg &b);
long double dist(const seg &a, const ray &b);

//=========================================
std::istream &operator>>(std::istream &is, pt &p);
std::ostream &operator<<(std::ostream &os, const pt &p);
std::istream &operator>>(std::istream &is, seg &p);
std::ostream &operator<<(std::ostream &os, const seg &p);
std::istream &operator>>(std::istream &is, krug &p);
std::ostream &operator<<(std::ostream &os, const krug &p);

#endif // def gMath_included
