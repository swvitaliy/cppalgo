#include<iostream>
#include<vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

typedef long key_type;
const long INF = 0x7fffffffffffffff;
const long NEG_INF = -0x7fffffffffffffff - 1;

enum node_type {
	left_node = 0,
	right_node = 1,
	root_node = 2
};

struct input_t {
	key_type key;
	node_type type;
	int parent;
	int left;
	int right;
};

template<typename T>
T min(T a, T b) {
	return a < b ? a : b;
}

template<typename T>
T max(T a, T b) {
	return a > b ? a : b;
}

int find(const vector<input_t> &a, int i, key_type key, node_type child_type) {
	if (i < 0)
		return -1;

	if (child_type == right_node && a[i].key == key)
		return i;

	return find(a, a[i].parent, key, a[i].type);		
}

bool walk_pre(vector<input_t> &a, int i, key_type min_key, key_type max_key, int parent, node_type type) {
	const size_t n = a.size();

	if (i < 0 || i >= n)
		return true;

	a[i].parent = parent;
	a[i].type = type;

	bool correct = (min_key < a[i].key && a[i].key < max_key) || 
		find(a, parent, a[i].key, type) != -1;
//cout << i << min_key << " " << a[i].key << " " << max_key << endl;

	return correct && walk_pre(a, a[i].left, min_key, min(max_key, a[i].key), i, left_node) &&
		walk_pre(a, a[i].right, max(min_key, a[i].key), max_key, i, right_node);
}

int main(int argc, char ** argv) {
	size_t n;
	cin >> n;
	vector<input_t> a(n);
	for (size_t i = 0; i < n; ++i)
		cin >> a[i].key >> a[i].left >> a[i].right;

	cout << (walk_pre(a, 0, NEG_INF, INF, -1, root_node) ? "CORRECT" : "INCORRECT") << endl;
	return 0;
}
