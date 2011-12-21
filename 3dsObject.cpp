#include <math.h>
#include "3dsObject.h"

namespace ns_3ds
{
	c3dsObject::c3dsObject()
	{
		cf_hidden=false;
		cf_notShadowing=false;
		cf_notCast=false;
		cf_matte=false;
		cf_externalProcessed=false;
		cf_verticesList=NULL;
		cf_verticesCount=0;
		cf_indexList=NULL;
		cf_indexCount=0;
		cf_texList=NULL;
		cf_texCount=0;
		/*cf_vertexBuffer=NULL;
		cf_vertexVBO=0;*/
		cf_sphereRadius=0;
		cf_occluderBuffer=NULL;
		cf_buffered=false;
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
		if (cf_verticesCount>0)
		{
			cf_minX=cf_verticesList[0];
			cf_maxX=cf_verticesList[0];
			cf_minY=cf_verticesList[1];
			cf_maxY=cf_verticesList[1];
			cf_minZ=cf_verticesList[2];
			cf_maxZ=cf_verticesList[2];
			for (int i=1; i<cf_verticesCount; i++)
			{
				int _t=3*i;
				cf_maxX=(cf_verticesList[_t]>cf_maxX)?cf_verticesList[_t]:cf_maxX;
				cf_minX=(cf_verticesList[_t]<cf_minX)?cf_verticesList[_t]:cf_minX;
				cf_maxY=(cf_verticesList[_t+1]>cf_maxY)?cf_verticesList[_t+1]:cf_maxY;
				cf_minY=(cf_verticesList[_t+1]<cf_minY)?cf_verticesList[_t+1]:cf_minY;
				cf_maxZ=(cf_verticesList[_t+2]>cf_maxZ)?cf_verticesList[_t+2]:cf_maxZ;
				cf_minZ=(cf_verticesList[_t+2]<cf_minZ)?cf_verticesList[_t+2]:cf_minZ;
			}
			cm_RecalcFrustum(1.0);
			cm_RecalcOccluder(1.0);
		}
		if (!cf_buffered)
		{
			for (stdext::hash_map<std::string, std::vector<tChunkID> *>::iterator _it=cf_faceMaterial.begin(); _it!=cf_faceMaterial.end(); ++_it)
			{
				std::vector<tChunkID> *_faceList=_it->second;
				tChunkID _faceCount=_faceList->size();
				sVertexNormalTex *_temp=new sVertexNormalTex [3*3*_faceCount];
				tFloat _zeros[3]={0.0f, 0.0f, 0.0f};
				int _currVertex=0;
				for(int i=0; i<_faceCount; i++) // faces
				{
					int _face=3*(*_faceList)[i];
					int _tempCurrVertex=_currVertex;
					for (int j=0; j<3; j++) // indexes
					{
						int _vertexNum=cf_indexList[_face+j];
						_temp[_currVertex].SetCoord(&cf_verticesList[3*_vertexNum]);
						if (cf_texList)
						{
							_temp[_currVertex].SetTex(&cf_texList[2*_vertexNum]);
						} else
						{
							_temp[_currVertex].SetTex(_zeros);
						}
						_currVertex++;
					}
					// ���������� �������
					vec3 a0=_temp[_tempCurrVertex].Vec3Coordinate();
					vec3 a1=_temp[_tempCurrVertex+1].Vec3Coordinate();
					vec3 a2=_temp[_tempCurrVertex+2].Vec3Coordinate();
					vec3 a1a0=a0-a1,a1a2=a2-a1;
					vec3 normal=normalize(cross(a1a2,a1a0));
					// ���������� ������� ��� 3 ����� ��������
					_temp[_tempCurrVertex].SetNormal(&normal.x);
					_temp[_tempCurrVertex+1].SetNormal(&normal.x);
					_temp[_tempCurrVertex+2].SetNormal(&normal.x);
				}
				cf_vertexBuffer[_it->first]=_temp;
			}

			if (!c3ds::hasVBO) { return false; } // no VBO
			if (c3ds::newOGL) // OGL>=1.5
			{
				for (stdext::hash_map<std::string, sVertexNormalTex *>::iterator _it=cf_vertexBuffer.begin(); _it!=cf_vertexBuffer.end(); ++_it)
				{
					tUint _vertexVBO=0;
					// ����� ���������
					int _vertexCount=3*3*cf_faceMaterial[_it->first]->size();
					// ��������� ����� ��� ��������� �������
					glGenBuffers(1, &_vertexVBO);
					// �������� ������
					glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
					// �������� � ������������� ������� �������� ������ ��� ��������� �������
					glBufferData(GL_ARRAY_BUFFER, _vertexCount*sizeof(sVertexNormalTex), _it->second, GL_STATIC_DRAW);
					// ���������� ������
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					// ��������� ������ ������ � hash_map
					cf_vertexVBO[_it->first]=_vertexVBO;
				}
				_retVal=true;
			}
			else // old OGL
			{
				for (stdext::hash_map<std::string, sVertexNormalTex *>::iterator _it=cf_vertexBuffer.begin(); _it!=cf_vertexBuffer.end(); ++_it)
				{
					tUint _vertexVBO=0;
					// ����� ���������
					int _vertexCount=3*3*cf_faceMaterial[_it->first]->size();
					// ��������� ����� ��� ��������� �������
					glGenBuffersARB(1, &_vertexVBO);
					// �������� ������
					glBindBufferARB(GL_ARRAY_BUFFER_ARB, _vertexVBO);
					// �������� � ������������� ������� �������� ������ ��� ��������� �������
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, _vertexCount*sizeof(sVertexNormalTex), _it->second, GL_STATIC_DRAW);
					// ���������� ������
					glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
					// ��������� ������ ������ � hash_map
					cf_vertexVBO[_it->first]=_vertexVBO;
				}
				_retVal=true;
			}
		}
		return _retVal;
	}

	bool c3dsObject::cm_Render(c3ds *a_3ds)
	{
		bool _retVal=false;
		if (!cf_hidden)
		{
			glPushMatrix(); // save current matrix
			// ���������� ������� ��������
			glMultMatrixf(a_3ds->cf_scaleMatrix);
			// ��������� �����
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
					for (stdext::hash_map<std::string, tUint>::iterator _it=cf_vertexVBO.begin(); _it!=cf_vertexVBO.end(); ++_it)
					{
						sVertexNormalTex *_vertexBuffer=cf_vertexBuffer[_it->first];
						int _indexCount=3*cf_faceMaterial[_it->first]->size();
						// �������� ������
						glBindBuffer(GL_ARRAY_BUFFER, _it->second);
						// ��������� ��������� �� ������ ������ �� VBO
						glVertexPointer(3, GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)_vertexBuffer[0].sf_coordinate-(char *)_vertexBuffer));
						// ��������� ��������� �� ������ �������� �� VBO
						glNormalPointer(GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)_vertexBuffer[0].sf_normal-(char *)_vertexBuffer));
						// ��������� ������� ���������� ���������
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						// ��������� ���������
						c3dsMaterial *_material=a_3ds->cf_material[_it->first];
						if (_material)
						{
							_material->cm_Use();
						}
						// ��������� ��������� �� ������ ���������� ��������� �� VBO
						//GLvoid * tmpK=(GLvoid *)((char *)indexVertexNormal[i][0].tex-(char *)indexVertexNormal[i]);
						glTexCoordPointer(2, GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)_vertexBuffer[0].sf_tex-(char *)_vertexBuffer));
						// ��������� ������
						glDrawArrays(GL_TRIANGLES, 0, 3*_indexCount);
						glDisable(GL_TEXTURE_2D);
						// ���������� ������� ���������� ���������
						glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					}
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
					for (stdext::hash_map<std::string, tUint>::iterator _it=cf_vertexVBO.begin(); _it!=cf_vertexVBO.end(); ++_it)
					{
						sVertexNormalTex *_vertexBuffer=cf_vertexBuffer[_it->first];
						int _indexCount=3*cf_faceMaterial[_it->first]->size();
						// �������� ������
						glBindBufferARB(GL_ARRAY_BUFFER, _it->second);
						// ��������� ��������� �� ������ ������ �� VBO
						glVertexPointer(3, GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)_vertexBuffer[0].sf_coordinate-(char *)_vertexBuffer));
						// ��������� ��������� �� ������ �������� �� VBO
						glNormalPointer(GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)_vertexBuffer[0].sf_normal-(char *)_vertexBuffer));
						// ��������� ������� ���������� ���������
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						// ��������� ���������
						c3dsMaterial *_material=a_3ds->cf_material[_it->first];
						if (_material)
						{
							_material->cm_Use();
						}
						// ��������� ��������� �� ������ ���������� ��������� �� VBO
						//GLvoid * tmpK=(GLvoid *)((char *)indexVertexNormal[i][0].tex-(char *)indexVertexNormal[i]);
						glTexCoordPointer(2, GL_FLOAT, sizeof(sVertexNormalTex), (GLvoid *)((char *)_vertexBuffer[0].sf_tex-(char *)_vertexBuffer));
						// ��������� ������
						glDrawArrays(GL_TRIANGLES, 0, 3*_indexCount);
						glDisable(GL_TEXTURE_2D);
						// ���������� ������� ���������� ���������
						glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					}
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
				for (stdext::hash_map<std::string, sVertexNormalTex *>::iterator _it=cf_vertexBuffer.begin(); _it!=cf_vertexBuffer.end(); ++_it)
				{
					sVertexNormalTex *_vertexBuffer=cf_vertexBuffer[_it->first];
					int _indexCount=3*cf_faceMaterial[_it->first]->size();
					// ��������� ��������� �� ������ ������
					glVertexPointer(3, GL_FLOAT, sizeof(sVertexNormalTex), _vertexBuffer[0].sf_coordinate);
					// ��������� ��������� �� ������ ��������
					glNormalPointer(GL_FLOAT, sizeof(sVertexNormalTex), _vertexBuffer[0].sf_normal);
					// ��������� ������� ���������� ���������
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					// ��������� ���������
					c3dsMaterial *_material=a_3ds->cf_material[_it->first];
					if (_material)
					{
						_material->cm_Use();
					}
					// ��������� ��������� �� ������ ���������� ��������� �� VBO
					//GLvoid * tmpK=(GLvoid *)((char *)indexVertexNormal[i][0].tex-(char *)indexVertexNormal[i]);
					glTexCoordPointer(2, GL_FLOAT, sizeof(sVertexNormalTex), _vertexBuffer[0].sf_tex);
					// ��������� ������
					glDrawArrays(GL_TRIANGLES, 0, 3*_indexCount);
					glDisable(GL_TEXTURE_2D);
					// ���������� ������� ���������� ���������
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				_retVal=true;
				//glPopMatrix(); // restore matrix
			}
			glPopMatrix(); // restore matrix
		}
		return _retVal;
	}
	
	bool c3dsObject::cm_FrustumTest(c3ds *a_3ds)
	{
		float _distance;
		tFrustum *_frustum=a_3ds->cm_GetCamera()->GetFrustum();
		for(int i=0;i<6;i++)
		{
			_distance=(*_frustum)[i].x*cf_sphere.x+(*_frustum)[i].y*cf_sphere.y+(*_frustum)[i].z*cf_sphere.z+(*_frustum)[i].w;
			if(_distance<-cf_sphereRadius)
			{
				cf_distance=0.0f;
				return false;
			}
		}
		cf_distance=_distance+cf_sphereRadius;
		return true;
	}

	void c3dsObject::cm_ScaleChanged(tDouble a_newScale)
	{
		cm_RecalcFrustum(a_newScale);
		cm_RecalcOccluder(a_newScale);
	}

	void c3dsObject::cm_RecalcFrustum(tDouble a_newScale)
	{
		cf_sphere.x=(cf_maxX+cf_minX)/2;
		cf_sphere.y=(cf_maxY+cf_minY)/2;
		cf_sphere.z=(cf_maxZ+cf_minZ)/2;
		cf_sphere*=a_newScale;
		glm::vec3 _v(cf_minX, cf_minY, cf_minZ);
		_v*=a_newScale;
		cf_sphereRadius=glm::length(cf_sphere-_v);
	}

	void c3dsObject::cm_RecalcOccluder(tDouble a_newScale)
	{
		tFloat _minX=cf_minX*a_newScale,_maxX=cf_maxX*a_newScale;
		tFloat _minY=cf_minY*a_newScale,_maxY=cf_maxY*a_newScale;
		tFloat _minZ=cf_minZ*a_newScale,_maxZ=cf_maxZ*a_newScale;

		sVertex _occluder[8]={
			sVertex(_minX, _minY, _maxZ),
			sVertex(_minX, _maxY, _maxZ),
			sVertex(_minX, _maxY, _minZ),
			sVertex(_minX, _minY, _minZ),

			sVertex(_maxX, _minY, _maxZ),
			sVertex(_maxX, _maxY, _maxZ),
			sVertex(_maxX, _maxY, _minZ),
			sVertex(_maxX, _minY, _minZ), 
		};

		tUint _indexes[]={
			0, 4, 5, 1,
			3, 0, 1, 2,
			7, 3, 2, 6,
			4, 7, 6, 5,
			4, 0, 3, 7,
			1, 5, 6, 2,
		};

		if (cf_occluderBuffer)
		{
			delete []cf_occluderBuffer;
		} else
		{
			glGenBuffers(1, &cf_occluderVBO);
		}
		cf_occluderBuffer=new sVertex[24];
		for(int i=0;i<24;i++) // read indexes
		{
			cf_occluderBuffer[i]=_occluder[_indexes[i]];
		}

		// �������� ������
		glBindBuffer(GL_ARRAY_BUFFER, cf_occluderVBO);
		// �������� � ������������� ������� �������� ������ ��� ��������� �������
		glBufferData(GL_ARRAY_BUFFER, 24*sizeof(sVertex), cf_occluderBuffer, GL_STATIC_DRAW);
		// ���������� ������
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bool c3dsObject::cm_OcclusionTest(c3ds *a_3ds)
	{
		return true;
	}

	bool operator<(c3dsObject &a_object, c3dsObject &a_otherObject)
	{
		return (a_object.cf_distance<a_otherObject.cf_distance);
	}

	bool Compare3dsObjects(c3dsObject *a_object, c3dsObject *a_otherObject)
	{
		return ((*a_object)<(*a_otherObject));
	}
}