#include <fstream>
#include "stdafx.h"
#include "object3ds.h"
#include "3ds_info.h"

using namespace std;

bool object3DS::newOGL=false;
bool object3DS::hasVBO=false;

void object3DS::checkExtensions()
{
	object3DS::newOGL=(atof((const char *)glGetString(GL_VERSION))>=1.5f); // check OGL version
	object3DS::hasVBO=glutExtensionSupported("GL_ARB_vertex_buffer_object")!=0; // check if VBO supported
}

// ����������� ������ ������� � VBO
void object3DS::buffer()
{
	if (!hasVBO) { return; } // no VBO
	if (newOGL) // OGL>=1.5
	{
		for (unsigned int i=0; i<indexCount.size(); i++)
		{
			GLuint vBO;
			// ��������� ����� ��� ��������� �������
			glGenBuffers(1, &vBO);
			// �������� ������
			glBindBuffer(GL_ARRAY_BUFFER, vBO);
			// �������� � ������������� ������� �������� ������ ��� ��������� �������
			glBufferData(GL_ARRAY_BUFFER, (indexCount[i])*3*sizeof(vertexNormalTex), indexVertexNormal[i], GL_STATIC_DRAW);
			// ���������� ������
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			vertexVBO.push_back(vBO);
		}
	}
	else // old OGL
	{
		for (unsigned int i=0; i<indexCount.size(); i++)
		{
			GLuint vBO;
			// ��������� ����� ��� ��������� �������
			glGenBuffersARB(1, &vBO);
			// �������� ������
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vBO);
			// �������� � ������������� ������� �������� ������ ��� ��������� �������
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, (indexCount[i])*3*sizeof(vertexNormalTex), indexVertexNormal[i], GL_STATIC_DRAW);
			// ���������� ������
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
			vertexVBO.push_back(vBO);
		}
	}
}

void object3DS::init()
{
	if (directionMatrix)
	{
		delete []directionMatrix;
	}
	// ������� ��������
	directionMatrix=new GLfloat[16];
	for (int j=0; j<4; j++)
	{
		for (int i=0; i<4; i++)
		{
			directionMatrix[j*4+i]=0; // set to zero
		}
		directionMatrix[j*4+j]=1; // set 1 by main diagonal
	}
	// ������� ��������� � �������
	directionMatrix[12]=position.coordinate[0];
	directionMatrix[13]=position.coordinate[1];
	directionMatrix[14]=position.coordinate[2];
}

// ����������� �� ���������
object3DS::object3DS():directionMatrix(NULL),tex(NULL)
{
	position=vertex(0,0,0);
	init();
}

// ����������� �� �������� �������
object3DS::object3DS(vertex &pos):directionMatrix(NULL),tex(NULL)
{
	position=pos;
	init();
};

object3DS::~object3DS()
{
	if (directionMatrix)
	{
		delete [] directionMatrix;
	}
	if (tex)
	{
		delete tex;
	}
	for (int i=0; i<indexCount.size(); i++)
	{
		delete [] indexVertexNormal[i];
		delete [] localMatrix[i];
	}
	name.clear();
	indexCount.clear();
	indexVertexNormal.clear();
	localMatrix.clear();
	vertexVBO.clear();
}

void object3DS::render(int filterMode)
{
	glPushMatrix(); // save current matrix
	// ���������� ������� ��������
	glMultMatrixf(directionMatrix);
	// ��������� �����
	glColor3f(position.color[0],position.color[1],position.color[2]);
	if (hasVBO) // has VBO
	{
		if (newOGL) // OGL>=1.5
		{
			for (unsigned int i=0; i<indexCount.size(); i++)
			{
				// ��������� ������� ������
				glEnableClientState(GL_VERTEX_ARRAY);
				// ��������� ������� ��������
				glEnableClientState(GL_NORMAL_ARRAY);
				//glPushMatrix(); // save current matrix
				//glMultMatrixf(localMatrix[i]);
				// �������� ������
				glBindBuffer(GL_ARRAY_BUFFER, vertexVBO[i]);
				// ��������� ��������� �� ������ ������ �� VBO
				glVertexPointer(3, GL_FLOAT, sizeof(vertexNormalTex), (GLvoid *)((char *)indexVertexNormal[i][0].coordinate-(char *)indexVertexNormal[i]));
				// ��������� ��������� �� ������ �������� �� VBO
				glNormalPointer(GL_FLOAT, sizeof(vertexNormalTex), (GLvoid *)((char *)indexVertexNormal[i][0].normal-(char *)indexVertexNormal[i]));
				glEnable(GL_TEXTURE_2D);
				glClientActiveTexture(GL_TEXTURE0);
				// ��������� ������� ���������� ���������
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				// �������� ��������
				tex->bind();
				// ��������� ��������� �� ������ ���������� ��������� �� VBO
				//GLvoid * tmpK=(GLvoid *)((char *)indexVertexNormal[i][0].tex-(char *)indexVertexNormal[i]);
				glTexCoordPointer(2, GL_FLOAT, sizeof(vertexNormalTex), (GLvoid *)((char *)indexVertexNormal[i][0].tex-(char *)indexVertexNormal[i]));
				// ��������� ������
				glDrawArrays(GL_TRIANGLES, 0, 3*indexCount[i]);
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
				//glPopMatrix(); // restore matrix
			}
		} else // old OGL
		{
			for (unsigned int i=0; i<indexCount.size(); i++)
			{
				// ��������� ������� ������
				glEnableClientState(GL_VERTEX_ARRAY);
				// ��������� ������� ��������
				glEnableClientState(GL_NORMAL_ARRAY);
				//glPushMatrix(); // save current matrix
				//glMultMatrixf(localMatrix[i]);
				// �������� ������
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexVBO[i]);
				// ��������� ��������� �� ������ ������ �� VBO
				glVertexPointer(3, GL_FLOAT, sizeof(vertexNormal), (GLvoid *)((char *)indexVertexNormal[i][0].coordinate-(char *)indexVertexNormal[i]));
				// ��������� ��������� �� ������ �������� �� VBO
				glNormalPointer(GL_FLOAT, sizeof(vertexNormal), (GLvoid *)((char *)indexVertexNormal[i][0].normal-(char *)indexVertexNormal[i]));
				// ��������� ������
				glDrawArrays(GL_TRIANGLES, 0, 3*indexCount[i]);
				// ���������� ������� ������
				glDisableClientState(GL_VERTEX_ARRAY);
				// ���������� ������� ��������
				glDisableClientState(GL_NORMAL_ARRAY);
				// ���������� ������
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
				// ���������� ������
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
				//glPopMatrix(); // restore matrix
			}
		}
	} else // no VBO
	{
		for (unsigned int i=0; i<indexCount.size(); i++)
		{
			//glPushMatrix(); // save current matrix
			//glMultMatrixf(localMatrix[j]);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(vertexNormal), indexVertexNormal[i][0].coordinate);
			glNormalPointer(GL_FLOAT, sizeof(vertexNormal), indexVertexNormal[i][0].normal);
			glDrawArrays(GL_TRIANGLES, 0, 3*indexCount[i]);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			//glPopMatrix(); // restore matrix
		}
	}
	glPopMatrix(); // restore matrix
}

unsigned int object3DS::findChunk(ifstream& ifs, unsigned short id, bool parent)
{
	// ������������� �����
	unsigned short chunkId;
	// ����� �����
	unsigned int chunkLen;
	if (parent)
	{
		// ������ �������������� �����
		ifs.read((char*)&chunkId,2);
		// ������ ����� �����
		ifs.read((char*)&chunkLen,4);
		if(chunkId==obj3ds::OBJECT) // need to read object name if it is EDIT_OBJECT chunk
		{
			unsigned char ch;
			do
			{
				ifs.read((char*)&ch,1); // reading one char
			} while(ch!='\0' && !ifs.eof()); // reading until string ends
		}
	}
	do
	{
		// ������ �������������� �����
		ifs.read((char*)&chunkId,2);
		// ������ ����� �����
		ifs.read((char*)&chunkLen,4);

		if(chunkId!=id) ifs.ignore(chunkLen-6); // skip if not necessary id
		else
		{
			// ��������� ������� �������
			int pos=ifs.tellg();
			// ������� � ������ �������� �����
			ifs.seekg(pos-6);
			// ������� ������� ������� � �����
			return (ifs.tellg());
		}
	}while(!ifs.eof());
	return false;
}

void object3DS::setScale(GLfloat scale)
{
	for (int j=0; j<3; j++)
	{
		directionMatrix[j*4+j]=scale;
	}
}

bool object3DS::load(wstring fname)
{
	// ������������� �����
	unsigned short chunkId;
	// ������� ����� � �����
	unsigned int chunkPos;
	// ������� ����� �������� �����
	unsigned int chunkTmpPos;
	// ����� �����
	unsigned int chunkLen;
	// ����� ����� � ��������
	unsigned int chunkModels;
	// ����� ����� ������
	unsigned int chunkModelPos;
	// ������� ����� ������
	unsigned int chunkModelLen;
	// ���� ���������� �������� ���� �� ����� ������
	bool hasAnyLoaded=false;
	// ���� ���������� �������� ��������� ������
	bool lastFailed;
	// ���� ������� ���������
	GLfloat oneUnit=1.0f;

	ifstream ifs;
	ifs.open(fname.c_str(),ios_base::in|ios_base::binary|ios_base::beg);
	if(!ifs.is_open())
	{
		return false; // error while opening file
	}
	else
	{
		// ������ �������������� �����
		ifs.read((char*)&chunkId,2);
		// ������ ����� �����
		ifs.read((char*)&chunkLen,4);
		if(chunkId!=obj3ds::MAIN) return false; // not 3ds
		// ����������� � ������ �����
		ifs.seekg((int)ifs.tellg()-6);
		// ����� ���������� ������������ �����
		chunkPos=findChunk(ifs,obj3ds::EDITOR);
		if(chunkPos==0) return false; // not found
		// ������ �������������� �����
		ifs.read((char*)&chunkId,2);
		// ������ ����� �����
		ifs.read((char*)&chunkModels,4);
		// ����������� � ������ �����
		//ifs.seekg((int)ifs.tellg()-6);
		// ���������� ����� ����� �������
		chunkModels+=chunkPos;

		// ���������� ��������� �����
		chunkTmpPos=ifs.tellg();
		chunkPos=findChunk(ifs,obj3ds::ONEUNIT,false);
		if(chunkPos!=0) // found
		{
			ifs.ignore(6);
			ifs.read((char*)&oneUnit,sizeof(GLfloat));
			// ������� � ������������ ���������
			ifs.seekg(chunkTmpPos);
		}

		// ������� �������� �������
		GLfloat lMatrixTemplate[]={
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

		// ���������� ���������
		unsigned short iCount;
		// ������ ��������� ������ � �� �������� �� ��������
		vertexNormalTex *iVertex;
		// ���������� ������
		unsigned short vCount;
		// ���������� ���������� ��������� ������
		unsigned short texCount;
		// ������ ������
		GLfloat *vList;
		// ������ ���������� ���������
		GLfloat *texCoord;
		// ������� �������� �������
		GLfloat *lMatrix;

		do
		{
			// ��� �������
			string objectName="";

			lastFailed=false;
			// ����� ���������� ������������ �����
			chunkPos=findChunk(ifs,obj3ds::OBJECT,false);
			if(chunkPos==0) { lastFailed=(chunkPos==0); break; }//return false; // not found
			chunkModelPos=chunkPos;
			ifs.read((char*)&chunkId,2);
			// ������ ����� �����
			ifs.read((char*)&chunkModelLen,4);
			unsigned char ch;
			do
			{
				ifs.read((char*)&ch,1); // reading one char
				objectName+=ch; // put char to name string
			} while(ch!='\0' && !ifs.eof()); // reading until string ends

			// ���������� ��������� �����
			chunkTmpPos=ifs.tellg();

			// ����� ���������� ������������ �����
			//chunkPos=findChunk(ifs,obj3ds::HIDDEN,false);
			//if(chunkPos!=0) { lastFailed=false; break; }//return false; // not found

			// ������� � ������������ ���������
			ifs.seekg(chunkTmpPos);

			// ����� ���������� ������������ �����
			chunkPos=findChunk(ifs,obj3ds::TRIMESH,false);
			if(chunkPos==0) { lastFailed=(chunkPos==0); break; } // not found
			
			chunkTmpPos=chunkPos;

			// ����� ���������� ������������ �����
			chunkPos=findChunk(ifs,obj3ds::VERTEXLIST);
			if(chunkPos==0) return false; // not found
			// ������� 6 ������ (chunkId, chunkLen)
			ifs.ignore(6);
			// ������ ���������� �����
			ifs.read((char*)&vCount,2);
			// ��������� ������ ��� �������� �����
			vList=new float[3*vCount];
			for(int i=0;i<vCount;i++) // read vertexes
			{
				ifs.read((char*)&(vList[3*i]),4); vList[3*i]*=oneUnit;
				ifs.read((char*)&(vList[3*i+2]),4); vList[3*i+2]*=oneUnit;
				ifs.read((char*)&(vList[3*i+1]),4); vList[3*i+1]*=oneUnit;
			}

			// ������� � ������������ ���������
			ifs.seekg(chunkTmpPos);

			// ����� ���������� ������������ �����
			chunkPos=findChunk(ifs,obj3ds::MAPPINGCOORDS);
			if(chunkPos==0) return false; // not found
			// ������� 6 ������ (chunkId, chunkLen)
			ifs.ignore(6);
			// ������ ���������� ���������� ���������
			ifs.read((char*)&texCount,2);
			// ��������� ������ ��� �������� ���������� ���������
			texCoord=new float[2*texCount];
			for(int i=0;i<texCount;i++) // read texes coords
			{
				ifs.read((char*)&(texCoord[2*i]),4);
				ifs.read((char*)&(texCoord[2*i+1]),4);
			}

			// ������� � ������������ ���������
			ifs.seekg(chunkTmpPos);

			// ����� ���������� ������������ �����
			chunkPos=findChunk(ifs,obj3ds::FACESLIST);
			if(chunkPos==0) { lastFailed=(chunkPos==0); break; } // not found
			// ������� 6 ������ (chunkId, chunkLen)
			ifs.ignore(6);
			// ������ ���������� ���������
			ifs.read((char*)&iCount,2);
			// ��������� ������ ��� �������� ������ � �������� �� ��������
			iVertex=new vertexNormalTex [3*iCount];
			for(int i=0;i<iCount;i++) // read indexes
			{
				unsigned short tmp;
				// ������ �������� � ���������� ������
				ifs.read((char*)&(tmp),2); iVertex[3*i].copyCoord(&vList[3*tmp]); iVertex[3*i].copyTex(&texCoord[2*tmp]);
				ifs.read((char*)&(tmp),2); iVertex[3*i+2].copyCoord(&vList[3*tmp]); iVertex[3*i+2].copyTex(&texCoord[2*tmp]);
				ifs.read((char*)&(tmp),2); iVertex[3*i+1].copyCoord(&vList[3*tmp]);  iVertex[3*i+1].copyTex(&texCoord[2*tmp]);
				// ���������� �������
				vec3 a0=iVertex[3*i].vec3Coordinate();
				vec3 a1=iVertex[3*i+1].vec3Coordinate();
				vec3 a2=iVertex[3*i+2].vec3Coordinate();
				vec3 a1a0=a0-a1,a1a2=a2-a1;
				vec3 normal=normalize(cross(a1a2,a1a0));
				// ���������� ������� ��� 3 ����� ��������
				iVertex[3*i].copyNormal(&normal.x);
				iVertex[3*i+1].copyNormal(&normal.x);
				iVertex[3*i+2].copyNormal(&normal.x);
				ifs.ignore(2); // skip flags
			}

			// ������� � ������������ ���������
			ifs.seekg(chunkTmpPos);
			// ����� ���������� ������������ �����
			chunkPos=findChunk(ifs,obj3ds::LOCALMATRIX);
			if(chunkPos!=0) // found
			{
				lMatrix=new GLfloat[16];
				memcpy(lMatrix,lMatrixTemplate,sizeof(GLfloat)*16);

				float tmp[3];
				ifs.ignore(6);
				//for (int i=0; i<4; i++)
				for (int i=0; i<3; i++)
				{
					ifs.read((char*)(tmp),3*sizeof(float));
					lMatrix[4*i]=tmp[0];
					lMatrix[4*i+2]=tmp[1];
					lMatrix[4*i+1]=tmp[2];
				}
				/*ifs.read((char*)(tmp),3*sizeof(float));
				lMatrix[4*3]=-tmp[0];
				lMatrix[4*3+2]=-tmp[1];
				lMatrix[4*3+1]=-tmp[2];*/
			}

			// ������ ���� �������� ���������
			hasAnyLoaded=true;
			// ��������� ��������� ������ �� �������
			name.push_back(objectName);
			indexCount.push_back(iCount);
			indexVertexNormal.push_back(iVertex);
			localMatrix.push_back(lMatrix);
			delete []vList;
			delete []texCoord;
			// ��������� � ���������� �������
			ifs.seekg(chunkModelPos+chunkModelLen);
		} while (!lastFailed);
		return hasAnyLoaded; // reading finished
	}
}