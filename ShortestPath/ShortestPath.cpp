//
// Created by yashal on 14/5/16.
//

#include <GL/glut.h>
#include <iostream>
#include <queue>
#include "ShortestPath.h"
#include "../Dijkstra/Dijkstra.h"
#include "../AStar/AStar.h"

using namespace std;
typedef ShortestPath SP;

// Structure to define the properties of a single square/node that will be drawn.
struct Square {
  int x;
  int y;
  float z;
  const float *color;
};

// Queue to draw the nodes that are visited by Djikstra and the open/close lists of A*
queue<Square> squareQueue;
// Queue to clear the nodes drawn by Djikstra
queue<Square> clearQueue;
// The nodes from GUI
int inputNodes[NUM_DIVISIONS][NUM_DIVISIONS];
int sourceNode[2], destinationNode[2];
int sourceNodeFlattened, destinationNodeFlattened;

static double finalResult[2][2];
int adMat[NUM_DIVISIONS * NUM_DIVISIONS][NUM_DIVISIONS * NUM_DIVISIONS];

int leftMouseButtonPressed = 0;
int obstacleBlockCount = 0;
int mode = MODE_START;

SP::ShortestPath() {
  for (int i = 0; i < NUM_DIVISIONS; i++) {
    for (int j = 0; j < NUM_DIVISIONS; j++) {
      inputNodes[i][j] = 1;
    }
  }
}

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
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glEnable(GL_DEPTH_TEST);

  glOrtho(0, w, 0, h, -1, 1);
}

void SP::timer(int t) {
  if (!squareQueue.empty()) {
    Square square = squareQueue.front();
    squareQueue.pop();
    drawSquare(square.x, square.y, square.z, square.color);
  }
  glutTimerFunc(timeDelay, timer, t);
}

void ShortestPath::clearDisplay() {
  while (!clearQueue.empty()) {
    Square square = clearQueue.front();
    clearQueue.pop();
    drawSquare(square.x, square.y, square.z, square.color);
  }
}

void  SP::addToSquareQueue(int i, int j, float z, const float *color) {
  if (mode == MODE_CALC1 || mode == MODE_CALC2 || mode == MODE_RESULT) {
    Square temp;
    temp.x = i;
    temp.y = j;
    temp.z = z;
    temp.color = color;
    squareQueue.push(temp);
  } else {
    drawSquare(i, j, z, color);
  }
}

void  SP::addToClearQueue(int i, int j, float z, const float *color) {
  Square temp;
  temp.x = i;
  temp.y = j;
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

void SP::drawBackground() {
  glColor3f(0, 0, 0);
  for (float i = 0.0; i < w + mouseDx; i = i + mouseDx) {
    glBegin(GL_LINE_STRIP);
    for (float j = 0.0; j < h + mouseDy; j = j + mouseDy)
      glVertex3f(i, j, 5);
    glEnd();
  }
  for (float i = 0.0; i < h + mouseDy; i = i + mouseDy) {
    glBegin(GL_LINE_STRIP);
    for (float j = 0.0; j < w + mouseDx; j = j + mouseDx)
      glVertex3f(j, i, 5);
    glEnd();
  }
}

void SP::reshape(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h); // Resize viewport to show new w and h.
  glOrtho(0, w, 0, h, -10, 10);

}

void SP::setTile(int i, int j) {
  if (inputNodes[j][i] != 1) {
    return;
  }
  switch (mode) {
    case MODE_START:
      inputNodes[j][i] = MODE_START;
      sourceNode[0] = j;
      sourceNode[1] = i;
      mode = MODE_END;
      addToSquareQueue(i, j, 1, COLOR_GREEN);
      return;
    case MODE_END:
      inputNodes[j][i] = MODE_END;
      destinationNode[0] = j;
      destinationNode[1] = i;
      mode = MODE_BLOCK;
      addToSquareQueue(i, j, 1, COLOR_ORANGE);
      return;
    case MODE_BLOCK:
      obstacleBlockCount++;
      inputNodes[j][i] = MODE_BLOCK;
      addToSquareQueue(i, j, 0, COLOR_BLACK);
      return;
  }

}

void SP::getDistanceMatrix() {

  for (int i = 0; i < NUM_DIVISIONS * NUM_DIVISIONS; i++) {
    for (int j = 0; j < NUM_DIVISIONS * NUM_DIVISIONS; j++) {
      adMat[i][j] = 0;
    }
  }
  for (int i = 0; i < NUM_DIVISIONS; i++) {
    for (int j = 0; j < NUM_DIVISIONS; j++) {
      if (inputNodes[i][j] == 0) {
        continue;
      }
      int position = NUM_DIVISIONS * i + j;
      if (inputNodes[i][j] == -1) {

        sourceNodeFlattened = position;
      } else if (inputNodes[i][j] == -2) {
        destinationNodeFlattened = position;
      }

      if (i > 0) {
        if (inputNodes[i - 1][j] != 0) {
          adMat[position][NUM_DIVISIONS * (i - 1) + j] = 1;
          adMat[NUM_DIVISIONS * (i - 1) + j][position] = 1;
        } else {
          adMat[position][NUM_DIVISIONS * (i - 1) + j] = -1;
          adMat[NUM_DIVISIONS * (i - 1) + j][position] = -1;
        }
      }

      if (i < NUM_DIVISIONS - 1) {
        if (inputNodes[i + 1][j] != 0) {
          adMat[position][position + NUM_DIVISIONS] = 1;
          adMat[position + NUM_DIVISIONS][position] = 1;
        } else {
          adMat[position][position + NUM_DIVISIONS] = -1;
          adMat[position + NUM_DIVISIONS][position] = -1;
        }
      }

      if (j > 0) {
        if (inputNodes[i][j - 1] != 0) {
          adMat[position][NUM_DIVISIONS * i + (j - 1)] = 1;
          adMat[NUM_DIVISIONS * i + (j - 1)][position] = 1;
        } else {
          adMat[position][NUM_DIVISIONS * i + (j - 1)] = -1;
          adMat[NUM_DIVISIONS * i + (j - 1)][position] = -1;
        }
      }

      if (j < NUM_DIVISIONS - 1) {
        if (inputNodes[i][j + 1] != 0) {
          adMat[position][position + 1] = 1;
          adMat[position + 1][position] = 1;
        } else {
          adMat[position][position + 1] = -1;
          adMat[position + 1][position] = -1;
        }
      }
    }
  }
}

// Input
void SP::mouseClickListener(int btn, int state, int x, int y) {
  int i = x / mouseDx;
  int j = y / mouseDy;

  if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    setTile(i, j);
    if (mode == MODE_BLOCK) {
      leftMouseButtonPressed = 1;
    }
  } else if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    leftMouseButtonPressed = 0;
  } else if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    globalCycle();
  }
  glutPostRedisplay();
}

void SP::keyboardListener(unsigned char key, int x, int y) {
  cout << "Mode " << mode<<endl;

  switch (key) {
    case 'z': {
      globalCycle();
    }
      break;
    case 'd': {
      getDistanceMatrix();
      mode = MODE_CALC1;
      Dijkstra *dijkstra = new Dijkstra();
      dijkstra->main(adMat, sourceNodeFlattened, destinationNodeFlattened, finalResult);
    }
      break;
    case 'a': {
      AStar *aStar = new AStar();
      aStar->main(sourceNode, destinationNode, inputNodes, finalResult);
      mode = MODE_RESULT;
    }
      break;
    case 'c': {
      clearDisplay();
      mode = MODE_CALC2;
    }
  }
  glutPostRedisplay();
};

void SP::mouseMotion(int x, int y) {
  if (mode == MODE_BLOCK && obstacleBlockCount > 0 && leftMouseButtonPressed) {
    int i = x / mouseDx;
    int j = y / mouseDy;
    setTile(i, j);
  }
}

void SP::globalCycle() {

  if (mode == MODE_CALC1) {
    mode = MODE_CALC2;
    clearDisplay();
  } else if (mode == MODE_CALC2) {
    AStar *aStar = new AStar();
    mode = MODE_RESULT;
    aStar->main(sourceNode, destinationNode, inputNodes, finalResult);
    cout << "Done" << mode;
  } else if (mode == MODE_RESULT) {
    char *temp;
    sprintf(temp, " zenity --info --title=\"Results\""
        " --text=\"Dijikstra\n\n"
        " Distance: %lf\n"
        " Time taken: %lf\n\n\n"
        " A* algorithm\n\n"
        " Distance: %lf\n"
        " Time taken: %lf\n\"", finalResult[0][0], finalResult[0][1], finalResult[1][0], finalResult[1][1]);
    system(temp);
  }
  else {
    getDistanceMatrix();
    mode = MODE_CALC1;
    Dijkstra *dijkstra = new Dijkstra();
    dijkstra->main(adMat, sourceNodeFlattened, destinationNodeFlattened, finalResult);
  }
}