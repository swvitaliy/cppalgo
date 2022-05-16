#include <iostream>
#include <vector>

using namespace std;

int n, k;
vector<vector<int> > g;
vector<int> bp;
vector<char> used;

bool kuhn(int v) {
  if (used[v]) return false;
  used[v] = true;
  for (int to : g[v]) {
    if (bp[to] == -1 || kuhn(bp[to])) {
      bp[to] = v;
      return true;
    }
  }
  return false;
}

int main() {
  cin >> n >> k;

  int t;
  g.assign(n, vector<int>());
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < k; ++j) {
      cin >> t;
      if (t) g[i].push_back(j);
    }
  }

  // cout << "run kuhn\n";

  bp.assign(k, -1);
  for (int v = 0; v < n; ++v) {
    used.assign(n, false);
    kuhn(v);
  }

  // cout << "print results\n";

  // g[i] массив из экипажей, которые могут i-м самолетом упралять
  // bp массив из k элементов вида [экипаж] => самолет

  vector<int> r(n, -1);
  for (int i = 0; i < k; ++i) {
    if (bp[i] != -1) r[bp[i]] = i;
  }

  // r массив из n элементов вида самолет => экипаж

  for (int i = 0; i < n; ++i) {
    cout << (r[i] == -1 ? -1 : r[i] + 1);
    if (i < n - 1) cout << " ";
  }

  cout << "\n";

  return 0;
}