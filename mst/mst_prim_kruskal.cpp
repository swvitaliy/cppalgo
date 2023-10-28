#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::set;
using std::sort;
using std::string;
using std::swap;
using std::vector;

constexpr int INF = std::numeric_limits<int>::max();

// ------------------------------------
// prim algorithm

struct mst_t {
  vector<int> par, cost;
  mst_t(vector<int> &&par, vector<int> &&cost)
      : par(std::move(par)), cost(std::move(cost)) {}
};

mst_t prim(vector<vector<pair<int, int>>> &g) {
  const size_t n = g.size();

  vector<int> cost(n, INF), par(n, -1);
  cost[0] = 0;
  set<pair<int, int>> q;
  q.insert(make_pair(0, 0));
  while (!q.empty()) {
    int v = q.begin()->second;
    q.erase(q.begin());

    for (auto p : g[v]) {
      int u = p.first;
      int c = p.second;

      if (c < cost[u]) {
        q.erase(make_pair(cost[u], u));
        cost[u] = c;
        par[u] = v;
        q.insert(make_pair(cost[u], u));
      }
    }
  }

  return {std::move(par), std::move(cost)};
}

// ------------------------------------
// kruskal algorithm

struct dsu {
  vector<int> a;
  vector<int> r;

  dsu(const size_t n) : a(n), r(n) {}
  void make_set(int v) {
    a[v] = v;
    r[v] = 1;
  }
  int find_set(int v) {
    if (a[v] == v) return v;
    return a[v] = find_set(a[v]);
  }
  void union_sets(int u, int v) {
    u = find_set(u);
    v = find_set(v);
    if (u != v) {
      if (r[u] > r[v]) swap(u, v);
      if (r[u] == r[v]) ++r[v];
      a[u] = v;
    }
  }
};

struct edge_t {
  int a, b, c;
  edge_t(const int a, const int b, const int c) : a(a), b(b), c(c) {}
};

vector<edge_t> kruskal(vector<edge_t> &g, size_t n) {
  dsu s(n);
  for (size_t i = 0; i < n; ++i) s.make_set(i);

  vector<edge_t> res;
  sort(g.begin(), g.end(),
       [](const edge_t &a, const edge_t &b) -> bool { return a.c < b.c; });
  for (auto e : g) {
    if (s.find_set(e.a) != s.find_set(e.b)) {
      res.push_back(e);
      s.union_sets(e.a, e.b);
    }
  }

  return res;
}

// ------------------------------------
// pretty print mst

typedef vector<vector<pair<int, int>>> rmst_t;

void reverse_mst(const mst_t &t, rmst_t &r, int &top) {
  const size_t n = t.par.size();

  top = -1;
  for (size_t i = 0; i < n; ++i) {
    int pi = t.par[i];
    if (pi == -1)
      top = i;
    else
      r[pi].emplace_back(i, t.cost[i]);
  }
}

void print_rmst(const rmst_t &t, const string &str, int i, int c,
                bool is_last) {
  if (i < (int)t.size()) {
    cout << str;
    cout << (is_last ? "└──" : "├──");
    if (c == -1)
      cout << " " << i + 1 << "\n";
    else
      cout << " " << i + 1 << " (" << c << ")\n";

    const size_t m = t[i].size();
    for (size_t j = 0; j < m; ++j)
      print_rmst(t, str + (is_last ? "    " : "│   "), t[i][j].first,
                 t[i][j].second, j == m - 1);
  }
}

int main(int argc, char **argv) {
  size_t n, m;
  cin >> n >> m;
  vector<vector<pair<int, int>>> g(n, vector<pair<int, int>>());
  for (size_t i = 0; i < m; ++i) {
    int a, b, c;
    cin >> a >> b >> c;
    g[a - 1].push_back(make_pair(b - 1, c));
  }

  auto t = prim(g);
  rmst_t rt(n);
  int top;
  reverse_mst(t, rt, top);

  cout << "prim result mst:\n";
  print_rmst(rt, "", top, -1, true);
  cout << endl;

  // convert graph to edges array for kruskal algorithm
  vector<edge_t> edges;
  for (size_t i = 0; i < n; ++i) {
    for (auto e : g[i]) edges.emplace_back(i, e.first, e.second);
  }

  // clean results
  rt.assign(n, vector<pair<int, int>>());

  auto mst_edges = kruskal(edges, n);
  for (auto e : mst_edges) rt[e.a].emplace_back(e.b, e.c);

  cout << "kruskal result mst:\n";
  print_rmst(rt, "", top, -1, true);
  cout << endl;

  return 0;
}