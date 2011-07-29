#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include "shader.h"
#include "vertex.h"

using namespace std;

bool shader::newOGL=false; // OGL version
bool shader::hasFS=false; // fragment shader support
bool shader::hasVS=false; // vertex shader support

// проверка расширений
void shader::checkExtensions()
{
	shader::newOGL=atof((const char *)glGetString(GL_VERSION))>=1.5f; // check OGL version
	shader::hasFS=glutExtensionSupported("GL_ARB_fragment_shader")!=0; // check if fragment shader is supported
	shader::hasVS=glutExtensionSupported("GL_ARB_vertex_shader")!=0; // check if vertex shader is supported
}

// загрузка шейдерного объекта из файла
GLenum shader::load(string fname, GLenum type)
{
	ifstream shaderFile(fname.c_str(), ios::in|ios::binary);
	if (!shaderFile)
	{
		cerr<<"Shader file "<<fname<<" unavailable"<<endl; // file can't be opened for reading
		return 0;
	}
	// получаем размер файла
	shaderFile.seekg(0,ios::end);
	unsigned int size=shaderFile.tellg();
	shaderFile.seekg(0,ios::beg);
	// проверка файла на пустоту
	if (size)
	{
		// подготовка буфера необходимого размера
		char *buf=new char[size+1];
		// заполнение буфера символами 0
		memset(buf,0,size+1);
		// чтение файла в буфер
		shaderFile.read(buf,size);
		// разбиение строки буфера на подстроки
		char *tmp=strtok(buf,"\n");
		// список строк
		vector<char *> strings;
		while(tmp)
		{
			// добавление строки в список
			strings.push_back(tmp);
			// получение очередной подстроки
			tmp=strtok(NULL,"\n");
		}
		// получение количества строк
		int numStrings=strings.size();
		// подготовка массива
		char **cstrings=new char *[numStrings];
		// перенос строк в массив
		for(int i=0; i<numStrings; i++)
		{
			cstrings[i]=strings[i];
		}
		// идентификатор объекта
		GLenum id;
		// статус компиляции
		int status;
		if (newOGL)
		{
			// создание шейдерного объекта
			id=glCreateShader(type);
			// загрузка кода шейдера
			glShaderSource(id, numStrings, const_cast<const GLchar **>(cstrings), NULL);
			try
			{
				// компиляция кода
				glCompileShader(id);
			} catch (...)
			{
				cerr<<"Error while trying to compile using driver. Please check that you using lastest version of driver"<<endl;
				cerr<<"Deleting comments from shaders can help"<<endl; // ugly Intel OGL
			}
			// получение статуса компиляции
			glGetShaderiv(id,GL_COMPILE_STATUS,&status);
		} else
		{
			// создание шейдерного объекта
			id=glCreateShaderObjectARB(type);
			// загрузка кода шейдера
			glShaderSourceARB(id, numStrings, const_cast<const GLchar **>(cstrings), NULL);
			try
			{
				// компиляция кода
				glCompileShaderARB(id);
			} catch (...)
			{
				cerr<<"Error while trying to compile using driver. Please check that you using lastest version of driver"<<endl;
				cerr<<"Deleting comments from shaders can help"<<endl; // ugly Intel OGL
			}
			// получение статуса компиляции
			glGetObjectParameterivARB(id,GL_OBJECT_COMPILE_STATUS_ARB,&status);
		}
		// удаление временных переменных
		delete []buf;
		delete []cstrings;
		strings.clear();

		if (newOGL)
		{
			// проверка статуса на ошибочность
			if (status==GL_FALSE)
			{
				// длина сообщения, количество считанных символов
				int len,readed;
				// получение длины сообщения
				glGetShaderiv(id,GL_INFO_LOG_LENGTH,&len);
				// подготовка буфера для сообщения
				char *log=new char[len+1];
				// заполнение буфера символами 0
				memset(log,0,len+1);
				// получение текста сообщения
				glGetShaderInfoLog(id,len,&readed,log);
				// выдача текста сообщения
				cerr<<"Error while compiling "<<fname<<endl<<log<<endl;
				// удаление объекта
				glDeleteShader(id);
				// удаление буфера
				delete []log;
				return 0;
			}
		} else
		{
			// проверка статуса на ошибочность
			if (!status)
			{
				// длина сообщения, количество считанных символов
				int len,readed;
				// получение длины сообщения
				glGetObjectParameterivARB(id,GL_OBJECT_INFO_LOG_LENGTH_ARB,&len);
				// подготовка буфера для сообщения
				char *log=new char[len+1];
				// заполнение буфера символами 0
				memset(log,0,len+1);
				// получение текста сообщения
				glGetInfoLogARB(id,len,&readed,log);
				// выдача текста сообщения
				cerr<<"Error while compiling "<<fname<<endl<<log<<endl;
				// удаление объекта
				glDeleteObjectARB(id);
				// удаление буфера
				delete []log;
				return 0;
			}
		}
		return id;
	}
	return 0;
};

bool shader::loadFragmentShader(string fname)
{
	// наличие поддержки фрагментных шейдеров
	if (hasFS)
	{
		// загрузка шейдера
		fS=load(fname,GL_FRAGMENT_SHADER);
		// установка признака успешной загрузки
		fSLoaded=fS!=0;
	}
	return fSLoaded;
}

bool shader::loadVertexShader(string fname)
{
	// наличие поддержки вертексных шейдеров
	if (hasVS)
	{
		// загрузка шейдера
		vS=load(fname,GL_VERTEX_SHADER);
		// установка признака успешной загрузки
		vSLoaded=vS!=0;
	}
	return vSLoaded;
}

bool shader::createProgram()
{
	// проверка успешности загрузки шейдеров
	if (fSLoaded && vSLoaded)
	{
		// статус линковки
		int status;
		if (newOGL)
		{
			// создание программы
			idProgram=glCreateProgram();
			// подключение шейдеров
			glAttachShader(idProgram, vS);
			glAttachShader(idProgram, fS);
			// линковка программы
			glLinkProgram(idProgram);
			// получение статуса линковки
			glGetProgramiv(idProgram,GL_COMPILE_STATUS,&status);
			// проверка на ошибочность
			//if (status==GL_FALSE)
			//{
			//	// длина сообщения, количество считанных символов
			//	int len,readed;
			//	// получение длины сообщения
			//	glGetProgramiv(idProgram,GL_INFO_LOG_LENGTH,&len);
			//	// подготовка буфера для сообщения
			//	char *log=new char[len+1];
			//	// заполнение буфера символами 0
			//	memset(log,0,len+1);
			//	// получение текста сообщения
			//	glGetProgramInfoLog(idProgram,len,&readed,log);
			//	// выдача текста сообщения
			//	cerr<<"Error while linking program"<<endl<<log<<endl;
			//	// удаление объекта
			//	glDeleteProgram(idProgram);
			//	// удаление буфера
			//	delete []log;
			//	return false;
			//}
		} else
		{
			// создание программы
			idProgram=glCreateProgramObjectARB();
			// подключение шейдеров
			glAttachObjectARB(idProgram, vS);
			glAttachObjectARB(idProgram, fS);
			// линковка программы
			glLinkProgramARB(idProgram);
			// получение статуса линковки
			glGetObjectParameterivARB(idProgram,GL_OBJECT_LINK_STATUS_ARB,&status);
			// проверка на ошибочность
			if (!status)
			{
				// длина сообщения, количество считанных символов
				int len,readed;
				// получение длины сообщения
				glGetObjectParameterivARB(idProgram,GL_OBJECT_INFO_LOG_LENGTH_ARB,&len);
				// подготовка буфера для сообщения
				char *log=new char[len+1];
				// заполнение буфера символами 0
				memset(log,0,len+1);
				// получение текста сообщения
				glGetInfoLogARB(idProgram,len,&readed,log);
				// выдача текста сообщения
				cerr<<"Error while linking program"<<endl<<log<<endl;
				// удаление объекта
				glDeleteObjectARB(idProgram);
				// удаление буфера
				delete []log;
				return false;
			}
		}
		programLoaded=true;
	}
	return programLoaded;
}

// активация программного объекта
bool shader::activate()
{
	// проверка успешной загрузки программы
	if (programLoaded)
	{
		if (newOGL)
		{
			// активация программного объекта
			glUseProgram(idProgram);
		} else
		{
			// активация программного объекта
			glUseProgramObjectARB(idProgram);
		}
	}
	return programLoaded;
}

// отключение программного объекта
void shader::disactivate()
{
	// проверка поддержки шейдеров
	if (hasFS||hasVS)
	{
		if (newOGL)
		{
			// отключение программного объекта
			glUseProgram(NULL);
		} else
		{
			// отключение программного объекта
			glUseProgramObjectARB(NULL);
		}
	}
}