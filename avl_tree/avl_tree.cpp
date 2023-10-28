#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::vector;

struct bin_tree {
  struct node {
    int key;
    int count;
    int h;
    node* p;
    node* l;
    node* r;

    node(node* par, int k)
        : key(k), count(1), h(1), p(par), l(nullptr), r(nullptr) {}

    ~node() {
      if (l != nullptr) delete l;
      if (r != nullptr) delete r;
    }

    void recalc_height() {
      h = 1 + max(l == nullptr ? 0 : l->h, r == nullptr ? 0 : r->h);
    }

    node* find(int k) {
      if (key == k) return this;
      if (k < key)
        return (l == nullptr) ? this : l->find(k);
      else
        return (r == nullptr) ? this : r->find(k);
    }
    node* next() {
      if (r != nullptr)
        return r->left_descendant();
      else
        return right_ancestor();
    }
    node* left_descendant() {  // левый потомок
      node* c = this;
      while (true) {
        if (c->l == nullptr) return c;
        c = c->l;
      }
      return nullptr;
    }
    node* right_ancestor() {  // правый предок
      node* c = this;
      while (c != nullptr) {
        if (c->key > key) return c;
        c = c->p;
      }
      return nullptr;
    }
    vector<node*> range(int l, int r) {
      vector<node*> a;
      node* c = find(l);
      while (c->key <= r) {
        if (c->key >= l) a.push_back(c);
        c = c->next();
      }

      return a;
    }
    void emplace_left(int k) { l = new node(this, k); }
    void emplace_right(int k) { r = new node(this, k); }
    void replace(node* x) {
      if (p != nullptr) {
        if (p->l == this)
          p->l = x;
        else
          p->r = x;
      }
    }
    node* remove(node*& xp) {
      if (l == nullptr && r == nullptr) {
        replace(nullptr);
        return this;
      }

      if (r == nullptr) {
        replace(l);
      } else {
        node* x = next();
        replace(x);
        x->replace(x->r);
        // x->l == nullptr
        x->l = l;
        x->r = r;
        xp = x->p;
      }

      l = nullptr;
      r = nullptr;
      return this;
    }
  };

  node* parent() { return p; }
  node* find(int k) { return (p == nullptr) ? nullptr : p->find(k); }
  vector<node*> range(int l, int r) {
    return (p == nullptr) ? vector<node*>() : p->range(l, r);
  }
  virtual void insert(int k, bool ah = true) {
    if (p == nullptr) {
      p = new node(nullptr, k);
      return;
    }

    node* n = p->find(k);
    if (k == n->key)
      ++n->count;
    else {
      if (k < n->key)
        n->emplace_left(k);
      else
        n->emplace_right(k);

      if (ah) adjust_height(n);
    }
  }
  void rotate_right(node* x) {
    if (x == nullptr) return;
    node* P = x->p;
    node* y = x->l;
    node* B = y->r;
    if (y == nullptr) return;
    x->l = B;
    B->p = x;
    y->r = x;
    y->p = P;
    x->replace(y);
    x->p = y;

    x->recalc_height();
    adjust_height(y);
  }
  void adjust_height(node* c) {
    while (c != nullptr) {
      c->recalc_height();
      c = c->p;
    }
  }

 protected:
  node* p;
};

struct avl_tree : bin_tree {
  virtual void insert(int k) { avl_insert(k); }

  void avl_insert(int k) {
    bin_tree::insert(k, false);
    node* n = find(k);
    rebalance(n);
  }

  virtual void remove(node* n) { avl_remove(n); }

  void avl_remove(node* n) {
    node* m;
    n->remove(m);
    delete n;
    rebalance(m);
  }

  void rebalance(node* n) {
    node* P = n->p;
    int lh = n->l == nullptr ? 0 : n->l->h;
    int rh = n->r == nullptr ? 0 : n->r->h;
    if (lh > rh + 1)
      rebalance_left(n);
    else if (rh > lh + 1)
      rebalance_right(n);

    n->recalc_height();
    if (P != nullptr) rebalance(P);
  }

 private:
  void rebalance_right(node* n) {
    node* m = n->l;
    int mlh = m->l == nullptr ? 0 : m->l->h;
    int mrh = m->r == nullptr ? 0 : m->r->h;

    if (mrh > mlh) rotate_left(m);

    rotate_right(n);
  }
};
