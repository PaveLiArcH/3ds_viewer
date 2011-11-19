#ifndef __3dsObject_h__
#define __3dsObject_h__

#include <string>
#include "3dsTypes.h"
#include <hash_map>
#include <vector>

namespace ns_3ds
{
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

	public:
		c3dsObject();
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
	};
}

#endif