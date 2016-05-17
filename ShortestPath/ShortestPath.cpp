//
// Created by yashal on 14/5/16.
//

#include <GL/glut.h>
#include <iostream>
#include <queue>
#include "ShortestPath.h"
#include "../Dijkstra/Dijkstra.h"
#include "../AStar/AStar.h"

struct Square {
  int i;
  int j;
  float z;
  const float *color;
};

float w = 720.0f, h = 720.0f;
float mouseDx = w / NUM_DIVISIONS;
float mouseDy = h / NUM_DIVISIONS;
typedef ShortestPath SP;

int vertice[NUM_DIVISIONS][NUM_DIVISIONS];
std::queue<Square> squareQueue;
std::queue<Square> clearQueue;

const int MODE_START = -1;
const int MODE_END = -2;
const int MODE_BLOCK = 0;
const int MODE_CALC1 = 1;
const int MODE_CALC2 = 2;
int left_click = 0;
int block_count = 0;
int mode = MODE_START;
const int timeDelay = 10;

int SP::main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(w, h);
  glutCreateWindow("Shortest path");
  glInit();
  glutDisplayFunc(display);
  glutMouseFunc(mouseClickListener);
  glutMotionFunc(mouseMotion);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboardListener);
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
  for (float i = 0.0; i < w + mouseDx; i = i + mouseDx) {
    glBegin(GL_LINE_STRIP);
    for (float j = 0.0; j < h + mouseDy; j = j + mouseDy)
      glVertex3f(i, j,5);
    glEnd();
  }
  for (float i = 0.0; i < h + mouseDy; i = i + mouseDy) {
    glBegin(GL_LINE_STRIP);
    for (float j = 0.0; j < w + mouseDx; j = j + mouseDx)
      glVertex3f(j, i,5);
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

void ShortestPath::clearDisplay() {
  // drawBackground();
  while (!clearQueue.empty()) {
    Square square = clearQueue.front();
    clearQueue.pop();
    drawSquare(square.i, square.j, square.z, square.color);
  }

}
void ShortestPath::disable() {
  glDisable(GL_DEPTH_TEST);
}
void  SP::addToSquareQueue(int i, int j, float z, const float *color) {
  if (mode == MODE_CALC1 || mode == MODE_CALC2) {
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

void  SP::addToClearQueue(int i, int j, float z, const float *color) {
  Square temp;
  temp.i = i;
  temp.j = j;
  temp.z = z;
  temp.color = color;
  clearQueue.push(temp);
}


void SP::drawSquare(int i, int j, float z, const float *color) {
  j = NUM_DIVISIONS - 1 - j;
  glColor4fv(color);
  glBegin(GL_QUADS);
  glVertex3f((i) * mouseDx, (j) * mouseDy, z);
  glVertex3f((i + 1) * mouseDx, (j) * mouseDy, z);
  glVertex3f((i + 1) * mouseDx, (j + 1) * mouseDy, z);
  glVertex3f((i) * mouseDx, (j + 1) * mouseDy, z);
  glEnd();
  glFlush();
}


void SP::reshape(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h); // Resize viewport to show new w and h.
  glOrtho(0, w, 0, h, -10, 10);

}
int sourceV[2], destinationV[2];

void SP::setTile(int i, int j) {
  if (vertice[j][i] != 1) {
    return;
  }
  switch (mode) {
    case MODE_START:
      vertice[j][i] = MODE_START;
      sourceV[0] = j;
      sourceV[1] = i;
      mode = MODE_END;
      addToSquareQueue(i, j, 1, COLOR_GREEN);
      return;
    case MODE_END:
      vertice[j][i] = MODE_END;
      destinationV[0] = j;
      destinationV[1] = i;
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
  int sourcePoint, destinationPoint;
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

        sourcePoint = position;
      } else if (vertice[i][j] == -2) {
        destinationPoint = position;
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
  mode = MODE_CALC1;
  Dijkstra *dijkstra = new Dijkstra();
  dijkstra->main(adMat, sourcePoint, destinationPoint);
}

void SP::mouseClickListener(int btn, int state, int x, int y) {
  int i = x / mouseDx;
  int j = y / mouseDy;

  if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    setTile(i, j);
    if (mode == MODE_BLOCK) {
      left_click = 1;
    }
  } else if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    left_click = 0;
  } else if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    if (mode == MODE_CALC1) {
      //glInit();
      mode = MODE_CALC2;
      clearDisplay();
      /* AStar *aStar = new AStar();
       aStar->main(sourceV, destinationV, vertice);*/
    } else if (mode == MODE_CALC2) {
      AStar *aStar = new AStar();
      aStar->main(sourceV, destinationV, vertice);
    } else {
      adMat();
    }
  }
  glutPostRedisplay();
}

void SP::keyboardListener(unsigned char key, int x, int y) {
  switch (key) {
    case 'z': {
      if (mode == MODE_CALC1) {
        //glInit();
        mode = MODE_CALC2;
        clearDisplay();
        /* AStar *aStar = new AStar();
         aStar->main(sourceV, destinationV, vertice);*/
      } else if (mode == MODE_CALC2) {
        AStar *aStar = new AStar();
        aStar->main(sourceV, destinationV, vertice);
      } else {
        adMat();
      }
    }
      break;
    case 'd': {
      adMat();
    }
      break;
    case 'a': {
      AStar *aStar = new AStar();
      aStar->main(sourceV, destinationV, vertice);
    }
      break;
    case 'c': {
      clearDisplay();
    }
  }
  glutPostRedisplay();
};
void SP::mouseMotion(int x, int y) {
  if (mode == MODE_BLOCK && block_count > 0 && left_click) {
    int i = x / mouseDx;
    int j = y / mouseDy;
    setTile(i, j);
  }
}