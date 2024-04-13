#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

namespace trie {
struct node {
  char parentChar;
  unordered_map<char, node*> child;
  node(const char c) : parentChar(c), child() {}
  ~node() {
    for (auto p : child) delete p.second;
  }
};

void add_string(node* root, string s) {
  node* cur = root;
  for (char c : s) {
    if (cur->child.find(c) == cur->child.end()) cur->child[c] = new node(c);
    cur = cur->child[c];
  }
  cur->child['$'] = new node('$');
}

bool find_string(node* root, string s) {
  node* cur = root;
  for (char c : s) {
    if (cur->child.find(c) == cur->child.end()) return false;
    cur = cur->child[c];
  }
  if (cur->child.find('$') == cur->child.end()) return false;
  return true;
}

bool find_prefix(node* root, string s) {
  node* cur = root;
  for (char c : s) {
    if (cur->child.find(c) == cur->child.end()) return false;
    cur = cur->child[c];
  }
  return true;
}

void delete_string(node* root, string s) {
  node* cur = root;
  for (char c : s) {
    if (cur->child.find(c) == cur->child.end()) return;
    cur = cur->child[c];
  }
  if (cur->child.find('$') == cur->child.end()) return;
  cur->child.erase('$');
}

}  // namespace trie
