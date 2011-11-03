#include "3dsHeader.h"

namespace n3ds
{
	tistream & operator >> (tistream &a_istream, s3dsHeader &a_header)
	{
		a_istream.read((char *)&a_header.id, 2);
		a_istream.read((char *)&a_header.len, 4);
		return a_istream;
	}

	tostream & operator << (tostream &a_ostream, const s3dsHeader &a_header)
	{
		a_ostream<<a_header.id;
		a_ostream<<a_header.len;
		return a_ostream;
	}
}