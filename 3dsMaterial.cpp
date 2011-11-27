#include <math.h>
#include "3dsMaterial.h"

namespace ns_3ds
{
	c3dsMaterial::c3dsMaterial()
	{
		cf_materialName="";
		//заполнение значениями по умолчанию
		for(int i=0; i<4; i++)
		{
			cf_ambient[i]=((i==3)?1.0f:0.2f);
			cf_diffuse[i]=((i==3)?1.0f:0.8f);
			cf_specular[i]=((i==3)?1.0f:0.0f);
			cf_emission[i]=((i==3)?1.0f:0.0f);
		}
		cf_shininessPercent=0.0f;
		cf_shininessStrengthPercent=0.0f;
		cf_transparencyPercent=0.0f;
		cf_transparencyFalloffPercent=0.0f;
		cf_textureMap=NULL;
	}
	c3dsMaterial::c3dsMaterial(GLfloat *amb, GLfloat *diff, GLfloat *spec, GLfloat *emiss, GLfloat shin)
	{
		// заполнение переданными значениями или значениями по умолчанию при переданном NULL
		for(int i=0; i<4; i++)
		{
			cf_ambient[i]=(amb)?amb[i]:((i==3)?1.0f:0.0f);
			cf_diffuse[i]=(diff)?diff[i]:((i==3)?1.0f:0.0f);
			cf_specular[i]=(spec)?spec[i]:((i==3)?1.0f:0.0f);
			cf_emission[i]=(emiss)?emiss[i]:((i==3)?1.0f:0.0f);
		}
		// контроль диапазона 0.0<=shininess<=128.0
		shin=((shin>=0.0f)&&(shin<=128.0f))?shin:0.0f;
		cf_shininessPercent=sqrtf(shin)*sqrtf(10000.0f);
		cf_shininessStrengthPercent=sqrtf(shin)*sqrtf(10000.0f);
		cf_transparencyPercent=0.0f;
		cf_transparencyFalloffPercent=0.0f;
		cf_textureMap=NULL;
	}

	void c3dsMaterial::SetMaterialName(std::string a_name)
	{
		cf_materialName=a_name;
	}

	std::string c3dsMaterial::GetMaterialName()
	{
		return cf_materialName;
	}

	void c3dsMaterial::SetAmbient(tFloat *a_ambient)
	{
		if (a_ambient)
		{
			for (int i=0; i<4; i++)
			{
				cf_ambient[i]=a_ambient[i];
			}
		}
	}

	void c3dsMaterial::SetDiffuse(tFloat *a_diffuse)
	{
		if (a_diffuse)
		{
			for (int i=0; i<4; i++)
			{
				cf_diffuse[i]=a_diffuse[i];
			}
		}
	}

	void c3dsMaterial::SetSpecular(tFloat *a_specular)
	{
		if (a_specular)
		{
			for (int i=0; i<4; i++)
			{
				cf_specular[i]=a_specular[i];
			}
		}
	}

	void c3dsMaterial::SetEmission(tFloat *a_emission)
	{
		if (a_emission)
		{
			for (int i=0; i<4; i++)
			{
				cf_emission[i]=a_emission[i];
			}
		}
	}

	void c3dsMaterial::SetShininessPercent(tFloat a_shininess)
	{
		// контроль диапазона 0.0<=cf_shininess<=100.0
		cf_shininessPercent=((a_shininess>=0.0f)&&(a_shininess<=100.0f))?a_shininess:0.0f;
	}

	void c3dsMaterial::SetShininessStrengthPercent(tFloat a_shininess)
	{
		// контроль диапазона 0.0<=cf_shininess<=100.0
		cf_shininessStrengthPercent=((a_shininess>=0.0f)&&(a_shininess<=100.0f))?a_shininess:0.0f;
	}

	void c3dsMaterial::SetTransparencyPercent(tFloat a_transparency)
	{
		// контроль диапазона 0.0<=cf_transparencyPercent<=100.0
		cf_transparencyPercent=((a_transparency>=0.0f)&&(a_transparency<=100.0f))?a_transparency:0.0f;
	}

	void c3dsMaterial::SetTransparencyFalloffPercent(tFloat a_transparency)
	{
		// контроль диапазона 0.0<=cf_transparencyFalloffPercent<=100.0
		cf_transparencyFalloffPercent=((a_transparency>=0.0f)&&(a_transparency<=100.0f))?a_transparency:0.0f;
	}

	void c3dsMaterial::SetReflectionBlurPercent(tFloat a_reflectionBlur)
	{
		// контроль диапазона 0.0<=cf_reflectionBlurPercent<=100.0
		cf_reflectionBlurPercent=((a_reflectionBlur>=0.0f)&&(a_reflectionBlur<=100.0f))?a_reflectionBlur:0.0f;
	}

	void c3dsMaterial::SetTextureMap(c3dsTextureDevIL *a_textureMap)
	{
		// ToDo: добавить проверку на инициализированность текстуры
		cf_textureMap=a_textureMap;
	}

	bool c3dsMaterial::cm_Use(tEnum a_face)
	{
		// контроль переданных данных
		if (a_face==GL_FRONT||a_face==GL_FRONT_AND_BACK||a_face==GL_FRONT)
		{
			// установка параметров материала
			glMaterialfv(a_face,GL_AMBIENT,cf_ambient);
			glMaterialfv(a_face,GL_DIFFUSE,cf_diffuse);
			glMaterialfv(a_face,GL_SPECULAR,cf_specular);
			glMaterialfv(a_face,GL_EMISSION,cf_emission);
			tFloat _shininess=((cf_shininessPercent*cf_shininessStrengthPercent)/10000.0f)*128.0f;
			glMaterialfv(a_face,GL_SHININESS,&_shininess);
			if (cf_textureMap)
			{
				glEnable(GL_TEXTURE_2D);
				glClientActiveTexture(GL_TEXTURE0);
				cf_textureMap->bind();
			}
			return true;
		}
		return false;
	}
}