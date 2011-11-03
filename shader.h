#ifndef __shader_h__
#define __shader_h__

#include "glee.h"
#include "glut.h"
#include <string>
#include <vector>
#include "vertex.h"

using namespace std;

class shader
{
	// идентификатор программного объекта
	GLuint idProgram;
	// идентификаторы шейдерных объектов
	GLenum fS,vS;
	// признаки успешкой загрузки объектов
	bool fSLoaded, vSLoaded, programLoaded;
	// признаки наличия необходимых расширений
	static bool newOGL, hasFS, hasVS;
	// загрузка шейдерного объекта из файла
	GLenum load(string filename, GLenum type);
public:
	// конструктор по умолчанию
	shader():fSLoaded(false), vSLoaded(false), programLoaded(false) {};
	// загрузка из файла фрагментного шейдера
	bool loadFragmentShader(string filename);
	// загрузка из файла вертексного шейдера
	bool loadVertexShader(string filename);
	// линковка программы
	bool createProgram();
	// активация программного объекта
	bool activate();
	// получение идентификатора программного объекта
	GLuint getProgramId()
	{
		return programLoaded?idProgram:0;
	}
	// проверка расширений
	static void checkExtensions();
	// отключение программного объекта
	static void disactivate();
};

#endif