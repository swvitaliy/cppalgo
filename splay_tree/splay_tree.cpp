#include <iostream>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::vector;

namespace splay_tree {

typedef struct node {
  node* parent;
  long key;
  long sum;
  node* left;
  node* right;

  node() { node(-1); }

  node(const long k) : parent(nullptr), key(k), left(nullptr), right(nullptr) {
    sum = k;
  }

  ~node() {
    if (left != nullptr) delete left;
    if (right != nullptr) delete right;
  }
} node;

node* set_left(node* dst, node* src) {
  dst->left = src;
  if (src != nullptr) src->parent = dst;
  return src;
}

node* set_right(node* dst, node* src) {
  dst->right = src;
  if (src != nullptr) src->parent = dst;
  return src;
}

void zig(node* x, node* p) {
  node* g = p->parent;
  if (x == p->left) {
    long a = x->left ? x->left->sum : 0;
    long b = x->right ? x->right->sum : 0;
    long c = p->right ? p->right->sum : 0;

    set_left(p, x->right);
    set_right(x, p);
    x->sum = a + b + c + x->key + p->key;
    p->sum = b + c + p->key;
  } else {
    long a = p->left ? p->left->sum : 0;
    long b = x->left ? x->left->sum : 0;
    long c = x->right ? x->right->sum : 0;

    set_right(p, x->left);
    set_left(x, p);
    x->sum = a + b + c + x->key + p->key;
    p->sum = a + b + p->key;
  }

  x->parent = g;
  if (g != nullptr) {
    if (p == g->left)
      g->left = x;
    else
      g->right = x;
  }
}

void zigzig(node* x, node* p, node* g) {
  zig(p, g);
  zig(x, p);
}

void zigzag(node* x, node* p, node* g) {
  zig(x, p);
  zig(x, g);
}

void splay(node* x) {
  if (x == nullptr) return;
  if (x->parent == nullptr) return;
  node* p = x->parent;
  node* g = p->parent;
  if (g == nullptr)
    zig(x, p);
  else if (x == p->left && p == g->left || x == p->right && p == g->right)
    zigzig(x, p, g);
  else if (x == p->left && p == g->right || x == p->right && p == g->left)
    zigzag(x, p, g);
}

pair<node*, node*> search(node* x, long k) {
  node* p = nullptr;
  while (x != nullptr) {
    p = x->parent;
    if (x->key == k) {
      while (x->parent != nullptr) splay(x);

      return make_pair(x, x);
    }

    x = k < x->key ? x->left : x->right;
  }

  if (p != nullptr)
    while (p->parent != nullptr) splay(p);

  return make_pair(nullptr, p);
}

node* insert(node* x, long k) {
  node* p = nullptr;
  bool x_left;
  node* r = x;
  while (x != nullptr) {
    if (x->key == k) return r;
    p = x;
    x_left = k < x->key;
    x = x_left ? x->left : x->right;
  }

  if (x_left)
    set_left(p, x = new node(k));
  else
    set_right(p, x = new node(k));

  while (p != nullptr) {
    p->sum += k;
    p = p->parent;
  }

  while (x->parent != nullptr) splay(x);

  return x;
}

pair<node*, node*> split(node* x, long k) {
  search(x, k);
  node* r = x->right;
  x->right = nullptr;
  r->parent = nullptr;
  return make_pair(x, r);
}

node* merge(node* v1, node* v2) {
  if (v1 == nullptr) return v2;

  if (v2 == nullptr) return v1;

  v1->parent = nullptr;
  v2->parent = nullptr;

  while (v1->right != nullptr) v1 = v1->right;

  while (v1->parent != nullptr) splay(v1);

  v1->right = v2;
  v2->parent = v1;
  if (v2 != nullptr) v1->sum += v2->sum;
  return v1;
}

node* remove(node* x, long k) {
  if (x == nullptr) return x;
  auto p = search(x, k);
  if (p.first == nullptr) return x;

  node* new_root;
  new_root = merge(p.second->left, p.second->right);
  if (new_root != nullptr) new_root->parent = nullptr;

  node* rem = p.second;
  rem->left = nullptr;
  rem->right = nullptr;
  if (rem->parent != nullptr) {
    node* pp = rem->parent;
    if (pp->left == rem)
      pp->left = nullptr;
    else
      pp->right = nullptr;
  }
  delete rem;

  return new_root;
}

long seg_sum(node* x, long l, long r) {
  if (x == nullptr) return 0;

  long s = x->sum;
  long sl = 0;
  long sr = 0;
  node* b = nullptr;

  node* root = x;

  while (x != nullptr) {
    if (x->key < l) {
      x = x->right;
    }

    else if (x->key == l) {
      b = x;
      break;
    }

    else {
      b = x;
      x = x->left;
    }
  }

  if (b == nullptr)
    sl = 0;
  else {
    if (b->key > r) return 0;
    while (b->parent != nullptr) splay(b);
    x = b;
    sl = b->left ? b->left->sum : 0;
  }

  x = b ? b : root;
  b = nullptr;
  while (x != nullptr) {
    if (x->key > r) {
      x = x->left;
    }

    else if (x->key == r) {
      b = x;
      break;
    }

    else {
      b = x;
      x = x->right;
    }
  }

  if (b == nullptr)
    sr = 0;
  else {
    if (b->key < l) return 0;
    while (b->parent != nullptr) splay(b);
    x = b;
    sr = b->right ? b->right->sum : 0;
  }

  return s - sl - sr;
}

void print(splay_tree::node* x) {
  if (x != nullptr) {
    cout << x->key << " " << (x->left == nullptr ? -1 : x->left->key) << " "
         << (x->right == nullptr ? -1 : x->right->key) << " "
         << (x->parent ? x->parent->key : -1) << endl;

    print(x->left);
    print(x->right);
  }
}

}  // namespace splay_tree

int main(int argc, char** argv) {
  size_t n;
  cin >> n;

  constexpr long p = 1000000001;

  long s = 0;
  auto f = [&s, p](long x) -> long { return (x % p + s % p) % p; };

  char c;
  long k, b, e;
  splay_tree::node* r = nullptr;
  for (size_t i = 0; i < n; ++i) {
    cin >> c;
    // cout << "s:" << s << " c:" << c << endl;
    if (c == '+') {
      cin >> k;
      // cout << "k:" << k << " f(k):" << f(k) << endl;
      k = f(k);
      if (r == nullptr)
        r = new splay_tree::node(k);
      else
        r = splay_tree::insert(r, k);
    }

    else if (c == '-') {
      cin >> k;
      // cout << "k:" << k << " f(k):" << f(k) << endl;
      k = f(k);
      r = splay_tree::remove(r, k);
    }

    else if (c == '?') {
      cin >> k;
      // cout << "k:" << k << " f(k):" << f(k) << endl;
      k = f(k);
      auto p = splay_tree::search(r, k);
      cout << (p.first == nullptr ? "Not found" : "Found") << endl;
    }

    else if (c == 's') {
      cin >> b >> e;
      // cout << "b:" << b << " f(b):" << f(b) << endl;
      // cout << "e:" << e << " f(e):" << f(e) << endl;
      b = f(b);
      e = f(e);
      s = splay_tree::seg_sum(r, b, e);
      cout << s << endl;
    }

    while (r != nullptr && r->parent != nullptr) r = r->parent;

    // cout << "tree:" << endl;
    // splay_tree::print(r);

    // cout << "total:" << (r ? r->sum : 0) << endl;
  }

  delete r;
  return 0;
}