#ifndef __3dsTypes_h__
#define __3dsTypes_h__

#include <iostream>
#include "GLee.h"

namespace ns_3ds
{
	typedef GLfloat tFloat;
	typedef GLint tInt;
	typedef GLuint tUint;
	typedef GLenum tEnum;
	typedef unsigned short tChunkID;
	typedef unsigned int tChunkLen;
	typedef unsigned int t3dsVersion;
	typedef tFloat *tVec;
	typedef tFloat tVec3[3];
	typedef tFloat tVec4[4];
	typedef std::ostream tostream;
	typedef std::istream tistream;
}

#endif