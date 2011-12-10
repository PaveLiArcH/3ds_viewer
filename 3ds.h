#ifndef __3ds_h__
#define __3ds_h__

//Glee
#include "glee.h"
//VC++
#include <string>
#include <vector>
#include <map>
#include <hash_map>
//glm
#include "glm/glm.hpp"
//glut
#include "glut.h"
//self implemented
#include "vertex.h"
#include "3dsTypes.h"
#include "3dsLoader.h"
#include "3dsVertex.h"
#include "3dsTextureDevIL.h"

#include "3dsCamera.h"

using namespace std;
using namespace glm;

namespace ns_3ds
{
	class c3dsMaterial;
	class c3dsObject;
	//class c3dsCamera;

	class c3ds
	{
		friend class ns_3ds::c3dsLoader;
		friend class ns_3ds::c3dsObject;
	private:
		// ������ 3ds
		t3dsVersion cf_version;
		// ������ ����
		t3dsVersion cf_meshVersion;
		// ���������
		hash_map<string, c3dsMaterial *> cf_material;
		// �������� �������
		tFloat cf_unit;
		// �������
		tDouble cf_scale;
		// ������� ���������������
		tFloat cf_scaleMatrix[16];
		// �������
		vector<c3dsObject *> cf_object;
		static bool newOGL, hasVBO;
		// �������������
		void init();
		c3dsCamera cf_camera;
	public:
		// ����������� �� ���������
		c3ds();
		// ����������� �� �������� �������
		c3ds(sVertex &pos);
		// ����������
		~c3ds();
		// �������� ����� 3ds � ������ � ������ VBO
		bool load(wstring fname);
		// ����� ������� �� �����
		void render(int filterMode);
		// ����������� � VBO
		void buffer();
		// ��������� ��������
		void cm_SetScale(tDouble a_scale);
		// ���������� �������
		void cm_UpScale();
		// ���������� �������
		void cm_DownScale();
		// �������� ����������
		static void checkExtensions();
		c3dsCamera * cm_GetCamera()
		{
			return &cf_camera;
		}
	};
}

#include "3dsMaterial.h"
#include "3dsObject.h"

#endif