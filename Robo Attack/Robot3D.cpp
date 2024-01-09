
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/freeglut.h>
#include <utility>
#include "Robot3D.h"


GLfloat robotBody_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotBody_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotBody_mat_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotBody_mat_shininess[] = { 0.0f };

GLfloat robotLimbs_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotLimbs_mat_diffuse[] = { 0.1f, 0.2f, 0.1f, 1.0f };
GLfloat robotLimbs_mat_specular[] = { 0.1f, 0.2f, 0.1f, 1.0f };
GLfloat robotLimbs_mat_shininess[] = { 110.8F };

GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };

void Robot3D::drawRobot()
{
	if (alive) {
		glPushMatrix();
		//translate whole bot based on current center

		glTranslatef(center.x, center.y, center.z);
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glPushMatrix();
		//rotate body joint
		glRotatef(bodyAngle, 0.0, 1.0, 0.0);
		drawBody();
		drawCannon();
		glPopMatrix();

		drawLowerBody();
		glPopMatrix();
	}
}


void Robot3D::drawBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	  glScalef(robotBodyWidth, robotBodyLength, robotBodyDepth);
	  makeCube();
	glPopMatrix();
}

void Robot3D::drawCannon()
{

	GLUquadricObj* cylinder = gluNewQuadric();
	gluQuadricTexture(cylinder, GL_TRUE);
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	glPolygonMode(GL_FRONT, GL_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);

	glPushMatrix();
	// Position whole cannon with respect to parent (body)
	  glTranslatef(0, -0.25*robotBodyLength, 0.5*robotBodyDepth); // this will be done last
	  //Rotate whole cannon
	  glRotatef(cannonAngle, 0.0, 0.0, 1.0);
	  
	  //Cannon Base
	  glPushMatrix();
	    glScalef(cannonBaseRadius, cannonBaseRadius, cannonBaseDepth);
	    gluCylinder(cylinder, 1.0, 1.0, 1.0, 20, 10);
	  glPopMatrix();
	
	  //Base Disk
	  glPushMatrix();
	    glTranslatef(0, 0, cannonBaseDepth);
		glScalef(cannonBaseRadius, cannonBaseRadius, 1.0);
	    gluDisk(gluNewQuadric(), 0, 1.0, 20, 10);
	  glPopMatrix();

	  //Barrell Ring
	  glPushMatrix();
	    glTranslatef(0, 0, 3*cannonBaseDepth);
	    glScalef(cannonBaseRadius, cannonBaseRadius, 0.22*cannonBaseDepth);
		glutSolidTorus(0.175, 0.7, 20, 10);
	  glPopMatrix();
	
	  //barrels
	  glPushMatrix();
	    glTranslatef(0.35 * cannonBaseRadius, 0.35*cannonBaseRadius, 0);
        glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 4 * cannonBaseDepth);
		gluCylinder(cylinder, 1.0, 1.0, 1.0, 20, 100);   
      glPopMatrix();
	  glPushMatrix();
		glTranslatef(0.35 * cannonBaseRadius, -0.35 * cannonBaseRadius, 0);
		glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 4 * cannonBaseDepth);
		gluCylinder(cylinder, 1.0, 1.0, 1.0, 20, 100);
	  glPopMatrix();
	  glPushMatrix();
		glTranslatef(-0.35 * cannonBaseRadius, 0.35 * cannonBaseRadius, 0);
	    glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 4 * cannonBaseDepth);
		gluCylinder(cylinder, 1.0, 1.0, 1.0, 20, 100);
	  glPopMatrix();
	  glPushMatrix();
		glTranslatef(-0.35 * cannonBaseRadius, -0.35 * cannonBaseRadius, 0);
		glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 4 * cannonBaseDepth);
		gluCylinder(cylinder, 1.0, 1.0, 1.0, 20, 100);
	  glPopMatrix();

	  //details
	  glPushMatrix();
	    glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 2 * cannonBaseDepth);
	    glTranslatef(0.0, 4.5 * cannonBaseRadius, 0.0);
	    makeCube();
	  glPopMatrix();
	  glPushMatrix();
	    glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 2 * cannonBaseDepth);
	    glTranslatef(0.0, -4.5 * cannonBaseRadius, 0.0);
        makeCube();
	  glPopMatrix();
	  glPushMatrix();
	    glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 2 * cannonBaseDepth);
	    glTranslatef(4.5 * cannonBaseRadius, 0.0, 0.0);
	    makeCube();
	  glPopMatrix();
	  glPushMatrix();
	    glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 2 * cannonBaseDepth);
	    glTranslatef(-4.5 * cannonBaseRadius, 0.0, 0.0);	
	    makeCube();
	  glPopMatrix();

	  //disks to simulate barrel darkness
	  glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	  glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	  glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	  glMaterialfv(GL_FRONT, GL_SHININESS, black);
	  glPushMatrix();
	    glTranslatef(0.35 * cannonBaseRadius, 0.35 * cannonBaseRadius, 4 * cannonBaseDepth);
	    glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 1.0);
	    gluDisk(gluNewQuadric(), 0, 1.0, 20, 10);
	  glPopMatrix();
	  glPushMatrix();
	    glTranslatef(0.35 * cannonBaseRadius, -0.35 * cannonBaseRadius, 4 * cannonBaseDepth);
	    glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 1.0);
	    gluDisk(gluNewQuadric(), 0, 1.0, 20, 10);
	  glPopMatrix();
	  glPushMatrix();
	    glTranslatef(-0.35 * cannonBaseRadius, 0.35 * cannonBaseRadius, 4 * cannonBaseDepth);
	    glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 1.0);
	    gluDisk(gluNewQuadric(), 0, 1.0, 20, 10);
	  glPopMatrix();
	  glPushMatrix();
	    glTranslatef(-0.35 * cannonBaseRadius, -0.35 * cannonBaseRadius, 4 * cannonBaseDepth);
	    glScalef(0.3 * cannonBaseRadius, 0.3 * cannonBaseRadius, 1.0);
	    gluDisk(gluNewQuadric(), 0, 1.0, 20, 10);
	  glPopMatrix();

	 //glPopMatrix();

	glPopMatrix();
}

void Robot3D::drawLowerBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);
	glPushMatrix();
	  glTranslatef(0, -(0.5*robotBodyLength + 0.5*pelvisLength), 0.125*pelvisDepth); // this will be done last
	  glPushMatrix();
	    glScalef(pelvisWidth,  pelvisLength, pelvisDepth);
		makeCube();
	  glPopMatrix();

	  drawRightLeg();
	  drawLeftLeg();

	glPopMatrix();
}


void Robot3D::drawRightLeg() {

	
	glPushMatrix();

	  //Rotate leg at hip joint (middle of pelvis)
	  glTranslatef(-(0.5 * pelvisWidth + 0.5 * upperLegWidth), 0.0, 0.0);
	  glRotatef(rightHipAngle, 1.0, 0.0, 0.0);
	  glTranslatef(0.5 * pelvisWidth + 0.5 * upperLegWidth, 0.0, 0.0);
	
	  // Position leg with respect to parent pelvis
	  glTranslatef(-(0.5*pelvisWidth + 0.5*upperLegWidth), -0.5*upperLegLength, 0.0);
	
	  //build upper leg
	  glPushMatrix();
	    glScalef(upperLegWidth, upperLegLength, upperLegWidth);
	    makeCube();
	  glPopMatrix();

	  glPushMatrix();
	    //Rotate lower leg at knee
	    glTranslatef(0.0, -(0.5*upperLegLength - 0.5 * lowerLegWidth), 0.0);
	    glRotatef(rightKneeAngle, 1.0, 0.0, 0.0);
	    glTranslatef(0.0, 0.5*upperLegLength - 0.5 * lowerLegWidth, 0.0);
	  
	    // Position lower leg with respect to parent upper leg
	    glTranslatef(0.0 , -(0.5*upperLegLength + 0.5*lowerLegLength - 0.5 * lowerLegWidth), 0.0);

	    //build lower leg
		glPushMatrix();
	      glScalef(lowerLegWidth, lowerLegLength, lowerLegWidth);
	      makeCube();
		glPopMatrix();

		glPushMatrix();
		  glTranslatef(0, -0.5 * lowerLegLength, -0.6 * lowerLegWidth);
		  glRotatef(restingKneeAngle, 1.0, 0.0, 0.0);
		  glutSolidCone(2.0 * lowerLegWidth, 2.0 * lowerLegWidth, 20, 10);
		glPopMatrix();

	  glPopMatrix();
	  
	glPopMatrix();

}

void Robot3D::drawLeftLeg() {
	
	glPushMatrix();

	  //Rotate leg at hip joint (middle of pelvis)
	  glTranslatef(0.5 * pelvisWidth + 0.5 * upperLegWidth, 0.0, 0.0);
	  glRotatef(leftHipAngle, 1.0, 0.0, 0.0);
	  glTranslatef(-(0.5 * pelvisWidth + 0.5 * upperLegWidth), 0.0, 0.0);

	  // Position leg with respect to parent pelvis
	  glTranslatef((0.5 * pelvisWidth + 0.5 * upperLegWidth), -0.5 * upperLegLength, 0.0);

	  //build upper leg
	  glPushMatrix();
	    glScalef(upperLegWidth, upperLegLength, upperLegWidth);
	    makeCube();
	  glPopMatrix();

	  glPushMatrix();
	    //Rotate lower leg at knee
	    glTranslatef(0.0, -(0.5 * upperLegLength - 0.5 * lowerLegWidth), 0.0);
	    glRotatef(leftKneeAngle, 1.0, 0.0, 0.0);
	    glTranslatef(0.0, 0.5 * upperLegLength - 0.5 * lowerLegWidth, 0.0);

	    // Position lower leg with respect to parent upper leg
	    glTranslatef(0.0, -(0.5 * upperLegLength + 0.5 * lowerLegLength - 0.5 * lowerLegWidth), 0.0);

	    //build lower leg
		glPushMatrix();
	      glScalef(lowerLegWidth, lowerLegLength, lowerLegWidth);
	      makeCube();
		glPopMatrix();

		glPushMatrix();
		  glTranslatef(0, -0.5 * lowerLegLength, -0.6 * lowerLegWidth);
		  glRotatef(restingKneeAngle, 1.0, 0.0, 0.0);
		  glutSolidCone(2.0*lowerLegWidth, 2.0*lowerLegWidth, 20, 10);
		glPopMatrix();

	  glPopMatrix();

	glPopMatrix();

}

bool Robot3D::checkCollision(VECTOR3D shotPos) {
	if (bboxMin.x <= shotPos.x && shotPos.x <= bboxMax.x &&
		bboxMin.y <= shotPos.y && shotPos.y <= bboxMax.y &&
		bboxMin.z <= shotPos.z && shotPos.z <= bboxMax.z
		) 
	{
		alive = false;
		shotFired = false;
		return true;
	}
	return false;
}

void Robot3D::setCenter(float x, float y, float z) {
	center.Set(x, y, z);
	setBbox();
}

void Robot3D::forwardStep(float z) {
	center.SetZ(center.z - z);
	setBbox();
}

void Robot3D::turnBody() {
	if (bodyAngle > angleLimit || bodyAngle < -angleLimit) {
		turnFactor *= -1.0;
	}
	bodyAngle += turnFactor;
}

void Robot3D::prepareShot() {
	if (alive) {
		botShotPos = center;
		botShotPos.SetY(botShotPos.y - (0.25 * robotBodyLength));
		botShotUnit = VECTOR3D(tan(bodyAngle * 3.14159 / 180) * shotUnitDistance, 2 * shotUnitDistance / center.z, shotUnitDistance);
		shotFired = true;
	}
}

void Robot3D::animateShot() {
	
	if (botShotPos.z >= 0.0) {
	botShotPos -= botShotUnit;
	}
}

void Robot3D::setBbox() {
	bboxMin.Set(center.x - robotBodyWidth * 0.6, center.y - robotBodyLength * 0.6, center.z - robotBodyDepth * 0.6);
	bboxMax.Set(center.x + robotBodyWidth * 0.6, center.y + robotBodyLength * 0.5, center.z + robotBodyDepth * 0.6);
}

void Robot3D::resetShot() {
	shotFired = false;
}

bool Robot3D::isAlive() {
	return alive;
}

bool Robot3D::isShotFired() {
	return shotFired;
}

VECTOR3D Robot3D::getBotShotPos() {
	return botShotPos;
}

void Robot3D::animate() {
	if (alive) {
		forwardStep(0.015);
		animateStep();
		turnBody();
		if (shotFired) {
			animateShot();
		}
		cannonAngle += 0.3;
	}
	if (center.z < -robotBodyDepth) {
		alive = false;
	}
}

void Robot3D::animateStep(){
		float targetHipAngle = hipStepAngles[currentStepStage];
		float hipAngleChange = (targetHipAngle - hipStepAngles[currentStepStage - 1]) * stepSpeed;
		float kneeAngleChange = (kneeStepAngles[currentStepStage] - kneeStepAngles[currentStepStage - 1]) * stepSpeed;
		float activeHipAngle = leftStep ? leftHipAngle : rightHipAngle;
		if (fabs((double)targetHipAngle - activeHipAngle) > fabs(hipAngleChange)) {
			if (leftStep) {
				leftHipAngle += hipAngleChange;
				leftKneeAngle += kneeAngleChange;
			}
			else {
				rightHipAngle += hipAngleChange;
				rightKneeAngle += kneeAngleChange;
			}
		}
		else if (currentStepStage != stepStagesNum) {
			currentStepStage += 1;
		}
		else {
			leftStep = !leftStep;
			currentStepStage = 1;
		}
}

void Robot3D::makeCube(){
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.5f, 0.5f, -0.5f);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.5f, -0.5f, -0.5f);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5f, 0.5f, 0.5f);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.5f, -0.5f, -0.5f);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-0.5f, 0.5f,  -0.5f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5f, 0.5f,  -0.5f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.5f, -0.5f, -0.5f);
 
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glEnd();
}

