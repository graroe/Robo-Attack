#include <stdio.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include "MeshLoader.h"

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

GLuint vertBufID, indBufID, normBufID;
double* vCoords;
unsigned int* indices;
double* vertexNormals;
int totalVertices;
int totalQuads;

/************************************************************************************
 *
 *
 * 3D Window and Surface of Revolution Code 
 *
 * Fill in the code in the empty functions
 ************************************************************************************/
GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };

// Set up lighting/shading and material properties for surface 
GLfloat quadMat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat quadMat_specular[] = { 0.45, 0.55, 0.45, 1.0 };
GLfloat quadMat_diffuse[] = { 0.1, 0.35, 0.1, 1.0 };
GLfloat quadMat_shininess[] = { 10.0 };

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

void drawPlayerCannon() {

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, quadMat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, quadMat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, quadMat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, quadMat_shininess);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertBufID);
	glVertexPointer(3, GL_DOUBLE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normBufID);
	glNormalPointer(GL_DOUBLE, 0, 0);

	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);

	
	glDrawElements(GL_QUADS, totalQuads * 4, GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void generateArrays() {
	
	std::ifstream inFile;
	inFile.open("savedMesh.txt");
	
	inFile >> totalVertices;
	inFile >> totalQuads;
	vCoords = new double[totalVertices * 3];
	indices = new unsigned int[totalQuads * 4];
	vertexNormals = new double[totalVertices * 3];

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

