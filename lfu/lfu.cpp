#include <exception>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

using std::swap;
using std::unordered_map;
using std::vector;

namespace lfu {

namespace heap {

template <typename T, typename F, typename G>
long sieve_up(vector<T> &h, long i, F less, G fn) {
  long p;
  while (i > 0) {
    p = i >> 1;
    if (less(h[i], h[p])) {
      swap(h[i], h[p]);
      fn(i, p);
    } else
      break;
    i = p;
  }

  return i;
}

template <typename T, typename F, typename G>
long sieve_down(vector<T> &h, long i, F less, G fn) {
  long l, r;
  long n = (long)h.size();
  while (i < n) {
    l = (i << 1) + 1;
    r = (i << 1) + 2;
    bool bl = l < n && less(h[l], h[i]);
    bool br = r < n && less(h[r], h[i]);
    if (bl && br) {
      if (less(h[l], h[r])) {
        swap(h[l], h[i]);
        fn(l, i);
        i = l;
      } else {
        swap(h[r], h[i]);
        fn(r, i);
        i = r;
      }
    } else if (bl) {
      swap(h[l], h[i]);
      fn(l, i);
      i = l;
    } else if (br) {
      swap(h[r], h[i]);
      fn(r, i);
      i = r;
    } else
      break;
  }
  return i;
}

template <typename T, typename F, typename G>
size_t insert(vector<T> &h, const T v, F less, G fn) {
  h.push_back(v);
  return sieve_up(h, h.size() - 1, less, fn);
}

template <typename T, typename F, typename G>
T extract(vector<T> &h, F less, G fn) {
  T ans;
  if (h.size() == 1) {
    ans = h[0];
    h.pop_back();
    return ans;
  }

  ans = h[0];
  h[0] = h.back();
  h.pop_back();
  sieve_down(h, 0, less, fn);
  return ans;
}

};  // namespace heap

typedef long freq;

template <typename K, typename T>
struct node {
  freq cnt;
  unsigned long long ndx;
  K key;
  T value;
};

// LDR algorithm
template <typename K, typename T>
struct less_ldr {
  struct tag {};
  unsigned long long &ndx;
  less_ldr(unsigned long long &n) : ndx(n) {}
  bool operator()(node<K, T> a, node<K, T> b) {
    return a.cnt / (ndx - a.ndx) < b.cnt / (ndx - b.ndx);
  }
};

template <typename K, typename T>
struct less {
  struct tag {};
  bool operator()(node<K, T> a, node<K, T> b) {
    if (a.cnt < b.cnt) return true;
    if (a.cnt == b.cnt) return a.ndx < b.ndx;
    return false;
  }
};

template <typename K, typename T, typename F>
struct cache {
 private:
  size_t cap;
  unsigned long long ndx;
  unordered_map<K, size_t> keys;
  vector<node<K, T>> data;
  F eLess_;

  template <typename It>
  size_t updateKey(K k, It &it) {
    size_t i = it->second;
    data[i].cnt++;
    data[i].ndx = ndx++;
    size_t j = heap::sieve_down(data, i, less<K, T>(), [=](long i, long j) {
      keys[data[i].key] = i;
      keys[data[j].key] = j;
    });

    // keys.erase(it);
    keys[k] = j;
    return j;
  }

 public:
  cache(const size_t n, typename less<K, T>::tag)
      : cap(n), keys(n), ndx(1), eLess_(less<K, T>()) {}
  cache(const size_t n, typename less_ldr<K, T>::tag)
      : cap(n), keys(n), ndx(1), eLess_(less_ldr<K, T>(ndx)) {}
  bool try_get(K k, T &ans) {
    if (cap == 0) return false;
    auto it = keys.find(k);
    if (it != keys.end()) {
      size_t j = updateKey(k, it);
      ans = data[j].value;
      return true;
    }
    return false;
  }
  void put(K k, T v) {
    if (cap == 0) return;
    auto it = keys.find(k);
    if (it != keys.end()) {
      size_t j = updateKey(k, it);
      data[j].value = v;
    } else {
      if (data.size() == cap) {
        auto minEl = heap::extract(data, eLess_, [=](long i, long j) {
          keys[data[i].key] = i;
          keys[data[j].key] = j;
        });
        auto minIt = keys.find(minEl.key);
        if (minIt == keys.end()) throw std::exception();
        keys.erase(minIt);
      }

      size_t i = heap::insert(data, {(freq)1, ndx++, k, v}, less<K, T>(),
                              [=](long i, long j) {
                                keys[data[i].key] = i;
                                keys[data[j].key] = j;
                              });
      keys.emplace(k, i);
    }
  }
};

}  // namespace lfu