#include<iostream>
#include<vector>
#include<set>
#include<algorithm>
#include<utility>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::set;
using std::pair;
using std::make_pair;

typedef pair<int,int> job;
set<job> _1sumu(vector<int> &p, vector<int> &d) {
  const size_t n = p.size();
  std::sort(d.begin(), d.end());
  set<job> s;
  int t = 0;
  for (int i = 0; i < (int)n; ++i) {
    s.emplace(p[i], i);
    t += p[i];
    if (t > d[i]) {
      job max_job = *s.rbegin();
      s.erase(max_job);
      t -= max_job.first;
    }
  }
  return s;
}

int main(int argc, char ** argv) {
  size_t n;
  cin >> n;
  vector<int> p(n);
  vector<int> d(n);
  for (size_t i = 0; i < n; ++i)
    cin >> p[i] >> d[i];
  auto b = _1sumu(p, d);
  for (auto bi : b)
    cout << bi.second << " " ;
  cout << "\n";
	return 0;
}
