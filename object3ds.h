//Glee
#include "glee.h"
//VC++
#include <string>
#include <vector>
//glm
#include "glm/glm.hpp"
//glut
#include "glut.h"
//self implemented
#include "vertex.h"
#include "texture.h"

using namespace std;
using namespace glm;

#ifndef __object3ds_h__
#define __object3ds_h__

class object3DS
{
private:
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
	object3DS();
	// конструктор по заданной позиции
	object3DS(vertex &pos);
	// деструктор
	~object3DS();
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

#endif