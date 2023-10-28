#include <sys/types.h>

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

template <class T>
using vec2_t = vector<vector<T>>;

template <typename T>
vec2_t<T> vec2(const size_t m, const size_t n) {
  vec2_t<T> a(m);
  for (size_t i = 0; i < m; i++) a[i] = vector<T>(n);
  return a;
}
template <typename T>
vec2_t<T> vec2(const size_t m, const size_t n, const T &&val) {
  vec2_t<T> a(m);
  for (size_t i = 0; i < m; i++) a[i] = vector<T>(n, val);
  return a;
}

template <typename T>
vector<T> lcs(const vector<T> &a, const vector<T> &b) {
  const size_t m = a.size();
  const size_t n = b.size();

  auto d = vec2<int>(m, n);
  for (size_t i = 0; i < m; i++) d[i][0] = 0;
  for (size_t j = 0; j < n; j++) d[0][j] = 0;

  for (size_t i = 1; i < m; i++) {
    for (size_t j = 1; j < n; j++) {
      if (a[i] == b[j])
        d[i][j] = d[i - 1][j - 1] + 1;
      else
        d[i][j] = std::max(d[i - 1][j], d[i][j - 1]);
    }
  }

  vector<int> ans;
  ssize_t i = m - 1;
  ssize_t j = n - 1;
  while (i > 0 && j > 0) {
    if (a[i] == b[j]) {
      ans.push_back(a[i]);
      --i;
      --j;
    } else {
      if (d[i][j] == d[i - 1][j])
        --i;
      else
        --j;
    }
  }

  std::reverse(ans.begin(), ans.end());
  return ans;
}

int main(int argc, char **argv) {
  size_t m, n;
  cin >> m >> n;
  vector<int> a(m);
  vector<int> b(n);
  for (size_t i = 0; i < m; i++) cin >> a[i];
  for (size_t j = 0; j < n; j++) cin >> b[j];

  auto c = lcs(a, b);

  for (auto ci : c) cout << ci << " ";
  cout << "\n";

  return 0;
}