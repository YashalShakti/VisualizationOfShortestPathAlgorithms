//
// Created by yashal on 14/5/16.
//

#ifndef CG2016_DIJKSTRA_H
#define CG2016_DIJKSTRA_H
#include "../ShortestPath/ShortestPath.h"

const int V=NUM_DIVISIONS*NUM_DIVISIONS;

class Dijkstra {
 public:
  int main(int graph[V][V], int source, int destination);
};


#endif //CG2016_DIJKSTRA_H
