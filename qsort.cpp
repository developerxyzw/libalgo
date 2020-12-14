#include "qsort.h"
using namespace std;

int g_partition(vector<int> &a, int i, int j) {
  /* How `partition' works:
          |i|            |m|          |k|          |j|          -> indices
  +-><----+-+-----><-----+-----><-----+------><----+-+----><-+  vector<int>& a
  | ><    |p|     ><     |     ><     |      ><    | |    >< |  -> values
  +-><----+-+-----><-----+-----><-----+------><----+-+----><-+
    other | | Group lt p | Group ge p | Unclassified | other    -> groups
  While have unclassified elements take first unclassified element `x' and
  * if x < p then swap it with firsy element in group `ge p' and move group `lt p' border
  * if x >= p then do nothing
  */
  int mid = (i + j) / 2;
  if (a[mid] < a[i])
    swap(a[i], a[mid]);
  if (a[j] < a[i])
    swap(a[i], a[j]);
  if (a[mid] < a[j])
    swap(a[j], a[mid]);
  swap(a[i], a[j]);

  int p = a[i];
  int m = i;
  for (int k = i + 1; k <= j; ++k) {
    if (a[k] < p) {
      m++;
      swap(a[k], a[m]);
    }
  }
  swap(a[i], a[m]);
  return m;
}

bool g_is_not_sorted(vector<int> &a, int low, int high) {
  for (int i = 0; i < high; ++i)
    if (a[i] > a[i + 1])
      return true;
  return false;
}

void g_quick_sort(vector<int> &a, int low, int high) {
  int m = g_partition(a, low, high);
  if (low < m - 1 && g_is_not_sorted(a, low, m - 1))
    g_quick_sort(a, low, m - 1);
  if (m + 1 < high && g_is_not_sorted(a, m + 1, high))
    g_quick_sort(a, m + 1, high);
}