#include<iostream>
#include<vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;


typedef char color_t;
constexpr color_t WHITE = (char)0;
constexpr color_t GRAY = (char)1;
constexpr color_t BLACK = (char)2;

constexpr size_t INF = 0xffffffffffffffff;

size_t cycle(const vector<vector<int>> &a, vector<color_t> &c, vector<size_t> &par, size_t i) {
	c[i] = GRAY;
	for (size_t j = 0; j < a[i].size(); ++j)
		if (a[i][j]) {
			if (c[j] == WHITE) {
				par[j] = i;
				size_t l = cycle(a, c, par, j);
				if (l != INF)
					return l;
			} else if (c[j] == GRAY)
				return i;
		}
	c[i] = BLACK;
	return INF;
}

int main(int argc, char ** argv) {
	size_t n;
	cin >> n;
	vector<vector<int>> a(n, vector<int>(n));
	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < n; ++j)
			cin >> a[i][j];

	vector<color_t> c(n, WHITE);
	vector<size_t> par(n);

	for (size_t i = 0; i < n; ++i) {
		par[i] = INF;
		size_t p = cycle(a, c, par, i);
		while (p != INF) {
			cout << p << " ";
			p = par[p];
		}
		cout << endl;
	}

	return 0;
}
