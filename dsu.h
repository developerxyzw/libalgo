#ifndef gDSU_included
#define gDSU_included

// #include <stdexcept>
#include <vector>
using namespace std;

struct DSU_Data {
  int parent;
  int depth;
  void* data;
  DSU_Data(int parent = -1, int depth = -1, void* data = nullptr)
      : parent(parent), depth(depth), data(data) {}
};

struct DSU {
 private:
  vector<DSU_Data> d;
  bool way_opt;  // 0:disable, 1:enable
  int rank_opt;  // 0:disable, 1:depth, 2:count

 public:
  DSU(int size = 0, bool bUseWayOpt = true, int iUseRankOpt = 1)
      : d(size), way_opt(bUseWayOpt), rank_opt(iUseRankOpt) {}

  void shrink_to_fit();
  void reserve(int x);
  void resize(int new_size);
  int size();

  void make_set(int v, void* data = nullptr);
  int find_set(int v);
  bool union_sets(int a, int b);
  void* get_data(int a);

 private:
  // Unsafe begin
  inline int& d_parent(int v);
  inline int& d_depth(int v);
  // Unsafe end

  int find_set_impl(int v);
  bool union_sets_impl(int a, int b);
};

#endif // ndef gDSU_included