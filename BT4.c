#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libfdr/jrb.h"
#include "libfdr/dllist.h"

#define INFINITIVE_VALUE 10000000
#define Queue Dllist
#define Stack Dllist

int a, b, c, d, r, m, n;

int x, y;

typedef struct
{
  int first;
  int second;
} Pair;

typedef struct
{
  JRB edges;
  JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, int weight);
int getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
void dropGraph(Graph graph);
int shortestPath(Graph graph, int s, int t, int numV, int *path, int *length);

//Queue API
void Enqueue(Queue *q, Jval jval)
{
  dll_append(*q, jval);
}

void Dequeue(Queue *q)
{
  Queue tmp = dll_first(*q);
  dll_delete_node(tmp);
}

Jval Front(Queue q)
{
  return dll_first(q)->val;
}
//////////////////

// Stack API
void Put(Stack *S, Jval jval)
{
  dll_prepend(*S, jval);
}

void Pop(Stack *S)
{
  Stack tmp = dll_first(*S);
  dll_delete_node(tmp);
}

Jval Top(Stack s)
{
  return dll_first(s)->val;
}
///////////////////

void readFile(Graph g, char *filename)
{
  int v1, v2, w;
  FILE *f = fopen(filename, "r");
  if (f == NULL)
  {
    printf("Can not read file!!!\n");
    return;
  }
  else
  {
    fscanf(f, "%d %d%*c", &n, &m);

    for (int i = 0; i < m; i++)
    {
      fscanf(f, "%d %d %d%*c", &v1, &v2, &w);
      addEdge(g, v1, v2, w);
      addEdge(g, v2, v1, w);
    }

    fscanf(f, "%d %d%*c", &a, &b);
    fscanf(f, "%d %d%*c", &c, &d);
    fscanf(f, "%d%*c", &r);
  }

  fclose(f);
}

double distance(Graph g, int v1, int v2)
{
  int path[n], length = 0;

  return shortestPath(g, v1, v2, n, path, &length);
}

int main()
{
  int nFind, v1, v2, w;
  Graph g = createGraph();

  // Use readFile or use input from keyboard

  // Use readFile
  // readFile(g, "robot.txt");

  // Use input from keyboard
  fscanf(stdin, "%d %d%*c", &n, &m);

  for (int i = 0; i < m; i++)
  {
    fscanf(stdin, "%d %d %d%*c", &v1, &v2, &w);
    addEdge(g, v1, v2, w);
    addEdge(g, v2, v1, w);
  }

  fscanf(stdin, "%d %d%*c", &a, &b);
  fscanf(stdin, "%d %d%*c", &c, &d);
  fscanf(stdin, "%d%*c", &r);

  ///////////////////////////
  int tmpPath[n], length = 0;
  int dist[n][n];
  Pair pair[n][n];
  int output[n];

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      dist[i][j] = shortestPath(g, i, j, n, tmpPath, &length);
    }
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      pair[i][j].first = pair[i][j].second = 0;
    }
  }

  //Setup
  Queue q = new_dllist();
  Enqueue(&q, new_jval_iarray(a, b));
  pair[a][b].first = pair[a][b].second = -1;

  while (!dll_empty(q))
  {
    int u = Front(q).iarray[0];
    int v = Front(q).iarray[1];
    Dequeue(&q);

    nFind = outdegree(g, u, output);
    for (int i = 0; i < nFind; i++)
    {
      int uu = output[i];
      if ((pair[uu][v].first == 0) && (pair[uu][v].second == 0) && (dist[uu][v] > r))
      {
        pair[uu][v].first = u;
        pair[uu][v].second = v;
        Enqueue(&q, new_jval_iarray(uu, v));
      }
    }

    nFind = outdegree(g, v, output);
    for (int i = 0; i < nFind; i++)
    {
      int vv = output[i];
      if ((pair[u][vv].first == 0) && (pair[u][vv].second == 0) && (dist[u][vv] > r))
      {
        pair[u][vv].first = u;
        pair[u][vv].second = v;
        Enqueue(&q, new_jval_iarray(u, vv));
      }
    }

    if (pair[c][d].first != 0 && pair[c][d].second != 0)
      break;
  }

  if (pair[c][d].first == 0 && pair[c][d].second == 0)
  {
    printf("Khong the\n");
  }
  else
  {
    // Print
    Stack s = new_dllist();

    while (pair[c][d].first != -1 && pair[c][d].second != -1)
    {
      Put(&s, new_jval_iarray(c, d));
      int x = pair[c][d].first;
      int y = pair[c][d].second;
      c = x;
      d = y;
    }

    printf("Lich di chuyen\t\tKhoang cach\n");
    printf("%d %d\t\t%19d\n", a, b, dist[a][b]);

    while (!dll_empty(s))
    {
      int first = Top(s).iarray[0];
      int second = Top(s).iarray[1];

      printf("%d %d\t\t%19d\n", first, second, dist[first][second]);
      Pop(&s);
    }
  }

  printf("\n");

  dropGraph(g);
}

Graph createGraph()
{
  Graph g;
  g.edges = make_jrb();
  g.vertices = make_jrb();
  return g;
}

void addVertex(Graph g, int id, char *name)
{
  JRB node = jrb_find_int(g.vertices, id);
  if (node == NULL) // only add new vertex
    jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));
}

char *getVertex(Graph g, int id)
{
  JRB node = jrb_find_int(g.vertices, id);
  if (node == NULL)
    return NULL;
  else
    return jval_s(node->val);
}

void addEdge(Graph graph, int v1, int v2, int weight)
{
  JRB node, tree;
  if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE)
  {
    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
    {
      tree = make_jrb();
      jrb_insert_int(graph.edges, v1, new_jval_v(tree));
    }
    else
    {
      tree = (JRB)jval_v(node->val);
    }
    jrb_insert_int(tree, v2, new_jval_i(weight));
  }
}

int getEdgeValue(Graph graph, int v1, int v2)
{
  JRB node, tree;
  node = jrb_find_int(graph.edges, v1);
  if (node == NULL)
    return INFINITIVE_VALUE;
  tree = (JRB)jval_v(node->val);
  node = jrb_find_int(tree, v2);
  if (node == NULL)
    return INFINITIVE_VALUE;
  else
    return jval_i(node->val);
}

int indegree(Graph graph, int v, int *output)
{
  JRB tree, node;
  int total = 0;
  jrb_traverse(node, graph.edges)
  {
    tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v))
    {
      output[total] = jval_i(node->key);
      total++;
    }
  }
  return total;
}

int outdegree(Graph graph, int v, int *output)
{
  JRB tree, node;
  int total;
  node = jrb_find_int(graph.edges, v);
  if (node == NULL)
    return 0;
  tree = (JRB)jval_v(node->val);
  total = 0;
  jrb_traverse(node, tree)
  {
    output[total] = jval_i(node->key);
    total++;
  }
  return total;
}

int shortestPath(Graph g, int s, int t, int numV, int *path, int *length)
{
  int *isSelectMin = (int *)calloc(numV, sizeof(int));
  int *inPQ = (int *)calloc(numV, sizeof(int));
  int distance[numV], min_dist, w, total;
  int min, u;
  int previous[numV], tmp[numV];
  int n, output[numV], v;
  Dllist ptr, queue, node;

  for (int i = 0; i < numV; i++)
    distance[i] = INFINITIVE_VALUE;
  distance[s] = 0;
  previous[s] = s;

  queue = new_dllist();
  dll_append(queue, new_jval_i(s));

  while (!dll_empty(queue))
  {
    // get u from the priority queue
    min_dist = INFINITIVE_VALUE;
    dll_traverse(ptr, queue)
    {
      u = jval_i(ptr->val);
      if (min_dist > distance[u])
      {
        min_dist = distance[u];
        min = u;
        node = ptr;
      }
    }
    dll_delete_node(node);
    u = min;
    inPQ[u] = 0;

    if (u == t)
      break;

    isSelectMin[u] = 1;
    n = outdegree(g, u, output);
    for (int i = 0; i < n; i++)
    {
      v = output[i];
      if (!isSelectMin[v])
      {
        w = getEdgeValue(g, u, v);
        if (distance[v] > distance[u] + w)
        {
          distance[v] = distance[u] + w;
          previous[v] = u;
        }
        if (!inPQ[v])
        {
          dll_append(queue, new_jval_i(v));
          inPQ[v] = 1;
        }
      }
    }
  }

  free(isSelectMin);
  free(inPQ);
  free(queue);

  total = distance[t];
  if (total != INFINITIVE_VALUE)
  {
    tmp[0] = t;
    n = 1;
    while (t != s)
    {
      t = previous[t];
      tmp[n++] = t;
    }
    for (int i = n - 1; i >= 0; i--)
      path[n - i - 1] = tmp[i];
    *length = n;
  }

  return total;
}

void dropGraph(Graph graph)
{
  JRB node, tree;
  jrb_traverse(node, graph.edges)
  {
    tree = (JRB)jval_v(node->val);
    jrb_free_tree(tree);
  }

  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
}