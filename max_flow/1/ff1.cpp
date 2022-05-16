/*
Алгоритм Форда-Фалкерсона для графа без кратных ребер
source: https://forum.vingrad.ru/topic-316902.html
*/

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int INF = 1 << 30;
int n, m;

// граф, пропускная способность, поток, остаточная пропускная способность
vector<vector<int> > a, c, f, cf;

// обратный граф - т.е. множество входящих в вершину ребер
vector<vector<int> > ia;

// исток и сток
int s, t;

vector<int> augmenting_path;

// с помощью поиска в ширину ищем увеличивающий путь
bool path_exists(int *cmin) {
  vector<int> used(n);
  vector<int> path(n);
  queue<int> q;

  q.push(s);
  used[s] = true;
  path[s] = -1;
  bool found = false;
  int from, to;

  // пока не просмотрим все вершины или пока не найдем сток
  while (!q.empty() && !found) {
    from = q.front(), q.pop();

    // для всех исходящих ребер
    for (size_t j = 0; j < a[from].size() && !found; ++j) {
      to = a[from][j];
      if (!used[to] && cf[from][to] > 0) {
        q.push(to);
        used[to] = true;
        path[to] = from;
        // если достигли сток, то можно выходить
        found = to == t;
      }
    }

    // и для всех входящих
    for (size_t j = 0; j < ia[from].size() && !found; ++j) {
      to = ia[from][j];
      if (!used[to] && cf[from][to] > 0) {
        q.push(to);
        used[to] = true;
        path[to] = from;
        // если достигли сток, то можно выходить
        found = to == t;
      }
    }
  }

  // если сток недостижим, то увеличивающего путь не существует
  if (!found) {
    return false;
  }

  // если достигли сток, то строим увеличивающий путь
  augmenting_path.clear();

  // начинаем с конца
  *cmin = INF;

  //пока не достигли начала
  for (to = t; to != s;) {
    augmenting_path.push_back(to);  // заносим номер текущей вершины

    // переходим на след. вершину - по которой пришли в i в пути
    from = path[to];
    // минимальный добавляемый поток - самый слабый из потоков в пути
    if (*cmin > cf[from][to]) {
      *cmin = cf[from][to];
    }

    to = from;
  }

  augmenting_path.push_back(s);
  return true;
}
int main() {
  int from, to, cap;
  cin >> n >> m;

  a.resize(n);
  ia.resize(n);

  c.assign(n, vector<int>(n));
  f.assign(n, vector<int>(n));
  cf.assign(n, vector<int>(n));

  // читаем граф
  for (int i = 0; i < m; ++i) {
    cin >> from >> to >> cap;
    from--, to--;

    a[from].push_back(to);   // исходящие ребра
    ia[to].push_back(from);  // входящие ребра
    c[from][to] = cap;
    cf[from][to] = cap;
  }

  s = 0, t = n - 1;

  // тут будем хранить увеличивающий путь
  augmenting_path.reserve(n);

  // минимальная пропускная способность потока
  int cmin;

  // пока существует увеличивающий путь
  while (path_exists(&cmin)) {
    // вдоль всего увеличивающего пути увеличиваем поток
    for (size_t i = augmenting_path.size() - 1; i > 0; --i) {
      from = augmenting_path[i];
      to = augmenting_path[i - 1];
      f[from][to] = f[from][to] + cmin;
      f[to][from] = -f[from][to];
      cf[from][to] = c[from][to] - f[from][to];
      cf[to][from] = c[to][from] - f[to][from];
    }
  }

  int flow = 0;
  for (size_t i = 0; i < a[s].size(); i++) {
    to = a[s][i];
    flow = flow + f[s][to];
  }

  cout << flow;

  return 0;
}