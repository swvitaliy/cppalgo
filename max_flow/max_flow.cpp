#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

typedef int ndx_t;
typedef int wei_t;

struct edge_t {
  ndx_t u, v;
  wei_t c, f;
  ndx_t rev;
};

void add_edge(vector<vector<edge_t>> &g, ndx_t s, ndx_t t, ndx_t c) {
  edge_t a = {s, t, c, 0, (int)g[t].size()};
  edge_t b = {t, s, c, 0, (int)g[s].size()};
  g[s].push_back(a);
  g[t].push_back(b);
}

edge_t &back_edge(vector<vector<edge_t>> &g, edge_t &e) {
  return g[e.v][e.rev];
}

// ford falkenson algorithm
struct ff_max_flow {
  ndx_t t;
  vector<vector<edge_t>> g;
  vector<char> used;

  ff_max_flow(const ndx_t t, const vector<vector<edge_t>> &g) : t(t), g(g) {}

  ndx_t dfs(ndx_t u, wei_t c_min) {
    if (u == t) return c_min;

    used[u] = true;
    for (auto &e : g[u]) {
      if (!used[e.v] && e.f < e.c) {
        wei_t d = dfs(e.v, std::min(c_min, e.c - e.f));
        if (d > 0) {
          e.f += d;
          back_edge(g, e).f -= d;
          return d;
        }
      }
    }

    return 0;
  }
};

void dek_bfs(vector<vector<edge_t>> &g, ndx_t s, ndx_t t, vector<edge_t *> &p) {
  std::queue<ndx_t> q;
  q.push(s);
  while (!q.empty()) {
    ndx_t u = q.front();
    q.pop();

    for (auto &e : g[u]) {
      if (p[e.v] == nullptr && e.v != s && e.c > e.f) {
        p[e.v] = &e;
        q.push(e.v);
      }
    }
  }
}

const wei_t INF = std::numeric_limits<int>::max();

// dinitz-edmond-karp algorithm
wei_t dek_max_flow(vector<vector<edge_t>> &g, ndx_t s, ndx_t t) {
  const size_t n = g.size();

  wei_t c_min;
  wei_t flow = 0;
  while (true) {
    vector<edge_t *> p(n, nullptr);
    dek_bfs(g, s, t, p);
    if (p[t] == nullptr) break;

    // cout << "found path\n";

    c_min = INF;
    for (edge_t *e = p[t]; e != nullptr; e = p[e->u])
      c_min = std::min(c_min, e->c - e->f);

    // cout << "found c_min: " << c_min << "\n";

    for (edge_t *e = p[t]; e != nullptr; e = p[e->u]) {
      e->f += c_min;
      back_edge(g, *e).f -= c_min;
    }

    // cout << "update graph\n";

    flow += c_min;
  }

  /* wei_t r = 0;
  for (auto &e : g[t])
    r += e.f;

  assert(r == flow); */

  return flow;
}

int main(int argc, char **argv) {
  int c[][3] = {{0, 3, 2}, {0, 0, 2}, {0, 0, 0}};

  size_t n = 3;
  vector<vector<edge_t>> g(n);
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      if (c[i][j] != 0) add_edge(g, i, j, c[i][j]);

  cout << dek_max_flow(g, 0, 2) << "\n";

  return 0;
}