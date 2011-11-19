#ifndef __3dsMaterial_h__
#define __3dsMaterial_h__

#include <string>
#include "3dsTypes.h"

namespace ns_3ds
{
	class c3dsMaterial
	{
		std::string cf_materialName;
		// ������������ ���������
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
		// ����������� �� ���������
		c3dsMaterial();
		// ����������� �� ������������ ���������
		c3dsMaterial(tFloat *amb, tFloat *diff=NULL, tFloat *spec=NULL, tFloat *emiss=NULL, tFloat shin=0.0f);
		// ��������� ����� ���������
		void SetMaterialName(std::string a_name);
		// ��������� ����� ���������
		std::string GetMaterialName();
		// ��������� ���������� ������������
		void SetAmbient(tFloat *a_ambient);
		// ��������� ��������� ������������
		void SetDiffuse(tFloat *a_diffuse);
		// ��������� ����������� ������������
		void SetSpecular(tFloat *a_specular);
		// ��������� ����������� ������������
		void SetEmission(tFloat *a_emission);
		// ��������� ������
		void SetShininessPercent(tFloat a_shininess);
		// ��������� ������
		void SetShininessStrengthPercent(tFloat a_shininess);
		// ��������� ������������
		void SetTransparencyPercent(tFloat a_transparency);
		// ��������� ������������
		void SetTransparencyFalloffPercent(tFloat a_transparency);
		// ��������� ���������� ���������
		void SetReflectionBlurPercent(tFloat a_reflectionBlur);
		// ������������� ��������� ��� ������ face
		bool cm_Use(tEnum a_face=GL_FRONT_AND_BACK);
	};
}

#endif