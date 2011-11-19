#ifndef __3dsMaterial_h__
#define __3dsMaterial_h__

#include <string>
#include "3dsTypes.h"

namespace ns_3ds
{
	class c3dsMaterial
	{
		std::string cf_materialName;
		// составляющие материала
		tFloat cf_ambient[4];
		tFloat cf_diffuse[4];
		tFloat cf_specular[4];
		tFloat cf_emission[4];
		tFloat cf_shininessPercent;
		tFloat cf_shininessStrengthPercent;
		tFloat cf_transparencyPercent;
		tFloat cf_transparencyFalloffPercent;
		tFloat cf_reflectionBlurPercent;
	public:
		// конструктор по умолчанию
		c3dsMaterial();
		// конструктор по передаваемым значениям
		c3dsMaterial(tFloat *amb, tFloat *diff=NULL, tFloat *spec=NULL, tFloat *emiss=NULL, tFloat shin=0.0f);
		// установка имени материала
		void SetMaterialName(std::string a_name);
		// получение имени материала
		std::string GetMaterialName();
		// установка амбиентной составляющей
		void SetAmbient(tFloat *a_ambient);
		// установка диффузной составляющей
		void SetDiffuse(tFloat *a_diffuse);
		// установка спекулярной составляющей
		void SetSpecular(tFloat *a_specular);
		// установка эмиссионной составляющей
		void SetEmission(tFloat *a_emission);
		// установка блеска
		void SetShininessPercent(tFloat a_shininess);
		// установка блеска
		void SetShininessStrengthPercent(tFloat a_shininess);
		// установка прозрачности
		void SetTransparencyPercent(tFloat a_transparency);
		// установка прозрачности
		void SetTransparencyFalloffPercent(tFloat a_transparency);
		// установка размытости отражения
		void SetReflectionBlurPercent(tFloat a_reflectionBlur);
		// использование материала для граней face
		bool cm_Use(tEnum a_face=GL_FRONT_AND_BACK);
	};
}

#endif