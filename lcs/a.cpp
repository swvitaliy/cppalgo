#include <iostream>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::vector;

using ivec = vector<int>;

ivec lcs(ivec &x, ivec &y) {
  if (x.size() < y.size()) std::swap(x, y);
  size_t m = x.size();
  size_t n = y.size();
  ivec a(n, 0);
  int d = 0;  // a[i-1][j-1]
  for (size_t i = 1; i < m; ++i)
    for (size_t j = 1; j < n; ++j) {
      int t = a[j];
      if (x[i] == y[j])
        a[j] = d + 1;
      else
        a[j] = a[j] >= a[j - 1] ? a[j] : a[j - 1];
      d = t;
    }
  return a;
}

int main(int argc, char **argv) {
  size_t n, m;
  cin >> n >> m;
  ivec x(n);
  ivec y(m);
  for (size_t i = 0; i < n; ++i) cin >> x[i];
  for (size_t i = 0; i < m; ++i) cin >> y[i];
  auto a = lcs(x, y);
  for (auto ai : a) cout << ai << " ";
  cout << "\n";
  return 0;
}
