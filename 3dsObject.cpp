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
		cf_vertexBuffer=NULL;
		cf_vertexVBO=0;
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
		//ToDo: ������� �������� �� ��� �������� ���
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
		//ToDo: ������� �������� �� ��� �������� ������ ���������
		cf_verticesList=a_verticesList;
	}

	void c3dsObject::SetVerticesCount(tChunkID a_verticesCount)
	{
		//ToDo: ������� �������� �� ��� �������� ������ ���������
		cf_verticesCount=a_verticesCount;
	}

	void c3dsObject::SetIndexList(tChunkID *a_indexList)
	{
		//ToDo: ������� �������� �� ��� �������� ������ ��������
		cf_indexList=a_indexList;
	}

	void c3dsObject::SetIndexCount(tChunkID a_indexCount)
	{
		//ToDo: ������� �������� �� ��� �������� ������ ��������
		cf_indexCount=a_indexCount;
	}

	void c3dsObject::SetFaceMaterial(std::string a_materialName, std::vector<tChunkID> *a_faces)
	{
		//ToDo: ������� �������� �� ��� �������� ��������
		cf_faceMaterial[a_materialName]=a_faces;
	}

	void c3dsObject::SetTexList(tFloat *a_texList)
	{
		//ToDo: ������� �������� �� ��� �������� ���������� ���������� ���������
		cf_texList=a_texList;
	}

	void c3dsObject::SetTexCount(tChunkID a_texCount)
	{
		//ToDo: ������� �������� �� ��� �������� ���������� ���������� ���������
		cf_texCount=a_texCount;
	}

	bool c3dsObject::cm_Buffer(c3ds *a_3ds)
	{
		bool _retVal=false;
		if (!cf_vertexBuffer)
		{
			cf_vertexBuffer=new sVertexNormalTex [3*cf_indexCount];
			for(int i=0;i<cf_indexCount;i++) // read indexes
			{
				// ������ �������� � ���������� ������
				for (int j=0; j<3; j++)
				{
					int _num=3*i+j;
					cf_vertexBuffer[_num].SetCoord(&cf_verticesList[3*cf_indexList[_num]]);
					cf_vertexBuffer[_num].SetTex(&cf_texList[2*cf_indexList[_num]]);
				}
				// ���������� �������
				vec3 a0=cf_vertexBuffer[3*i].Vec3Coordinate();
				vec3 a1=cf_vertexBuffer[3*i+1].Vec3Coordinate();
				vec3 a2=cf_vertexBuffer[3*i+2].Vec3Coordinate();
				vec3 a1a0=a0-a1,a1a2=a2-a1;
				vec3 normal=normalize(cross(a1a2,a1a0));
				// ���������� ������� ��� 3 ����� ��������
				cf_vertexBuffer[3*i].SetNormal(&normal.x);
				cf_vertexBuffer[3*i+1].SetNormal(&normal.x);
				cf_vertexBuffer[3*i+2].SetNormal(&normal.x);
			}

			if (!c3ds::hasVBO) { return false; } // no VBO
			if (c3ds::newOGL) // OGL>=1.5
			{
				// ��������� ����� ��� ��������� �������
				glGenBuffers(1, &cf_vertexVBO);
				// �������� ������
				glBindBuffer(GL_ARRAY_BUFFER, cf_vertexVBO);
				// �������� � ������������� ������� �������� ������ ��� ��������� �������
				glBufferData(GL_ARRAY_BUFFER, cf_indexCount*3*sizeof(sVertexNormalTex), cf_vertexBuffer, GL_STATIC_DRAW);
				// ���������� ������
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				_retVal=true;
			}
			else // old OGL
			{
				// ��������� ����� ��� ��������� �������
				glGenBuffersARB(1, &cf_vertexVBO);
				// �������� ������
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cf_vertexVBO);
				// �������� � ������������� ������� �������� ������ ��� ��������� �������
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, cf_indexCount*3*sizeof(sVertexNormalTex), cf_vertexBuffer, GL_STATIC_DRAW);
				// ���������� ������
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
				_retVal=true;
			}
		}
		return _retVal;
	}

	bool c3dsObject::cm_Render(c3ds *a_3ds)
	{
		bool _retVal=false;
		glPushMatrix(); // save current matrix
		//// ���������� ������� ��������
		//glMultMatrixf(directionMatrix);
		//// ��������� �����
		//glColor3f(position.color[0],position.color[1],position.color[2]);
		if (c3ds::hasVBO) // has VBO
		{
			if (c3ds::newOGL) // OGL>=1.5
			{
				// ��������� ������� ������
				glEnableClientState(GL_VERTEX_ARRAY);
				// ��������� ������� ��������
				glEnableClientState(GL_NORMAL_ARRAY);
				//glPushMatrix(); // save current matrix
				//glMultMatrixf(localMatrix[i]);
				// �������� ������
				glBindBuffer(GL_ARRAY_BUFFER, cf_vertexVBO);
				// ��������� ��������� �� ������ ������ �� VBO
				glVertexPointer(3, GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)cf_vertexBuffer[0].sf_coordinate-(char *)cf_vertexBuffer));
				// ��������� ��������� �� ������ �������� �� VBO
				glNormalPointer(GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)cf_vertexBuffer[0].sf_normal-(char *)cf_vertexBuffer));
				
				// ��������� ������� ���������� ���������
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				// �������� ��������
				//tex->bind();
				if (cf_faceMaterial.size()>0)
				{
					std::string _materialName=cf_faceMaterial.begin()->first;
					c3dsMaterial *_material=a_3ds->cf_material.at(_materialName);
					if (_material)
					{
						_material->cm_Use();
					}
				}
				// ��������� ��������� �� ������ ���������� ��������� �� VBO
				//GLvoid * tmpK=(GLvoid *)((char *)indexVertexNormal[i][0].tex-(char *)indexVertexNormal[i]);
				glTexCoordPointer(2, GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)cf_vertexBuffer[0].sf_tex-(char *)cf_vertexBuffer));
				// ��������� ������
				glDrawArrays(GL_TRIANGLES, 0, 3*cf_indexCount);
				glDisable(GL_TEXTURE_2D);
				// ���������� ������� ���������� ���������
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				// ���������� ������� ������
				glDisableClientState(GL_VERTEX_ARRAY);
				// ���������� ������� ��������
				glDisableClientState(GL_NORMAL_ARRAY);
				// ���������� ������
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// ���������� ������
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				_retVal=true;
				//glPopMatrix(); // restore matrix
			} else // old OGL
			{
				// ��������� ������� ������
				glEnableClientState(GL_VERTEX_ARRAY);
				// ��������� ������� ��������
				glEnableClientState(GL_NORMAL_ARRAY);
				//glPushMatrix(); // save current matrix
				//glMultMatrixf(localMatrix[i]);
				// �������� ������
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cf_vertexVBO);
				// ��������� ��������� �� ������ ������ �� VBO
				glVertexPointer(3, GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)cf_vertexBuffer[0].sf_coordinate-(char *)cf_vertexBuffer));
				// ��������� ��������� �� ������ �������� �� VBO
				glNormalPointer(GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)cf_vertexBuffer[0].sf_normal-(char *)cf_vertexBuffer));
				// ��������� ������
				glDrawArrays(GL_TRIANGLES, 0, 3*cf_indexCount);
				// ���������� ������� ������
				glDisableClientState(GL_VERTEX_ARRAY);
				// ���������� ������� ��������
				glDisableClientState(GL_NORMAL_ARRAY);
				// ���������� ������
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
				// ���������� ������
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
				_retVal=true;
				//glPopMatrix(); // restore matrix
			}
		} else // no VBO
		{
			//glPushMatrix(); // save current matrix
			//glMultMatrixf(localMatrix[j]);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(sVertexNormalTex), cf_vertexBuffer[0].sf_coordinate);
			glNormalPointer(GL_FLOAT, sizeof(sVertexNormalTex), cf_vertexBuffer[0].sf_normal);
			glDrawArrays(GL_TRIANGLES, 0, 3*cf_indexCount);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			_retVal=true;
			//glPopMatrix(); // restore matrix
		}
		glPopMatrix(); // restore matrix
		return _retVal;
	}
}