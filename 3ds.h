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
#include "texture.h"

#include "3dsTypes.h"
#include "3dsLoader.h"

using namespace std;
using namespace glm;

namespace ns_3ds
{
	class c3dsMaterial;
	class c3dsObject;

	struct eqstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return strcmp(s1, s2) == 0;
		}
	};

	class c3ds
	{
		friend class ns_3ds::c3dsLoader;
		friend class ns_3ds::c3dsObject;
	//	friend ns_3ds::tistream & operator >> (ns_3ds::tistream & a_istream, c3ds & a_object);
	private:
		// ������ 3ds
		t3dsVersion cf_version;
		// ������ ����
		t3dsVersion cf_meshVersion;
		// ��� �������
		//vector<string> cf_name;
		// ���������
		hash_map<string, c3dsMaterial *> cf_material;
		// �������� �������
		tFloat cf_unit;
		//// ���������� ���������
		//vector<unsigned short> indexCount;
		//// ������ ��������� ������ � �� �������� �� ��������
		//vector<vertexNormalTex *> indexVertexNormal;
		//// ������� �������
		//vertex position;
		//// ������� �������� ��������
		//vector<GLfloat *> localMatrix;
		//// ������� �������� �������
		//GLfloat *directionMatrix;
		//// ����� VBO ��� ������
		//vector<GLuint> vertexVBO;
		// �������
		vector<c3dsObject *> cf_object;
		// ��������
		texture *tex;
		static bool newOGL, hasVBO;
		// �������������
		void init();
	public:
		// ����������� �� ���������
		c3ds();
		// ����������� �� �������� �������
		c3ds(vertex &pos);
		// ����������
		~c3ds();
		// �������� ����� 3ds � ������ � ������ VBO
		bool load(wstring fname);
		// ����� ������� �� �����
		void render(int filterMode);
		// ����������� � VBO
		void buffer();
		// ��������� ��������
		void setScale(GLfloat scale);
		//// ��������� ��������� �������
		//void setPosition(vertex &pos)
		//{
		//	position=pos;
		//	directionMatrix[12]=pos.coordinate[0];
		//	directionMatrix[13]=pos.coordinate[1];
		//	directionMatrix[14]=pos.coordinate[2];
		//}
		void copyTex(texture *t)
		{
			tex=t;
		}
		/*GLfloat * getColor()
		{
			return position.color;
		}*/
		static void checkExtensions();
	};
}

#include "3dsMaterial.h"
#include "3dsObject.h"

#endif