#ifndef __vertex_h__
#define __vertex_h__

#include "stdafx.h"
#include "glm\glm.hpp"

using namespace glm;

//class point
//{
//	GLfloat 
//}

// координаты и цвет точки
struct vertex
{
	GLfloat coordinate[3];
	GLfloat color[3];
	vertex()
	{
		coordinate[0]=0.0f; coordinate[1]=0.0f; coordinate[2]=0.0f;
		color[0]=1.0f; color[1]=1.0f; color[2]=1.0f;
	};
	vertex(GLfloat x, GLfloat y, GLfloat z)
	{
		coordinate[0]=x; coordinate[1]=y; coordinate[2]=z;
		color[0]=1.0f; color[1]=1.0f; color[2]=1.0f;
	}
	vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r,  GLfloat g, GLfloat b)
	{
		coordinate[0]=x; coordinate[1]=y; coordinate[2]=z;
		color[0]=r; color[1]=g; color[2]=b;
	}
	void copyColor(GLfloat *v)
	{
		memcpy(color,v,sizeof(GLfloat)*3);
	}
	void copyCoord(GLfloat *v)
	{
		memcpy(coordinate,v,sizeof(GLfloat)*3);
	}
	void copy(vertex &v)
	{
		memcpy(coordinate,v.coordinate,sizeof(GLfloat)*3);
		memcpy(color,v.color,sizeof(GLfloat)*3);
	}
	vec3 vec3Coordinate()
	{
		return vec3(coordinate[0],coordinate[1],coordinate[2]);
	}
	vec3 vec3Color()
	{
		return vec3(color[0],color[1],color[2]);
	}
};

struct vertexNormal
{
	GLfloat coordinate[3];
	GLfloat normal[3];
	vertexNormal()
	{
		coordinate[0]=0.0f; coordinate[1]=0.0f; coordinate[2]=0.0f;
		normal[0]=0.0f; normal[1]=1.0f; normal[2]=0.0f;
	};
	vertexNormal(GLfloat x, GLfloat y, GLfloat z)
	{
		coordinate[0]=x; coordinate[1]=y; coordinate[2]=z;
		normal[0]=0.0f; normal[1]=1.0f; normal[2]=0.0f;
	}
	vertexNormal(GLfloat x, GLfloat y, GLfloat z, GLfloat nx,  GLfloat ny, GLfloat nz)
	{
		coordinate[0]=x; coordinate[1]=y; coordinate[2]=z;
		normal[0]=nx; normal[1]=ny; normal[2]=nz;
	}
	void copyNormal(GLfloat *v)
	{
		memcpy(normal,v,sizeof(GLfloat)*3);
	}
	void copyCoord(GLfloat *v)
	{
		memcpy(coordinate,v,sizeof(GLfloat)*3);
	}
	void copy(vertexNormal &v)
	{
		memcpy(coordinate,v.coordinate,sizeof(GLfloat)*3);
		memcpy(normal,v.normal,sizeof(GLfloat)*3);
	}
	vec3 vec3Coordinate()
	{
		return vec3(coordinate[0],coordinate[1],coordinate[2]);
	}
	vec3 vec3Normal()
	{
		return vec3(normal[0],normal[1],normal[2]);
	}
};

struct vertexNormalTex
{
	GLfloat coordinate[3];
	GLfloat normal[3];
	GLfloat tex[2];
	vertexNormalTex()
	{
		coordinate[0]=0.0f; coordinate[1]=0.0f; coordinate[2]=0.0f;
		normal[0]=0.0f; normal[1]=1.0f; normal[2]=0.0f;
		tex[0]=0.0f; tex[1]=0.0f;
	};
	vertexNormalTex(GLfloat x, GLfloat y, GLfloat z)
	{
		coordinate[0]=x; coordinate[1]=y; coordinate[2]=z;
		normal[0]=0.0f; normal[1]=1.0f; normal[2]=0.0f;
		tex[0]=0.0f; tex[1]=0.0f;
	}
	vertexNormalTex(GLfloat x, GLfloat y, GLfloat z, GLfloat nx,  GLfloat ny, GLfloat nz)
	{
		coordinate[0]=x; coordinate[1]=y; coordinate[2]=z;
		normal[0]=nx; normal[1]=ny; normal[2]=nz;
		tex[0]=0.0f; tex[1]=0.0f;
	}
	vertexNormalTex(GLfloat x, GLfloat y, GLfloat z, GLfloat nx,  GLfloat ny, GLfloat nz, GLfloat u, GLfloat v)
	{
		coordinate[0]=x; coordinate[1]=y; coordinate[2]=z;
		normal[0]=nx; normal[1]=ny; normal[2]=nz;
		tex[0]=u; tex[1]=v;
	}
	void copyNormal(GLfloat *v)
	{
		memcpy(normal,v,sizeof(GLfloat)*3);
	}
	void copyCoord(GLfloat *v)
	{
		memcpy(coordinate,v,sizeof(GLfloat)*3);
	}
	void copyTex(GLfloat *v)
	{
		memcpy(tex,v,sizeof(GLfloat)*2);
	}
	void copy(vertexNormalTex &v)
	{
		memcpy(coordinate,v.coordinate,sizeof(GLfloat)*3);
		memcpy(normal,v.normal,sizeof(GLfloat)*3);
		memcpy(tex,v.tex,sizeof(GLfloat)*2);
	}
	void copy(vertexNormal &v)
	{
		memcpy(coordinate,v.coordinate,sizeof(GLfloat)*3);
		memcpy(normal,v.normal,sizeof(GLfloat)*3);
	}
	void copy(vertex &v)
	{
		memcpy(coordinate,v.coordinate,sizeof(GLfloat)*3);
	}
	vec3 vec3Coordinate()
	{
		return vec3(coordinate[0],coordinate[1],coordinate[2]);
	}
	vec3 vec3Normal()
	{
		return vec3(normal[0],normal[1],normal[2]);
	}
};

class material
{
	// составляющие материала
	GLfloat ambient[4],diffuse[4],specular[4],emission[4],shininess;
public:
	// конструктор по умолчанию
	material()
	{
		//заполнение значениями по умолчанию
		for(int i=0; i<4; i++)
		{
			ambient[i]=((i==3)?1.0f:0.2f);
			diffuse[i]=((i==3)?1.0f:0.8f);
			specular[i]=((i==3)?1.0f:0.0f);
			emission[i]=((i==3)?1.0f:0.0f);
		}
		shininess=0;
	}
	// конструктор по передаваемым значениям
	material(GLfloat *amb, GLfloat *diff=NULL, GLfloat *spec=NULL, GLfloat *emiss=NULL, GLfloat shin=0.0f)
	{
		// заполнение переданными значениями или значениями по умолчанию при переданном NULL
		for(int i=0; i<4; i++)
		{
			ambient[i]=(amb)?amb[i]:((i==3)?1.0f:0.0f);
			diffuse[i]=(diff)?diff[i]:((i==3)?1.0f:0.0f);
			specular[i]=(spec)?spec[i]:((i==3)?1.0f:0.0f);
			emission[i]=(emiss)?emiss[i]:((i==3)?1.0f:0.0f);
		}
		// контроль диапазона 0.0<=shininess<=128.0
		shininess=((shin>=0.0f)&&(shin<=128.0f))?shin:0.0f;
	}
	// использование материала для граней face
	bool use(GLenum face=GL_FRONT_AND_BACK)
	{
		// контроль переданных данных
		if (face==GL_FRONT||face==GL_FRONT_AND_BACK||face==GL_FRONT)
		{
			// установка параметров материала
			glMaterialfv(face,GL_AMBIENT,ambient);
			glMaterialfv(face,GL_DIFFUSE,diffuse);
			glMaterialfv(face,GL_SPECULAR,specular);
			glMaterialfv(face,GL_EMISSION,emission);
			glMaterialfv(face,GL_SHININESS,&shininess);
			return true;
		}
		return false;
	}
};

class lightSource
{
	// составляющие источника света
	GLfloat ambient[4],diffuse[4],specular[4],position[4],spotDirection[3],spotExponent,spotCutoff,constantAttenuation,linearAttenuation,quadraticAttenuation;
public:
	// конструктор по умолчанию
	lightSource()
	{
		//заполнение значениями по умолчанию
		for(int i=0; i<4; i++)
		{
			ambient[i]=((i==3)?1.0f:0.0f);
			diffuse[i]=((i==3)?1.0f:0.0f);
			specular[i]=((i==3)?1.0f:0.0f);
			position[i]=((i==3)?1.0f:0.0f);
			spotDirection[i]=((i==3)?1.0f:0.0f);
		}
		spotExponent=0;
		spotCutoff=0;
		constantAttenuation=1;
		linearAttenuation=0;
		quadraticAttenuation=0;
	}
	// конструктор по передаваемым значениям
	lightSource(GLfloat *amb, GLfloat *diff=NULL, GLfloat *spec=NULL, GLfloat *pos=NULL, GLfloat *sDir=NULL, GLfloat sExp=0.0f, GLfloat sCut=180.0f, GLfloat cAtt=1.0f, GLfloat lAtt=0.0f, GLfloat qAtt=0.0f)
	{
		// заполнение переданными значениями или значениями по умолчанию при переданном NULL
		for(int i=0; i<4; i++)
		{
			ambient[i]=(amb)?amb[i]:((i==3)?1.0f:0.0f);
			diffuse[i]=(diff)?diff[i]:((i==3)?1.0f:0.0f);
			specular[i]=(spec)?spec[i]:((i==3)?1.0f:0.0f);
			position[i]=(pos)?pos[i]:((i==3)?1.0f:0.0f);
			if (i<3)
			{
				spotDirection[i]=(sDir)?amb[i]:((i==3)?1.0f:0.0f);
			}
		}
		spotExponent=sExp;
		// контроль диапазона (0.0<=spotCutoff<=90.0)//(spotCutoff==180.0)
		spotCutoff=(((sCut>=0.0f)&&(sCut<=90.0f))||(sCut=180.0f))?sCut:0.0f;
		//spotCosCutoff=(((sCosCut>=0.0f)&&(sCosCut<=1.0f))||(sCosCut=-1.0f))?sCosCut:0.0f;
		constantAttenuation=cAtt;
		linearAttenuation=lAtt;
		quadraticAttenuation=qAtt;
	}
	// использование значений для источника light
	bool use(GLenum light=GL_LIGHT0)
	{
		// контроль переданных данных
		if ((light>=GL_LIGHT0)&&(light<=GL_LIGHT0+GL_MAX_LIGHTS))
		{
			glLightfv(light,GL_AMBIENT,ambient);
			glLightfv(light,GL_DIFFUSE,diffuse);
			glLightfv(light,GL_SPECULAR,specular);
			glLightfv(light,GL_POSITION,position);
			glLightfv(light,GL_SPOT_DIRECTION,spotDirection);
			glLightfv(light,GL_SPOT_EXPONENT,&spotExponent);
			glLightfv(light,GL_SPOT_CUTOFF,&spotCutoff);
			glLightfv(light,GL_CONSTANT_ATTENUATION,&constantAttenuation);
			glLightfv(light,GL_LINEAR_ATTENUATION,&linearAttenuation);
			glLightfv(light,GL_QUADRATIC_ATTENUATION,&quadraticAttenuation);
			return true;
		}
		return false;
	}
};

#endif