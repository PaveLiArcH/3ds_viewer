//VC++
#include "stdio.h"
#include <windows.h>
#include <iostream>
#include <fstream>
//glm
#include "glm/glm.hpp"
//self implemented
#include "stdafx.h"
#include "3dsCamera.h"

using namespace glm;
using namespace std;

namespace ns_3ds
{
	c3dsCamera::c3dsCamera(vec3 a_position, vec3 a_viewPoint)
	{
		cf_viewPoint.push_back(vec3(0,0,0));
		cf_position.push_back(vec3(0,30,20));
		cf_viewPoint.push_back(a_viewPoint);
		cf_position.push_back(a_position);
		cf_current=0;
		applyCamera();
	}

	tInt c3dsCamera::cm_Add(vec3 a_position, vec3 a_viewPoint)
	{
		cf_viewPoint.push_back(a_viewPoint);
		cf_position.push_back(a_position);
		return cf_viewPoint.size()-1;
	}

	tInt c3dsCamera::cm_NextCamera()
	{
		cf_current++;
		cf_current%=cf_viewPoint.size();
		return cf_current;
	}

	c3dsCamera::c3dsCamera()
	{
		cf_viewPoint.push_back(vec3(0,0,0));
		cf_position.push_back(vec3(0,30,20));
		cf_current=0;
		applyCamera();
	}

	void c3dsCamera::updatePositionMatrix()
	{
		positionMatrix[0]=1;	positionMatrix[1]=0;	positionMatrix[2]=0;	positionMatrix[3]=0;
		positionMatrix[4]=0;	positionMatrix[5]=1;	positionMatrix[6]=0;	positionMatrix[7]=0;
		positionMatrix[8]=0;	positionMatrix[9]=0;	positionMatrix[10]=1;	positionMatrix[11]=0;
		positionMatrix[12]=-cf_position[cf_current].x; 
		positionMatrix[13]=-cf_position[cf_current].y; 
		positionMatrix[14]=-cf_position[cf_current].z; 
		positionMatrix[15]=1;
	}

	void c3dsCamera::updateOrientationMatrix()
	{
		forward = cf_viewPoint[cf_current] - cf_position[cf_current];
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

	void c3dsCamera::applyCamera()
	{
		updateOrientationMatrix();
		updatePositionMatrix();
	}

	void c3dsCamera::resetCamera()
	{
		cf_current=0;
		applyCamera();
	}

	GLdouble * c3dsCamera::getOrientation()
	{
		return orientationMatrix;
	}

	GLdouble * c3dsCamera::getPosition()
	{
		return positionMatrix;
	}

	void c3dsCamera::moveForward()
	{
		if (length(cf_position[cf_current]+forward)>length(forward))
		{
			cf_position[cf_current]=cf_position[cf_current]+forward;
			applyCamera();
		}
	}

	void c3dsCamera::moveBack()
	{
		if (length(cf_position[cf_current]-forward)>length(forward))
		{
			cf_position[cf_current]=cf_position[cf_current]-forward;
			applyCamera();
		}
	}

	void c3dsCamera::moveRight()
	{
		vec3 vecBefore=cf_viewPoint[cf_current]-cf_position[cf_current];
		vecBefore.y=0;
		GLdouble lenBefore=length(vecBefore);
		cf_position[cf_current]=cf_position[cf_current]+right;
		applyCamera();
		vec3 vecAfter=cf_viewPoint[cf_current]-cf_position[cf_current];
		vecAfter.y=0;
		GLdouble lenAfter=length(vecAfter);
		vec3 diff=cf_viewPoint[cf_current]-cf_position[cf_current];
		diff.y=0;
		diff=normalize(diff);
		diff*=lenAfter-lenBefore;
		cf_position[cf_current]=cf_position[cf_current]+diff;
		applyCamera();
	}

	void c3dsCamera::moveLeft()
	{
		vec3 vecBefore=cf_viewPoint[cf_current]-cf_position[cf_current];
		vecBefore.y=0;
		GLdouble lenBefore=length(vecBefore);
		cf_position[cf_current]=cf_position[cf_current]-right;
		applyCamera();
		vec3 vecAfter=cf_viewPoint[cf_current]-cf_position[cf_current];
		vecAfter.y=0;
		GLdouble lenAfter=length(vecAfter);
		vec3 diff=cf_viewPoint[cf_current]-cf_position[cf_current];
		diff.y=0;
		diff=normalize(diff);
		diff*=lenAfter-lenBefore;
		cf_position[cf_current]=cf_position[cf_current]+diff;
		applyCamera();
	}

	void c3dsCamera::moveUp()
	{
		vec3 vecBefore=cf_viewPoint[cf_current]-cf_position[cf_current],vecTemp;
		GLdouble lenBefore=length(vecBefore);
		vecTemp=cf_position[cf_current]+up;
		vecTemp.y=0;
		if (length(vecTemp)>0.5)
		{
			cf_position[cf_current]=cf_position[cf_current]+up;
		}
		applyCamera();
		vec3 vecAfter=cf_viewPoint[cf_current]-cf_position[cf_current];
		GLdouble lenAfter=length(vecAfter);
		vec3 diff=cf_viewPoint[cf_current]-cf_position[cf_current];
		diff=normalize(diff);
		diff*=lenAfter-lenBefore;
		cf_position[cf_current]=cf_position[cf_current]+diff;
		applyCamera();
	}

	void c3dsCamera::moveDown()
	{
		vec3 vecBefore=cf_viewPoint[cf_current]-cf_position[cf_current];
		GLdouble lenBefore=length(vecBefore);
		if ((cf_position[cf_current]-up).y>0)
		{
			cf_position[cf_current]=cf_position[cf_current]-up;
		}
		applyCamera();
		vec3 vecAfter=cf_viewPoint[cf_current]-cf_position[cf_current];
		GLdouble lenAfter=length(vecAfter);
		vec3 diff=cf_viewPoint[cf_current]-cf_position[cf_current];
		diff=normalize(diff);
		diff*=lenAfter-lenBefore;
		cf_position[cf_current]=cf_position[cf_current]+diff;
		applyCamera();
	}

	c3dsCamera::~c3dsCamera()
	{
	}
}