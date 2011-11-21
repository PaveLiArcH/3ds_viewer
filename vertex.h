#ifndef __vertex_h__
#define __vertex_h__

#include "stdafx.h"
#include "glm\glm.hpp"

using namespace glm;

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