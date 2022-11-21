#include "cassert"
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

vector<int> li(vector<vector<int>> &g, vector<vector<int>> &gr, int s, int t) {
  const size_t n = g.size();
  vector<int> d(n, 0);
  int w = 0;
  d[s] = w;
  vector<int> f;
  f.push_back(s);

  while (!f.empty()) {
    const int m = (int)f.size();
    for (int i = m - 1; i >= 0; --i) {
      int v = f[i];
      f.pop_back();
      for (auto u : g[v]) {
        if (d[u] == 0) {
          d[u] = d[v] + 1;
          f.push_back(u);
        }
      }
    }
  }

  vector<int> ans;
  int p = t;
  while (p != s) {
    ans.push_back(p);
    int k = -1;
    for (auto f : gr[p])
      if (d[f] == d[p] - 1) {
        k = f;
      }
    assert(k != -1);
    p = gr[p][k];
  }
  ans.push_back(s);
  return ans;
}

int main(int argc, char **argv) { return 0; }
