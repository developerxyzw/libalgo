// This is an independent project of an individual developer.
// Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#:
// http://www.viva64.com

#include <vector>
using namespace std;

void DSU::shrink_to_fit() {
  d.shrink_to_fit();
}

void DSU::reserve(int x) {
  d.reserve(x);
}

void DSU::resize(int new_size) {
  d.resize(new_size);
}

int DSU::size() {
  return d.size();
}

void DSU::make_set(int v, void* data = nullptr) {
  if (size() <= v)
    resize(v + 1);
  d[v] = DSU_Data(v, 0, data);
  if (rank_opt == 2)
    d_depth(v) = 1;
}

int DSU::find_set(int v) {
  if (v < 0 || v >= size())
    throw std::out_of_range("DSU::find_set - 'v' out of range!");
  return find_set_impl(v);
}

bool DSU::union_sets(int a, int b) {
  if (a < 0 || a >= size())
    throw std::out_of_range("DSU::union_sets - 'a' out of range!");
  if (b < 0 || b >= size())
    throw std::out_of_range("DSU::union_sets - 'b' out of range!");
  return union_sets_impl(a, b);
}

void* DSU::get_data(int a) {
  if (a < 0 || a >= size())
    throw std::out_of_range("DSU::get_data - 'a' out of range!");
  return d[a].data;
}

// Unsafe
inline int& DSU::d_parent(int v) {
  return d[v].parent;
}

// Unsafe
inline int& DSU::d_depth(int v) {
  return d[v].depth;
}

int DSU::find_set_impl(int v) {
  if (v == d_parent(v))
    return v;
  int ancestor = find_set_impl(d_parent(v));
  // If we have turned on WayFold optimization, we redefine ancestor
  if (way_opt)
    d_parent(v) = ancestor;
  return ancestor;
}

bool DSU::union_sets_impl(int a, int b) {
  a = find_set_impl(a);
  b = find_set_impl(b);
  if (a != b) {
    // Set new ancestor for least tree (if rank_opt == 0, then always for
    // right)
    if (rank_opt != 0 && d_depth(a) < d_depth(b))
      swap(a, b);
    d_parent(b) = a;

    // Update ranks
    if (rank_opt == 1 && d_depth(a) == d_depth(b)) {
      ++d_depth(a);
    } else if (rank_opt == 2) {
      d_depth(a) += d_depth(b);
    }

    return true;
  }
  return false;
}
