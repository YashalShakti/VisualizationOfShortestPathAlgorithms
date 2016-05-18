//
// Created by yashal on 14/5/16.
//

#include "Dijkstra.h"

#include <stdio.h>
#include <limits.h>
#include <iostream>

// A C / C++ program for Dijkstra's single source shortest path algorithm.
// The program is for adjacency matrix representation of the graph


// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(int dist[], bool sptSet[]) {
  // Initialize min value
  int min = INT_MAX, min_index;

  for (int v = 0; v < V; v++) {
    if (sptSet[v] == false && dist[v] <= min) {
      min = dist[v], min_index = v;
    }
  }
  return min_index;
}

// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
int dijkstra(int graph[V][V], int src, int destination) {
  int dist[V]; // The output array.  dist[i] will hold the shortest
  // distance from src to i
  int parent[V];
  bool sptSet[V];// sptSet[i] will true if vertex i is included in shortest
  // path tree or shortest distance from src to i is finalized

  // Initialize all distances as INFINITE and stpSet[] as false
  int finalDistance = 0;

  for (int i = 0; i < V; i++)
    dist[i] = INT_MAX, sptSet[i] = false, parent[i] = 0;

  // Distance of source vertex from itself is always 0
  dist[src] = 0;

  // Find shortest path for all vertices
  for (int count = 0; count < V - 1; count++) {

    // Pick the minimum distance vertex from the set of vertices not
    // yet processed. u is always equal to src in first iteration.
    int u = minDistance(dist, sptSet);

    sptSet[u] = true;
    // Update dist value of the adjacent vertices of the picked vertex.
    for (int v = 0; v < V; v++) {
      if (!sptSet[v] && graph[u][v] && graph[u][v] != -1 && dist[u] != INT_MAX
          && dist[u] + graph[u][v] < dist[v]) {
        dist[v] = dist[u] + graph[u][v];
        parent[v] = u;
        if (v == destination) {
          finalDistance = dist[v];
          std::cout << "The distance of the route by Dijkstra(ms): " << dist[v] << std::endl;
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
    ShortestPath::addToSquareQueue(p % NUM_DIVISIONS, p / NUM_DIVISIONS, -0.5, COLOR_RANDOM);
    ShortestPath::addToClearQueue(p % NUM_DIVISIONS, p / NUM_DIVISIONS, 0, COLOR_WHITE);
    p = parent[p];
    if (p == src) {
      break;
    }
  }
  return finalDistance;
}

int Dijkstra::main(int graph[V][V], int source, int destination, double final[2][2]) {

  clock_t start = clock();
  final[0][0] = dijkstra(graph, source, destination);
  clock_t end = clock();
  double time_elapsed = double(end - start);
  final[0][1] = time_elapsed;
  std::cout << "Time to calculate the route by Dijkstra(ms): " << time_elapsed << std::endl;
  return 0;
}
