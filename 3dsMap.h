#ifndef __3dsMap_h__
#define __3dsMap_h__

#include <string>
#include "3dsTypes.h"

namespace ns_3ds
{
	class c3dsMap
	{
		std::string cf_mapFile;
		tFloat cf_uScale,cf_vScale;
		tFloat cf_uOffset,cf_vOffset;
		tFloat cf_rotationAngle;
	public:
		c3dsMap():cf_mapFile(""),cf_uScale(1.0f),cf_vScale(1.0f),cf_uOffset(0.0f),cf_vOffset(0.0f),cf_rotationAngle(0.0f) {}
		void SetMapFile(std::string a_mapFile) { cf_mapFile=a_mapFile; }
		std::string GetMapFile() { return cf_mapFile; }
		void SetUScale(tFloat a_uScale) { cf_uScale=a_uScale; }
		void SetVScale(tFloat a_vScale) { cf_vScale=a_vScale; }
		void SetUOffset(tFloat a_uOffset) { cf_uOffset=a_uOffset; }
		void SetVOffset(tFloat a_vOffset) { cf_vOffset=a_vOffset; }
		void SetRotationAngle(tFloat a_rotationAngle) { cf_rotationAngle=a_rotationAngle; }
	};
}

#endif