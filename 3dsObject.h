#ifndef __3dsObject_h__
#define __3dsObject_h__

#include "glm\glm.hpp"
#include <string>
#include "3dsTypes.h"
#include <hash_map>
#include <vector>
#include "3dsVertex.h"

namespace ns_3ds
{
	class c3ds;
	class c3dsMaterial;

	struct s3dsObjectPieceBuffered
	{
		c3dsMaterial *sf_material;
		sVertexNormalTex *sf_buffer;
		long long sf_count;
	};

	struct s3dsObjectPieceBufferedVBO
	{
		c3dsMaterial *sf_material;
		tUint sf_buffer;
		long long sf_count;
	};

	class c3dsObject
	{
		// �������� �������
		std::string cf_name;
		// ������ �������
		bool cf_hidden;
		// �� ����������
		bool cf_notShadowing;
		// �� ����������� (����)
		bool cf_notCast;
		// ������� �����������?!
		bool cf_matte;
		// ������� ������� ���������?!
		bool cf_externalProcessed;
		// ���� ����������������
		bool cf_buffered;
		// ������ ��������� ���������
		tFloat *cf_verticesList;
		// ����� ���������
		tChunkID cf_verticesCount;
		// ������ ��������
		tChunkID *cf_indexList;
		// ����� ��������
		tChunkID cf_indexCount;
		// ��������� ������
		stdext::hash_map<std::string, std::vector<tChunkID> *> cf_faceMaterial;
		// ������ ���������� ��������� ���������
		tFloat *cf_texList;
		// ����� ���������
		tChunkID cf_texCount;
		// ����� VBO
		//stdext::hash_map<std::string, tUint> cf_vertexVBO;
		s3dsObjectPieceBufferedVBO *cf_vertexVBO;
		// �������������� ������
		//stdext::hash_map<std::string, sVertexNormalTex *> cf_vertexBuffer;
		s3dsObjectPieceBuffered *cf_vertexBuffer;
		tUint cf_vertexBufferedCount;
		// frustum occluder
		glm::vec3 cf_sphere;
		tFloat cf_sphereRadius;
		tFloat cf_distance;
		// OC occluder
		sVertex *cf_occluderBuffer;
		tUint cf_occluderVBO;
		tUint cf_queryId;
		bool cf_wasVisible;
		bool cf_occlusionTestPassed;
		int cf_checkTimer;
		// occluder common data
		tFloat cf_minX,cf_maxX,cf_minY,cf_maxY,cf_minZ,cf_maxZ;
	public:
		c3dsObject();
		~c3dsObject();
		void SetName(std::string a_name);
		void SetHidden(bool a_hidden);
		void SetNotShadowing(bool a_notShadowing);
		void SetNotCast(bool a_notCast);
		void SetMatte(bool a_matte);
		void SetExternalProcessed(bool a_externalProcessed);
		void SetVerticesList(tFloat *a_verticesList);
		void SetVerticesCount(tChunkID a_verticesCount);
		void SetIndexList(tChunkID *a_indexList);
		void SetIndexCount(tChunkID a_indexCount);
		void SetFaceMaterial(std::string a_materialName, std::vector<tChunkID> *a_faces);
		void SetTexList(tFloat *a_texList);
		void SetTexCount(tChunkID a_texCount);

		bool cm_Buffer(c3ds *a_3ds);
		bool cm_Render(c3ds *a_3ds);
		bool cm_FrustumTest(c3ds *a_3ds);
		int cm_OcclusionTest(c3ds *a_3ds, int a_number);
		void cm_ScaleChanged(tDouble a_newScale);
		void cm_RecalcFrustum(tDouble a_newScale);
		void cm_RecalcOccluder(tDouble a_newScale);

		friend bool operator<(c3dsObject &a_object, c3dsObject &a_otherObject);
	};

	bool operator<(c3dsObject &a_object, c3dsObject &a_otherObject);

	bool Compare3dsObjects(c3dsObject *a_object, c3dsObject *a_otherObject);
}

#include "3ds.h"
#include "3dsMaterial.h"

#endif