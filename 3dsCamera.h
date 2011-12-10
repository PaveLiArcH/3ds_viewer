#ifndef __3dsCamera_h__
#define __3dsCamera_h__

//Glee
#include "glee.h"
//glut (freeglut now)
#include "glut.h"
//glm
#include "glm/glm.hpp"
#include "3dsTypes.h"
#include <vector>

using namespace glm;

namespace ns_3ds
{
	class c3dsCamera
	{
		std::vector<vec3> cf_viewPoint;
		std::vector<vec3> cf_position;
		tInt cf_current;
		vec3 right,up,forward;
		GLdouble orientationMatrix[16], positionMatrix[16];
		// ��������� ���������� ������� ������� ������. ���������� ���.
		void updatePositionMatrix();
		// ��������� ���������� ������� ���������� ������. ���������� ���.
		void updateOrientationMatrix();
		// ������������ ��� Frustum Culling
		tFrustum cf_frustum;
	public:
		// ����������� �� ���������. ������ ����������� ��������� ������.
		c3dsCamera();
		// ����������, ����������� ���������� ������ � ������ ��� ����������� �������.
		~c3dsCamera();
		// ����������� � �����������. ������ ����� ������ � ���������� ������. ��������� � 2 ���������������� �������, �������� ��������� � ����� ������.
		c3dsCamera(vec3 a_position, vec3 a_viewPoint);
		// ����������� � �����������. ������ ����� ������ � ���������� ������. ��������� � 2 ���������������� �������, �������� ��������� � ����� ������.
		tInt cm_Add(vec3 a_position, vec3 a_viewPoint);
		tInt cm_NextCamera();
		tInt cm_PrevCamera();
		// ����� ������� ����������� ������ ������.
		void applyCamera();
		// ����� ������ � ��������� �� ���������.
		void resetCamera();
		// ��������� ������� ���������� ������.
		GLdouble * getOrientation();
		// ��������� ������� ������� ������.
		GLdouble * getPosition();
		// ����������� ������ �� ��������� ������.
		void moveForward();
		// ����������� ����� �� ��������� ������.
		void moveBack();
		// ����������� ������ �� ����� �� ��������� ������.
		void moveRight();
		// ����������� ����� �� ����� �� ��������� ������.
		void moveLeft();
		// ����������� ����� �� ����� �� ��������� ������.
		void moveUp();
		// ����������� ���� �� ����� �� ��������� ������.
		void moveDown();
		// ���������� ������������� ��� Frustum Culling-�
		void cm_UpdateFrustum();
		// ��������� �������������
		tFrustum * GetFrustum();
	};
}

#endif