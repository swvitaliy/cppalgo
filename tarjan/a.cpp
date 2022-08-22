#include <iostream>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::vector;
// Tarjan
struct dsu {
  vector<int> p;
  vector<int> r;
  vector<int> a;
  vector<char> u;

  dsu(size_t n) : p(n), r(n, 1), a(n), u(n, false) {}
  int find_set(int v) { return p[v] == v ? p[v] : p[v] = find_set(p[v]); }
  void union_sets(int u, int v, int x) {
    u = find_set(u);
    v = find_set(v);
    if (u != v) {
      if (r[u] > r[v]) std::swap(u, v);
      // r[v] >= r[u]
      p[u] = v;
      a[v] = x;
      if (r[u] == r[v]) ++r[v];
    }
  }
  void dfs(const vector<vector<int>> &g, vector<vector<pair<int, int>>> &q,
           int v) {
    p[v] = v;
    a[v] = v;
    u[v] = true;
    for (auto i : g[v])
      if (!u[i]) {
        dfs(g, q, i);
        union_sets(v, i, v);
      }

    for (auto &qi : q[v])
      if (u[qi.first]) qi.second = a[find_set(qi.first)];
  }
};

void lsa(const vector<vector<int>> &g, vector<vector<pair<int, int>>> &q) {
  dsu a(g.size());
  a.dfs(g, q, 0);
}

int main(int argc, char **argv) {
  size_t n, m, l;
  cin >> n >> m;

  vector<vector<int>> g(n, vector<int>());
  for (size_t i = 0; i < n; ++i) {
    cin >> l;
    for (size_t j = 0; j < l; ++j) {
      int t;
      cin >> t;
      g[i].push_back(t);
    }
  }

  vector<vector<pair<int, int>>> q(m, vector<pair<int, int>>());
  for (size_t i = 0; i < m; ++i) {
    int a, b;
    cin >> a >> b;
    q[a].emplace_back(b, -1);
  }

  lsa(g, q);

  for (size_t i = 0; i < m; ++i)
    for (size_t j = 0; j < q[i].size(); ++j)
      cout << "lsa(" << i << ", " << q[i][j].first << ") = " << q[i][j].second
           << "\n";

  return 0;
}
