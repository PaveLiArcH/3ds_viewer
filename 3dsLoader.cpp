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
		case(chunks::MESHVERSION):
			return &cm_chunkReaderMeshVersion;
		case(chunks::MATERIAL):
			return &cm_chunkReaderMaterialBlock;
		case(chunks::ONEUNIT):
			return &cm_chunkReaderOneUnit;
		default:
			return &cm_chunkReaderUnknown;
		}
	}

	bool c3dsLoader::cm_chunkReaderUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"��������� ����������� ���� "<<std::hex<<a_header.id<<", �������."<<std::endl;
		if (a_header.len<6)
		{
			std::cerr<<"\t������������ ����� �����"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMain (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ������� ���� "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _pos=a_istream.tellg();
		if (_pos!=6)
		{
			std::cerr<<"\t������� ���� ��������� �� � ������ �����"<<std::endl;
			// TODO throw an exception
			return false;
		}
		return true;
	}

	bool c3dsLoader::cm_chunkReaderVersion (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ������ "<<std::hex<<a_header.id<<std::endl;
		t3dsVersion _version;
		a_istream.read((char *)&_version, 4);
		a_object.cf_version=_version;
		if (_version<4)
		{
			std::cerr<<"\t������ 3ds ������ 4-� ("<<_version<<"), ����� ���������� ���������� ���������"<<std::endl;
			return true;
		} else
		{
			std::cerr<<"\t������ 3ds "<<_version<<std::endl;
		}
		return true;
	}

	bool c3dsLoader::cm_chunkReaderEditor (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ���� "<<std::hex<<a_header.id<<std::endl;
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMeshVersion (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ������ ���� "<<std::hex<<a_header.id<<std::endl;
		t3dsVersion _version;
		a_istream.read((char *)&_version, 4);
		a_object.cf_meshVersion=_version;
		std::cerr<<"\t������ ���� "<<_version<<std::endl;
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialBlock (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ����� ��������� "<<std::hex<<a_header.id<<std::endl;
		a_object.cf_material.push_back(new c3dsMaterial);
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		bool _res=cm_chunkReaderMaterial(a_istream, _maxoff, a_object);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderOneUnit (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� �������� ������� "<<std::hex<<a_header.id<<std::endl;
		tFloat _unit;
		a_istream.read((char *)&_unit, 4);
		a_object.cf_unit=_unit;
		return true;
	}

	#pragma region MaterialChunks
	bool c3dsLoader::cm_chunkReaderMaterial (tistream & a_istream, std::streamoff & a_maxoffset, c3ds & a_object)
	{
		s3dsHeader _header;
		bool _res;
		while ((a_istream.tellg()<a_maxoffset)&&!(a_istream.fail()||a_istream.eof()||((a_istream>>_header).eof())))
		{
			ptChunkReaderMaterial _chunkReaderMaterial=cm_getChunkReaderMaterial(_header);
			_res=(*_chunkReaderMaterial)(a_istream, _header, a_object);
		}
		return true;
	}

	ptChunkReaderMaterial c3dsLoader::cm_getChunkReaderMaterial(s3dsHeader & a_header)
	{
		switch (a_header.id)
		{
		case(chunks::MATERIALNAME):
			return &cm_chunkReaderMaterialName;
		case(chunks::MATERIALAMBIENTCOLOR):
			return &cm_chunkReaderMaterialAmbient;
		case(chunks::DIFFUSECOLOR):
			return &cm_chunkReaderMaterialDiffuse;
		case(chunks::SPECULARCOLOR):
			return &cm_chunkReaderMaterialSpecular;
		case(chunks::SHININESSPERCENT):
			return &cm_chunkReaderMaterialShininess;
		case(chunks::SHININESSSTRENGTHPERCENT):
			return &cm_chunkReaderMaterialShininessStrength;
		case(chunks::TRANSPARENCYPERCENT):
			return &cm_chunkReaderMaterialTransparency;
		case(chunks::TRANSPARENCYFALLOFFPERCENT):
			return &cm_chunkReaderMaterialTransparencyFalloff;
		case(chunks::REFLECTIONBLURPERCENT):
			return &cm_chunkReaderMaterialReflectionBlur;
		default:
			return &cm_chunkReaderMaterialUnknown;
		}
	}

	bool c3dsLoader::cm_chunkReaderMaterialName (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� �������� ��������� "<<std::hex<<a_header.id<<std::endl;
		char _c;
		string _temp;
		while (a_istream.read(&_c,1)&&(!a_istream.eof())&&_c)
		{
			_temp+=_c;
		}
		a_object.cf_material.back()->SetMaterialName(_temp);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialAmbient (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ���������� ������������ ��������� "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tVec _color=cm_chunkReaderColor(a_istream, _maxoff);
		a_object.cf_material.back()->SetAmbient(_color);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialDiffuse (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ��������� ������������ ��������� "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tVec _color=cm_chunkReaderColor(a_istream, _maxoff);
		a_object.cf_material.back()->SetDiffuse(_color);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialSpecular (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ����������� ������������ ��������� "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tVec _color=cm_chunkReaderColor(a_istream, _maxoff);
		a_object.cf_material.back()->SetSpecular(_color);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialShininess (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ��������� ������������ ��������� "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_object.cf_material.back()->SetShininessPercent(_percent);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialShininessStrength (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ���� ��������� ������������ ��������� "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_object.cf_material.back()->SetShininessStrengthPercent(_percent);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialTransparency (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ���������� ������������ ��������� "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_object.cf_material.back()->SetTransparencyPercent(_percent);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialTransparencyFalloff (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ����� ���������� ������������ ��������� "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_object.cf_material.back()->SetTransparencyFalloffPercent(_percent);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialReflectionBlur (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ���� ���������� ��������� ��������� "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_object.cf_material.back()->SetReflectionBlurPercent(_percent);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"������ ����������� ���� "<<std::hex<<a_header.id<<" � ����� ������������ ������"<<std::endl;
		if (a_header.len<6)
		{
			std::cerr<<"\t������������ ����� �����"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return true;
	}
	#pragma endregion ������������ �����

	#pragma region ColorChunks
	tVec c3dsLoader::cm_chunkReaderColor (tistream & a_istream, std::streamoff & a_maxoffset)
	{
		s3dsHeader _header;
		tVec _res=NULL;
		while ((a_istream.tellg()<a_maxoffset)&&!(a_istream.fail()||a_istream.eof()||((a_istream>>_header).eof())))
		{
			ptChunkReaderColor _chunkReaderColor=cm_getChunkReaderColor(_header);
			if (_res!=NULL)
			{
				delete _res;
				std::cerr<<"����� ��� ���� �������� ���� ������"<<std::endl;
			}
			_res=(*_chunkReaderColor)(a_istream, _header);
		}
		return _res;
	}

	ptChunkReaderColor c3dsLoader::cm_getChunkReaderColor(s3dsHeader & a_header)
	{
		switch (a_header.id)
		{
		case(chunks::RGBf):
			return &cm_chunkReaderColorRGBf;
		case(chunks::RGBb):
			return &cm_chunkReaderColorRGBb;
		case(chunks::RGBGAMMACORRECTEDb):
			return &cm_chunkReaderColorRGBgb;
		case(chunks::RGBGAMMACORRECTEDf):
			return &cm_chunkReaderColorRGBgf;
		default:
			return &cm_chunkReaderColorUnknown;
		}
	}

	tVec c3dsLoader::cm_chunkReaderColorRGBf (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"������ �������� ���� RGBf "<<std::hex<<a_header.id<<std::endl;
		tVec _res=new tVec4;
		_res[3]=1.0f;
		a_istream.read((char *)&_res, 4*3);
		return _res;
	}

	tVec c3dsLoader::cm_chunkReaderColorRGBb (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"������ �������� ���� RGBb "<<std::hex<<a_header.id<<std::endl;
		tVec _res=new tVec4;
		_res[3]=1.0f;
		for (int i=0; i<3; i++)
		{
			unsigned char _byte;
			a_istream.read((char *)&_byte, 1);
			_res[i]=_byte/256.0f;
		}
		return _res;
	}

	tVec c3dsLoader::cm_chunkReaderColorRGBgf (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"������ �������� ���� RGBf ��������� �� ����� "<<std::hex<<a_header.id<<std::endl;
		tVec _res=new tVec4;
		_res[3]=1.0f;
		a_istream.read((char *)&_res, 4*3);
		return _res;
	}

	tVec c3dsLoader::cm_chunkReaderColorRGBgb (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"������ �������� ���� RGBb ��������� �� ����� "<<std::hex<<a_header.id<<std::endl;
		tVec _res=new tVec4;
		_res[3]=1.0f;
		for (int i=0; i<3; i++)
		{
			unsigned char _byte;
			a_istream.read((char *)&_byte, 1);
			_res[i]=_byte/256.0f;
		}
		return _res;
	}

	tVec c3dsLoader::cm_chunkReaderColorUnknown (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"������ ����������� ���� "<<std::hex<<a_header.id<<" � ����� �������� ������"<<std::endl;
		tVec _res=new tVec4;
		_res[3]=1.0f;
		for (int i=0; i<3; _res[i++]=0.0f);
		if (a_header.len<6)
		{
			std::cerr<<"\t������������ ����� �����"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return _res;
	}
	#pragma endregion �������� �����

	#pragma region PercentChunks
	tFloat c3dsLoader::cm_chunkReaderPercent (tistream & a_istream, std::streamoff & a_maxoffset)
	{
		s3dsHeader _header;
		tFloat _res;
		bool _moreThanOne=false;
		while ((a_istream.tellg()<a_maxoffset)&&!(a_istream.fail()||a_istream.eof()||((a_istream>>_header).eof())))
		{
			ptChunkReaderPercent _chunkReaderPercent=cm_getChunkReaderPercent(_header);
			if (_moreThanOne)
			{
				std::cerr<<"����� ��� ���� ���������� ���� ������"<<std::endl;
			}
			_moreThanOne=true;
			_res=(*_chunkReaderPercent)(a_istream, _header);
		}
		return _res;
	}

	ptChunkReaderPercent c3dsLoader::cm_getChunkReaderPercent(s3dsHeader & a_header)
	{
		switch (a_header.id)
		{
		case(chunks::PERCENTi):
			return &cm_chunkReaderPercenti;
		case(chunks::PERCENTf):
			return &cm_chunkReaderPercentf;
		default:
			return &cm_chunkReaderPercentUnknown;
		}
	}

	tFloat c3dsLoader::cm_chunkReaderPercenti (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"������ ���������� ���� Percenti "<<std::hex<<a_header.id<<std::endl;
		unsigned short _temp;
		a_istream.read((char *)&_temp, 2);
		tFloat _res=_temp;
		return _res;
	}

	tFloat c3dsLoader::cm_chunkReaderPercentf (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"������ ���������� ���� Percentf "<<std::hex<<a_header.id<<std::endl;
		tFloat _res;
		a_istream.read((char *)&_res, 4);
		return _res;
	}

	tFloat c3dsLoader::cm_chunkReaderPercentUnknown (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"������ ����������� ���� "<<std::hex<<a_header.id<<" � ����� ���������� ������"<<std::endl;
		tFloat _res=0.0f;
		if (a_header.len<6)
		{
			std::cerr<<"\t������������ ����� �����"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return _res;
	}
	#pragma endregion ���������� �����
}