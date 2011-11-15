#ifndef __3ds_h__
#define __3ds_h__

//Glee
#include "glee.h"
//VC++
#include <string>
#include <vector>
#include <map>
//glm
#include "glm/glm.hpp"
//glut
#include "glut.h"
//self implemented
#include "vertex.h"
#include "texture.h"

#include "3dsTypes.h"
#include "3dsLoader.h"

using namespace std;
using namespace glm;

namespace n3ds
{
	class c3dsMaterial;
	class c3dsObject;

	class c3ds
	{
		friend class n3ds::c3dsLoader;
	//	friend n3ds::tistream & operator >> (n3ds::tistream & a_istream, c3ds & a_object);
	private:
		// версия 3ds
		t3dsVersion cf_version;
		// версия меша
		t3dsVersion cf_meshVersion;
		// имя объекта
		//vector<string> cf_name;
		// материалы
		vector<c3dsMaterial *> cf_material;
		// величина единицы
		tFloat cf_unit;
		//// количество полигонов
		//vector<unsigned short> indexCount;
		//// список координат вершин и их нормалей по индексам
		//vector<vertexNormalTex *> indexVertexNormal;
		//// позиция объекта
		//vertex position;
		//// матрицы поворота объектов
		//vector<GLfloat *> localMatrix;
		//// матрица поворота объекта
		//GLfloat *directionMatrix;
		//// буфер VBO для вершин
		//vector<GLuint> vertexVBO;
		// объекты
		vector<c3dsObject *> cf_object;
		// текстура
		texture *tex;
		static bool newOGL, hasVBO;
		// инициализация
		void init();
	public:
		// конструктор по умолчанию
		c3ds();
		// конструктор по заданной позиции
		c3ds(vertex &pos);
		// деструктор
		~c3ds();
		// загрузка файла 3ds и запись в буферы VBO
		bool load(wstring fname);
		// вывод объекта на экран
		void render(int filterMode);
		// буферизация в VBO
		void buffer();
		// установка масштаба
		void setScale(GLfloat scale);
		//// установка положения объекта
		//void setPosition(vertex &pos)
		//{
		//	position=pos;
		//	directionMatrix[12]=pos.coordinate[0];
		//	directionMatrix[13]=pos.coordinate[1];
		//	directionMatrix[14]=pos.coordinate[2];
		//}
		void copyTex(texture *t)
		{
			tex=t;
		}
		/*GLfloat * getColor()
		{
			return position.color;
		}*/
		static void checkExtensions();
	};
}

#include "3dsMaterial.h"
#include "3dsObject.h"

#endif