#include <math.h>
#include "3dsObject.h"

namespace ns_3ds
{
	c3dsObject::c3dsObject()
	{
		cf_verticesList=NULL;
		cf_verticesCount=0;
		cf_indexList=NULL;
		cf_indexCount=0;
		cf_texList=NULL;
		cf_texCount=0;
	}

	c3dsObject::~c3dsObject()
	{
		if (cf_verticesList)
		{
			delete []cf_verticesList;
		}
		if (cf_indexList)
		{
			delete []cf_indexList;
		}
		if (cf_texList)
		{
			delete []cf_texList;
		}
		if (cf_faceMaterial.size()>0)
		{
			stdext::hash_map<std::string, std::vector<tChunkID> *>::iterator _it;
			_it=cf_faceMaterial.begin();
			for (; _it!=cf_faceMaterial.end(); _it++)
			{
				if (_it->second)
				{
					_it->second->clear();
					delete (_it->second);
				}
			}
		}
	}

	void c3dsObject::SetName(std::string a_name)
	{
		//ToDo: сделать проверку на уже заданное имя
		cf_name=a_name;
	}

	void c3dsObject::SetHidden(bool a_hidden)
	{
		cf_hidden=a_hidden;
	}

	void c3dsObject::SetNotShadowing(bool a_notShadowing)
	{
		cf_notShadowing=a_notShadowing;
	}

	void c3dsObject::SetNotCast(bool a_notCast)
	{
		cf_notCast=a_notCast;
	}

	void c3dsObject::SetMatte(bool a_matte)
	{
		cf_matte=a_matte;
	}

	void c3dsObject::SetExternalProcessed(bool a_externalProcessed)
	{
		cf_externalProcessed=a_externalProcessed;
	}

	void c3dsObject::SetVerticesList(tFloat *a_verticesList)
	{
		//ToDo: сделать проверку на уже заданный список вертексов
		cf_verticesList=a_verticesList;
	}

	void c3dsObject::SetVerticesCount(tChunkID a_verticesCount)
	{
		//ToDo: сделать проверку на уже заданный список вертексов
		cf_verticesCount=a_verticesCount;
	}

	void c3dsObject::SetIndexList(tChunkID *a_indexList)
	{
		//ToDo: сделать проверку на уже заданный список индексов
		cf_indexList=a_indexList;
	}

	void c3dsObject::SetIndexCount(tChunkID a_indexCount)
	{
		//ToDo: сделать проверку на уже заданный список индексов
		cf_indexCount=a_indexCount;
	}

	void c3dsObject::SetFaceMaterial(std::string a_materialName, std::vector<tChunkID> *a_faces)
	{
		//ToDo: сделать проверку на уже заданный материал
		cf_faceMaterial[a_materialName]=a_faces;
	}

	void c3dsObject::SetTexList(tFloat *a_texList)
	{
		//ToDo: сделать проверку на уже заданные текстурные координаты вертексов
		cf_texList=a_texList;
	}

	void c3dsObject::SetTexCount(tChunkID a_texCount)
	{
		//ToDo: сделать проверку на уже заданные текстурные координаты вертексов
		cf_texCount=a_texCount;
	}
}