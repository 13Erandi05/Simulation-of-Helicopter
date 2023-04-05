#include <GL/glut.h>  
#include <math.h>

const float PI = 3.1415926;

const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;

const int GRID_SIZE = 200;
const float AMPLITUDE = 0.1;
const float WAVELENGTH = 0.4;
const float SPEED = 0.1;

float grid[GRID_SIZE][GRID_SIZE];
float time = 0.0;

//helicopter dimentions
GLfloat rotate_angle = 30.0;
GLfloat move = 0.0;
GLint start = 0;

//Animation
GLfloat animYRot = 0.0;

//variables to move the camera
GLfloat camX = 0.0;
GLfloat camY = 0.0;
GLfloat camZ = 0.0;

//variables to move the scene
GLfloat sceX = 0.0;
GLfloat sceY = 0.0;
GLfloat sceZ = 0.0;

//variables to rotate the helicopter
GLfloat objX = 0.0;
GLfloat objY = 0.0;
GLfloat objZ = 0.0;

//variable to move helicopter
GLfloat mvx = 0;
GLfloat mvz = 0;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
}

void drawGrid() {
    GLfloat step = 1.0f;
    GLint line;

    glBegin(GL_LINES);
    for (line = -20; line <= 20; line += step) {
        glVertex3f(line, -0.4, 20);
        glVertex3f(line, -0.4, -20);

        glVertex3f(20, -0.4, line);
        glVertex3f(-20, -0.4, line);
    }
    glEnd();
}

void drawAxes() {

    glBegin(GL_LINES);

    glLineWidth(1.5);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-200, 0, 0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(200, 0, 0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, -200, 0);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(0, 200, 0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, -200);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0, 0, 200);

    glEnd();
}

void draw_body() {
    glPushMatrix();
    glScalef(1, 1, 2);
    glutSolidSphere(5, 32, 32);
    glScalef(1, 1, 0.5);
    glPopMatrix();
}

void draw_rotor() {
    glPushMatrix();

    //fan stick
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 10, 0);
    glEnd();
    glLineWidth(1);

    //rotor blades
    glScalef(2, 1, 2);
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(rotate_angle, 0.0, 1.0, 0.0);
        glTranslatef(0, 10, 0);
        glRotatef(30, 1, 0, 0);
        glTranslatef(0, -10, 0);

        glBegin(GL_QUADS);
        glVertex3f(0, 10, -0.5);
        glVertex3f(10, 10, -1);
        glVertex3f(10, 10, 1);
        glVertex3f(0, 10, 0.5);
        glEnd();

        glPopMatrix();
        glRotatef(90, 0, 1, 0);
    }
    glPopMatrix();
}

void draw_tail() {
    glPushMatrix();

    glBegin(GL_QUADS);
    glVertex3f(1.2, -2, -9);
    glVertex3f(1.2, 2, -9);
    glVertex3f(0.2, 0.2, -29);
    glVertex3f(0.2, -0.2, -29);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(1.2, 2, -9);
    glVertex3f(-1.2, 2, -9);
    glVertex3f(-0.2, 0.2, -29);
    glVertex3f(0.2, 0.2, -29);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(1.2, -2, -9);
    glVertex3f(-1.2, -2, -9);
    glVertex3f(-0.2, -0.2, -29);
    glVertex3f(0.2, -0.2, -29);
    glEnd();

    glPopMatrix();
}

void draw_legs() {
    glPushMatrix();
    glLineWidth(4);
    //first leg
    glBegin(GL_LINES);
    glVertex3f(5, -7, 7);
    glVertex3f(5, -7, -7);
    glVertex3f(5, -7, 7);
    glVertex3f(5, -6, 8);
    glVertex3f(5, -7, -7);
    glVertex3f(5, -6, -8);
    glVertex3f(0, 0, 5);
    glVertex3f(5, -7, 5);
    glVertex3f(0, 0, -5);
    glVertex3f(5, -7, -5);

    //second leg
    glVertex3f(-5, -7, 7);
    glVertex3f(-5, -7, -7);
    glVertex3f(-5, -7, 7);
    glVertex3f(-5, -6, 8);
    glVertex3f(-5, -7, -7);
    glVertex3f(-5, -6, -8);
    glVertex3f(0, 0, 5);
    glVertex3f(-5, -7, 5);
    glVertex3f(0, 0, -5);
    glVertex3f(-5, -7, -5);
    glEnd();

    glLineWidth(1);
    glPopMatrix();

}

void draw_tail_fan() {
    glPushMatrix();
    glTranslatef(0, 0, -28);
    glRotatef(-90, 0, 0, 1);
    glTranslatef(0, 0, 28);
    glRotatef(animYRot, 0, 1, 0);
    glTranslatef(0, 0, -28);
    glScalef(0.15, 0.11, 0.15);
    draw_rotor();
    glPopMatrix();
}

void draw_helipad(float r, float g, float b) {
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(0, -9, -1);
    glRotatef(-90, 1, 0, 0);
    glScalef(1, 1, 0.1);
    glutSolidTorus(13, 13, 100, 100);
    glScalef(1, 1, 1);

    glLineWidth(25);
    glColor3f(1, 1, 1);
    //write H
    glBegin(GL_QUADS);
    glVertex3f(-9.5, -1.5, 16);
    glVertex3f(-9.5, 1.5, 16);
    glVertex3f(9.5, 1.5, 16);
    glVertex3f(9.5, -1.5, 16);

    glVertex3f(9.5, 11.5, 16);
    glVertex3f(9.5, -11.5, 16);
    glVertex3f(11.5, -11.5, 16);
    glVertex3f(11.5, 11.5, 16);

    glVertex3f(-9.5, 11.5, 16);
    glVertex3f(-9.5, -11.5, 16);
    glVertex3f(-11.5, -11.5, 16);
    glVertex3f(-11.5, 11.5, 16);
    glEnd();

    glLineWidth(1);
    glPopMatrix();
}

void draw_helicopter() {
    glTranslatef(0, move, 0);
    glRotatef(objX, 1, 0, 0);
    glRotatef(objY, 0, 1, 0);
    glRotatef(objZ, 0, 0, 1);
    glTranslatef(mvx, 0, mvz);
    draw_body();
    draw_rotor();
    draw_tail();
    draw_legs();
    draw_tail_fan();
}

void draw_window() {
    glBegin(GL_POLYGON);
    glColor3f(0.8, 0.8, 0.8);
    glVertex3f(-4, 7, 0);
    glVertex3f(4, 7, 0);
    glVertex3f(4, -7, 0);
    glVertex3f(-4, -7, 0);
    glEnd();

    glTranslatef(0, 0, 0.1);
    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(-4, 7, 0);
    glVertex3f(4, -7, 0);
    glVertex3f(4, 7, 0);
    glVertex3f(-4, -7, 0);
    glVertex3f(-4, 7, 0);
    glVertex3f(4, 7, 0);
    glVertex3f(4, 7, 0);
    glVertex3f(4, -7, 0);
    glVertex3f(4, -7, 0);
    glVertex3f(-4, -7, 0);
    glVertex3f(-4, -7, 0);
    glVertex3f(-4, 7, 0);
    glEnd();
}

void draw_wall1() {
    glBegin(GL_POLYGON);
    glColor3f(0.4, 0, 0);
    glVertex3f(-27, 12, 0);
    glVertex3f(27, 12, 0);
    glVertex3f(27, -12, 0);
    glVertex3f(-27, -12, 0);
    glEnd();

    glTranslatef(0, 0, 0.1);
    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex3f(-27, 12, 0);
    glVertex3f(27, 12, 0);
    glVertex3f(27, 12, 0);
    glVertex3f(27, -12, 0);
    glVertex3f(27, -12, 0);
    glVertex3f(-27, -12, 0);
    glVertex3f(-27, -12, 0);
    glVertex3f(-27, 12, 0);
    glEnd();

    glTranslatef(-18, 0, 1.9);
    draw_window();
    glTranslatef(12, 0, 0);
    draw_window();
    glTranslatef(12, 0, 0);
    draw_window();
    glTranslatef(12, 0, 0);
    draw_window();

    glLineWidth(1);
}

void draw_wall2() {
    glBegin(GL_POLYGON);
    glColor3f(0.4, 0, 0);
    glVertex3f(-27, 12, 0);
    glVertex3f(27, 12, 0);
    glVertex3f(27, -12, 0);
    glVertex3f(-27, -12, 0);
    glEnd();

    glTranslatef(0, 0, 0.1);
    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex3f(-27, 12, 0);
    glVertex3f(27, 12, 0);
    glVertex3f(27, -12, 0);
    glVertex3f(-27, -12, 0);
    glEnd();

    glTranslatef(-18, 0, 1.9);
    draw_window();
    glTranslatef(12, 0, 0);
    draw_window();
    glTranslatef(12, 0, 0);
    draw_window();
    glTranslatef(12, 0, 0);
    draw_window();

    glLineWidth(1);
}

void draw_floor() {
    glPushMatrix();
    glPushMatrix();
    glBegin(GL_POLYGON);
    glColor3f(0.4, 0, 0);
    glVertex3f(-27, 12, 0);
    glVertex3f(27, 12, 0);
    glVertex3f(27, 12, -107);
    glVertex3f(-27, 12, -107);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    draw_wall1();
    glRotatef(90, 0, 1, 0);
    glTranslatef(29, 0, 9);
    draw_wall2();
    glTranslatef(35, 0, -2.5);
    draw_wall2();
    glPopMatrix();

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0, 0, 107);
    draw_wall1();
    glPopMatrix();

    glRotatef(-90, 0, 1, 0);
    glTranslatef(-27.3, 0, 26.7);
    draw_wall2();
    glTranslatef(-71.7, 0, -2);
    draw_wall2();
    glPopMatrix();
}

void draw_building(int x) {
    for (int i = 0; i < x; i++) {
        draw_floor();
        glTranslatef(0, 24, 0);
    }
}

void drawFractalTree(float length, float theta, float phi, int depth, double R, double G)
{
    if (depth == 0) return;

    // Draw the main branch
    glPushMatrix();
    glRotatef(theta * 180.0 / PI, 0.0, 1.0, 0.0);
    glRotatef(phi * 180.0 / PI, 0.0, 0.0, 1.0);
    glColor3f(R, G, 0.0);
    theta == 0 ? glLineWidth(20) : glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, length, 0.0);
    glEnd();
    glTranslatef(0.0, length, 0.0);

    // Draw the smaller branches
    drawFractalTree(length * 0.7, theta + PI / 15, phi + PI / 6, depth - 1, R == 0 ? 0 : R -= 0.02, G == 1 ? 1 : G += 0.02);
    drawFractalTree(length * 0.7, theta - PI / 15, phi - PI / 6, depth - 1, R == 0 ? 0 : R -= 0.02, G == 1 ? 1 : G += 0.02);

    glPopMatrix();
}

void draw_grass() {
    glColor3f(0.13333, 0.545098, 0.13333);//forest green
    double i;
    glBegin(GL_LINES); //draw mesh
    for (i = -2500.0; i <= 2500; i += 0.5) {
        glVertex3d(-2500, -11, i);
        glVertex3d(2500, -11, i);
        glVertex3d(i, -11, -2500);
        glVertex3d(i, -11, 2500);
    }
    glEnd();
}

void update_oceanGrid(double x) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            float x = (float)i / (float)GRID_SIZE - 0.5;
            float z = (float)j / (float)GRID_SIZE - 0.5;
            grid[i][j] = AMPLITUDE * sin(2 * PI * (x / WAVELENGTH + time * SPEED)) * sin(2 * PI * (z / WAVELENGTH + time * SPEED));
        }
    }
    time += x;
}

void draw_oceanGrid(double r, double g, double b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    for (int i = 0; i < GRID_SIZE - 1; i++) {
        for (int j = 0; j < GRID_SIZE - 1; j++) {
            glVertex3f((float)i / (float)GRID_SIZE - 0.5, grid[i][j], (float)j / (float)GRID_SIZE - 0.5);
            glVertex3f((float)(i + 1) / (float)GRID_SIZE - 0.5, grid[i + 1][j], (float)j / (float)GRID_SIZE - 0.5);
            glVertex3f((float)(i + 1) / (float)GRID_SIZE - 0.5, grid[i + 1][j + 1], (float)(j + 1) / (float)GRID_SIZE - 0.5);
            glVertex3f((float)i / (float)GRID_SIZE - 0.5, grid[i][j + 1], (float)(j + 1) / (float)GRID_SIZE - 0.5);
        }
    }
    glEnd();
}

void draw_oceanpart() {
    update_oceanGrid(0.007);
    draw_oceanGrid(0.02, 0.25, 0.45);
    update_oceanGrid(0.007);
    draw_oceanGrid(0.46, 0.71, 0.77);
    update_oceanGrid(0.007);
    draw_oceanGrid(0.49, 0.8, 1);
}

void draw_house() {
    glColor3f(0.98, 0.98, 0.98);

    //front wall
    glBegin(GL_POLYGON);
    glVertex3f(-5, 0, 0);
    glVertex3f(5, 0, 0);
    glVertex3f(5, 10, 0);
    glVertex3f(-5, 10, 0);
    glEnd();

    //right wall
    glBegin(GL_POLYGON);
    glVertex3f(5, 0, 0);
    glVertex3f(5, 0, 15);
    glVertex3f(5, 10, 15);
    glVertex3f(5, 10, 0);
    glEnd();

    //back wall
    glBegin(GL_POLYGON);
    glVertex3f(-5, 0, 15);
    glVertex3f(5, 0, 15);
    glVertex3f(5, 10, 15);
    glVertex3f(-5, 10, 15);
    glEnd();

    //left wall
    glBegin(GL_POLYGON);
    glVertex3f(-5, 0, 0);
    glVertex3f(-5, 0, 15);
    glVertex3f(-5, 10, 15);
    glVertex3f(-5, 10, 0);
    glEnd();

    //bottom
    glBegin(GL_POLYGON);
    glVertex3f(-5, 0, 0);
    glVertex3f(5, 0, 0);
    glVertex3f(5, 0, 15);
    glVertex3f(-5, 0, 15);
    glEnd();

    //top
    glBegin(GL_POLYGON);
    glVertex3f(-5, 10, 0);
    glVertex3f(5, 10, 0);
    glVertex3f(5, 10, 15);
    glVertex3f(-5, 10, 15);
    glEnd();

    //roof left
    glColor3f(0.64, 0.29, 0.29);
    glBegin(GL_POLYGON);
    glVertex3f(0, 13, -3);
    glVertex3f(-8, 9.5, -3);
    glVertex3f(-8, 9.5, 17);
    glVertex3f(0, 13, 17);
    glEnd();

    //roof right
    glBegin(GL_POLYGON);
    glVertex3f(0, 13, -3);
    glVertex3f(8, 9.5, -3);
    glVertex3f(8, 9.5, 17);
    glVertex3f(0, 13, 17);
    glEnd();

    //front door & window
    glBegin(GL_POLYGON);
    glVertex3f(1, 0, -0.2);
    glVertex3f(4, 0, -0.2);
    glVertex3f(4, 7, -0.2);
    glVertex3f(1, 7, -0.2);
    glEnd();

    glColor3f(0.1, 0.63, 0.84);
    glBegin(GL_POLYGON);
    glVertex3f(-4, 4, -0.2);
    glVertex3f(-1, 4, -0.2);
    glVertex3f(-1, 7, -0.2);
    glVertex3f(-4, 7, -0.2);
    glEnd();

    //side windows
    glBegin(GL_POLYGON);
    glVertex3f(5.2, 3, 2);
    glVertex3f(5.2, 3, 6);
    glVertex3f(5.2, 7, 6);
    glVertex3f(5.2, 7, 2);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(5.2, 3, 9);
    glVertex3f(5.2, 3, 13);
    glVertex3f(5.2, 7, 13);
    glVertex3f(5.2, 7, 9);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-5.2, 3, 2);
    glVertex3f(-5.2, 3, 6);
    glVertex3f(-5.2, 7, 6);
    glVertex3f(-5.2, 7, 2);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-5.2, 3, 9);
    glVertex3f(-5.2, 3, 13);
    glVertex3f(-5.2, 7, 13);
    glVertex3f(-5.2, 7, 9);
    glEnd();

    //back door
    glColor3f(0.64, 0.29, 0.29);
    glBegin(GL_POLYGON);
    glVertex3f(-2, 0, 15.2);
    glVertex3f(2, 0, 15.2);
    glVertex3f(2, 7, 15.2);
    glVertex3f(-2, 7, 15.2);
    glEnd();
}

void draw_river() {
    glColor3f(0, 0, 1);
    glBegin(GL_POLYGON);
    glVertex3f(-20, 0, -2000);
    glVertex3f(20, 0, -2000);
    glVertex3f(20, 0, 2000);
    glVertex3f(-20, 0, 2000);
    glEnd();

    glColor3f(0.3f, 0.5f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex3f(-30, 0.2, -2000);
    glVertex3f(-20, 0.2, -2000);
    glVertex3f(-20, 0.2, 2000);
    glVertex3f(-30, 0.2, 200);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(20, 0.2, -2000);
    glVertex3f(30, 0.2, -2000);
    glVertex3f(30, 0.2, 2000);
    glVertex3f(20, 0.2, 2000);
    glEnd();
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Lighting
    glEnable(GL_LIGHT0);
    GLfloat lightIntensity[] = { 1,1,1,1 };
    GLfloat light_position[] = { 30,10,30,0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glEnable(GL_LIGHT1);
    GLfloat lightIntensity_1[] = { 1,1,1,1 };
    GLfloat light_position_1[] = { -30,10,-30,0 };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightIntensity_1);

    glEnable(GL_LIGHT2);
    GLfloat lightIntensity_2[] = { 1,1,1,1 };
    GLfloat light_position_2[] = { 30,10,-30,0 };
    glLightfv(GL_LIGHT2, GL_POSITION, light_position_2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightIntensity_2);

    glEnable(GL_LIGHT3);
    GLfloat lightIntensity_3[] = { 1,1,1,1 };
    GLfloat light_position_3[] = { -30,10,30,0 };
    glLightfv(GL_LIGHT3, GL_POSITION, light_position_3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightIntensity_3);

    glPushMatrix();
    glTranslatef(sceX, 40, sceZ);
    // camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    gluLookAt(camX, 50.0 + camY, 60.0 + camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(sceY, 0.0, 1.0, 0.0);

    glPushMatrix();
    glTranslatef(0, -2.5, 0);
    draw_grass();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-50, 0, 50);
    glScalef(0.3, 0.3, 0.3);
    draw_building(3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(40, 0, 0);
    glScalef(15, 15, 15);
    drawFractalTree(1.0, 0.0, 0.0, 20, 0.5, 0.3);
    glPopMatrix();

    glScalef(0.3, 0.3, 0.3);
    glPushMatrix();
    draw_building(5);
    glTranslatef(0, -2, -55);
    draw_helipad(0.125, 0.125, 0.125);
    glColor3f(0.3, 0.3, 0.3);
    draw_helicopter();
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();

}

void display_ocean() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Lighting
    glEnable(GL_LIGHT0);
    GLfloat lightIntensity[] = { 1,1,1,1 };
    GLfloat light_position[] = { 30,10,30,0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glEnable(GL_LIGHT1);
    GLfloat lightIntensity_1[] = { 1,1,1,1 };
    GLfloat light_position_1[] = { -30,10,-30,0 };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightIntensity_1);

    glEnable(GL_LIGHT2);
    GLfloat lightIntensity_2[] = { 1,1,1,1 };
    GLfloat light_position_2[] = { 30,10,-30,0 };
    glLightfv(GL_LIGHT2, GL_POSITION, light_position_2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightIntensity_2);

    glEnable(GL_LIGHT3);
    GLfloat lightIntensity_3[] = { 1,1,1,1 };
    GLfloat light_position_3[] = { -30,10,30,0 };
    glLightfv(GL_LIGHT3, GL_POSITION, light_position_3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightIntensity_3);

    glPushMatrix();
    glTranslatef(sceX, 0, sceZ);
    // camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    gluLookAt(camX, 90.0 + camY, 50.0 + camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(sceY, 0.0, 1.0, 0.0);

    glPushMatrix();
    glScalef(2, 2, 2);
    glTranslatef(0, 18, 0);
    draw_helipad(0.5, 0.5, 0.5);
    glColor3f(0.29, 0.32, 0.13);
    draw_helicopter();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 15, 0);
    glColor3f(0.5, 0.2, 0);
    glScalef(150, 2, 150);
    glutSolidCube(1);
    glPopMatrix();

    glScalef(800, 75, 800);
    glPushMatrix();
    glTranslatef(0, 0, 0);
    draw_oceanpart();
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
}

void display_land() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Lighting
    glEnable(GL_LIGHT0);
    GLfloat lightIntensity[] = { 1,1,1,1 };
    GLfloat light_position[] = { 30,10,30,0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glEnable(GL_LIGHT1);
    GLfloat lightIntensity_1[] = { 1,1,1,1 };
    GLfloat light_position_1[] = { -30,10,-30,0 };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightIntensity_1);

    glEnable(GL_LIGHT2);
    GLfloat lightIntensity_2[] = { 1,1,1,1 };
    GLfloat light_position_2[] = { 30,10,-30,0 };
    glLightfv(GL_LIGHT2, GL_POSITION, light_position_2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightIntensity_2);

    glEnable(GL_LIGHT3);
    GLfloat lightIntensity_3[] = { 1,1,1,1 };
    GLfloat light_position_3[] = { -30,10,30,0 };
    glLightfv(GL_LIGHT3, GL_POSITION, light_position_3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightIntensity_3);

    glPushMatrix();
    glTranslatef(sceX, 0, sceZ);
    // camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    gluLookAt(camX, 60.0 + camY, 60.0 + camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(-140 + sceY, 0.0, 1.0, 0.0);

    glPushMatrix();
    glTranslatef(-300, 0, 0);
    draw_river();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -2.5, 0);
    draw_grass();
    glPopMatrix();

    glScalef(2, 2, 2);

    glPushMatrix();
    glTranslatef(60, 0.1, 60);
    draw_house();
    glTranslatef(-60, 0, 0);
    draw_house();
    glTranslatef(-60, 0, 0);
    draw_house();
    glTranslatef(30, 0, 40);
    draw_house();
    glTranslatef(60, 0, 0);
    draw_house();
    glTranslatef(80, 0, -80);
    draw_house();
    glTranslatef(0, 0, -60);
    draw_house();
    glTranslatef(40, 0, 30);
    draw_house();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 2, 0);
    draw_helipad(0, 0, 0);
    glColor3f(0.3, 0.3, 0.3);
    draw_helicopter();
    glPopMatrix();

    glutSwapBuffers();
}

//Animation timer function
void timer(int x) {
    if (start) {
        if (rotate_angle == 360.0)
            rotate_angle = 0.0;
        else {
            rotate_angle += 20.0;
        }
        glutPostRedisplay();

        glutTimerFunc(30.0, timer, 1);
    }
    else {
        glutTimerFunc(30.0, timer, 1);
    }
}

void idle() {
    glutPostRedisplay();
}

void keyboardSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        camY += 1;

    if (key == GLUT_KEY_DOWN)
        camY -= 1;

    if (key == GLUT_KEY_RIGHT)
        sceY += 1;

    if (key == GLUT_KEY_LEFT)
        sceY -= 1;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '1')
        glEnable(GL_LIGHTING);

    if (key == '!')
        glDisable(GL_LIGHTING);

    if (key == 'b')
        sceZ += 1;

    if (key == 'B')
        sceZ -= 1;

    if (key == 'a')
        sceX += 1;

    if (key == 'A')
        sceX -= 1;

    if (key == 'H')
        camZ += 1;

    if (key == 'h')
        camZ -= 1;

    if (key == 'N')
        camX += 1;

    if (key == 'n')
        camX -= 1;

    if (key == 'x') {
        objX += 5;
        camX += 1;
    }

    if (key == 'X') {
        objX -= 5;
        camX -= 1;
    }

    if (key == 'y') {
        objY += 5;
        camY += 1;
    }

    if (key == 'Y') {
        objY -= 5;
        camY -= 1;
    }

    if (key == 'z') {
        objZ += 5;
        camZ += 1;
    }

    if (key == 'Z') {
        objZ -= 5;
        camZ -= 1;
    }

    if (key == 'u') {
        move += 0.5;
        camY += 1;
    }

    if (key == 'd') {
        move -= 0.5;
        camY -= 1;
    }

    if (key == 's')
        start = 1;

    if (key == 'e')
        start = 0;

    if (key == 'f') {
        mvx += 1;
        mvz += 1;
    }

    //wire frame
    if (key == 'w')
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (key == 'q')
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //move the scene
    if (key == 'O')
        glutDisplayFunc(display_ocean);

    if (key == 'L')
        glutDisplayFunc(display_land);

    if (key == 'M')
        glutDisplayFunc(display);

    glutPostRedisplay();
}

void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Define the Perspective projection frustum 
    // (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
    gluPerspective(120.0, aspect_ratio, 1.0, 10000.0);

}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Flying and Landing the Helicopter");
    glutDisplayFunc(display_land);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    // keyboard function activation
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);

    glutTimerFunc(30.0, timer, 1);
    init();
    glutMainLoop();


    return 0;
}