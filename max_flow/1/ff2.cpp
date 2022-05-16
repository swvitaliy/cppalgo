/*
Алгоритм Форда-Фалкерсона для графов с кратными ребрами
source: https://forum.vingrad.ru/topic-316902.html
*/

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;
const unsigned long long INF = 0 - 1;  // 1 << 30;

// для каждого ребра храним информацию об его обратном ребре
struct edge {
  unsigned long long from, to;
  unsigned long long inv_edge;
  unsigned long long cost;
  unsigned long long flow;
};

unsigned long long n, m;

// граф, пропускная способность, поток, остаточная пропускная способность для
// каждого из ребер
vector<edge> edges;

// граф обратный граф - т.е. множество входящих в вершину ребер
vector<vector<unsigned long long> > a, ia;

// исток и сток
unsigned long long s, t;
vector<unsigned long long> augmenting_path;

// с помощью поиска в ширину ищем увеличивающий путь
bool path_exists(unsigned long long *cmin) {
  vector<unsigned long long> used(n);

  // тут сохраняем номер ребра, по которому мы пришли в вершину
  vector<unsigned long long> path(n);
  queue<unsigned long long> q;
  q.push(s);
  used[s] = true;
  path[s] = -1;
  bool found = false;
  unsigned long long from;
  edge e;

  // пока не просмотрим все вершины или пока не найдем сток
  while (!q.empty() && !found) {
    from = q.front(), q.pop();
    // для всех исходящих ребер
    for (size_t j = 0; j < a[from].size() && !found; ++j) {
      e = edges[a[from][j]];
      if (!used[e.to] && (e.cost - e.flow) > 0) {
        q.push(e.to);
        used[e.to] = true;
        path[e.to] = a[from][j];
        // если достигли сток, то можно выходить
        found = e.to == t;
      }
    }

    // и для всех входящих
    for (size_t j = 0; j < ia[from].size() && !found; ++j) {
      e = edges[ia[from][j]];
      if (!used[e.to] && (e.cost - e.flow) > 0) {
        q.push(e.to);
        used[e.to] = true;
        path[e.to] = ia[from][j];
        // если достигли сток, то можно выходить
        found = e.to == t;
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
  for (int i = path[t]; i != -1;) {
    augmenting_path.push_back(i);  // заносим ребро, по которому пришли сюда
    e = edges[i];

    // минимальный добавляемый поток - самый слабый из потоков в пути
    if (*cmin > e.cost - e.flow) {
      *cmin = e.cost - e.flow;
    }

    i = path[e.from];
  }

  return true;
}

int main() {
  unsigned long long from, to, cap;
  cin >> n >> m;
  s = 0, t = n - 1;

  a.resize(n);
  ia.resize(n);
  edges.resize(2 * m);

  vector<unsigned long long> stock_edges;

  // cout << "[debug] input\n";

  // читаем граф
  for (unsigned long long i = 0; i < m; ++i) {
    cin >> from >> to >> cap;
    // from--, to--;

    // тут храним ссылки на ребра
    a[from].push_back(i);     // исходящие ребра
    ia[to].push_back(m + i);  // входящие ребра

    // а в ребрах - вся информация
    // от 0 до m-1 - прямые ребра, от m до 2m-1 - обратные
    edges[i].from = from;
    edges[i].to = to;
    edges[i].inv_edge = m + i;
    edges[i].cost = cap;
    edges[m + i].from = to;
    edges[m + i].to = from;
    edges[m + i].inv_edge = i;

    // запоминаем ребра, выходящие из стока
    if (from == s) {
      stock_edges.push_back(i);
    }
  }

  // cout << "[debug] run max_flow\n";

  // тут будем хранить увеличивающий путь
  augmenting_path.reserve(n);

  // минимальная пропускная способность потока
  unsigned long long cmin;

  // пока существует увеличивающий путь
  while (path_exists(&cmin)) {
    // вдоль всего увеличивающего пути увеличиваем поток
    for (size_t i = 0; i < augmenting_path.size(); ++i) {
      unsigned long long edge_ndx = augmenting_path[i];
      edges[edge_ndx].flow += cmin;
      edges[edges[edge_ndx].inv_edge].flow = -edges[edge_ndx].flow;
    }
  }

  unsigned long long flow = 0;
  for (size_t i = 0; i < stock_edges.size(); i++) {
    flow = flow + edges[stock_edges[i]].flow;
  }

  cout << flow;
  return 0;
}