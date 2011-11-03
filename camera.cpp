//VC++
#include "stdio.h"
#include <windows.h>
#include <iostream>
#include <fstream>
//glm
#include "glm/glm.hpp"
//self implemented
#include "stdafx.h"
#include "camera.h"

using namespace glm;
using namespace std;

camera::camera(vec3 newPosition, vec3 newViewPoint)
{
	viewPoint=newViewPoint;
	position=newPosition;
	applyCamera();
}

camera::camera()
{
	viewPoint=vec3(0,0,0);
	position=vec3(0,30,20);
	applyCamera();
}

void camera::updatePositionMatrix()
{
	positionMatrix[0]=1;	positionMatrix[1]=0;	positionMatrix[2]=0;	positionMatrix[3]=0;
	positionMatrix[4]=0;	positionMatrix[5]=1;	positionMatrix[6]=0;	positionMatrix[7]=0;
	positionMatrix[8]=0;	positionMatrix[9]=0;	positionMatrix[10]=1;	positionMatrix[11]=0;
	positionMatrix[12]=-position.x; 
	positionMatrix[13]=-position.y; 
	positionMatrix[14]=-position.z; 
	positionMatrix[15]=1;
}

void camera::updateOrientationMatrix()
{
	forward = viewPoint - position;
	forward = normalize(forward);

	up = vec3(0,1,0);
	right = cross(forward,up);
	right = normalize(right);

	up  = cross(right,forward);
	up = normalize(up);

	// вектор вправо
	orientationMatrix[0]=right.x;
	orientationMatrix[4]=right.y;
	orientationMatrix[8]=right.z;
	orientationMatrix[12]=0;

	// вектор вверх
	orientationMatrix[1]=up.x;
	orientationMatrix[5]=up.y;
	orientationMatrix[9]=up.z;
	orientationMatrix[13]=0;

	// вектор -вперед
	orientationMatrix[2]=-forward.x;
	orientationMatrix[6]=-forward.y;
	orientationMatrix[10]=-forward.z;
	orientationMatrix[14]=0;

	// последн€€ строка
	orientationMatrix[3]=0;
	orientationMatrix[7]=0;
	orientationMatrix[11]=0;
	orientationMatrix[15]=1;
}

void camera::applyCamera()
{
	updateOrientationMatrix();
	updatePositionMatrix();
}

void camera::resetCamera()
{
	viewPoint=vec3(0,0,0);
	position=vec3(0,30,20);
	applyCamera();
}

GLdouble * camera::getOrientation()
{
	return orientationMatrix;
}

GLdouble * camera::getPosition()
{
	return positionMatrix;
}

void camera::moveForward()
{
	if (length(position+forward)>length(forward))
	{
		position=position+forward;
		applyCamera();
	}
}

void camera::moveBack()
{
	if (length(position-forward)>length(forward))
	{
		position=position-forward;
		applyCamera();
	}
}

void camera::moveRight()
{
	vec3 vecBefore=viewPoint-position;
	vecBefore.y=0;
	GLdouble lenBefore=length(vecBefore);
	position=position+right;
	applyCamera();
	vec3 vecAfter=viewPoint-position;
	vecAfter.y=0;
	GLdouble lenAfter=length(vecAfter);
	vec3 diff=viewPoint-position;
	diff.y=0;
	diff=normalize(diff);
	diff*=lenAfter-lenBefore;
	position=position+diff;
	applyCamera();
}

void camera::moveLeft()
{
	vec3 vecBefore=viewPoint-position;
	vecBefore.y=0;
	GLdouble lenBefore=length(vecBefore);
	position=position-right;
	applyCamera();
	vec3 vecAfter=viewPoint-position;
	vecAfter.y=0;
	GLdouble lenAfter=length(vecAfter);
	vec3 diff=viewPoint-position;
	diff.y=0;
	diff=normalize(diff);
	diff*=lenAfter-lenBefore;
	position=position+diff;
	applyCamera();
}

void camera::moveUp()
{
	vec3 vecBefore=viewPoint-position,vecTemp;
	GLdouble lenBefore=length(vecBefore);
	vecTemp=position+up;
	vecTemp.y=0;
	if (length(vecTemp)>0.5)
	{
		position=position+up;
	}
	applyCamera();
	vec3 vecAfter=viewPoint-position;
	GLdouble lenAfter=length(vecAfter);
	vec3 diff=viewPoint-position;
	diff=normalize(diff);
	diff*=lenAfter-lenBefore;
	position=position+diff;
	applyCamera();
}

void camera::moveDown()
{
	vec3 vecBefore=viewPoint-position;
	GLdouble lenBefore=length(vecBefore);
	if ((position-up).y>0)
	{
		position=position-up;
	}
	applyCamera();
	vec3 vecAfter=viewPoint-position;
	GLdouble lenAfter=length(vecAfter);
	vec3 diff=viewPoint-position;
	diff=normalize(diff);
	diff*=lenAfter-lenBefore;
	position=position+diff;
	applyCamera();
}

camera::~camera()
{
}