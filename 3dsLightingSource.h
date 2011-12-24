#ifndef __3dsLightingSource_h__
#define __3dsLightingSource_h__

#include "glm\glm.hpp"
#include "3dsTypes.h"

namespace ns_3ds
{
	class c3dsLightingSource
	{
		// составляющие источника света
		tFloat cf_ambient[4];
		tFloat cf_diffuse[4];
		tFloat cf_specular[4];
		tFloat cf_position[4];
		tFloat cf_spotDirection[3];
		tFloat cf_spotExponent;
		tFloat cf_spotCutoff;
		tFloat cf_constantAttenuation;
		tFloat cf_linearAttenuation;
		tFloat cf_quadraticAttenuation;
	public:
		// конструктор по умолчанию
		c3dsLightingSource();
		// конструктор по передаваемым значениям
		c3dsLightingSource(GLfloat *amb, GLfloat *diff=NULL, GLfloat *spec=NULL, GLfloat *pos=NULL, GLfloat *sDir=NULL, GLfloat sExp=0.0f, GLfloat sCut=180.0f, GLfloat cAtt=1.0f, GLfloat lAtt=0.0f, GLfloat qAtt=0.0f);
		//
		void cm_SetAmbient(tFloat *a_ambient);
		void cm_SetDiffuse(tFloat *a_diffuse);
		void cm_SetSpecular(tFloat *a_specular);
		void cm_SetPosition(tFloat *a_position);
		void cm_SetSpotDirection(tFloat *a_spotDirection);
		void cm_SetSpotExponent(tFloat a_spotExponent);
		void cm_SetSpotCutoff(tFloat a_spotCutoff);
		void cm_SetConstantAttenuation(tFloat a_constantAttenuation);
		// использование значений для источника light
		bool cm_Use(GLenum a_light=GL_LIGHT0);
	};
}
#endif