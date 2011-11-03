#ifndef __3dsLoader_h__
#define __3dsLoader_h__

#include "3dsTypes.h"
#include "3dsHeader.h"
//#include "3ds.h"

namespace n3ds
{
	class c3ds;

	typedef bool (*ptChunkReader) (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);

	static class c3dsLoader
	{
		static ptChunkReader cm_getChunkReader(s3dsHeader & a_header);

		static bool cm_chunkReaderUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMain (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderVersion (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderEditor (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);

	public:
		static bool load (tistream & a_istream, c3ds & a_object);
	};
}

#include "3ds.h"

#endif