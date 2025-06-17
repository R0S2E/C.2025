#include <GL/freeglut.h>

GLfloat rotate_angle = 0.0f;
int rotate_axis = 0; // 0: Y축, 1: X축, 2: Z축, 3: 대각선(X+Y), 4: 대각선(X+Z), 5: 대각선(Y+Z)
int rotate_dir = 1;  // 1: 정방향, -1: 역방향

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 하늘색(light blue)으로 색상 설정
    glColor3f(0.53f, 0.81f, 0.92f);

    // 회전 적용
    glPushMatrix();
    switch (rotate_axis) {
    case 0: // Y축 기준 회전
        glRotatef(rotate_angle, 0.0f, 1.0f, 0.0f);
        break;
    case 1: // X축 기준 회전
        glRotatef(rotate_angle, 1.0f, 0.0f, 0.0f);
        break;
    case 2: // Z축 기준 회전
        glRotatef(rotate_angle, 0.0f, 0.0f, 1.0f);
        break;
    case 3: // 대각선(X+Y) 축 기준 회전
        glRotatef(rotate_angle, 1.0f, 1.0f, 0.0f);
        break;
    case 4: // 대각선(X+Z) 축 기준 회전
        glRotatef(rotate_angle, 1.0f, 0.0f, 1.0f);
        break;
    case 5: // 대각선(Y+Z) 축 기준 회전
        glRotatef(rotate_angle, 0.0f, 1.0f, 1.0f);
        break;
    }
    glutWireTeapot(0.5f);
    glPopMatrix();

    glutSwapBuffers();

    // 회전 각도 증가/감소
    rotate_angle += 0.5f * rotate_dir;
    if (rotate_angle > 360.0f) rotate_angle -= 360.0f;
    if (rotate_angle < 0.0f) rotate_angle += 360.0f;
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 약 60fps
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    gluLookAt(0.0f, 0.0f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        // 회전 축 변경
        rotate_axis = (rotate_axis + 1) % 6;
        // 역방향/정방향 번갈아가며 변경 가능(선택)
        // rotate_dir *= -1; // 이 줄을 활성화하면 R키로 회전 방향도 바뀜
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rotating Wireframe Teapot");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
