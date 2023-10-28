#include <cstdio>
#include<iostream>
#include<vector>
#include<utility>
#include<set>
#include<limits>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

// Knuth-Morris-Pratt

// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where
// the pattern starts in the text.
vector<int> prefix(const string& p) {
  const size_t n = p.size();
  vector<int> s(n);
  s[0] = 0;
  size_t b = 0;
  for (size_t i = 1; i < n; ++i) {
    while (b > 0 && p[i] != p[b]) {
      b = s[b-1];
    }
    if (p[i] == p[b])
      ++b;
    else
      b = 0;
    s[i] = b;
  }

  return s;
}

vector<int> find_pattern(const string& p, const string& t) {
  const size_t m = p.size();
  const size_t n = t.size();

  string s = p + '$' + t;
  vector<int> a = prefix(s);

  vector<int> ans;
  for (size_t i = m+1; i < n+m+1; ++i) {
    if (a[i] == (int)m)
      ans.push_back(i - 2*m);
  }

  return ans;
}

int main() {
  string pattern, text;
  cin >> pattern;
  cin >> text;
  vector<int> result = find_pattern(pattern, text);
  for (int i = 0; i < (int)result.size(); ++i) {
    printf("%d ", result[i]);
  }
  printf("\n");
  return 0;
}
