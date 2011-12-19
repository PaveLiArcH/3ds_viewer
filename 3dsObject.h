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

	class c3dsObject
	{
		// название объекта
		std::string cf_name;
		// объект невидим
		bool cf_hidden;
		// не затеняется
		bool cf_notShadowing;
		// не отбрасывает (тень)
		bool cf_notCast;
		// матовая поверхность?!
		bool cf_matte;
		// требует внешней обработки?!
		bool cf_externalProcessed;
		// список координат вертексов
		tFloat *cf_verticesList;
		// число вертексов
		tChunkID cf_verticesCount;
		// список индексов
		tChunkID *cf_indexList;
		// число индексов
		tChunkID cf_indexCount;
		// материалы сторон
		stdext::hash_map<std::string, std::vector<tChunkID> *> cf_faceMaterial;
		// список текстурных координат вертексов
		tFloat *cf_texList;
		// число вертексов
		tChunkID cf_texCount;
		// буфер VBO
		tUint cf_vertexVBO;
		// буферизованные данные
		sVertexNormalTex *cf_vertexBuffer;
		// frustum occluder
		glm::vec3 cf_sphere;
		tFloat cf_sphereRadius;
		tFloat cf_distance;
		// OC occluder
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
		void cm_ScaleChanged(tDouble a_newScale);

		friend bool operator<(c3dsObject &a_object, c3dsObject &a_otherObject);
	};

	bool operator<(c3dsObject &a_object, c3dsObject &a_otherObject);
}

#include "3ds.h"

#endif