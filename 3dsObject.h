#ifndef __3dsObject_h__
#define __3dsObject_h__

#include <string>
#include "3dsTypes.h"

namespace n3ds
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
		tUint cf_verticesCount;
		// список индексов
		tUint *cf_indexList;
		// число индексов
		tUint cf_indexCount;

	public:
		c3dsObject();
		void SetName(std::string a_name);
		void SetHidden(bool a_hidden);
		void SetNotShadowing(bool a_notShadowing);
		void SetNotCast(bool a_notCast);
		void SetMatte(bool a_matte);
		void SetExternalProcessed(bool a_externalProcessed);
	};
}

#endif