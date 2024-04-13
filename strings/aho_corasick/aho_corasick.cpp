#include <cstddef>
#include <iostream>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::make_pair;
using std::pair;
using std::queue;
using std::string;
using std::unordered_map;
using std::vector;

namespace aho_corasick {

typedef int pos;
typedef int ndx;

struct Node {
  unordered_map<char, Node *> children;
  Node *suf{};
  vector<int> terms;
  Node() = default;
  bool has_child(char c) const { return children.find(c) != children.end(); }
};

Node *add_string(Node *root, const string &s, int ndx) {
  Node *p = root;
  for (char c : s) {
    auto it = p->children.find(c);
    if (it != p->children.end())
      p = it->second;
    else {
      auto t = new Node();
      p->children[c] = t;
      p = t;
    }
  }
  p->terms.push_back(ndx);
  return p;
}

void build_suf_links(Node *root);
string graphviz(Node *root);

vector<pair<pos, ndx>> find_all(const vector<string> &patterns, const string &s) {
  Node *root = new Node();
  int i = 0;
  for (const string &p : patterns) add_string(root, p, i++);
  build_suf_links(root);

  cout << graphviz(root) << std::endl;

  vector<pair<pos, ndx>> ans;
  Node *node = root;
  int pos = 0;
  for (const char c : s) {
    while (node != root && !node->has_child(c)) node = node->suf;

    auto it = node->children.find(c);
    if (it != node->children.end()) node = it->second;

    for (int t : node->terms) ans.emplace_back(pos, t);

    pos++;
  }

  return ans;
}

void build_suf_links(Node *root) {
  queue<Node *> q;
  for (auto [c, u] : root->children) {
    q.push(u);
    u->suf = root;
  }

  while (!q.empty()) {
    Node *v = q.front();
    q.pop();
    for (auto [c, u] : v->children) {
      q.push(u);

      auto x = v->suf;
      while (x != root && !x->has_child(c)) x = x->suf;

      auto it = x->children.find(c);
      if (it == x->children.end())
        u->suf = root;
      else
        u->suf = it->second;

       for (auto t : u->suf->terms)
         u->terms.push_back(t);
    }
  }
}

string graphviz(Node *root) {
  const string term_node = R"([style="bold" color="blue"])";


  string s;
  int i = 0;
  unordered_map<Node *, int> ids;
  auto get_id = [&](Node *v) {
    if (ids.find(v) == ids.end()) {
      ids[v] = i++;
    }
    return ids[v];
  };
  ids[root] = i++;
  Node *v = root;
  queue<Node *> q;
  q.push(v);
  while (!q.empty()) {
    v = q.front();
    q.pop();

    if (!v->terms.empty()) {
      s += std::to_string(get_id(v)) + term_node + "\n";
    }

    int k = get_id(v);
    if (v->suf != nullptr) {
      s += std::to_string(k) + "->" + std::to_string(get_id(v->suf)) + R"([color="red" style="dashed"])" + "\n";
    }

    for (auto [c, u] : v->children) {
      q.push(u);

      int j = get_id(u);
      s += std::to_string(k) + "->" + std::to_string(j) + "[label=\"" + c + "\"]" + "\n";
    }
  }
  return "strict digraph {\n" + s + "\n}" + "\n";
}


}  // end of namespace aho_corasick

int main(int argc, char **argv) {
  int n;
  string s;
  cin >> n >> s;
  vector<string> patterns(n);
  for (int i = 0; i < n; i++)
    cin >> patterns[i];
  auto ans = aho_corasick::find_all(patterns, s);
  for (auto const &r : ans)
    cout << r.first << " " << r.second << "\n";
  return 0;
}