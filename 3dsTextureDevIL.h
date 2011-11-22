#ifndef __3dsTextureDevIL_h__
#define __3dsTextureDevIL_h__

//Glee
#include "glee.h"
//VC++
#include <string>
#include <wchar.h>
//DevIL
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
//glut
#include "glut.h"
#include "3dsTypes.h"

namespace ns_3ds
{
	class c3dsTetureDevIL
	{
		ILuint devilID;
		tUint openglID;
		bool loaded, mipMap;
		static tUint mode;
		static tInt g_nMaxAnisotropy;
	public:
		static void changeMode();
		static void useMode(bool mipMap=true);
		c3dsTetureDevIL():loaded(false),mipMap(true) {};
		bool load(std::wstring file, bool useMipMap=true, bool repeatS=true, bool repeatT=true);
		bool bind();
		void allowMimMap(bool allow);
		~c3dsTetureDevIL();
		tUint getTextureId();
	};
}

#endif