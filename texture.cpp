#include "texture.h"

GLuint texture::mode=0;
GLint texture::g_nMaxAnisotropy=0;

void texture::changeMode()
{
	mode+=1;
	mode%=4;
}

void texture::useMode(bool mipMap)
{
	if (mipMap)
	{
		switch (mode)
		{
			case 0:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,1);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				break;
			case 1:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				break;
			case 2:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
				break;
			case 3:		
				glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&g_nMaxAnisotropy);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,g_nMaxAnisotropy);
				break;
		}
	} else
	{
		switch (mode)
		{
			case 0:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,1);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				break;
			case 1:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				break;
			case 2:
			case 3:		
				glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&g_nMaxAnisotropy);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,g_nMaxAnisotropy);
				break;
		}
	}
}

texture::~texture()
{
	if (loaded)
	{
		glDeleteTextures(1,&openglID);
		ilDeleteImages(1,&devilID);
		loaded=false;
	}
}

bool texture::bind()
{
	if (loaded)
	{
		glBindTexture(GL_TEXTURE_2D,openglID);
		useMode(mipMap);
		return true;
	};
	glBindTexture(GL_TEXTURE_2D,0);
	return false;
}

bool texture::load(wstring file, bool useMipMap, bool repeatS, bool repeatT)
{
	mipMap=useMipMap;
	this->~texture();
	ILuint devilError;
	GLenum openglError;
	devilError=ilGetError();
	openglError=glGetError();
	ilGenImages(1,&devilID);
	ilBindImage(devilID);
	ilLoadImage(file.c_str()); // load into the current bound image
	devilError=ilGetError();
	if(devilError!=IL_NO_ERROR)
	{
		wprintf(L"Devil Error (ilLoadImage): %s\nError while processing file: %s\n",iluErrorString(devilError),file.c_str());
		return false;
	}
	openglID=(mipMap)?ilutGLBindMipmaps():ilutGLBindTexImage(); // generate the GL texture
	devilError=ilGetError();
	openglError=glGetError();
	if(devilError!=IL_NO_ERROR)
	{
		wprintf(L"Devil Error (ilLoadImage): %s\nError while processing file: %s\n",iluErrorString(devilError),file.c_str());
		return false;
	}
	if(openglError!=GL_NO_ERROR)
	{
		printf("Opengl Error (ilutGLBindTexImage): %s\n", gluErrorString(openglError));
		return false;
	}
	GLint currTexId=0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D,&currTexId);
	glBindTexture(GL_TEXTURE_2D,openglID);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,(repeatS)?GL_REPEAT:GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,(repeatT)?GL_REPEAT:GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D,currTexId);
	loaded=true;
	return true;
}