#ifndef __camera_h__
#define __camera_h__

//#define _HAS_ITERATOR_DEBUGGING 0

//Glee
#include "glee.h"
//glut (freeglut now)
#include "glut.h"
//glm
#include "glm/glm.hpp"

using namespace glm;

class camera
{
	GLdouble orientationMatrix[16], positionMatrix[16];
	vec3 right,up,forward,viewPoint,position;
	// ��������� ���������� ������� ������� ������. ���������� ���.
	void updatePositionMatrix();
	// ��������� ���������� ������� ���������� ������. ���������� ���.
	void updateOrientationMatrix();
public:
	// ����������� �� ���������. ������ ����������� ��������� ������.
	camera();
	// ����������, ����������� ���������� ������ � ������ ��� ����������� �������.
	~camera();
	// ����������� � �����������. ������ ����� ������ � ���������� ������. ��������� � 2 ���������������� �������, �������� ��������� � ����� ������.
	camera(vec3 newPosition, vec3 newViewPoint);
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
};

#endif