#include <iostream>
#include "main.h"
#include "ShortestPath/ShortestPath.h"
int main(int argc, char *argv[]) {
  system(" zenity --info --title=\"Visualizing shortest path algorithms\""
             " --text=\"Click to select a source,"
             " destination and then draw obstruction\n"
             "\nPress middle button to start the visualization\""
             "\nPress OK to continue");
  ShortestPath *shortestPath = new ShortestPath();
  shortestPath->main(argc, argv);
}