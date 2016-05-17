//
// Created by yashal on 14/5/16.
//

#include <GL/glut.h>
#include <iostream>
#include <queue>
#include "ShortestPath.h"
#include "../Dijkstra/Dijkstra.h"

struct Square {
  int i;
  int j;
  float z;
  const float *color;
};

float w = 720.0f, h = 720.0f;
float dx = w / NUM_DIVISIONS;
float dy = h / NUM_DIVISIONS;
typedef ShortestPath SP;

int vertice[NUM_DIVISIONS][NUM_DIVISIONS];
std::queue<Square> squareQueue;

const int MODE_START = -1;
const int MODE_END = -2;
const int MODE_BLOCK = 0;
const int MODE_CALC = 1;
int left_click = 0;
int block_count = 0;
int mode = MODE_START;
const int timeDelay = 23;

int SP::main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(w, h);
  glutCreateWindow("Shortest path");
  glInit();
  glutDisplayFunc(display);
  glutMouseFunc(mouseClickListener);
  glutMotionFunc(mouseMotion);
  glutReshapeFunc(reshape);
  glutTimerFunc(timeDelay, timer, 0);
  glutMainLoop();
}

void SP::display() {
  drawBackground();
  glFlush();
}

void SP::glInit() {

  for (int i = 0; i < NUM_DIVISIONS; i++) {
    for (int j = 0; j < NUM_DIVISIONS; j++) {
      vertice[i][j] = 1;
    }
  }

  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

   glEnable(GL_DEPTH_TEST);

  glOrtho(0, w, 0, h, -1, 1);
}

void SP::drawBackground() {
  glColor3f(0, 0, 0);
  for (float i = 0.0; i < w + dx; i = i + dx) {
    glBegin(GL_LINE_STRIP);
    for (float j = 0.0; j < h + dy; j = j + dy)
      glVertex2f(i, j);
    glEnd();
  }
  for (float i = 0.0; i < h + dy; i = i + dy) {
    glBegin(GL_LINE_STRIP);
    for (float j = 0.0; j < w + dx; j = j + dx)
      glVertex2f(j, i);
    glEnd();
  }
}

void SP::timer(int t) {
  if (!squareQueue.empty()) {
    Square square = squareQueue.front();
    squareQueue.pop();
    drawSquare(square.i, square.j, square.z, square.color);
  }
  glutTimerFunc(timeDelay, timer, t);
}
void ShortestPath::disable() {
  glDisable(GL_DEPTH_TEST);
}
void  SP::addToSquareQueue(int i, int j, float z, const float *color) {
  if (mode == MODE_CALC) {
    Square temp;
    temp.i = i;
    temp.j = j;
    temp.z = z;
    temp.color = color;
    squareQueue.push(temp);
  } else {
    drawSquare(i, j, z, color);
  }
}

void SP::drawSquare(int i, int j, float z, const float *color) {
  j = NUM_DIVISIONS - 1 - j;
  glColor4fv(color);
  glBegin(GL_QUADS);
  glVertex3f((i) * dx, (j) * dy, z);
  glVertex3f((i + 1) * dx, (j) * dy, z);
  glVertex3f((i + 1) * dx, (j + 1) * dy, z);
  glVertex3f((i) * dx, (j + 1) * dy, z);
  glEnd();
  glFlush();
}


void SP::reshape(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h); // Resize viewport to show new w and h.
  glOrtho(0, w, 0, h, -10, 10);

}

void SP::setTile(int i, int j) {
  if (vertice[j][i] != 1) {
    return;
  }
  switch (mode) {
    case MODE_START:
      vertice[j][i] = MODE_START;
      mode = MODE_END;
      addToSquareQueue(i, j, 1, COLOR_GREEN);
      return;
    case MODE_END:
      vertice[j][i] = MODE_END;
      mode = MODE_BLOCK;
      addToSquareQueue(i, j, 1, COLOR_ORANGE);
      return;
    case MODE_BLOCK:
      block_count++;
      vertice[j][i] = MODE_BLOCK;
      addToSquareQueue(i, j, 0, COLOR_BLACK);
      return;
  }

}
void SP::adMat() {
  int source, destination;

  int adMat[NUM_DIVISIONS * NUM_DIVISIONS][NUM_DIVISIONS * NUM_DIVISIONS];
  for (int i = 0; i < NUM_DIVISIONS * NUM_DIVISIONS; i++) {
    for (int j = 0; j < NUM_DIVISIONS * NUM_DIVISIONS; j++) {
      adMat[i][j] = 0;
    }
  }
  for (int i = 0; i < NUM_DIVISIONS; i++) {
    for (int j = 0; j < NUM_DIVISIONS; j++) {
      if (vertice[i][j] == 0) {
        continue;
      }
      int position = NUM_DIVISIONS * i + j;
      if (vertice[i][j] == -1) {
        source = position;
      } else if (vertice[i][j] == -2) {
        destination = position;
      }

      if (i > 0) {
        if (vertice[i - 1][j] != 0) {
          adMat[position][NUM_DIVISIONS * (i - 1) + j] = 1;
          adMat[NUM_DIVISIONS * (i - 1) + j][position] = 1;
        } else {
          adMat[position][NUM_DIVISIONS * (i - 1) + j] = -1;
          adMat[NUM_DIVISIONS * (i - 1) + j][position] = -1;
        }
      }

      if (i < NUM_DIVISIONS - 1) {
        if (vertice[i + 1][j] != 0) {
          adMat[position][position + NUM_DIVISIONS] = 1;
          adMat[position + NUM_DIVISIONS][position] = 1;
        } else {
          adMat[position][position + NUM_DIVISIONS] = -1;
          adMat[position + NUM_DIVISIONS][position] = -1;
        }
      }

      if (j > 0) {
        if (vertice[i][j - 1] != 0) {
          adMat[position][NUM_DIVISIONS * i + (j - 1)] = 1;
          adMat[NUM_DIVISIONS * i + (j - 1)][position] = 1;
        } else {
          adMat[position][NUM_DIVISIONS * i + (j - 1)] = -1;
          adMat[NUM_DIVISIONS * i + (j - 1)][position] = -1;
        }
      }

      if (j < NUM_DIVISIONS - 1) {
        if (vertice[i][j + 1] != 0) {
          adMat[position][position + 1] = 1;
          adMat[position + 1][position] = 1;
        } else {
          adMat[position][position + 1] = -1;
          adMat[position + 1][position] = -1;
        }
      }
    }
  }

  mode = MODE_CALC;
  Dijkstra *dijkstra = new Dijkstra();
  dijkstra->main(adMat, source, destination);
}

void SP::mouseClickListener(int btn, int state, int x, int y) {
  int i = x / dx;
  int j = y / dy;

  if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    setTile(i, j);
    if (mode == MODE_BLOCK) {
      left_click = 1;
    }
  } else if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    left_click = 0;
  } else if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    adMat();
  }
  glutPostRedisplay();
}

void SP::mouseMotion(int x, int y) {
  if (mode == MODE_BLOCK && block_count > 0 && left_click) {
    int i = x / dx;
    int j = y / dy;
    setTile(i, j);
  }
}