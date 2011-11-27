#include "stdafx.h"
#include "3dsTypes.h"
#include "3dsLoader.h"
#include "3dsChunks.h"

namespace ns_3ds
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
		case(chunks::OBJECT):
			return &cm_chunkReaderObjectBlock;
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
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMain (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден главный чанк "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _pos=a_istream.tellg();
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
		std::cerr<<"Найден чанк меша "<<std::hex<<a_header.id<<std::endl;
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMeshVersion (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк версии меша "<<std::hex<<a_header.id<<std::endl;
		t3dsVersion _version;
		a_istream.read((char *)&_version, 4);
		a_object.cf_meshVersion=_version;
		std::cerr<<"\tВерсия меша "<<_version<<std::endl;
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialBlock (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк блока материала "<<std::hex<<a_header.id<<std::endl;
		c3dsMaterial *_material=new c3dsMaterial();
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		bool _res=cm_chunkReaderMaterial(a_istream, _maxoff, _material);
		a_object.cf_material[_material->GetMaterialName()]=_material;
		return true;
	}

	bool c3dsLoader::cm_chunkReaderOneUnit (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк величины единицы "<<std::hex<<a_header.id<<std::endl;
		tFloat _unit;
		a_istream.read((char *)&_unit, 4);
		a_object.cf_unit=_unit;
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectBlock (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк блока объекта "<<std::hex<<a_header.id<<std::endl;
		a_object.cf_object.push_back(new c3dsObject);
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		char _c;
		string _temp;
		while (a_istream.read(&_c,1)&&(!a_istream.eof())&&_c)
		{
			_temp+=_c;
		}
		a_object.cf_object.back()->SetName(_temp);
		bool _res=cm_chunkReaderObject(a_istream, _maxoff, a_object);
		return true;
	}

	#pragma region ObjectChunks
	bool c3dsLoader::cm_chunkReaderObject (tistream & a_istream, std::streamoff & a_maxoffset, c3ds & a_object)
	{
		s3dsHeader _header;
		bool _res;
		while ((a_istream.tellg()<a_maxoffset)&&!(a_istream.fail()||a_istream.eof()||((a_istream>>_header).eof())))
		{
			ptChunkReaderObject _chunkReaderObject=cm_getChunkReaderObject(_header);
			_res=(*_chunkReaderObject)(a_istream, _header, a_object);
		}
		return true;
	}

	ptChunkReaderObject c3dsLoader::cm_getChunkReaderObject(s3dsHeader & a_header)
	{
		switch (a_header.id)
		{
		case(chunks::HIDDEN):
			return &cm_chunkReaderObjectHidden;
		case(chunks::NOTSHADOWING):
			return &cm_chunkReaderObjectNotShadowing;
		case(chunks::NOTCAST):
			return &cm_chunkReaderObjectNotCast;
		case(chunks::MATTE):
			return &cm_chunkReaderObjectMatte;
		case(chunks::EXTERNALPROCESS):
			return &cm_chunkReaderObjectExternalProcessed;
		case(chunks::TRIMESH):
			return &cm_chunkReaderObjectTrimeshBlock;
		default:
			return &cm_chunkReaderObjectUnknown;
		}
	}

	bool c3dsLoader::cm_chunkReaderObjectHidden (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк скрытости объекта "<<std::hex<<a_header.id<<std::endl;
		a_object.cf_object.back()->SetHidden(true);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectNotShadowing (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк незатеняемости объекта "<<std::hex<<a_header.id<<std::endl;
		a_object.cf_object.back()->SetNotShadowing(true);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectNotCast (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк нетенеобразующего объекта "<<std::hex<<a_header.id<<std::endl;
		a_object.cf_object.back()->SetNotCast(true);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectMatte (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк матового объекта "<<std::hex<<a_header.id<<std::endl;
		a_object.cf_object.back()->SetMatte(true);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectExternalProcessed (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк внешнеобрабатываемого объекта "<<std::hex<<a_header.id<<std::endl;
		a_object.cf_object.back()->SetExternalProcessed(true);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectTrimeshBlock (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк тримеш блока объекта "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		bool _res=cm_chunkReaderObjectTrimesh(a_istream, _maxoff, a_object);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден неизвестный чанк "<<std::hex<<a_header.id<<" в блоке объектных чанков"<<std::endl;
		if (a_header.len<6)
		{
			std::cerr<<"\tНекорректная длина чанка"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return true;
	}
	#pragma endregion Объектные чанки
	
	#pragma region ObjectTrimeshChunks
	bool c3dsLoader::cm_chunkReaderObjectTrimesh (tistream & a_istream, std::streamoff & a_maxoffset, c3ds & a_object)
	{
		s3dsHeader _header;
		bool _res;
		while ((a_istream.tellg()<a_maxoffset)&&!(a_istream.fail()||a_istream.eof()||((a_istream>>_header).eof())))
		{
			ptChunkReaderObjectTrimesh _chunkReaderObjectTrimesh=cm_getChunkReaderObjectTrimesh(_header);
			_res=(*_chunkReaderObjectTrimesh)(a_istream, _header, a_object);
		}
		return true;
	}

	ptChunkReaderObjectTrimesh c3dsLoader::cm_getChunkReaderObjectTrimesh(s3dsHeader & a_header)
	{
		switch (a_header.id)
		{
		case (chunks::VERTEXLIST):
			return &cm_chunkReaderObjectTrimeshVertexList;
		case (chunks::FACESLIST):
			return &cm_chunkReaderObjectTrimeshFaceList;
		case (chunks::MAPPINGCOORDS):
			return &cm_chunkReaderObjectTrimeshMappingList;
		default:
			return &cm_chunkReaderObjectTrimeshUnknown;
		}
	}

	bool c3dsLoader::cm_chunkReaderObjectTrimeshVertexList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк координат вертексов объекта "<<std::hex<<a_header.id<<std::endl;
		tChunkID _vertexCount=0;
		a_istream.read((char *)&_vertexCount, 2);
		tFloat *_vertexList=new tFloat[_vertexCount*3];
		for (int i=0; i<_vertexCount; i++)
		{
			a_istream.read((char*)&(_vertexList[3*i]),4);
			a_istream.read((char*)&(_vertexList[3*i+2]),4);
			a_istream.read((char*)&(_vertexList[3*i+1]),4);
		}
		a_object.cf_object.back()->SetVerticesCount(_vertexCount);
		a_object.cf_object.back()->SetVerticesList(_vertexList);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectTrimeshFaceList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк индексов вертексов объекта "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tChunkID _indexCount=0;
		a_istream.read((char *)&_indexCount, 2);
		tChunkID *_indexList=new tChunkID[_indexCount*3];
		for (int i=0; i<_indexCount; i++)
		{
			a_istream.read((char*)&(_indexList[3*i]),2);
			a_istream.read((char*)&(_indexList[3*i+2]),2);
			a_istream.read((char*)&(_indexList[3*i+1]),2);
			a_istream.ignore(2); //ToDo: прочитать флаги видимости
		}
		a_object.cf_object.back()->SetIndexCount(_indexCount);
		a_object.cf_object.back()->SetIndexList(_indexList);
		bool _res=cm_chunkReaderObjectTrimeshFace(a_istream, _maxoff, a_object);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectTrimeshMappingList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк текстурных координат вертексов объекта "<<std::hex<<a_header.id<<std::endl;
		tChunkID _texCount=0;
		a_istream.read((char *)&_texCount, 2);
		tFloat *_texList=new tFloat[_texCount*2];
		for (int i=0; i<_texCount; i++)
		{
			a_istream.read((char*)&(_texList[2*i]),4);
			a_istream.read((char*)&(_texList[2*i+1]),4);
		}
		a_object.cf_object.back()->SetTexCount(_texCount);
		a_object.cf_object.back()->SetTexList(_texList);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectTrimeshUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден неизвестный чанк "<<std::hex<<a_header.id<<" в блоке объектных тримеш-чанков"<<std::endl;
		if (a_header.len<6)
		{
			std::cerr<<"\tНекорректная длина чанка"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return true;
	}
	#pragma endregion Объектные тримеш-чанки

	#pragma region ObjectTrimeshFaceChunks
	bool c3dsLoader::cm_chunkReaderObjectTrimeshFace (tistream & a_istream, std::streamoff & a_maxoffset, c3ds & a_object)
	{
		s3dsHeader _header;
		bool _res;
		while ((a_istream.tellg()<a_maxoffset)&&!(a_istream.fail()||a_istream.eof()||((a_istream>>_header).eof())))
		{
			ptChunkReaderObjectTrimeshFace _chunkReaderObjectTrimeshFace=cm_getChunkReaderObjectTrimeshFace(_header);
			_res=(*_chunkReaderObjectTrimeshFace)(a_istream, _header, a_object);
		}
		return true;
	}

	ptChunkReaderObjectTrimeshFace c3dsLoader::cm_getChunkReaderObjectTrimeshFace(s3dsHeader & a_header)
	{
		switch (a_header.id)
		{
		case (chunks::MATERIALLIST):
			return &cm_chunkReaderObjectTrimeshFaceMaterialList;
		case (chunks::SMOOTHLIST):
			return &cm_chunkReaderObjectTrimeshFaceSmoothList;
		default:
			return &cm_chunkReaderObjectTrimeshFaceUnknown;
		}
	}

	bool c3dsLoader::cm_chunkReaderObjectTrimeshFaceMaterialList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден чанк материалов сторон объекта "<<std::hex<<a_header.id<<std::endl;
		char _c;
		string _materialName;
		while (a_istream.read(&_c,1)&&(!a_istream.eof())&&_c)
		{
			_materialName+=_c;
		}
		tChunkID _materialFaceCount=0;
		a_istream.read((char *)&_materialFaceCount, 2);
		vector<tChunkID> *_faceList=new vector<tChunkID>(_materialFaceCount);
		for (int i=0; i<_materialFaceCount; i++)
		{
			a_istream.read((char*)&((*_faceList)[i]),2);
		}
		a_object.cf_object.back()->SetFaceMaterial(_materialName,_faceList);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectTrimeshFaceSmoothList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		//ToDo: написать корректную обработку
		std::cerr<<"Найден чанк сглаживающей группы сторон объекта "<<std::hex<<a_header.id<<std::endl;
		if (a_header.len<6)
		{
			std::cerr<<"\tНекорректная длина чанка"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderObjectTrimeshFaceUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object)
	{
		std::cerr<<"Найден неизвестный чанк "<<std::hex<<a_header.id<<" в блоке описаний сторон объектных тримеш-чанков"<<std::endl;
		if (a_header.len<6)
		{
			std::cerr<<"\tНекорректная длина чанка"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return true;
	}
	#pragma endregion Объектные тримеш-чанки сторон

	#pragma region MaterialChunks
	bool c3dsLoader::cm_chunkReaderMaterial (tistream & a_istream, std::streamoff & a_maxoffset, c3dsMaterial * a_material)
	{
		s3dsHeader _header;
		bool _res;
		while ((a_istream.tellg()<a_maxoffset)&&!(a_istream.fail()||a_istream.eof()||((a_istream>>_header).eof())))
		{
			ptChunkReaderMaterial _chunkReaderMaterial=cm_getChunkReaderMaterial(_header);
			_res=(*_chunkReaderMaterial)(a_istream, _header, a_material);
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
		case(chunks::TEXTUREMAP1):
			return &cm_chunkReaderMaterialTextureMap1;
		default:
			return &cm_chunkReaderMaterialUnknown;
		}
	}

	bool c3dsLoader::cm_chunkReaderMaterialName (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк названия материала "<<std::hex<<a_header.id<<std::endl;
		char _c;
		string _temp;
		while (a_istream.read(&_c,1)&&(!a_istream.eof())&&_c)
		{
			_temp+=_c;
		}
		a_material->SetMaterialName(_temp);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialAmbient (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк амбиентной составляющей материала "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tVec _color=cm_chunkReaderColor(a_istream, _maxoff);
		a_material->SetAmbient(_color);
		delete []_color;
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialDiffuse (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк диффузной составляющей материала "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tVec _color=cm_chunkReaderColor(a_istream, _maxoff);
		a_material->SetDiffuse(_color);
		delete []_color;
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialSpecular (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк спекулярной составляющей материала "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tVec _color=cm_chunkReaderColor(a_istream, _maxoff);
		a_material->SetSpecular(_color);
		delete []_color;
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialShininess (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк блесковой составляющей материала "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_material->SetShininessPercent(_percent);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialShininessStrength (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк силы блесковой составляющей материала "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_material->SetShininessStrengthPercent(_percent);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialTransparency (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк прозрачной составляющей материала "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_material->SetTransparencyPercent(_percent);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialTransparencyFalloff (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк спада прозрачной составляющей материала "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_material->SetTransparencyFalloffPercent(_percent);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialReflectionBlur (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк размытости отражения материала "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		tFloat _percent=cm_chunkReaderPercent(a_istream, _maxoff);
		a_material->SetReflectionBlurPercent(_percent);
		return true;
	}
	
	bool c3dsLoader::cm_chunkReaderMaterialTextureMap1 (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден чанк текстурной карты материала "<<std::hex<<a_header.id<<std::endl;
		std::streamoff _maxoff=a_istream.tellg();
		_maxoff=_maxoff+(a_header.len-6);
		c3dsMap *_map=cm_chunkReaderMap(a_istream, _maxoff);
		c3dsTextureDevIL *_texture=new c3dsTextureDevIL();
		_texture->load(_map);
		delete _map;
		a_material->SetTextureMap(_texture);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMaterialUnknown (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material)
	{
		std::cerr<<"Найден неизвестный чанк "<<std::hex<<a_header.id<<" в блоке материальных чанков"<<std::endl;
		if (a_header.len<6)
		{
			std::cerr<<"\tНекорректная длина чанка"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return true;
	}
	#pragma endregion Материальные чанки

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
				std::cerr<<"Более чем один цветовой чанк подряд"<<std::endl;
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
		std::cerr<<"Найден цветовой чанк RGBf "<<std::hex<<a_header.id<<std::endl;
		tVec _res=new tVec4;
		_res[3]=1.0f;
		a_istream.read((char *)&_res, 4*3);
		return _res;
	}

	tVec c3dsLoader::cm_chunkReaderColorRGBb (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"Найден цветовой чанк RGBb "<<std::hex<<a_header.id<<std::endl;
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
		std::cerr<<"Найден цветовой чанк RGBf смещенный по гамме "<<std::hex<<a_header.id<<std::endl;
		tVec _res=new tVec4;
		_res[3]=1.0f;
		a_istream.read((char *)&_res, 4*3);
		return _res;
	}

	tVec c3dsLoader::cm_chunkReaderColorRGBgb (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"Найден цветовой чанк RGBb смещенный по гамме "<<std::hex<<a_header.id<<std::endl;
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
		std::cerr<<"Найден неизвестный чанк "<<std::hex<<a_header.id<<" в блоке цветовых чанков"<<std::endl;
		tVec _res=new tVec4;
		_res[3]=1.0f;
		for (int i=0; i<3; _res[i++]=0.0f);
		if (a_header.len<6)
		{
			std::cerr<<"\tНекорректная длина чанка"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return _res;
	}
	#pragma endregion Цветовые чанки

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
				std::cerr<<"Более чем один процентный чанк подряд"<<std::endl;
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
		std::cerr<<"Найден процентный чанк Percenti "<<std::hex<<a_header.id<<std::endl;
		unsigned short _temp;
		a_istream.read((char *)&_temp, 2);
		tFloat _res=_temp;
		return _res;
	}

	tFloat c3dsLoader::cm_chunkReaderPercentf (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"Найден процентный чанк Percentf "<<std::hex<<a_header.id<<std::endl;
		tFloat _res;
		a_istream.read((char *)&_res, 4);
		return _res;
	}

	tFloat c3dsLoader::cm_chunkReaderPercentUnknown (tistream & a_istream, s3dsHeader & a_header)
	{
		std::cerr<<"Найден неизвестный чанк "<<std::hex<<a_header.id<<" в блоке процентных чанков"<<std::endl;
		tFloat _res=0.0f;
		if (a_header.len<6)
		{
			std::cerr<<"\tНекорректная длина чанка"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return _res;
	}
	#pragma endregion Процентные чанки

	#pragma region MapChunks
	c3dsMap * c3dsLoader::cm_chunkReaderMap (tistream & a_istream, std::streamoff & a_maxoffset)
	{
		s3dsHeader _header;
		bool _res;
		c3dsMap *_retMap=new c3dsMap();
		while ((a_istream.tellg()<a_maxoffset)&&!(a_istream.fail()||a_istream.eof()||((a_istream>>_header).eof())))
		{
			ptChunkReaderMap _chunkReaderMap=cm_getChunkReaderMap(_header);
			_res=(*_chunkReaderMap)(a_istream, _header, _retMap);
		}
		return _retMap;
	}

	ptChunkReaderMap c3dsLoader::cm_getChunkReaderMap(s3dsHeader & a_header)
	{
		switch (a_header.id)
		{
		case (chunks::MAPFILENAME):
			return &cm_chunkReaderMapFile;
		default:
			return &cm_chunkReaderMapUnknown;
		}
	}

	bool c3dsLoader::cm_chunkReaderMapFile (tistream & a_istream, s3dsHeader & a_header, c3dsMap *a_map)
	{
		std::cerr<<"Найден map чанк имени файла "<<std::hex<<a_header.id<<std::endl;
		char _c;
		string _temp;
		while (a_istream.read(&_c,1)&&(!a_istream.eof())&&_c)
		{
			_temp+=_c;
		}
		a_map->SetMapFile(_temp);
		return true;
	}

	bool c3dsLoader::cm_chunkReaderMapUnknown (tistream & a_istream, s3dsHeader & a_header, c3dsMap *a_map)
	{
		std::cerr<<"Найден неизвестный чанк "<<std::hex<<a_header.id<<" в блоке map чанков"<<std::endl;
		tFloat _res=0.0f;
		if (a_header.len<6)
		{
			std::cerr<<"\tНекорректная длина чанка"<<std::endl;
			// TODO throw an exception
			return false;
		}
		a_istream.seekg(a_header.len-6, std::ios::cur);
		return _res;
	}
	#pragma endregion Map чанки
}