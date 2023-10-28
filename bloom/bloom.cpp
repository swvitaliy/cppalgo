#include <cassert>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

std::function<long(string)> Hash() {
  double rnd = (double)rand() / RAND_MAX;
  int seed = std::floor(rnd * 32) + 32;
  auto fn = [seed](string v) -> long {
    long r = 1;
    for (char c : v) r = (seed * r + c) & 0xffffff;
    return r;
  };
  return std::function<long(string)>(fn);
}

template <typename H>
struct bloom {
  vector<bool> bits;
  vector<H> hashes;

 public:
  bloom(const size_t n, const vector<H> h) : bits(n, 0), hashes(h) {}
  void add(const string v) {
    const size_t n = bits.size();
    for (size_t i = 0; i < hashes.size(); i++) bits[hashes[i](v) % n] = 1;
  }
  bool test(const string v) const {
    const size_t n = bits.size();
    for (size_t i = 0; i < hashes.size(); i++)
      if (!bits[hashes[i](v) % n]) return false;
    return true;
  }
};

int main(int argc, char **argv) {
  constexpr size_t M = 30;
  vector<std::function<long(string)>> hashes(M);
  for (size_t i = 0; i < M; i++) hashes[i] = Hash();
  bloom<std::function<long(string)>> a(1000, hashes);
  a.add("asdgasdr fasdfsdfasd");
  a.add("asdasdgasdf qwrq23rqwefasdfsdrwe");
  a.test("asdgasdr fasdfsdfasd");
  assert(a.test("asdgasdr fasdfsdfasd"));
  assert(a.test("zrhsseryhsdrtsdf"));
  return 0;
}