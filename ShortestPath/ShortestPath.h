//
// Created by yashal on 14/5/16.
//

#ifndef CG2016_SHORTESTPATH_H
#define CG2016_SHORTESTPATH_H
const int NUM_DIVISIONS = 38;


const float COLOR_RED[4] = {1.0, 0.0, 0.0, 150.0};
const float COLOR_WHITE[4] = {1.0, 1.0, 1.0, 150.0};
const float COLOR_GREEN[4] = {0.0, 1.0, 0.0, 150.0};
const float COLOR_GRAY[4] = {0.5, 0.5, 0.5, 150.0};
const float COLOR_BLACK[4] = {0.0, 0.0, 0.0, 150.0};
const float COLOR_BLUE[4] = {0.0, 1.0, 1.0, 150.0};
const float COLOR_ORANGE[4] = {1.0, 1.0, 0.0, 150.0};
const float COLOR_RANDOM[4] = {0.6, 0.2, 0.4, 150.0};


class ShortestPath {
 public:
  int main(int argc, char *argv[]);
  static void disable();
  static void addToSquareQueue(int i, int j, float z, const float *color);
  static void addToClearQueue(int i, int j, float z, const float *color);
 private:
  static void glInit();
  static void display();
  static void drawBackground();
  static void mouseClickListener(int btn, int state, int x, int);
  static void mouseMotion(int x, int y);
  static void setTile(int i, int j);
  static void adMat();
  static void reshape(int w, int h);
  static void timer(int t);
  static void drawSquare(int i, int j, float z, const float *color);
  static void clearDisplay();
  static void keyboardListener(unsigned char key, int x, int y);
};


#endif //CG2016_SHORTESTPATH_H
