//#define _HAS_ITERATOR_DEBUGGING 0

//Glee
#include "glee.h"
//glut (freeglut now)
#include "glut.h"
//glm
#include "glm/glm.hpp"
//libconfig
#include "libconfig/libconfig.h++"

using namespace glm;
using namespace libconfig;

#ifndef __camera_h__
#define __camera_h__

class camera
{
	GLdouble orientationMatrix[16], positionMatrix[16];
	vec3 right,up,forward,viewPoint,position;
	Config *cfg;
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
	// �������� ��������� ������.
	bool loadCamera(Config *cfg);
	// ���������� ��������� ������.
	bool saveCamera();
};

#endif