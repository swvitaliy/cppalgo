#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template <typename T>
using matrix = vector<vector<T>>;

template <typename T>
vector<T> gauss(matrix<T> a) {
  const size_t n = a.size();
  if (n == 0) return vector<T>(0);
  for (size_t i = 0; i < n - 1;
       ++i) {  // проходим сверху вниз до предпоследней строки
    for (size_t j = i + 1; j < n;
         ++j) {  // для каждой i-й строки проходимся по всем строкам ниже
      double coeff = a[j][i] / a[i][i];
      for (size_t k = i; k <= n;
           ++k) {  // и из j-й строки вычитаем i-ю, умноженную на coeff,
                   // так что a[j][i] будет равно 0
        a[j][k] -= coeff * a[i][k];
      }
    }
  }

  // for (size_t i = 0; i < n; ++i) {
  //	for (size_t j = 0; j <= n; ++j)
  //		cout << a[i][j] << " ";
  //	cout << endl;
  //}

  vector<T> r(n);
  for (int i = (int)n - 1; i >= 0; --i) {
    T s(0);
    for (size_t j = i + 1; j < n; ++j) s += r[j] * a[i][j];
    r[i] = (a[i][n] - s) / a[i][i];
  }
  return r;
}

int main(int argc, char** argv) {
  /*vector<double> r = gauss(
          matrix<double>({
                  {2,-1,1,1},
                  {4,1,-1,5},
                  {1,1,1,0}
          }));
*/
  vector<double> r =
      gauss(matrix<double>({{1, 1, 1, 2}, {2, 1, 1, 3}, {1, -1, 3, 8}}));
  for (size_t i = 0; i < 3; ++i) cout << r[i] << " ";
  cout << endl;
  return 0;
}