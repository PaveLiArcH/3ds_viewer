//#define _HAS_ITERATOR_DEBUGGING 0

#include "glee.h"
#include "glut.h"
#include "glm/glm.hpp"
using namespace glm;

#ifndef __camera_h__
#define __camera_h__

class camera
{
	GLdouble orientationMatrix[16], positionMatrix[16];
	vec3 right,up,forward,viewPoint,position;

	// Выполняет обновление матрицы позиции камеры. Параметров нет.
	void updatePositionMatrix();
	// Выполняет обновление матрицы ориентации камеры. Параметров нет.
	void updateOrientationMatrix();
public:
	// Конструктор по умолчанию. Задает стандартное положение камеры.
	camera();
	// Деструктор, выполняющий сохранение данных о камере при уничтожении объекта.
	~camera();
	// Конструктор с параметрами. Задает точку обзора и ориентацию камеры. Параметры – 2 трехкомпонентных вектора, задающих положение и точку обзора.
	camera(vec3 newPosition, vec3 newViewPoint);
	// Вызов функций перерасчета матриц камеры.
	void applyCamera();
	// Сброс камеры в состояние по умолчанию.
	void resetCamera();
	// Получение матрицы ориентации камеры.
	GLdouble * getOrientation();
	// Получение матрицы позиции камеры.
	GLdouble * getPosition();
	// Перемещение вперед на единичный вектор.
	void moveForward();
	// Перемещение назад на единичный вектор.
	void moveBack();
	// Перемещение вправо по сфере на единичный вектор.
	void moveRight();
	// Перемещение влево по сфере на единичный вектор.
	void moveLeft();
	// Перемещение вверх по сфере на единичный вектор.
	void moveUp();
	// Перемещение вниз по сфере на единичный вектор.
	void moveDown();
	// Загрузка состояния камеры.
	bool loadCamera();
	// Сохранение состояния камеры.
	bool saveCamera();
};

#endif