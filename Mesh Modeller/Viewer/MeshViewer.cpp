#include <stdio.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include "MeshViewer.h"

GLdouble worldLeft = -12;
GLdouble worldRight = 12;
GLdouble worldBottom = -9;
GLdouble worldTop = 9;
GLdouble worldCenterX = 0.0;
GLdouble worldCenterY = 0.0;
GLdouble wvLeft = -12;
GLdouble wvRight = 12;
GLdouble wvBottom = -9;
GLdouble wvTop = 9;

GLint glutWindowWidth = 800;
GLint glutWindowHeight = 600;
GLint viewportWidth = glutWindowWidth;
GLint viewportHeight = glutWindowHeight;

int  window3D;
int window3DSizeX = 800, window3DSizeY = 600;
GLdouble aspect = (GLdouble)window3DSizeX / window3DSizeY;

GLuint vertBufID, indBufID, normBufID;
double* vCoords;
unsigned int* indices;
double* vertexNormals;
int totalVertices;
int totalQuads;

int main(int argc, char* argv[])
{
	
	glutInit(&argc, (char **)argv); 
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(glutWindowWidth,glutWindowHeight);
	glutInitWindowPosition(50,100);  
	
	// The 3D Window
	window3D = glutCreateWindow("Surface of Revolution"); 
	glewInit();
	glutPositionWindow(900,100);  
	glutDisplayFunc(display3D);
	glutReshapeFunc(reshape3D);
	glutMouseFunc(mouseButtonHandler3D);
	glutMouseWheelFunc(mouseScrollWheelHandler3D);
	glutMotionFunc(mouseMotionHandler3D);
	glutKeyboardFunc(keyboardHandler3D);

	init3DSurfaceWindow();

	generateArrays();
	// Annnd... ACTION!!
	glutMainLoop(); 

	return 0;
}




/************************************************************************************
 *
 *
 * 3D Window and Surface of Revolution Code 
 *
 * Fill in the code in the empty functions
 ************************************************************************************/
 // Ground Mesh material
GLfloat groundMat_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat groundMat_specular[] = { 0.01, 0.01, 0.01, 1.0 };
GLfloat groundMat_diffuse[] = { 0.4, 0.4, 0.7, 1.0 };
GLfloat groundMat_shininess[] = { 1.0 };

GLfloat light_position0[] = { 4.0, 8.0, 8.0, 1.0 };
GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_position1[] = { -4.0, 8.0, 8.0, 1.0 };
GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat model_ambient[] = { 0.5, 0.5, 0.5, 1.0 };

GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };

// Set up lighting/shading and material properties for surface 
GLfloat quadMat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat quadMat_specular[] = { 0.45, 0.55, 0.45, 1.0 };
GLfloat quadMat_diffuse[] = { 0.1, 0.35, 0.1, 1.0 };
GLfloat quadMat_shininess[] = { 10.0 };

GLdouble fov = 60.0;

int lastMouseX = 0;
int lastMouseY = 0;

boolean drawAsLines = false;
boolean drawAsPoints = false;
boolean drawNormals = false;

GLdouble yaw = 0.0;
GLdouble pitch = 0.5;

GLdouble radius = 10.0;

GLdouble eyeX = cos(yaw) * cos(pitch) * radius;
GLdouble eyeY = sin(pitch) * radius;
GLdouble eyeZ = sin(yaw) * cos(pitch) * radius;

GLdouble zNear = 0.1, zFar = 40.0;



void init3DSurfaceWindow()
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, model_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, model_ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  // Color and depth for glClear

	glViewport(0, 0, (GLsizei)window3DSizeX, (GLsizei)window3DSizeY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void reshape3D(int w, int h)
{
	glutWindowWidth = (GLsizei) w;
	glutWindowHeight = (GLsizei) h;
	glViewport(0, 0, glutWindowWidth, glutWindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov,aspect,zNear,zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display3D()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	// Set up the Viewing Transformation (V matrix)	
	gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	drawGround();

	// Draw quad mesh
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, quadMat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, quadMat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, quadMat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, quadMat_shininess);

	generateVBOs();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertBufID);
	glVertexPointer(3, GL_DOUBLE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normBufID);
	glNormalPointer(GL_DOUBLE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
	
	if (drawAsLines)
	  drawQuadsAsLines();
	else if (drawAsPoints)
		drawQuadsAsPoints();
	else
	  drawQuads();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPopMatrix();
	glutSwapBuffers();
}

void drawGround() 
{
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, groundMat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, groundMat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, groundMat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, groundMat_shininess);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-22.0f, -4.0f, -22.0f);
	glVertex3f(-22.0f, -4.0f, 22.0f);
	glVertex3f(22.0f, -4.0f, 22.0f);
	glVertex3f(22.0f, -4.0f, -22.0f);
	glEnd();
	glPopMatrix();
}

void generateArrays() {
	
	std::ifstream inFile;
	inFile.open("savedMesh.txt");
	
	inFile >> totalVertices;
	inFile >> totalQuads;
	vCoords = (double*)malloc(totalVertices * sizeof(double) * 3);
	indices = (unsigned int*)malloc(totalQuads * sizeof(unsigned int) * 4);
	vertexNormals = (double*)malloc(totalVertices * sizeof(double) * 3);

	for (int i = 0; i < totalVertices * 3; i++) {
		inFile >> vCoords[i];
	}
	for (int i = 0; i < totalQuads * 4; i++) {
		inFile >> indices[i];
	}
	for (int i = 0; i < totalVertices * 3; i++) {
		inFile >> vertexNormals[i];
	}
	inFile.close();

}

void generateVBOs() {
	//generating vertex VBO
	glGenBuffers(1, &vertBufID);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufID);
	glBufferData(GL_ARRAY_BUFFER, totalVertices * sizeof(double) * 3, vCoords, GL_STATIC_DRAW);

	// generating index VBO
	glGenBuffers(1, &indBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalQuads * sizeof(unsigned int) * 4, indices, GL_STATIC_DRAW);

	// generating normals VBO
	glGenBuffers(1, &normBufID);
	glBindBuffer(GL_ARRAY_BUFFER, normBufID);
	glBufferData(GL_ARRAY_BUFFER, totalVertices * sizeof(double) * 3, vertexNormals, GL_STATIC_DRAW);

}

void drawQuads() 
{
	glDrawElements(GL_QUADS, totalQuads*4, GL_UNSIGNED_INT, 0);
}

void drawQuadsAsPoints()
{
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
	glDrawElements(GL_POINTS, totalQuads * 4, GL_UNSIGNED_INT, 0);
	glPopMatrix();
}
void drawQuadsAsLines() 
{
	glDrawElements(GL_LINE_STRIP, totalQuads*4, GL_UNSIGNED_INT, 0);
	
	if (drawNormals)
	{
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);

		for (int i = 0; i < totalVertices *3 ; i += 3) {
			glBegin(GL_LINES);
			glVertex3f(vCoords[i], vCoords[i + 1], vCoords[i + 2]);
			glVertex3f(vCoords[i] + vertexNormals[i], vCoords[i+1] + vertexNormals[i+1], vCoords[i + 2] + vertexNormals[i + 2]);
			glEnd();
		}
		
		glPopMatrix();

	}
}



int currentButton;

void mouseButtonHandler3D(int button, int state, int x, int y)
{
	currentButton = button;
	lastMouseX = x;
	lastMouseY = y;
}

void mouseScrollWheelHandler3D(int button, int dir, int xMouse, int yMouse)
{
	fov -= (double)dir;
	if (fov < 10.0)
		fov = 10.0;
	if (fov > 100.0)
		fov = 100.0;
	glutPostRedisplay();
}

void mouseMotionHandler3D(int x, int y)
{
	int dx = x - lastMouseX;
	int dy = y - lastMouseY;
	if (currentButton == GLUT_LEFT_BUTTON)
	{
		yaw += (dx * 0.005);

	}
	if (currentButton == GLUT_RIGHT_BUTTON)
	{
		pitch += (dy * 0.005);
		if (pitch < 0)
			pitch = 0;
		if (pitch > 1)
			pitch = 1;
	}

	eyeX = cos(yaw) * cos(pitch) * radius;
	eyeY = sin(pitch) * radius;
	eyeZ = sin(yaw) * cos(pitch) * radius;
	lastMouseX = x;
	lastMouseY = y;
	glutPostRedisplay();
}

void keyboardHandler3D(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'q':
	case 'Q':
	case 27:
		// Esc, q, or Q key = Quit 
		exit(0);
		break;
	case 'l':
		if (drawAsLines)
			drawAsLines = false;
		else
			drawAsLines = true;
		drawAsPoints = false;
		break;
	case 'p':
		if (drawAsPoints)
			drawAsPoints = false;
		else
			drawAsPoints = true;
		drawAsLines = false;
		break;
	case 'n':
		if (drawNormals)
			drawNormals = false;
		else
			drawNormals = true;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}