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
		// Выполняет обновление матрицы позиции камеры. Параметров нет.
		void updatePositionMatrix();
		// Выполняет обновление матрицы ориентации камеры. Параметров нет.
		void updateOrientationMatrix();
		// коэффициенты для Frustum Culling
		tFrustum cf_frustum;
	public:
		// Конструктор по умолчанию. Задает стандартное положение камеры.
		c3dsCamera();
		// Деструктор, выполняющий сохранение данных о камере при уничтожении объекта.
		~c3dsCamera();
		// Конструктор с параметрами. Задает точку обзора и ориентацию камеры. Параметры – 2 трехкомпонентных вектора, задающих положение и точку обзора.
		c3dsCamera(vec3 a_position, vec3 a_viewPoint);
		// Конструктор с параметрами. Задает точку обзора и ориентацию камеры. Параметры – 2 трехкомпонентных вектора, задающих положение и точку обзора.
		tInt cm_Add(vec3 a_position, vec3 a_viewPoint);
		tInt cm_NextCamera();
		tInt cm_PrevCamera();
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
		// обновление коэффициентов для Frustum Culling-а
		void cm_UpdateFrustum();
		// получение коэффициентов
		tFrustum * GetFrustum();
	};
}

#endif