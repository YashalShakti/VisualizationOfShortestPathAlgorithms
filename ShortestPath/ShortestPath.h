//
// Created by yashal on 14/5/16.
//

#ifndef CG2016_SHORTESTPATH_H
#define CG2016_SHORTESTPATH_H
#include "Constants.h"

const int NUM_DIVISIONS = 38;


class ShortestPath {
 public:
  ShortestPath();
  int main(int argc, char *argv[]);
  static void addToSquareQueue(int i, int j, float z, const float *color);
  static void addToClearQueue(int i, int j, float z, const float *color);
  static void drawSquare(int i, int j, float z, const float *color);

 private:
  static void glInit();
  static void display();
  static void drawBackground();
  static void mouseClickListener(int btn, int state, int x, int);
  static void mouseMotion(int x, int y);
  static void setTile(int i, int j);
  static void getDistanceMatrix();
  static void reshape(int w, int h);
  static void timer(int t);
  static void clearDisplay();
  static void keyboardListener(unsigned char key, int x, int y);
  static void globalCycle();

  static const int timeDelay = 23;
  static constexpr float w = 720.0f, h = 720.0f;
  static constexpr float mouseDx = w / NUM_DIVISIONS;
  static constexpr float mouseDy = h / NUM_DIVISIONS;
};


#endif //CG2016_SHORTESTPATH_H
