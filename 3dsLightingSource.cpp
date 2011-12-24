#include "3dsLightingSource.h"

namespace ns_3ds
{
		c3dsLightingSource::c3dsLightingSource()
		{
			//заполнение значениями по умолчанию
			for(int i=0; i<4; i++)
			{
				cf_ambient[i]=((i==3)?1.0f:0.0f);
				cf_diffuse[i]=((i==3)?1.0f:0.0f);
				cf_specular[i]=((i==3)?1.0f:0.0f);
				cf_position[i]=((i==3)?1.0f:0.0f);
				cf_spotDirection[i]=((i==3)?1.0f:0.0f);
			}
			cf_spotExponent=0;
			cf_spotCutoff=180.0f;
			cf_constantAttenuation=1.0;
			cf_linearAttenuation=0;
			cf_quadraticAttenuation=0;
		}
		// конструктор по передаваемым значениям
		c3dsLightingSource::c3dsLightingSource(GLfloat *amb, GLfloat *diff, GLfloat *spec, GLfloat *pos, GLfloat *sDir, GLfloat sExp, GLfloat sCut, GLfloat cAtt, GLfloat lAtt, GLfloat qAtt)
		{
			// заполнение переданными значениями или значениями по умолчанию при переданном NULL
			for(int i=0; i<4; i++)
			{
				cf_ambient[i]=(amb)?amb[i]:((i==3)?1.0f:0.0f);
				cf_diffuse[i]=(diff)?diff[i]:((i==3)?1.0f:0.0f);
				cf_specular[i]=(spec)?spec[i]:((i==3)?1.0f:0.0f);
				cf_position[i]=(pos)?pos[i]:((i==3)?1.0f:0.0f);
				if (i<3)
				{
					cf_spotDirection[i]=(sDir)?sDir[i]:((i==3)?1.0f:0.0f);
				}
			}
			cf_spotExponent=sExp;
			// контроль диапазона (0.0<=spotCutoff<=90.0)//(spotCutoff==180.0)
			cf_spotCutoff=(((sCut>=0.0f)&&(sCut<=90.0f))||(sCut=180.0f))?sCut:0.0f;
			//spotCosCutoff=(((sCosCut>=0.0f)&&(sCosCut<=1.0f))||(sCosCut=-1.0f))?sCosCut:0.0f;
			cf_constantAttenuation=cAtt;
			cf_linearAttenuation=lAtt;
			cf_quadraticAttenuation=qAtt;
		}
		//
		void c3dsLightingSource::cm_SetAmbient(tFloat *a_ambient)
		{
			for(int i=0; i<4; i++)
			{
				cf_ambient[i]=(a_ambient)?a_ambient[i]:((i==3)?1.0f:0.0f);
			}
		}
		void c3dsLightingSource::cm_SetDiffuse(tFloat *a_diffuse)
		{
			for(int i=0; i<4; i++)
			{
				cf_diffuse[i]=(a_diffuse)?a_diffuse[i]:((i==3)?1.0f:0.0f);
			}
		}
		void c3dsLightingSource::cm_SetSpecular(tFloat *a_specular)
		{
			for(int i=0; i<4; i++)
			{
				cf_specular[i]=(a_specular)?a_specular[i]:((i==3)?1.0f:0.0f);
			}
		}
		void c3dsLightingSource::cm_SetPosition(tFloat *a_position)
		{
			for(int i=0; i<4; i++)
			{
				cf_position[i]=(a_position)?a_position[i]:((i==3)?1.0f:0.0f);
			}
		}
		void c3dsLightingSource::cm_SetSpotDirection(tFloat *a_spotDirection)
		{
			for(int i=0; i<3; i++)
			{
				cf_spotDirection[i]=(a_spotDirection)?a_spotDirection[i]:((i==3)?1.0f:0.0f);
			}
		}
		void c3dsLightingSource::cm_SetSpotExponent(tFloat a_spotExponent)
		{
			cf_spotExponent=a_spotExponent;
		}
		void c3dsLightingSource::cm_SetSpotCutoff(tFloat a_spotCutoff)
		{
			cf_spotCutoff=a_spotCutoff;
		}
		void c3dsLightingSource::cm_SetConstantAttenuation(tFloat a_constantAttenuation)
		{
			cf_constantAttenuation=a_constantAttenuation;
		}
		// использование значений для источника light
		bool c3dsLightingSource::cm_Use(GLenum a_light)
		{
			// контроль переданных данных
			if ((a_light>=GL_LIGHT0)&&(a_light<=GL_LIGHT0+GL_MAX_LIGHTS))
			{
				glLightfv(a_light,GL_AMBIENT,cf_ambient);
				glLightfv(a_light,GL_DIFFUSE,cf_diffuse);
				glLightfv(a_light,GL_SPECULAR,cf_specular);
				glLightfv(a_light,GL_POSITION,cf_position);
				glLightfv(a_light,GL_SPOT_DIRECTION,cf_spotDirection);
				glLightfv(a_light,GL_SPOT_EXPONENT,&cf_spotExponent);
				glLightfv(a_light,GL_SPOT_CUTOFF,&cf_spotCutoff);
				glLightfv(a_light,GL_CONSTANT_ATTENUATION,&cf_constantAttenuation);
				glLightfv(a_light,GL_LINEAR_ATTENUATION,&cf_linearAttenuation);
				glLightfv(a_light,GL_QUADRATIC_ATTENUATION,&cf_quadraticAttenuation);
				return true;
			}
			return false;
		}
}