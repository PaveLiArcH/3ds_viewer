#ifndef __3dsObject_h__
#define __3dsObject_h__

#include <string>
#include "3dsTypes.h"

namespace n3ds
{
	class c3dsObject
	{
		// �������� �������
		std::string cf_name;
		// ������ �������
		bool cf_hidden;
		// �� ����������
		bool cf_notShadowing;
		// �� ����������� (����)
		bool cf_notCast;
		// ������� �����������?!
		bool cf_matte;
		// ������� ������� ���������?!
		bool cf_externalProcessed;
		// ������ ��������� ���������
		tFloat *cf_verticesList;
		// ����� ���������
		tChunkID cf_verticesCount;
		// ������ ��������
		tChunkID *cf_indexList;
		// ����� ��������
		tChunkID cf_indexCount;

	public:
		c3dsObject();
		void SetName(std::string a_name);
		void SetHidden(bool a_hidden);
		void SetNotShadowing(bool a_notShadowing);
		void SetNotCast(bool a_notCast);
		void SetMatte(bool a_matte);
		void SetExternalProcessed(bool a_externalProcessed);
		void SetVerticesList(tFloat *a_verticesList);
		void SetVerticesCount(tChunkID a_verticesCount);
		void SetIndexList(tChunkID *a_indexList);
		void SetIndexCount(tChunkID a_indexCount);
	};
}

#endif