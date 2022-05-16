/*
алгоритм Эдмонда Карпа для графов без кратных ребер
source: https://forum.vingrad.ru/topic-316902.html
*/

#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const int INF = 1 << 30;
int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int> > c(n, vector<int>(n)), f(n, vector<int>(n)), g(n);
  int from, to, cost;
  for (int i = 0; i < m; ++i) {
    cin >> from >> to >> cost;
    to--, from--;
    g[from].push_back(to);
    c[from][to] = cost;
  }
  // s - исток, t - сток
  int s = 0, t = n - 1;
  bool found;
  while (1) {
    vector<int> path(n, -1);
    queue<int> q;
    // помещаем s в очередь
    q.push(s);
    path[s] = s;
    found = false;
    while (!q.empty() && !found) {
      from = q.front();
      q.pop();
      for (size_t j = 0; j < g[from].size() && !found; j++) {
        to = g[from][j];
        if (path[to] == -1 && c[from][to] - f[from][to] > 0) {
          q.push(to);
          path[to] = from;
          found = to == t;
        }
      }
    }
    // если увеличивающий путь из s в t не найден
    if (!found) {
      break;
    }
    // ищем минимальную величину, на которую можно увеличить поток
    int cf = INF;
    for (to = n - 1; to != s;) {
      from = path[to];
      cf = min(cf, c[from][to] - f[from][to]);
      to = from;
    }
    // увеличиваем поток на эту величину, уменьшая при этом поток для обратныъ
    // ребер
    for (to = n - 1; to != s;) {
      from = path[to];
      f[from][to] += cf;
      f[to][from] -= cf;
      to = from;
    }
  }
  int flow = 0;
  for (size_t j = 0; j < g[s].size(); ++j) {
    flow += f[s][g[s][j]];
  }
  cout << flow;
}