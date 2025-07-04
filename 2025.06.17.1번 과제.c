#include <GL/freeglut.h>
#include <math.h>

float rotateX = 20.0f, rotateY = 30.0f;
int lastMouseX, lastMouseY;
int isDragging = 0;

int currentLine = 0;
int currentFace = 0;
int gridProgress = 0;
const int GRID_DIV = 10; // 10x10 그리드

GLfloat cubeVertices[8][3] = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
    {-1, -1,  1}, {1, -1,  1}, {1, 1,  1}, {-1, 1,  1}
};

int cubeEdges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

int faceIndices[6][4] = {
    {4, 5, 6, 7}, {0, 1, 2, 3}, {0, 3, 7, 4},
    {1, 2, 6, 5}, {3, 2, 6, 7}, {0, 1, 5, 4}
};

float faceColors[6][4][3] = {
    {{1,0,0},{1,0.5,0},{1,1,0},{0,1,0}},          // 앞면
    {{0,1,0},{0,1,1},{0,0,1},{0.5,0,0.5}},        // 뒷면
    {{1,0,0},{0.5,0,0.5},{0,1,1},{1,1,0}},        // 왼쪽면
    {{1,0.5,0},{0,0,1},{0,1,0},{1,0,0}},          // 오른쪽면
    {{1,1,0},{1,0.5,0},{0.5,0,0.5},{0,1,1}},      // 윗면
    {{0,0,1},{0,1,0},{1,1,0},{1,0,0}}             // 아랫면
};

void drawCubeLines() {
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    for (int i = 0; i < currentLine; i++) {
        int* edge = cubeEdges[i];
        glVertex3fv(cubeVertices[edge[0]]);
        glVertex3fv(cubeVertices[edge[1]]);
    }
    glEnd();
}

void drawSingleFaceGrid(int faceIndex, int progress) {
    int* idx = faceIndices[faceIndex];
    GLfloat* v0 = cubeVertices[idx[0]];
    GLfloat* v1 = cubeVertices[idx[1]];
    GLfloat* v2 = cubeVertices[idx[2]];
    GLfloat* v3 = cubeVertices[idx[3]];

    for (int i = 0; i < GRID_DIV; i++) {
        for (int j = 0; j < GRID_DIV; j++) {
            if (i * GRID_DIV + j >= progress)
                return;

            float s0 = (float)i / GRID_DIV;
            float s1 = (float)(i + 1) / GRID_DIV;
            float t0 = (float)j / GRID_DIV;
            float t1 = (float)(j + 1) / GRID_DIV;

            float p0[3], p1[3], p2[3], p3[3];
            for (int k = 0; k < 3; k++) {
                p0[k] = (1 - s0) * ((1 - t0) * v0[k] + t0 * v3[k]) + s0 * ((1 - t0) * v1[k] + t0 * v2[k]);
                p1[k] = (1 - s1) * ((1 - t0) * v0[k] + t0 * v3[k]) + s1 * ((1 - t0) * v1[k] + t0 * v2[k]);
                p2[k] = (1 - s1) * ((1 - t1) * v0[k] + t1 * v3[k]) + s1 * ((1 - t1) * v1[k] + t1 * v2[k]);
                p3[k] = (1 - s0) * ((1 - t1) * v0[k] + t1 * v3[k]) + s0 * ((1 - t1) * v1[k] + t1 * v2[k]);
            }

            // 보간된 색상 계산
            float r = (1 - s0) * ((1 - t0) * faceColors[faceIndex][0][0] + t0 * faceColors[faceIndex][3][0])
                + s0 * ((1 - t0) * faceColors[faceIndex][1][0] + t0 * faceColors[faceIndex][2][0]);
            float g = (1 - s0) * ((1 - t0) * faceColors[faceIndex][0][1] + t0 * faceColors[faceIndex][3][1])
                + s0 * ((1 - t0) * faceColors[faceIndex][1][1] + t0 * faceColors[faceIndex][2][1]);
            float b = (1 - s0) * ((1 - t0) * faceColors[faceIndex][0][2] + t0 * faceColors[faceIndex][3][2])
                + s0 * ((1 - t0) * faceColors[faceIndex][1][2] + t0 * faceColors[faceIndex][2][2]);

            glColor3f(r, g, b);

            glBegin(GL_QUADS);
            glVertex3fv(p0);
            glVertex3fv(p1);
            glVertex3fv(p2);
            glVertex3fv(p3);
            glEnd();
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    glTranslatef(0, 0, -6);
    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateY, 0, 1, 0);

    drawCubeLines();

    if (currentLine >= 12 && currentFace >= 0) {
        for (int i = 0; i <= currentFace && i < 6; i++) {
            if (i == currentFace) {
                drawSingleFaceGrid(i, gridProgress);
            }
            else {
                drawSingleFaceGrid(i, GRID_DIV * GRID_DIV);
            }
        }
    }

    glutSwapBuffers();
}

void timer(int value) {
    if (currentLine < 12) {
        currentLine++;
        glutPostRedisplay();
        glutTimerFunc(100, timer, 0);
    }
    else if (currentFace < 6) {
        if (gridProgress < GRID_DIV * GRID_DIV) {
            gridProgress++;
            glutPostRedisplay();
            glutTimerFunc(30, timer, 0);
        }
        else {
            currentFace++;
            gridProgress = 0;
            glutPostRedisplay();
            glutTimerFunc(100, timer, 0);
        }
    }
}

void mouseDrag(int x, int y) {
    if (isDragging) {
        rotateY += (x - lastMouseX);
        rotateX += (y - lastMouseY);
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = 1;
            lastMouseX = x;
            lastMouseY = y;
        }
        else {
            isDragging = 0;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Rainbow Cube Grid Coloring");

    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseDrag);

    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glutTimerFunc(200, timer, 0);
    glutMainLoop();
    return 0;
}
