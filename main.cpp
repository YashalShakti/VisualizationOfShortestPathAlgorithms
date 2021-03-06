#include <iostream>
#include "main.h"
#include "ShortestPath/ShortestPath.h"
#include "AStar/AStar.h"
int main(int argc, char *argv[]) {
  system(" zenity --info --title=\"Visualizing shortest path algorithms\""
             " --text=\"Click to select a source,"
             " destination and then draw obstacles\n"
             "\nPress middle button/z to toggle the states\n"
             "\nA: Running A* only\n"
             "\nD: Running Djikstras' only\n"
             "\nPress OK to continue\"");
  ShortestPath *shortestPath = new ShortestPath();
  shortestPath->main(argc, argv);
}