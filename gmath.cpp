// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "gmath.h"
using namespace std;

//=========================================
// Math Lib 1.2
// (c) 2019-2020 Grigoriy Goryunov [t.me/developerxyz]

//=========================================
gMathConst *gMathConst::getInstance()
{
	static gMathConst gmc;
	return &gmc;
}

long double gabs(long double val)
{
    if (val > -the_eps)
        return val;
    return -val;
}


//=========================================
pt::pt(long double x /*= 0*/, long double y /*= 0*/)
	: x(x)
	, y(y)
{
}

pt::pt(const pt &a, const pt &b)
	: x(b.x - a.x)
	, y(b.y - a.y)
{
}

long double pt::len() const
{
	return hypot(x, y);
}

long double pt::len2() const
{
	return x * x + y * y;
}

void pt::norm()
{
	double dOldLen = len();
	if (gabs(dOldLen) < the_eps)
	{
		return;
	}
	x /= dOldLen;
	y /= dOldLen;
}

pt pt::getNorm() const
{
	pt z(*this);
	z.norm();
	return z;
}

long double pt::cp(const pt &w) const
{
	return x * w.y - w.x * y;
}

long double pt::dp(const pt &w) const
{
	return x * w.x + y * w.y;
}

long double pt::angle(const pt &w) const
{
	return gabs(atan2(cp(w), dp(w)));
}

long double pt::angle2(const pt &w) const
{
	long double a = relangle(w);
	if (a < the_eps)
	{
		a += 2 * the_PI;
	}
	return gabs(a);
}

long double pt::relangle(const pt &w) const
{
	return (atan2(cp(w), dp(w)));
}

pt pt::operator+(const pt &w) const
{
	return pt(x + w.x, y + w.y);
}

pt pt::operator-() const
{
	return opposite();
}

pt &pt::operator*(long double a)
{
	x *= a;
	y *= a;
	return *this;
}

pt pt::ortho() const
{
	return pt(-y, x);
}

pt pt::opposite() const
{
	return pt(-x, -y);
}

pt pt::rot(long double angle) const
{
	long double co = cos(angle), si = sin(angle);
	return pt(x * co - y * si, x * si + y * co);
}

bool pt::inAngle(const pt &A, const pt &O, const pt &B) const
{
	const pt &P = *this;
	if (pt(O, B).cp(pt(O, A)) * pt(O, B).cp(pt(O, P)) >= 0
		&& pt(O, A).cp(pt(O, B)) * pt(O, A).cp(pt(O, P)) >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//=========================================
seg::seg(const pt &A /*= pt(0, 0)*/, const pt &B /*= pt(0, 0)*/)
	: A(A)
	, B(B)
{
}

bool seg::operator^(const seg &b) const
{
	pt C = b.A, D = b.B;
	long double cp1 = pt(A, B).cp(pt(A, D)) * pt(A, B).cp(pt(A, C)),
		cp2 = pt(C, D).cp(pt(C, B)) * pt(C, D).cp(pt(C, A));
	bool Ai = b.cont(A),
		Bi = b.cont(B),
		Ci = cont(C),
		Di = cont(D);
	return (cp1 < 0 && cp2 < 0) || (Ai || Bi || Ci || Di);
}

bool seg::cont(const pt &p) const
{
	long double cp1 = pt(A, B).cp(pt(A, p));
	long double dp1 = pt(p, A).dp(pt(p, B));
	bool XinLine = (gabs(cp1) < the_eps);
	bool XbtwAB = (dp1 < the_eps);
	return XinLine && XbtwAB;
}

pt seg::mid() const
{
	return (A + B) * 0.5;
}


//=========================================
line::line(long double a /*= 0*/, long double b /*= 0*/, long double c /*= 0*/)
	: A(a)
	, B(b)
	, C(c)
{
}

line::line(const pt &q, const pt &w)
{
	long double a = q.x, b = q.y, c = w.x, d = w.y;
	A = d - b;
	B = a - c;
	C = (c - a) * b - (d - b) * a;
}

line::line(const pt &A, const pt &B, const pt &C)
	: line(B, B + seg(pt(B, A).getNorm(), pt(B, C).getNorm()).mid())
{
}

line::line(const pt &a, const line &b)
{
	pt P = b.getPoint();
	long double OH = dist(a, b);
	long double PH = sqrt(pt(a, P).len2() - OH * OH);
	pt H = P + b.norm().ortho().getNorm() * PH;
	if (gabs(dist(a, H) - OH) > the_eps)
	{
		H = P + -b.norm().ortho().getNorm() * PH;
	}
	*this = line(a, H);
}

bool line::operator^(const line &b) const
{
	return gabs(pt(A, B).cp(pt(b.A, b.B))) > the_eps;
}

bool line::operator==(const line &b) const
{
	return gabs(A * b.B - b.A * B) < the_eps
		&& gabs(A * b.C - b.A * C) < the_eps
		&& gabs(B * b.C - b.B * C) < the_eps;
}

pair<int, pt> line::operator*(const line &L2) const
{
	if (!(L2 ^ *this))
	{
		if (L2 == *this)
		{
			return{ 2, pt(0, 0) };
		}
		else
		{
			return{ 0, pt(0, 0) };
		}
	}
	else
	{
		long double D = pt(A, B).cp(pt(L2.A, L2.B)),
			Dx = pt(-C, B).cp(pt(-L2.C, L2.B)),
			Dy = pt(A, -C).cp(pt(L2.A, -L2.C));
		return{ 1, pt(Dx / D, Dy / D) };
	}
}

pt line::norm() const
{
	return pt(A, B);
}

long double line::operator()(const pt &a) const
{
	return A * a.x + B * a.y + C;
}

bool line::cont(const pt &a) const
{
	return gabs((*this)(a)) < the_eps;
}

pt line::getPoint() const
{
	double aaa = -C / (A * A + B * B);
	return pt(aaa * A, aaa * B);
}


//=========================================
ray::ray(const pt &A /*= pt(0, 0)*/, const pt &B /*= pt(1, 0)*/)
	: A(A)
	, B(B)
{
}

bool ray::operator^(const ray &b) const
{
	pair<int, pt> inter = ln() * b.ln();
	if (inter.first == 0) return false;
	if (inter.first == 1) return cont(inter.second) && b.cont(inter.second);
	if (areCodirect(b)) return true;
	return cont(b.A);
}

bool ray::cont(const pt &p) const
{
	return (A + -p).len2() < the_eps || (ln().cont(p) && (gabs((p + -A).cp(B + -A)) < the_eps) && ((p + -A).dp(B + -A) > -the_eps));
}

line ray::ln() const
{
	return line(A, B);
}

bool ray::areCodirect(const ray &b) const
{
	return !(ln() ^ b.ln()) && (gabs((B + -A).cp(b.B + -b.A)) < the_eps) && ((B + -A).dp(b.B + -b.A) > -the_eps);
}


//=========================================
poly::poly(vector<pt> p /*= vector<pt>(0)*/)
	: pts(p)
{
}

long double poly::area() const
{
	long double res = 0;
	for (int i = 1; i < (int)pts.size(); ++i)
	{
		res += pts[i].cp(pts[i - 1]);
	}
	res += pts.front().cp(pts.back());
	res /= 2;
	return gabs(res);
}

long double poly::per() const
{
	long double res = 0;
	for (int i = 1; i < (int)pts.size(); ++i)
	{
		res += pt(pts[i], pts[i - 1]).len();
	}
	res += dist(pts.front(), pts.back());
	return res;
}

bool poly::cont(const pt &a) const
{
	// first check A in SEGMENTS
	bool ok = false;
	int n = pts.size();
	const vector<pt> &v = pts;
	for (int i = 0; !ok && i < n; ++i)
	{
		int j = (i + 1) % n;
		ok = seg(v[i], v[j]).cont(a);
	}
	for (int i = 0; !ok && i < n; ++i)
	{
		ok = gabs(dist(v[i], a)) < the_eps;
	}
	if (!ok)
	{
		// check A in V
		long double an = 0;
		for (int i = 0; i < n; ++i)
		{
			int j = (i + 1) % n;
			an += pt(a, v[i]).relangle(pt(a, v[j]));
		}
		if ((int)round(gabs(an / (2 * the_PI))) == 1)
		{
			ok = true;
		}
		else
		{
			ok = false;
		}
	}
	return ok;
}

bool poly::cont_log(const pt &a) const
{
	if (dist(a, zero) < the_eps)
	{
		return true;
	}

	// bsearch
	long double an = -pt(zero, a).relangle(pt(0, -1));
	if (an < the_eps)
	{
		return false;
	}
	auto it = upper_bound(polar.begin(), polar.end(), an);
	if (it == polar.end() && gabs(an - polar.back()) < the_eps)
	{
		it = polar.end() - 1;
	}
	if (it != polar.end() && it != polar.begin())
	{
		int pos1 = it - polar.begin();
		int pos2 = pos1 - 1;
		pos1 += (i_zero + 1);
		pos2 += (i_zero + 1);
		pos1 %= pts.size();
		pos2 %= pts.size();
		// We know our AN lies in angle pos1-zero-pos2
		// Check it lies in that triangle
		// just check if it lies in angle pos1-pos2-zero
		bool inAngle = a.inAngle(pts[pos1], pts[pos2], zero);
		return inAngle;
	}
	return false;
}

void poly::cont_log_init()
{
	reverseCCW();
	i_zero = 0;
	zero = pts[0];
	for (int i = 1; i < pts.size(); ++i)
	{
		if (pts[i].x < zero.x || (gabs(pts[i].x - zero.x) < the_eps && pts[i].y < zero.y))
		{
			zero = pts[i];
			i_zero = i;
		}
	}
	polar.clear();
	for (int i = 0; i < pts.size(); ++i)
	{
		int j = (i + i_zero) % pts.size();
		if (j != i_zero)
		{
			polar.push_back(pt(zero, pts[j]).angle(pt(0, -1)));
		}
	}
}

bool poly::isConvex()
{
	int n = pts.size();
	vector<pt> p(pts.begin(), pts.end());
	reverseCW();

	p.push_back(p[0]);
	p.push_back(p[1]);
	bool ok = true;
	for (int i = 1; ok && i <= n; ++i)
	{
		if (pt(p[i], p[i - 1]).angle2(pt(p[i], p[i + 1])) > the_PI + the_eps)
		{
			ok = false;
		}
	}
	return ok;
}

void poly::reverseCW()
{
	int n = pts.size();
	vector<pt> &p = pts;
	long double an = 0;
	for (int i = 0; i < n; ++i)
	{
		int j = (i + 1) % n;
		int k = (i + 2) % n;
		an += pt(p[j], p[i]).angle2(pt(p[j], p[k]));
	}
	bool rev = gabs(an - the_PI * (n - 2)) < the_eps;
	if (!rev)
	{
		reverse(p.begin() + 1, p.end());
	}
}

void poly::reverseCCW()
{
	int n = pts.size();
	vector<pt> &p = pts;
	long double an = 0;
	for (int i = 0; i < n; ++i)
	{
		int j = (i + 1) % n;
		int k = (i + 2) % n;
		an += pt(p[j], p[i]).angle2(pt(p[j], p[k]));
	}
	bool rev = gabs(an - the_PI * (n - 2)) < the_eps;
	if (rev)
	{
		reverse(p.begin() + 1, p.end());
	}
}


//=========================================
krug::krug(const pt &O /*= pt(0, 0)*/, long double R /*= 0*/)
	: center(O)
	, rad(R)
{
}

vector<pt> krug::operator*(const krug &b) const
{
	long double d = pt(center, b.center).len2();
	if (gabs(d) < the_eps && gabs(rad - b.rad) < the_eps)
	{
		// INF int
		return vector<pt>(3);
	}
	else if (gabs(d - (rad - b.rad) * (rad - b.rad)) < the_eps || gabs(d - (rad + b.rad) * (rad + b.rad)) < the_eps)
	{
		// 1 kas
		pt O1O2(center, b.center);
		if (gabs(d - (rad - b.rad) * (rad - b.rad)) < the_eps && rad < b.rad)
		{
			// INNER case
			O1O2 = -O1O2;
		}
		pt norm_O1O2 = O1O2.getNorm();
		pt I = center + (norm_O1O2 * rad);
		return{ I };
	}
	else if (d < (rad - b.rad) * (rad - b.rad) || d > (rad + b.rad) * (rad + b.rad))
	{
		// 0 int
		return vector<pt>(0);
	}
	else
	{
		// 2 int
		pt O1 = center;
		pt O2 = b.center;
		long double R1 = rad;
		long double R2 = b.rad;
		d = sqrt(d);
		long double angle = acos((R1*R1 + d * d - R2 * R2) / (2 * d*R1));
		pt A = O1 + pt(O1, O2).rot(angle).getNorm() * R1;
		pt B = O1 + pt(O1, O2).rot(-angle).getNorm() * R1;
		return{ A, B };
	}
}


//=========================================
triangle::triangle(const pt &a /*= pt(0, 0)*/, const pt &b /*= pt(0, 0)*/, const pt &c /*= pt(0, 0)*/)
	: A(a)
	, B(b)
	, C(c)
{
}

triangle::type triangle::getType() const
{
	long double a1 = pt(A, B).angle(pt(A, C));
	long double a2 = pt(B, A).angle(pt(B, C));
	long double a3 = the_PI - (a1 + a2);
	if (gabs(a1 - the_PI2) < the_eps
		|| gabs(a2 - the_PI2) < the_eps
		|| gabs(a3 - the_PI2) < the_eps)
	{
		return type::right;
	}
	else if (a1 < the_PI2 && a2 < the_PI2 && a3 < the_PI2)
	{
		return type::acute;
	}
	else if (line(A, B).cont(C))
	{
		return type::inval;
	}
	else
	{
		return type::obtuse;
	}
}

long double triangle::area() const
{
	return pt(A, B).cp(pt(A, C)) * 0.5;
}

long double triangle::per() const
{
	return dist(A, B) + dist(A, C) + dist(B, C);
}

krug triangle::in() const
{
	pt I = (line(A, B, C) * line(B, A, C)).second;
	return krug(I, dist(I, line(A, B)));
}

krug triangle::out() const
{
	pt O = (line(seg(A, B).mid(), line(A, B)) * line(seg(B, C).mid(), line(B, C))).second;
	return krug(O, dist(O, A));
}

triangle triangle::fromLengths(long double a, long double b, long double c) const
{
	triangle t;
	t.A = pt(0, 0);
	t.B = pt(0, b);
	// c^2 = a^2 + b^2 - 2ab cos(ab)
	// cos(ab) = (a^2 - c^2 + b^2) / (2ab)
	long double co = (a * a + b * b - c * c) / (2 * a * b);
	t.C = t.B.getNorm().rot(acos(co)) * a;

	return t;
}


//=========================================
long double dist(const line &a, const pt &b)
{
	return gabs(a(b) / (a.norm().len()));
}

long double dist(const pt &a, const line &b)
{
	return dist(b, a);
}

long double dist(const pt &a, const pt &b)
{
	return pt(a, b).len();
}

long double dist(const pt &a, const seg &b)
{
	long double a1 = pt(b.A, a).angle(pt(b.A, b.B));
	long double a2 = pt(b.B, a).angle(pt(b.B, b.A));
	if (a1 > the_PI2 || a2 > the_PI2)
	{
		return min(dist(a, b.A), dist(a, b.B));
	}
	else if (b.cont(a))
	{
		return 0;
	}
	else
	{
		return dist(a, line(b.A, b.B));
	}
}

long double dist(const seg &a, const pt &b)
{
	return dist(b, a);
}

long double dist(const line &a, const line &b)
{
	if (a ^ b) return 0;
	return dist(a.getPoint(), b);
}

long double dist(const line &a, const seg &b)
{
	pair<int, pt> inter = a * line(b.A, b.B);
	if (inter.first == 1 && b.cont(inter.second)) return 0;
	return min(dist(b.A, a), dist(b.B, a));
}

long double dist(const seg &a, const line &b)
{
	return dist(b, a);
}

long double dist(const seg &a, const seg &b)
{
	if (a ^ b) return 0;
	return min(min(dist(a.A, b), dist(a.B, b)), min(dist(b.A, a), dist(b.B, a)));
}

long double dist(const pt &a, const ray &b)
{
	long double a1 = pt(b.A, a).angle(pt(b.A, b.B));
	if (a1 > the_PI2)
	{
		return dist(a, b.A);
	}
	else if (b.cont(a))
	{
		return 0;
	}
	else
	{
		return dist(a, line(b.A, b.B));
	}
}

long double dist(const ray &a, const pt &b)
{
	return dist(b, a);
}

long double dist(const line &a, const ray &b)
{
	pair<int, pt> inter = a * b.ln();
	if (inter.first == 0) return dist(a, b.ln());
	if (inter.first == 2) return 0;
	if (b.cont(inter.second)) return 0;
	return dist(b.A, a);
}

long double dist(const ray &a, const line &b)
{
	return dist(b, a);
}

long double dist(const ray &a, const ray &b)
{
	if (a ^ b) return 0;
	return min(dist(a, b.A), dist(a.A, b));
}

long double dist(const ray &a, const seg &b)
{
	if ((ray(b.A, b.B) ^ a) && (ray(b.B, b.A) ^ a)) return 0;
	return min(dist(a.A, b), min(dist(b.A, a), dist(b.B, a)));
}

long double dist(const seg &a, const ray &b)
{
	return dist(b, a);
}


//=========================================
istream &operator>>(istream &is, pt &p)
{
	is >> p.x >> p.y;
	return is;
}

ostream &operator<<(ostream &os, const pt &p)
{
	os << p.x << " " << p.y;
	return os;
}

istream &operator>>(istream &is, seg &p)
{
	is >> p.A >> p.B;
	return is;
}

ostream &operator<<(ostream &os, const seg &p)
{
	os << p.A << " " << p.B;
	return os;
}

istream &operator>>(istream &is, krug &p)
{
	is >> p.center >> p.rad;
	return is;
}

ostream &operator<<(ostream &os, const krug &p)
{
	os << p.center << " " << p.rad;
	return os;
}
