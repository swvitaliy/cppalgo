/* show cycles in oriented graph */

#include<iostream>
#include<vector>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

struct vertex_t {
	vertex_t() : val_()
	{}
	vertex_t(const string val) : val_(val)
	{}
	string val_;
};

bool adj(vector<bool> a, size_t n, size_t f, size_t t) {
	return a[f * n + t];
}

void set_adj(vector<bool> &a, size_t n, size_t f, size_t t, bool v = true) {
	a[f * n + t] = v;
}

typedef int color_t;
constexpr color_t WHITE = 0;
constexpr color_t GRAY = 1;
constexpr color_t BLACK = 2;

struct cycle_t {
	cycle_t(const size_t start, const size_t end, const vector<size_t> path) :
		start_(start),
		end_(end),
		path_(path)
	{}
	size_t start_;
	size_t end_;
	vector<size_t> path_;
};

int main(int argc, char ** argv) {
	size_t n;
	cin >> n;
	vector<vertex_t> v(n);
	for (size_t i = 0; i < n; ++i)
		cin >> v[i].val_;

	size_t m;
	cin >> m;
	vector<bool> a(n * n, false);
	for (size_t i = 0; i < m; ++i) {
		size_t f, t;
		cin >> f >> t;
		set_adj(a, n, f, t);
	}

	vector<color_t> c(n, WHITE);
	vector<cycle_t> cycles;
	vector<size_t> p(n, n+1);

	auto dfs = [n, v, a, &c, &p, &cycles](size_t i, const auto &lambda) -> void {
		c[i] = GRAY;
		for (size_t j = 0; j < n; ++j)
			if (adj(a, n, i, j)) {
				if (c[j] == WHITE) {
					p[j] = i;
					lambda(j, lambda);
				} else if (c[j] == GRAY) {
					cycles.emplace_back(j, i, p);
				}
			}
		c[i] = BLACK;
	};

	for (size_t i = 0; i < n; ++i)
		if (c[i] == WHITE)
			dfs(i, dfs);

	if (cycles.empty())
		cout << "no cycles" << endl;

	for (auto cycle : cycles) {
//		cout << "cycle " << cycle.start_ << " " << cycle.end_ << endl;
//		for (auto i : cycle.path_)
//			cout << i << " ";
//		cout << endl;

		for (size_t i = cycle.end_; i != cycle.start_; i = cycle.path_[i])
			cout << v[i].val_ << " <- ";
		cout << v[cycle.start_].val_ << endl;
	}

	return 0;
}
