#include <stdio.h>

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::vector;

template <typename T>
struct vertex_t {
  vertex_t(T val, size_t ndx) : val_(val), ndx_(ndx) {
    printf("create %d\n", val_);
  }
  void add_adjacent(shared_ptr<vertex_t<T>> a) {
    printf("add_adj %d\n", a->val_);
    adj_.push_back(a);
    a->adj_.push_back(make_shared<vertex_t<T>>(*this));
  }
  shared_ptr<vertex_t<T>> make_adjacent(T val, size_t ndx) {
    auto p = make_shared<vertex_t<int>>(val, ndx);
    add_adjacent(p);
    return p;
  }
  size_t ndx_;
  vector<shared_ptr<vertex_t<T>>> adj_;
  T val_;

 private:
};

template <typename T, typename Fn>
void dfs(vertex_t<T>& a, Fn fn, size_t vertex_n) {
  vector<shared_ptr<vertex_t<T>>> stack;
  vector<bool> visited(vertex_n);
  for (size_t i = 0; i < visited.size(); ++i) visited[i] = false;
  stack.push_back(make_shared<vertex_t<T>>(a));
  visited[a.ndx_] = true;
  while (!stack.empty()) {
    auto b = stack.back();
    stack.pop_back();
    fn(*b);
    for (auto p : b->adj_)
      if (!visited[p->ndx_]) {
        stack.push_back(p);
        visited[p->ndx_] = true;
      }
  }
}

template <typename T, typename Fn>
void bfs(vertex_t<T>& a, Fn fn, size_t vertex_n) {
  vector<shared_ptr<vertex_t<T>>> queue;
  vector<bool> visited(vertex_n);
  for (size_t i = 0; i < visited.size(); ++i) visited[i] = false;

  size_t q_top = 0;
  queue.push_back(make_shared<vertex_t<T>>(a));

  visited[a.ndx_] = true;
  while (q_top < queue.size()) {
    auto b = queue[q_top];
    ++q_top;

    fn(*b);
    for (auto p : b->adj_)
      if (!visited[p->ndx_]) {
        queue.push_back(p);
        visited[p->ndx_] = true;
      }
  }
}

int main(void) {
  vertex_t<int> r(0, 0);
  auto v1 = r.make_adjacent(1, 1);
  auto v2 = r.make_adjacent(2, 2);
  auto v3 = r.make_adjacent(3, 3);
  auto v4 = r.make_adjacent(4, 4);
  auto v5 = r.make_adjacent(5, 5);
  auto v6 = r.make_adjacent(6, 6);

  v2->add_adjacent(v4);
  v3->add_adjacent(v6);

  v5->make_adjacent(7, 7);

  bfs(
      r, [](vertex_t<int>& a) -> void { printf("visit %d\n", a.val_); }, 8);

  return 0;
}