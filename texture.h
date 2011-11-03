#ifndef __texture_h__
#define __texture_h__

//#define _HAS_ITERATOR_DEBUGGING 0
//Glee
#include "glee.h"
//VC++
#include <string>
#include <wchar.h>
//DevIL
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
//glut
#include "glut.h"

using namespace std;

class texture
{
	ILuint devilID;
	GLuint openglID;
	bool loaded, mipMap;
	static GLuint mode;
	static GLint g_nMaxAnisotropy;
public:
	static void changeMode();
	static void useMode(bool mipMap=true);
	texture():loaded(false),mipMap(true) {};
	bool load(wstring file, bool useMipMap=true, bool repeatS=true, bool repeatT=true);
	bool bind();
	void allowMimMap(bool allow)
	{
		if (loaded)
		{
			GLint currTexId=0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D,&currTexId);
			mipMap=allow;
			glBindTexture(GL_TEXTURE_2D,openglID);
			glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,(mipMap)?GL_TRUE:GL_FALSE);
			glBindTexture(GL_TEXTURE_2D,currTexId);
		}
	}
	~texture();
	GLuint getTextureId()
	{
		return (loaded)?openglID:0;
	}
};

#endif