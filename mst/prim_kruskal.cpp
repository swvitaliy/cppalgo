#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::set;
using std::vector;

typedef int ndx_t;
typedef int wei_t;

const wei_t INF = std::numeric_limits<int>::max();

vector<ndx_t> prim_mst(const vector<vector<pair<ndx_t, wei_t>>> &g) {
  const size_t n = g.size();
  vector<wei_t> d(n, INF);
  vector<ndx_t> p(n, -1);
  set<pair<wei_t, ndx_t>> q;
  q.emplace(d[0], 0);
  while (!q.empty()) {
    ndx_t v = q.begin()->second;
    q.erase(q.begin());
    for (auto up : g[v]) {
      ndx_t u = up.first;
      wei_t w = up.second;
      if (w < d[u]) {
        q.erase(make_pair(d[u], u));
        d[u] = w;
        p[u] = v;
        q.emplace(d[u], u);
      }
    }
  }

  return p;
}

struct dsu {
  vector<int> p;
  vector<int> r;
  dsu(const size_t n) : p(n), r(n) {}
  void make_all() {
    const size_t n = p.size();
    for (size_t i = 0; i < n; ++i) {
      p[i] = i;
      r[i] = 1;
    }
  }
  void make_set(int v) {
    p[v] = v;
    r[v] = 1;
  }
  int find_set(int v) {
    if (p[v] == v) return v;
    return p[v] = find_set(p[v]);
  }
  void union_sets(int u, int v) {
    u = find_set(u);
    v = find_set(v);
    if (u != v) {
      // r_u > r_v
      if (r[u] < r[v]) std::swap(u, v);
      p[v] = u;
      if (r[u] == r[v]) ++r[u];
    }
  }
};

vector<pair<ndx_t, ndx_t>> kruskal_mst(
    vector<pair<wei_t, pair<ndx_t, ndx_t>>> &g, const size_t n) {
  const size_t m = g.size();

  vector<pair<ndx_t, ndx_t>> res;
  vector<ndx_t> p(n);

  std::sort(g.begin(), g.end());

  dsu d(n);
  d.make_all();

  wei_t t = 0;
  for (size_t i = 0; i < m; ++i) {
    wei_t w = g[i].first;
    ndx_t u = g[i].second.first;
    ndx_t v = g[i].second.second;

    if (d.find_set(u) != d.find_set(v)) {
      t += w;
      res.push_back(g[i].second);
      d.union_sets(u, v);
    }
  }

  return res;
}
