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
	class c3ds
	{
		friend class n3ds::c3dsLoader;
	//	friend n3ds::tistream & operator >> (n3ds::tistream & a_istream, c3ds & a_object);
	private:
		t3dsVersion cf_version;
		// имя объекта
		vector<string> name;
		// количество полигонов
		vector<unsigned short> indexCount;
		// список координат вершин и их нормалей по индексам
		vector<vertexNormalTex *> indexVertexNormal;
		// позиция объекта
		vertex position;
		// матрицы поворота объектов
		vector<GLfloat *> localMatrix;
		// матрица поворота объекта
		GLfloat *directionMatrix;
		// буфер VBO для вершин
		vector<GLuint> vertexVBO;
		// текстура
		texture *tex;
		static bool newOGL, hasVBO;
		// поиск чанка по потоку
		unsigned int findChunk(ifstream& ifs, unsigned short id, bool parent=true);
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
		// установка положения объекта
		void setPosition(vertex &pos)
		{
			position=pos;
			directionMatrix[12]=pos.coordinate[0];
			directionMatrix[13]=pos.coordinate[1];
			directionMatrix[14]=pos.coordinate[2];
		}
		void copyTex(texture *t)
		{
			tex=t;
		}
		GLfloat * getColor()
		{
			return position.color;
		}
		static void checkExtensions();
	};
}

#endif