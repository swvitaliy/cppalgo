#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

// https://www.geeksforgeeks.org/radix-sort/
void countsort(vector<int> &a, vector<int> &b, int exp) {
  const size_t n = a.size();
  int i;
  int c[10] = {0};

  for (auto ai : a) ++c[(ai / exp) % 10];

  for (i = 1; i < 10; ++i) c[i] += c[i - 1];

  for (i = n - 1; i >= 0; --i) {
    b[c[(a[i] / exp) % 10] - 1] = a[i];
    --c[(a[i] / exp) % 10];
  }

  std::swap(a, b);
}

void radixsort(vector<int> &a) {
  vector<int> b(a.size());
  auto it = std::max_element(a.begin(), a.end());
  int m = *it;  // max element
  for (int exp = 1; m / exp > 0; exp *= 10) countsort(a, b, exp);
}

int main(int argc, char **argv) {
  size_t n;
  cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < n; ++i) cin >> a[i];
  radixsort(a);
  for (auto ai : a) cout << ai << " ";
  cout << "\n";
  return 0;
}