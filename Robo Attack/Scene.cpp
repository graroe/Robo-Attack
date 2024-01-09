//CPS 511 Assignment 3
//Graham Roebuck
//500986436
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctime>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "Robot3D.h"
#include "MeshLoader.h"

const int vWidth = 1000;    // Viewport width in pixels
const int vHeight = 700;    // Viewport height in pixels

// Light properties
GLfloat light_position0[] = { -0.0F, 16.0F, -25.0F, 1.0F };
GLfloat light_position1[] = { 0.0F, 16.0F, 5.0F, 1.0F };
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Ground Mesh material
GLfloat groundMat_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat groundMat_specular[] = { 0.01, 0.01, 0.01, 1.0 };
GLfloat groundMat_diffuse[] = { 0.4, 0.4, 0.7, 1.0 };
GLfloat groundMat_shininess[] = { 1.0 };

float botY = 6.5;
float botOneX = 7.0;
float botTwoX = -10.0;
float botOneZ = 40.0;
float botTwoZ = 60.0;

Robot3D botOne = Robot3D(7.0, 6.5, 40.0, 0.0, -0.05f);
Robot3D botTwo = Robot3D(-10.0, 6.5, 60.0, 30.0, 0.05f);

float pCannonVertAngle = 0.0f;
float pCannonHoriAngle = 70.0f;
float aimDistance = 60;
float shotTravelUnit = 2.0;

float shotX, shotY, shotZ = 0.0f;
float pCannonY = 0.0;
float pCannonZ = 0.0;

// distance from center of pCannon Bounding Box edge 
float pCannonBound = 2.0;

VECTOR3D target;
VECTOR3D shotUnit;
VECTOR3D shotPosition = VECTOR3D(0.0, 0.0, 0.0);
QuadMesh* groundMesh = NULL;
int meshSize = 64;

int lastMouseX = 0;
int lastMouseY = 0;
bool pShoot = false;
bool gameOver = false;
bool died = false;

GLdouble fov = 60.0;

int window3D;
int window3DSizeX = 800, window3DSizeY = 600;
GLdouble aspect = (GLdouble)window3DSizeX / window3DSizeY;

GLuint tex[2];
GLfloat textureMap[64][64][3];
GLfloat textureMap2[512][512][3];

void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void botAnimationHandler();
void projectileAnimationHandler(int param);
void deathAnimationHandler(int param);
void botShotTimer(int param);
void mouseMotionHandler3D(int x, int y); 
void keyboard(unsigned char, int x, int y);
float toRads(float degrees);
void makeTextures();
float randomizeStartPos(int lower, int upper);
void initBots();

int main(int argc, char** argv)
{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("Robot Attack");
	glewInit();
	initOpenGL(vWidth, vHeight);

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButtonHandler3D);
	glutPassiveMotionFunc(mouseMotionHandler3D);
	glutIdleFunc(botAnimationHandler);
	glutTimerFunc(100, botShotTimer, 0);
	glutMainLoop();

	return 0;
}

void initOpenGL(int w, int h)
{
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);   // This second light is currently off

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  // Color and depth for glClear
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 70.0);

	VECTOR3D origin = VECTOR3D(-60.0f, 0.0f, 50.0f);
	VECTOR3D dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
	VECTOR3D dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
	groundMesh = new QuadMesh(meshSize, 64.0);
	groundMesh->InitMesh(meshSize, origin, 120.0, 75.0, dir1v, dir2v);

	VECTOR3D ambient = VECTOR3D(0.0f, 0.05f, 0.0f);
	VECTOR3D diffuse = VECTOR3D(0.4f, 0.8f, 0.4f);
	VECTOR3D specular = VECTOR3D(0.04f, 0.04f, 0.04f);
	float shininess = 0.2;
	groundMesh->SetMaterial(ambient, diffuse, specular, shininess);
	generateArrays();
	generateVBOs();
	makeTextures();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 2.0, -4.5, 0.0, 2.0, 20.5, 0.0, 1.0, 0.0);

	glPushMatrix();
	groundMesh->DrawMesh(meshSize);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, tex[0]);
	if (botOne.isAlive()) {
		botOne.drawRobot();
	}
	if (botTwo.isAlive()) {
		botTwo.drawRobot();
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	if (pShoot) {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, groundMat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, groundMat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, groundMat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, groundMat_shininess);
		glBegin(GL_QUADS);
		glVertex3f(shotPosition.x + 0.2, shotPosition.y + 0.2, shotPosition.z);
		glVertex3f(shotPosition.x + 0.2, shotPosition.y - 0.2, shotPosition.z);
		glVertex3f(shotPosition.x - 0.2, shotPosition.y - 0.2, shotPosition.z);
		glVertex3f(shotPosition.x - 0.2, shotPosition.y + 0.2, shotPosition.z);
		glEnd();
		glPopMatrix();
	}
	
	if (botOne.isShotFired()) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, groundMat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, groundMat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, groundMat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, groundMat_shininess);
		VECTOR3D sP= botOne.getBotShotPos();
		glBegin(GL_QUADS);
		glVertex3f(sP.x + 0.2, sP.y + 0.2, sP.z);
		glVertex3f(sP.x + 0.2, sP.y - 0.2, sP.z);
		glVertex3f(sP.x - 0.2, sP.y - 0.2, sP.z);
		glVertex3f(sP.x - 0.2, sP.y + 0.2, sP.z);
		glEnd();
	}

	if (botTwo.isShotFired()) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, groundMat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, groundMat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, groundMat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, groundMat_shininess);
		VECTOR3D sP = botTwo.getBotShotPos();
		glBegin(GL_QUADS);
		glVertex3f(sP.x + 0.2, sP.y + 0.2, sP.z);
		glVertex3f(sP.x + 0.2, sP.y - 0.2, sP.z);
		glVertex3f(sP.x - 0.2, sP.y - 0.2, sP.z);
		glVertex3f(sP.x - 0.2, sP.y + 0.2, sP.z);
		glEnd();
	}

	glTranslatef(0.0, pCannonY, pCannonZ);
	glRotatef(pCannonHoriAngle, 0.0, 1.0, 0.0);
	glRotatef(pCannonVertAngle, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	drawPlayerCannon();
	glBindTexture(GL_TEXTURE_2D, 0);
	glutSwapBuffers();   
}

void reshape(int w, int h)
{

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 70.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 2.0, -4.5, 0.0, 2.0, 20.5, 0.0, 1.0, 0.0);
}

void makeTextures()
{
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++) {
			if ((i / 8 + j / 8) % 2 == 0) {
				textureMap[i][j][0], textureMap[i][j][1], textureMap[i][j][2] = 1.0;
			}
			else {
				textureMap[i][j][0] = 0.0;
				textureMap[i][j][1], textureMap[i][j][2] = 0.0;
			}
		}

	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++) {
			if ((i / 8 + j / 8) % 2 == 0) {
				textureMap[i][j][0], textureMap[i][j][1], textureMap[i][j][2] = 1.0;
			}
			else {
				textureMap[i][j][0] = 1.0;
				textureMap[i][j][1], textureMap[i][j][2] = 0.0;
			}
		}

	glGenTextures(2, tex);

	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMap);

	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_FLOAT, textureMap2);
}

void botAnimationHandler()
{
	botOne.animate();
	botTwo.animate();
	if (botOne.isShotFired() && botOne.getBotShotPos().z <= 0.0) {
		float hitX = botOne.getBotShotPos().x;
		if (hitX < pCannonBound && hitX > -pCannonBound) {
			gameOver = true;
			died = true;
			
		}
		botOne.resetShot();
	}
	if (botTwo.isShotFired() && botTwo.getBotShotPos().z <= 0.0) {
		float hitX = botTwo.getBotShotPos().x;
		if (hitX < pCannonBound && hitX > -pCannonBound) {
			botOne.resetShot();
			gameOver = true;
			died = true;
		}
		botTwo.resetShot();
	}

	glutPostRedisplay();

	if (died){
		glutTimerFunc(10, deathAnimationHandler, 0);
	}
}

void deathAnimationHandler(int param) {
	pCannonY -= 0.01;
	pCannonZ -= 0.02;
	glutPostRedisplay();
	if (pCannonY > -10.0 && died) {
		glutTimerFunc(10, deathAnimationHandler, 0);
	}
}

void botShotTimer(int param) {
	if (!botOne.isShotFired()) {
		botOne.prepareShot();
	}
	if (!botTwo.isShotFired()) {
		botTwo.prepareShot();
	}
	if (!gameOver) {
		glutTimerFunc(1000, botShotTimer, 0);
	}
}

void projectileAnimationHandler(int param)
{
	shotPosition += shotUnit;
	if (shotPosition.z > target.z || botOne.checkCollision(shotPosition) || botTwo.checkCollision(shotPosition)) {
		pShoot = false;
	}
	else {
		pShoot = true;
		glutTimerFunc(10, projectileAnimationHandler, 0);
	}

	if (!botOne.isAlive() && !botTwo.isAlive()) {
		gameOver = true;
	}
	glutPostRedisplay();
}


void mouseMotionHandler3D(int x, int y)
{
	int dx = x - lastMouseX;
	int dy = y - lastMouseY;
		
	if (!gameOver) {
		pCannonHoriAngle -= dx * 0.2;
		pCannonVertAngle += dy * 0.2;
	
	if (pCannonHoriAngle > 45.0)
		pCannonHoriAngle = 45.0;
	if (pCannonHoriAngle < -45.0)
		pCannonHoriAngle = -45.0;
	if (pCannonVertAngle > 75.0)
		pCannonVertAngle = 75.0;
	if (pCannonVertAngle < 50.0)
		pCannonVertAngle = 50.0;
	}
	

	lastMouseX = x;
	lastMouseY = y;
	glutPostRedisplay();
}

void mouseButtonHandler3D(int button, int state, int x, int y)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !pShoot && !gameOver) {
		float yaw = toRads(pCannonHoriAngle);
		float pitch = toRads(80 - pCannonVertAngle);
		target = VECTOR3D(tan(yaw) * aimDistance, tan(pitch) * aimDistance, aimDistance);
		shotUnit = target*(shotTravelUnit / target.GetLength());
		shotPosition = shotUnit*2;
		glutTimerFunc(10, projectileAnimationHandler, 0);
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == ' ' && !pShoot && !gameOver) {
		float yaw = toRads(pCannonHoriAngle);
		float pitch = toRads(80 - pCannonVertAngle);
		target = VECTOR3D(tan(yaw) * aimDistance, tan(pitch) * aimDistance, aimDistance);
		shotUnit = target * (shotTravelUnit / target.GetLength());
		shotPosition = shotUnit * 2;
		glutTimerFunc(10, projectileAnimationHandler, 0);
	}
	else if (key == 'r' && gameOver) {
		gameOver = false;
		died = false;
		pCannonY = 0.0;
		pCannonZ = 0.0;
		initBots();
		glutTimerFunc(100, botShotTimer, 0);
	}
	else if (key == 'q') {
		exit(0);
	}
}

void initBots(){
	srand(time(NULL));
	float initX1 = randomizeStartPos(-10.0, 10.0);
	float initX2 = randomizeStartPos(-10.0, 10.0);
	float initZ1 = randomizeStartPos(30, 60);
	float initZ2 = randomizeStartPos(30, 60);
	float initA1 = randomizeStartPos(-45, 45);
	float initA2 = randomizeStartPos(-45, 45);
	botOne = Robot3D(initX1, 6.5, initZ1, initA1, -0.05f);
	botTwo = Robot3D(initX2, 6.5, initZ2, initA2, 0.05f);
}

float randomizeStartPos(int lower, int upper){
	return ((rand() % (upper - lower + 1)) + lower) * 1.0;
}

float toRads(float degrees) {
	return degrees * 3.14159 / 180;
}
