//
// Created by yashal on 14/5/16.
//

#include "Dijkstra.h"

#include <stdio.h>
#include <limits.h>
#include <iostream>

int minDistance(int dist[], bool sptSet[]) {
  int min = INT_MAX, min_index;

  for (int v = 0; v < V; v++)
    if (sptSet[v] == false && dist[v] <= min)
      min = dist[v], min_index = v;

  return min_index;
}

int printSolution(int dist[], int n) {
  printf("Vertex   Distance from Source\n");
  for (int i = 0; i < V; i++)
    printf("%d \t\t %d\n", i, dist[i]);
}

void dijkstra(int graph[V][V], int src, int destination) {
  int dist[V];
  int parent[V];
  bool sptSet[V];

  for (int i = 0; i < V; i++)
    dist[i] = INT_MAX, sptSet[i] = false, parent[i] = 0;

  dist[src] = 0;

  for (int count = 0; count < V - 1; count++) {
    int u = minDistance(dist, sptSet);

    sptSet[u] = true;

    for (int v = 0; v < V; v++) {
      if (!sptSet[v] && graph[u][v] && graph[u][v] != -1 && dist[u] != INT_MAX
          && dist[u] + graph[u][v] < dist[v]) {
        dist[v] = dist[u] + graph[u][v];
        parent[v] = u;
        if (u == destination || v == destination) {
          goto result;
        }
        ShortestPath::addToSquareQueue(v % NUM_DIVISIONS, v / NUM_DIVISIONS, -1, COLOR_BLUE);
        ShortestPath::addToClearQueue(v % NUM_DIVISIONS, v / NUM_DIVISIONS, -0.5, COLOR_WHITE);
      } else if (graph[u][v] == -1) {
        ShortestPath::addToSquareQueue(v % NUM_DIVISIONS, v / NUM_DIVISIONS, 9, COLOR_RED);
        ShortestPath::addToClearQueue(v % NUM_DIVISIONS, v / NUM_DIVISIONS, 10, COLOR_BLACK);
      }
    }
  }
  result :
  int p = parent[destination];
  while (true) {
    std::cout << p << ">>";
    ShortestPath::addToSquareQueue(p % NUM_DIVISIONS, p / NUM_DIVISIONS, -0.5, COLOR_RANDOM);
    ShortestPath::addToClearQueue(p % NUM_DIVISIONS, p / NUM_DIVISIONS, 0, COLOR_WHITE);
    p = parent[p];
    if (p == src) {
      break;
    }
  }
  for (int i = 0; i < V; i++) {
  }
  printSolution(dist, V);
}

int Dijkstra::main(int graph[V][V], int source, int destination) {

  dijkstra(graph, source, destination);

  return 0;
}
