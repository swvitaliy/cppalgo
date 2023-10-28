#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

constexpr int L = 5;

int ndx(char c) {
  if (c == '$') return 0;
  if (c == 'A') return 1;
  if (c == 'C') return 2;
  if (c == 'G') return 3;
  if (c == 'T') return 4;
  return -1;
}

vector<int> sort_chars(const string &s) {
  const size_t n = s.size();
  vector<int> order(n);
  vector<int> count(L, 0);
  for (char c : s)
    count[ndx(c)]++;
  for (int i = 1; i < L; ++i)
    count[i] += count[i-1];
  for (int i = n-1; i >= 0; --i) {
    char c = s[i];
    count[ndx(c)]--;
    order[count[ndx(c)]] = i;
  }
  return order;
}

vector<int> compute_char_classes(const string &s, const vector<int> &order) {
  const size_t n = s.size();
  vector<int> classes(n);
  classes[order[0]] = 0;
  for (size_t i = 1; i < n; ++i) {
    if (s[order[i]] != s[order[i-1]]) {
      classes[order[i]] = classes[order[i-1]]+1;
    } else {
      classes[order[i]] = classes[order[i-1]];
    }
  }

  return classes;
}

vector<int> sort_doubled(const string &s, int l, const vector<int> &order, const vector<int> &classes) {
  const size_t n = s.size();
  vector<int> count(n, 0);
  vector<int> new_order(n);
  for (size_t i = 0; i < n; ++i)
    count[classes[i]]++;
  for (size_t j = 1; j < n; ++j) {
    count[j] += count[j-1];
  }

  for (int i = n - 1; i >= 0; --i) {
    int start = (order[i] - l + n) % n;
    int cl  = classes[start];
    count[cl] -= 1;
    new_order[count[cl]] = start;
  }

  return new_order;
}

vector<int> update_classes(const vector<int> &new_order, const vector<int> &classes, int l) {
  const size_t n = new_order.size();
  vector<int> new_classes(n);
  new_classes[new_order[0]] = 0;
  for (size_t i = 1; i < n; ++i) {
    int cur = new_order[i];
    int prev = new_order[i-1];
    int mid = cur + l;
    int midPrev = (prev + l) % n;
    if (classes[cur] != classes[prev] ||
        classes[mid] != classes[midPrev]) {
      new_classes[cur] = new_classes[prev] + 1;
    } else {
      new_classes[cur] = new_classes[prev];
    }
  }

  return new_classes;
}

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string& s) {
  const size_t n = s.size();
  vector<int> order = sort_chars(s);
  vector<int> classes = compute_char_classes(s, order);
  int l = 1;
  while (l < (int)n) {
    order = sort_doubled(s, l, order, classes);
    classes = update_classes(order, classes, l);
    l = l * 2;
  }
  return order;
}

bool gt(const string& p, const string &s, int i) {
  int j = 0;
  while (s[i] != '$') {
    if (p[j] > s[i])
      return true;
    else if (p[j] < s[i])
      return false;
    ++j;
  }
  
  return false;
}

bool lt(const string& p, const string &s, int i) {
  int j = 0;
  while (s[i] != '$') {
    if (p[j] > s[i])
      return false;
    else if (p[j] < s[i])
      return true;
    ++j;
  }
  
  return false;
}

vector<int> FindOccurrences(const string& pattern, const string& s, const vector<int>& suffix_array) {

  const size_t n = s.size();

  int min_index = 0;
  int max_index = (int)n;
  
  while (min_index < max_index) {
    int mid = (min_index + max_index) / 2;
    if (gt(pattern, s, suffix_array[mid]))
      min_index = mid + 1;
    else
      max_index = mid;
  }
  
  int start = min_index;
  max_index = n;
  
  while (min_index < max_index) {
    int mid = (min_index + max_index) / 2;
    if (lt(pattern, s, suffix_array[mid]))
      max_index = mid;
    else
      min_index = mid + 1;
  }
  
  int end = max_index;
  if (start > end)
    return {};

  return {start,end};
}

int main() {
  char buffer[100001];
  scanf("%s", buffer);
  string text = buffer;
  text += '$';
  vector<int> suffix_array = BuildSuffixArray(text);
  int pattern_count;
  scanf("%d", &pattern_count);
  vector<bool> occurs(text.length(), false);
  for (int pattern_index = 0; pattern_index < pattern_count; ++pattern_index) {
    scanf("%s", buffer);
    string pattern = buffer;
    vector<int> occurrences = FindOccurrences(pattern, text, suffix_array);
    for (int j = 0; j < occurrences.size(); ++j) {
      occurs[occurrences[j]] = true;
    }
  }
  for (int i = 0; i < occurs.size(); ++i) {
    if (occurs[i]) {
      printf("%d ", i);
    }
  }
  printf("\n");
  return 0;
}
