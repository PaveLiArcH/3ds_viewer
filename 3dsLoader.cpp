#include "stdafx.h"
#include "3dsTypes.h"
#include "3dsLoader.h"
#include "3dsChunks.h"

namespace n3ds
{
	bool c3dsLoader::load (tistream & a_istream, c3ds & a_object)
	{
		s3dsHeader _header;
		while (!(a_istream.fail()||a_istream.eof()||((a_istream>>_header).eof())))
		{
			ptChunkReader _chunkReader=cm_getChunkReader(_header);
			bool _res=(*_chunkReader)(a_istream, _header, a_object);
		}
		return true;
	}
	
	ptChunkReader c3dsLoader::cm_getChunkReader(s3dsHeader & a_header)
	{
		switch (a_header.id)
		{
		case(chunks::MAIN):
			return &cm_chunkReaderMain;
		case(chunks::VERSION):
			return &cm_chunkReaderVersion;
		case(chunks::EDITOR):
			return &cm_chunkReaderEditor;
		default:
			return &cm_chunkReaderUnknown;
		}
	}

	bool c3dsLoader::cm_chunkReaderUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Обнаружен неизвестный чанк "<<std::hex<<a_header.id<<", пропуск."<<std::endl;
		if (a_header.len<6)
		{
			std::cerr<<"\tНекорректная длина чанка"<<std::endl;
			// TODO throw an exception
			return false;
		}
		int _pos=a_istream.tellg();
		a_istream.clear();
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMain (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден главный чанк "<<std::hex<<a_header.id<<std::endl;
		int _pos=a_istream.tellg();
		if (_pos!=6)
		{
			std::cerr<<"\tГлавный чанк находится не в начале файла"<<std::endl;
			// TODO throw an exception
			return false;
		}
		return true;
	}

	bool c3dsLoader::cm_chunkReaderVersion (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк версии "<<std::hex<<a_header.id<<std::endl;
		t3dsVersion _version;
		a_istream.read((char *)&_version, 4);
		a_object.cf_version=_version;
		if (_version<4)
		{
			std::cerr<<"\tВерсия 3ds меньше 4-й ("<<_version<<"), могут возникнуть визуальные искажения"<<std::endl;
			return true;
		} else
		{
			std::cerr<<"\tВерсия 3ds "<<_version<<std::endl;
		}
		return true;
	}

	bool c3dsLoader::cm_chunkReaderEditor (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк редактора "<<std::hex<<a_header.id<<std::endl;
		return true;
	}
}