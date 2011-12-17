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
		applyCamera();
		return cf_current;
	}

	tInt c3dsCamera::cm_PrevCamera()
	{
		cf_current--;
		cf_current=(cf_current<0)?cf_viewPoint.size()+cf_current:cf_current;
		applyCamera();
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

		// последняя строка
		orientationMatrix[3]=0;
		orientationMatrix[7]=0;
		orientationMatrix[11]=0;
		orientationMatrix[15]=1;
	}

	void c3dsCamera::applyCamera()
	{
		updateOrientationMatrix();
		updatePositionMatrix();
		cm_UpdateFrustum();
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

	void c3dsCamera::cm_UpdateFrustum()
	{
		float   proj[16];
		float   modl[16];
		float   clip[16];
		float   t;
		
		/* Узнаем текущую матрицу PROJECTION */
		glGetFloatv( GL_PROJECTION_MATRIX, proj );
		
		/* Узнаем текущую матрицу MODELVIEW */
		glGetFloatv( GL_MODELVIEW_MATRIX, modl );
		
		/* Комбинируем матрицы(перемножаем) */
		clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
		clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
		clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
		clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
 
		clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
		clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
		clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
		clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
 
		clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
		clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
		clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
		clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
 
		clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
		clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
		clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
		clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
 
		/* Находим A, B, C, D для ПРАВОЙ плоскости */
		cf_frustum[0][0] = clip[ 3] - clip[ 0];
		cf_frustum[0][1] = clip[ 7] - clip[ 4];
		cf_frustum[0][2] = clip[11] - clip[ 8];
		cf_frustum[0][3] = clip[15] - clip[12];
 
		/* Приводим уравнение плоскости к нормальному виду */
		t = sqrt( cf_frustum[0][0] * cf_frustum[0][0] + cf_frustum[0][1] * cf_frustum[0][1] + cf_frustum[0][2] * cf_frustum[0][2] );
		cf_frustum[0][0] /= t;
		cf_frustum[0][1] /= t;
		cf_frustum[0][2] /= t;
		cf_frustum[0][3] /= t;
 
		/* Находим A, B, C, D для ЛЕВОЙ плоскости */
		cf_frustum[1][0] = clip[ 3] + clip[ 0];
		cf_frustum[1][1] = clip[ 7] + clip[ 4];
		cf_frustum[1][2] = clip[11] + clip[ 8];
		cf_frustum[1][3] = clip[15] + clip[12];
 
		/* Приводим уравнение плоскости к нормальному виду */
		t = sqrt( cf_frustum[1][0] * cf_frustum[1][0] + cf_frustum[1][1] * cf_frustum[1][1] + cf_frustum[1][2] * cf_frustum[1][2] );
		cf_frustum[1][0] /= t;
		cf_frustum[1][1] /= t;
		cf_frustum[1][2] /= t;
		cf_frustum[1][3] /= t;
 
		/* Находим A, B, C, D для НИЖНЕЙ плоскости */
		cf_frustum[2][0] = clip[ 3] + clip[ 1];
		cf_frustum[2][1] = clip[ 7] + clip[ 5];
		cf_frustum[2][2] = clip[11] + clip[ 9];
		cf_frustum[2][3] = clip[15] + clip[13];
 
		/* Приводим уравнение плоскости к нормальному */
		t = sqrt( cf_frustum[2][0] * cf_frustum[2][0] + cf_frustum[2][1] * cf_frustum[2][1] + cf_frustum[2][2] * cf_frustum[2][2] );
		cf_frustum[2][0] /= t;
		cf_frustum[2][1] /= t;
		cf_frustum[2][2] /= t;
		cf_frustum[2][3] /= t;
 
		/* ВЕРХНЯЯ плоскость */
		cf_frustum[3][0] = clip[ 3] - clip[ 1];
		cf_frustum[3][1] = clip[ 7] - clip[ 5];
		cf_frustum[3][2] = clip[11] - clip[ 9];
		cf_frustum[3][3] = clip[15] - clip[13];
 
		/* Нормальный вид */
		t = sqrt( cf_frustum[3][0] * cf_frustum[3][0] + cf_frustum[3][1] * cf_frustum[3][1] + cf_frustum[3][2] * cf_frustum[3][2] );
		cf_frustum[3][0] /= t;
		cf_frustum[3][1] /= t;
		cf_frustum[3][2] /= t;
		cf_frustum[3][3] /= t;
 
		/* ЗАДНЯЯ плоскость */
		cf_frustum[4][0] = clip[ 3] - clip[ 2];
		cf_frustum[4][1] = clip[ 7] - clip[ 6];
		cf_frustum[4][2] = clip[11] - clip[10];
		cf_frustum[4][3] = clip[15] - clip[14];
 
		/* Нормальный вид */
		t = sqrt( cf_frustum[4][0] * cf_frustum[4][0] + cf_frustum[4][1] * cf_frustum[4][1] + cf_frustum[4][2] * cf_frustum[4][2] );
		cf_frustum[4][0] /= t;
		cf_frustum[4][1] /= t;
		cf_frustum[4][2] /= t;
		cf_frustum[4][3] /= t;
 
		/* ПЕРЕДНЯЯ плоскость */
		cf_frustum[5][0] = clip[ 3] + clip[ 2];
		cf_frustum[5][1] = clip[ 7] + clip[ 6];
		cf_frustum[5][2] = clip[11] + clip[10];
		cf_frustum[5][3] = clip[15] + clip[14];
 
		/* Нормальный вид */
		t = sqrt( cf_frustum[5][0] * cf_frustum[5][0] + cf_frustum[5][1] * cf_frustum[5][1] + cf_frustum[5][2] * cf_frustum[5][2] );
		cf_frustum[5][0] /= t;
		cf_frustum[5][1] /= t;
		cf_frustum[5][2] /= t;
		cf_frustum[5][3] /= t;
	}

	tFrustum * c3dsCamera::GetFrustum()
	{
		return &cf_frustum;
	}
}