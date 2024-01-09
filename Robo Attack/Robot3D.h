
#include "VECTOR3D.h"
#ifndef Robot3D_H
#define Robot3D_H
class Robot3D
{
public:
	Robot3D(float cX, float cY, float cZ, float startingAngle, float startingFactor) {
		setCenter(cX, cY, cZ);
		bodyAngle = startingAngle;
		turnFactor = startingFactor;
		hipStepAngles = new float[3]{ restingHipAngle, 60.0,  restingHipAngle };
		kneeStepAngles = new float[3]{ restingKneeAngle, -100.0, restingKneeAngle };
		stepStagesNum = 2;
		rightHipAngle = restingHipAngle;
		leftHipAngle = restingHipAngle;
		rightKneeAngle = restingKneeAngle;
		leftKneeAngle = restingKneeAngle;
	}
	void drawRobot();
	void forwardStep(float z);
	void turnBody();
	void prepareShot();
	void animate();
	void animateShot();
	void resetShot();
	bool checkCollision(VECTOR3D shotPos);
	bool isAlive();
	bool isShotFired();
	VECTOR3D getBotShotPos();

private:
	void animateStep();
	void drawBody();
	void drawCannon();
	void drawLowerBody();
	void drawLeftLeg();
	void drawRightLeg();
	void setCenter(float x, float y, float z);
	void setBbox();
	void makeCube();

	VECTOR3D bboxMin;
	VECTOR3D bboxMax;
	VECTOR3D center;
	
	VECTOR3D botShotUnit;
	VECTOR3D botShotPos;

	// Control Robot body rotation on base
	float bodyAngle;
	float turnFactor;
	float angleLimit = 45.0;
	// Control cannon rotation
	float cannonAngle = 45.0;
	//Control rate at which leg angles change relative to target position
	float stepSpeed = 0.01;
	float shotUnitDistance = 0.2;
	
	int currentStepStage = 1;
	float restingHipAngle = 30.0;
	float restingKneeAngle = -60.0;
	float rightHipAngle;
	float leftHipAngle;
	float rightKneeAngle;
	float leftKneeAngle;
	float* hipStepAngles;
	float* kneeStepAngles;
	int stepStagesNum;


	bool leftStep = false;
	bool shotFired = false;
	bool alive = true;

	float robotBodyWidth = 2.25;
	float robotBodyLength = 4.0;
	float robotBodyDepth = 2.5;
	float upperLegLength = 0.5 * robotBodyLength;
	float upperLegWidth = 0.15 * robotBodyWidth;
	float lowerLegLength = 0.75 * robotBodyLength;
	float lowerLegWidth = upperLegWidth;
	float cannonBaseRadius = 0.15 * robotBodyWidth;
	float cannonBaseDepth = 0.22 * robotBodyDepth;
	float pelvisLength = 0.15 * robotBodyLength;
	float pelvisWidth = 0.75 * robotBodyWidth;
	float pelvisDepth = 0.5 * robotBodyDepth;

	float stanchionLength = 0.3 * robotBodyLength;
	float stanchionRadius = 0.1 * robotBodyDepth;
	float baseWidth = 2 * robotBodyWidth;
	float baseLength = 0.25 * stanchionLength;
};
#endif