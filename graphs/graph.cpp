/* graph eccentricity, radius, diam, center
https://codeforces.com/blog/entry/17974?locale=ru
*/
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

struct vertex_t {
  vertex_t() : val_() {}
  vertex_t(const string val) : val_(val) {}
  vertex_t(const vertex_t &v) : val_(v.val_) {}
  string val_;
};

bool adj(vector<bool> a, size_t n, size_t f, size_t t) { return a[f * n + t]; }

void set_adj(vector<bool> &a, size_t n, size_t f, size_t t, bool v = true) {
  a[f * n + t] = v;
}

const size_t INF = 100500;

template <typename T>
T min(T a, T b) {
  return a < b ? a : b;
}

template <typename T>
T max(T a, T b) {
  return a > b ? a : b;
}

int main(int argc, char **argv) {
  size_t n;
  cin >> n;
  vector<vertex_t> v(n);
  for (size_t i = 0; i < n; ++i) cin >> v[i].val_;

  size_t m;
  cin >> m;
  vector<bool> a(n * n, false);
  for (size_t i = 0; i < m; ++i) {
    size_t f, t;
    cin >> f >> t;
    set_adj(a, n, f, t);
  }

  // floyd-warshall
  vector<size_t> d(n * n, 1);

  for (size_t k = 0; k < n; ++k)
    for (size_t j = 0; j < n; ++j)
      for (size_t i = 0; i < n; ++i)
        d[i * n + j] = min(d[i * n + j], d[i * n + k] + d[k * n + j]);

  // eccentricity
  vector<size_t> e(n);

  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j) e[i] = min(e[i], d[i * n + j]);

  // radius
  // diameter
  size_t rad;
  size_t diam;
  for (size_t i = 0; i < n; ++i) {
    rad = min(rad, e[i]);
    diam = max(diam, e[i]);
  }

  // center
  vector<size_t> c;
  for (size_t i = 0; i < n; ++i)
    if (e[i] == rad) c.push_back(i);

  cout << "radius: " << rad << endl;
  cout << "diameter: " << diam << endl;
  cout << "center: " << endl;
  for (auto ci : c) cout << ci << " ";
  cout << endl;

  return 0;
}
