#include <fstream>
#include "stdafx.h"
#include "3ds.h"
#include "3dsChunks.h"
#include "3dsLoader.h"
#include <Windows.h>

using namespace std;

namespace ns_3ds
{

	bool c3ds::newOGL=false;
	bool c3ds::hasVBO=false;

	void c3ds::checkExtensions()
	{
		c3ds::newOGL=(atof((const char *)glGetString(GL_VERSION))>=1.5f); // check OGL version
		c3ds::hasVBO=glutExtensionSupported("GL_ARB_vertex_buffer_object")!=0; // check if VBO supported
	}

	// буферизация данных объекта в VBO
	void c3ds::buffer()
	{
		for (std::size_t i=0; i<cf_object.size(); i++)
		{
			cf_object[i]->cm_Buffer(this);
		}
	}

	void c3ds::init()
	{
		cf_unit=1.0f;
	}

	// конструктор по умолчанию
	c3ds::c3ds()
	{
		init();
	}

	// конструктор по заданной позиции
	c3ds::c3ds(sVertex &pos)
	{
		init();
	};

	c3ds::~c3ds()
	{
		if (cf_material.size()>0)
		{
			hash_map<string, c3dsMaterial *>::iterator _it;
			_it=cf_material.begin();
			for (; _it!=cf_material.end(); _it++)
			{
				delete (_it->second);
			}
			cf_material.clear();
		}
		if (cf_object.size()>0)
		{
			for (std::size_t i=0; i<cf_object.size(); i++)
			{
				delete (cf_object[i]);
			}
			cf_object.clear();
		}
	}

	void c3ds::render(int filterMode)
	{
		for (std::size_t i=0; i<cf_object.size(); i++)
		{
			cf_object[i]->cm_Render(this);
		}
	}

	void c3ds::setScale(GLfloat scale)
	{
		/*for (int j=0; j<3; j++)
		{
			directionMatrix[j*4+j]=scale;
		}*/
	}

	bool c3ds::load(wstring fname)
	{
		bool _retVal=false;
		std::cerr<<"=================================="<<std::endl;
		string _ansiName=wstringToString(fname);
		std::cerr<<"Начало чтения файла модели "<<_ansiName<<std::endl;
		ifstream ifs;
		ifs.open(fname.c_str(),ios_base::in|ios_base::binary|ios_base::beg);
		if(!ifs.is_open())
		{
			std::cerr<<"Возникли ошибки при попытке чтения файла "<<_ansiName<<std::endl;
			// error while opening file
		}
		else
		{
			wchar_t *_path, *_file;
			DWORD _length=GetFullPathNameW(fname.c_str(), NULL, NULL, NULL);
			_path=new wchar_t[_length];
			DWORD _totalLen=GetFullPathNameW(fname.c_str(), _length, _path, &_file);
			if (_file)
			{
				_file[0]=0;
			}
			std::wstring _directory=_path;
			// чтение идентификатора чанка
			_retVal=ns_3ds::c3dsLoader::load(ifs, *this, _directory);
			delete []_path;
		}
		std::cerr<<"Завершение чтения файла модели "<<_ansiName<<std::endl;
		return _retVal;
	}
}