#include<iostream>
#include<vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;


template<typename T>
T min(T a, T b) {
	return a < b ? a : b;
}

template<typename T>
T max(T a, T b) {
	return a > b ? a : b;
}

namespace seg_tree {

template<typename T>
void build(vector<T> a, vector<T> &t, size_t v, size_t tl, size_t tr) {
	if (tl == tr)
		t[v] = a[tl];
	else {
		size_t tm = (tl + tr) / 2;
		build(a, t, 2*v, tl, tm);
		build(a, t, 2*v+1, tm+1, tr);
		t[v] = t[2*v] + t[2*v+1];
	}
}

template<typename T>
T sum(vector<T> t, size_t v, size_t tl, size_t tr, size_t l, size_t r) {
	if (l > r)
		return 0;
	if (tl == tr)
		return t[v];
	else {
		size_t tm = (tl + tr) / 2;
		return sum(t, 2*v, tl, tm, l, min(r, tm)) +
			sum(t, 2*v+1, tm+1, tr, max(l, tm+1), r);
	}
}


template<typename T>
void update(vector<T> &t, size_t v, size_t tl, size_t tr, size_t p, T val) {
	if (tl == tr)
		t[v] = val;
	else {
		size_t tm = (tl + tr) / 2;
		if (p <= tm)
			update(t, 2*v, tl, tm, p, val);
		else
			update(t, 2*v+1, tm+1, tr, p, val);
		t[v] = t[2*v] + t[2*v+1];
	}
}

}

int main(int argc, char ** argv) {
	size_t n;
	cin >> n;

	vector<int> a(n);
	for (size_t i = 0; i < n; ++i)
		cin >> a[i];

	vector<int> t(4*n+1);
	seg_tree::build(a, t, 1, 0, n-1);
	cout << seg_tree::sum(t, 1, 0, n-1, 0, 2) << endl;

	return 0;
}
