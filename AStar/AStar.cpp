//
// Created by yashal on 17/5/16.
//

#include "AStar.h"

using namespace std;
typedef AStar AS;

// Determine priority (in the priority queue)
bool operator<(const node &a, const node &b) {
  return a.getPriority() > b.getPriority();
}

// Function to find the path
string AS::pathFind(const int &xStart, const int &yStart,
                    const int &xFinish, const int &yFinish) {
  static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
  static int pqi; // pq index
  static node *n0;
  static node *m0;
  static int i, j, x, y, xdx, ydy;
  static char c;
  pqi = 0;

  // Reset the node maps
  for (y = 0; y < m; y++) {
    for (x = 0; x < n; x++) {
      closed_nodes_map[x][y] = 0;
      open_nodes_map[x][y] = 0;
    }
  }

  // Create the start node and push into list of open nodes
  n0 = new node(xStart, yStart, 0, 0);
  n0->updatePriority(xFinish, yFinish);
  pq[pqi].push(*n0);

  open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map
  delete n0;
  // A* search
  while (!pq[pqi].empty()) {
    // get the current node w/ the highest priority
    // from the list of open nodes
    n0 = new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                  pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

    x = n0->getxPos();
    y = n0->getyPos();

    pq[pqi].pop(); // remove the node from the open list
    ShortestPath::addToSquareQueue(x,y,0.1,COLOR_BLUE);
    open_nodes_map[x][y] = 0;
    // mark it on the closed nodes map
    closed_nodes_map[x][y] = 1;

    // quit searching when the goal state is reached
    //if((*n0).estimate(xFinish, yFinish) == 0)
    if (x == xFinish && y == yFinish) {
      // generate the path from finish to start
      // by following the directions


      string path = "";
      while (true) {
        j = dir_map[x][y];
        c = '0' + (j + dir / 2) % dir;
        path = c + path;
        x += dx[j];
        y += dy[j];
        if((x == xStart && y == yStart)){
          break;
        }
        ShortestPath::addToSquareQueue(x,y,10,COLOR_RANDOM);
      }

      // garbage collection
      delete n0;
      // empty the leftover nodes
      while (!pq[pqi].empty()) pq[pqi].pop();
      return path;
    }

    // generate moves (child nodes) in all possible directions
    for (i = 0; i < dir; i++) {
      xdx = x + dx[i];
      ydy = y + dy[i];

      if (!(xdx < 0 || xdx > n - 1 || ydy < 0 || ydy > m - 1 || map[xdx][ydy] == 1
          || closed_nodes_map[xdx][ydy] == 1)) {
        // generate a child node
        m0 = new node(xdx, ydy, n0->getLevel(),
                      n0->getPriority());
        m0->nextLevel(i);
        m0->updatePriority(xFinish, yFinish);

        // if it is not in the open list then add into that
        if (open_nodes_map[xdx][ydy] == 0) {
          ShortestPath::addToSquareQueue(xdx,ydy,0.0,COLOR_GRAY);
          open_nodes_map[xdx][ydy] = m0->getPriority();
          pq[pqi].push(*m0);
          delete m0; // Only <-- new added by commenter // mark its parent node direction
          dir_map[xdx][ydy] = (i + dir / 2) % dir;
        }
        else if (open_nodes_map[xdx][ydy] > m0->getPriority()) {
          ShortestPath::addToSquareQueue(xdx,ydy,0.0,COLOR_RED);
          // update the priority info
          open_nodes_map[xdx][ydy] = m0->getPriority();
          // update the parent direction info
          dir_map[xdx][ydy] = (i + dir / 2) % dir;

          // replace the node
          // by emptying one pq to the other one
          // except the node to be replaced will be ignored
          // and the new node will be pushed in instead
          while (!(pq[pqi].top().getxPos() == xdx &&
              pq[pqi].top().getyPos() == ydy)) {
            pq[1 - pqi].push(pq[pqi].top());
            pq[pqi].pop();
          }
          pq[pqi].pop(); // remove the wanted node

          // empty the larger size pq to the smaller one
          if (pq[pqi].size() > pq[1 - pqi].size()) pqi = 1 - pqi;
          while (!pq[pqi].empty()) {
            pq[1 - pqi].push(pq[pqi].top());
            pq[pqi].pop();
          }
          pqi = 1 - pqi;
          pq[pqi].push(*m0);
          delete m0; // only 2nd item added new // add the better node instead
        }
        else delete m0; // garbage collection
      }
    }
    delete n0; // garbage collection
  }
  return ""; // no route found
}

int AS::main(int sourceV[2], int endV[2], int inputMatrix[n][m]) {

  for (int y = 0; y < m; y++) {
    for (int x = 0; x < n; x++) {
      if (inputMatrix[x][y] == 0) {
        map[y][x] = 1;
      } else {
        map[y][x] = 0;
      }
    }
  }

  int xA, yA, xB, yB;
  yA = sourceV[0];
  xA = sourceV[1];
  yB = endV[0];
  xB = endV[1];

  cout << "Map Size (X,Y): " << n << "," << m << endl;
  cout << "Start: " << xA << "," << yA << endl;
  cout << "Finish: " << xB << "," << yB << endl;

  // get the route
  clock_t start = clock();
  string route = AS::pathFind(xA, yA, xB, yB);
  if (route == "") cout << "An empty route generated!" << endl;
  clock_t end = clock();

  double time_elapsed = double(end - start);
  cout << "Time to calculate the route (ms): " << time_elapsed << endl;
  cout << "Route:" << endl;
  cout << route << endl << endl;
  return (0);
}

int node::max(int abs, int abs1) const {
  return abs>=abs1?abs:abs1;
}

