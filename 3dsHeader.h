#ifndef __3dsHeader_h__
#define __3dsHeader_h__

#include "stdafx.h"
#include "3dsTypes.h"

namespace n3ds
{
	struct s3dsHeader
	{
		tChunkID id;
		tChunkLen len;
	
		friend tistream & operator >> (tistream &a_istream, s3dsHeader &a_header);
		friend tostream & operator << (tostream &a_ostream, const s3dsHeader &a_header);
	};
}
#endif