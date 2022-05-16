#include <memory.h>
#include <stdio.h>

const int MAX_VERTICES = 50;

int NUM_VERTICES;            // число вершин в графе
const int INFINITY = 10000;  // условное число, обозначающее бесконечность

int f[MAX_VERTICES][MAX_VERTICES];  // f[i][j] - поток, текущий от вершины i к j
int c[MAX_VERTICES][MAX_VERTICES];  // c[i][j] - максимальная величина потока,
                                    // способная течь по ребру (i,j)

// набор вспомогательных переменных, используемых функцией FindPath - обхода в
// ширину
int Flow[MAX_VERTICES];  // Flow - значение потока через данную вершину на
                         // данном шаге поиска
int Link[MAX_VERTICES];  // Link[i] хранит номер предыдущей вешины на пути i ->
                         // исток
int Queue[MAX_VERTICES];  // очередь
int QP, QC;  // QP - указатель начала очереди и QC - число эл-тов в очереди

// поиск пути, по которому возможно пустить поток алгоритмом обхода графа в
// ширину функция ищет путь из истока в сток, по которому еще можно пустить
// поток, считая вместимость ребера (i,j) равной c[i][j] - f[i][j]

int FindPath(int source, int target)  // source - исток, target - сток
{
  QP = 0;
  QC = 1;
  Queue[0] = source;
  Link[target] = -1;
  memset(Flow, 0, sizeof(int) * NUM_VERTICES);
  Flow[source] = INFINITY;
  while (Link[target] == -1 && QP < QC) {
    int CurVertex = Queue[QP];
    for (int i = 0; i < NUM_VERTICES; i++)
      if ((c[CurVertex][i] - f[CurVertex][i]) > 0 && Flow[i] == 0) {
        Queue[QC] = i;
        QC++;
        Link[i] = CurVertex;
        if (c[CurVertex][i] - f[CurVertex][i] < Flow[CurVertex])
          Flow[i] = c[CurVertex][i];
        else
          Flow[i] = Flow[CurVertex];
      }
    QP++;
  }

  if (Link[target] == -1) return 0;
  int CurVertex = target;
  while (CurVertex != source) {
    f[Link[CurVertex]][CurVertex] += Flow[target];
    CurVertex = Link[CurVertex];
  }
  return Flow[target];
}

// основная функция поиска максимального потока
int MaxFlow(int source, int target)  // source - исток, target - сток
{
  memset(f, 0, sizeof(int) * MAX_VERTICES * MAX_VERTICES);
  int MaxFlow = 0;
  int AddFlow;
  do {
    AddFlow = FindPath(source, target);
    MaxFlow += AddFlow;
  } while (AddFlow > 0);
  return MaxFlow;
}

int main() {
  int n, m;
  scanf("%d %d", &n, &m);
  NUM_VERTICES = n;

  int source, target;
  // scanf("%d", &NUM_VERTICES);
  // scanf("%d %d", &source, &target);

  source = 0;
  target = n - 1;

  for (int i = 0; i < m; i++) {
    int v, u, a;
    scanf("%d %d %d", &v, &u, &a);
    c[v][u] = a;
  }

  printf("%d", MaxFlow(source, target));
  return 0;
}