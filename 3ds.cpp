#include <fstream>
#include "stdafx.h"
#include "3ds.h"
#include "3dsChunks.h"
#include "3dsLoader.h"

using namespace std;

namespace n3ds
{

	bool c3ds::newOGL=false;
	bool c3ds::hasVBO=false;

	void c3ds::checkExtensions()
	{
		c3ds::newOGL=(atof((const char *)glGetString(GL_VERSION))>=1.5f); // check OGL version
		c3ds::hasVBO=glutExtensionSupported("GL_ARB_vertex_buffer_object")!=0; // check if VBO supported
	}

	// буферизация данных объекта в VBO
	void c3ds::buffer()
	{
		//if (!hasVBO) { return; } // no VBO
		//if (newOGL) // OGL>=1.5
		//{
		//	for (unsigned int i=0; i<indexCount.size(); i++)
		//	{
		//		GLuint vBO;
		//		// генерация имени для буферного объекта
		//		glGenBuffers(1, &vBO);
		//		// привязка буфера
		//		glBindBuffer(GL_ARRAY_BUFFER, vBO);
		//		// создание и инициализация области хранения данных для буферного объекта
		//		glBufferData(GL_ARRAY_BUFFER, (indexCount[i])*3*sizeof(vertexNormalTex), indexVertexNormal[i], GL_STATIC_DRAW);
		//		// отключение буфера
		//		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//		vertexVBO.push_back(vBO);
		//	}
		//}
		//else // old OGL
		//{
		//	for (unsigned int i=0; i<indexCount.size(); i++)
		//	{
		//		GLuint vBO;
		//		// генерация имени для буферного объекта
		//		glGenBuffersARB(1, &vBO);
		//		// привязка буфера
		//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vBO);
		//		// создание и инициализация области хранения данных для буферного объекта
		//		glBufferDataARB(GL_ARRAY_BUFFER_ARB, (indexCount[i])*3*sizeof(vertexNormalTex), indexVertexNormal[i], GL_STATIC_DRAW);
		//		// отключение буфера
		//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		//		vertexVBO.push_back(vBO);
		//	}
		//}
	}

	void c3ds::init()
	{
		/*if (directionMatrix)
		{
			delete []directionMatrix;
		}*/
		// матрица поворота
		//directionMatrix=new GLfloat[16];
		//for (int j=0; j<4; j++)
		//{
		//	for (int i=0; i<4; i++)
		//	{
		//		directionMatrix[j*4+i]=0; // set to zero
		//	}
		//	directionMatrix[j*4+j]=1; // set 1 by main diagonal
		//}
		// задание координат в матрице
		/*directionMatrix[12]=position.coordinate[0];
		directionMatrix[13]=position.coordinate[1];
		directionMatrix[14]=position.coordinate[2];*/
		cf_unit=1.0f;
	}

	// конструктор по умолчанию
	c3ds::c3ds():/*directionMatrix(NULL),*/tex(NULL)
	{
		//position=vertex(0,0,0);
		init();
	}

	// конструктор по заданной позиции
	c3ds::c3ds(vertex &pos):/*directionMatrix(NULL),*/tex(NULL)
	{
		//position=pos;
		init();
	};

	c3ds::~c3ds()
	{
		/*if (directionMatrix)
		{
			delete [] directionMatrix;
		}*/
		if (tex)
		{
			delete tex;
		}
		/*for (unsigned int i=0; i<indexCount.size(); i++)
		{
			delete [] indexVertexNormal[i];
			delete [] localMatrix[i];
		}*/
		for (unsigned int i=0; i<cf_material.size(); i++)
		{
			delete cf_material[i];
		}
		//cf_name.clear();
		/*indexCount.clear();
		indexVertexNormal.clear();
		localMatrix.clear();
		vertexVBO.clear();*/
		cf_material.clear();
	}

	void c3ds::render(int filterMode)
	{
		//glPushMatrix(); // save current matrix
		//// применение матрицы поворота
		//glMultMatrixf(directionMatrix);
		//// установка цвета
		//glColor3f(position.color[0],position.color[1],position.color[2]);
		//if (hasVBO) // has VBO
		//{
		//	if (newOGL) // OGL>=1.5
		//	{
		//		for (unsigned int i=0; i<indexCount.size(); i++)
		//		{
		//			// включение массива вершин
		//			glEnableClientState(GL_VERTEX_ARRAY);
		//			// включение массива нормалей
		//			glEnableClientState(GL_NORMAL_ARRAY);
		//			//glPushMatrix(); // save current matrix
		//			//glMultMatrixf(localMatrix[i]);
		//			// привязка буфера
		//			glBindBuffer(GL_ARRAY_BUFFER, vertexVBO[i]);
		//			// установка указателя на массив вершин по VBO
		//			glVertexPointer(3, GL_FLOAT, sizeof(vertexNormalTex), (GLvoid *)((char *)indexVertexNormal[i][0].coordinate-(char *)indexVertexNormal[i]));
		//			// установка указателя на массив нормалей по VBO
		//			glNormalPointer(GL_FLOAT, sizeof(vertexNormalTex), (GLvoid *)((char *)indexVertexNormal[i][0].normal-(char *)indexVertexNormal[i]));
		//			glEnable(GL_TEXTURE_2D);
		//			glClientActiveTexture(GL_TEXTURE0);
		//			// включение массива текстурных координат
		//			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//			// привязка текстуры
		//			tex->bind();
		//			// установка указателя на массив текстурных координат по VBO
		//			//GLvoid * tmpK=(GLvoid *)((char *)indexVertexNormal[i][0].tex-(char *)indexVertexNormal[i]);
		//			glTexCoordPointer(2, GL_FLOAT, sizeof(vertexNormalTex), (GLvoid *)((char *)indexVertexNormal[i][0].tex-(char *)indexVertexNormal[i]));
		//			// отрисовка вершин
		//			glDrawArrays(GL_TRIANGLES, 0, 3*indexCount[i]);
		//			glDisable(GL_TEXTURE_2D);
		//			// отключение массива текстурных координат
		//			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//			// отключение массива вершин
		//			glDisableClientState(GL_VERTEX_ARRAY);
		//			// отключение массива нормалей
		//			glDisableClientState(GL_NORMAL_ARRAY);
		//			// отключение буфера
		//			glBindBuffer(GL_ARRAY_BUFFER, 0);
		//			// отключение буфера
		//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//			//glPopMatrix(); // restore matrix
		//		}
		//	} else // old OGL
		//	{
		//		for (unsigned int i=0; i<indexCount.size(); i++)
		//		{
		//			// включение массива вершин
		//			glEnableClientState(GL_VERTEX_ARRAY);
		//			// включение массива нормалей
		//			glEnableClientState(GL_NORMAL_ARRAY);
		//			//glPushMatrix(); // save current matrix
		//			//glMultMatrixf(localMatrix[i]);
		//			// привязка буфера
		//			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexVBO[i]);
		//			// установка указателя на массив вершин по VBO
		//			glVertexPointer(3, GL_FLOAT, sizeof(vertexNormal), (GLvoid *)((char *)indexVertexNormal[i][0].coordinate-(char *)indexVertexNormal[i]));
		//			// установка указателя на массив нормалей по VBO
		//			glNormalPointer(GL_FLOAT, sizeof(vertexNormal), (GLvoid *)((char *)indexVertexNormal[i][0].normal-(char *)indexVertexNormal[i]));
		//			// отрисовка вершин
		//			glDrawArrays(GL_TRIANGLES, 0, 3*indexCount[i]);
		//			// отключение массива вершин
		//			glDisableClientState(GL_VERTEX_ARRAY);
		//			// отключение массива нормалей
		//			glDisableClientState(GL_NORMAL_ARRAY);
		//			// отключение буфера
		//			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		//			// отключение буфера
		//			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
		//			//glPopMatrix(); // restore matrix
		//		}
		//	}
		//} else // no VBO
		//{
		//	for (unsigned int i=0; i<indexCount.size(); i++)
		//	{
		//		//glPushMatrix(); // save current matrix
		//		//glMultMatrixf(localMatrix[j]);
		//		glEnableClientState(GL_VERTEX_ARRAY);
		//		glEnableClientState(GL_NORMAL_ARRAY);
		//		glVertexPointer(3, GL_FLOAT, sizeof(vertexNormal), indexVertexNormal[i][0].coordinate);
		//		glNormalPointer(GL_FLOAT, sizeof(vertexNormal), indexVertexNormal[i][0].normal);
		//		glDrawArrays(GL_TRIANGLES, 0, 3*indexCount[i]);
		//		glDisableClientState(GL_VERTEX_ARRAY);
		//		glDisableClientState(GL_NORMAL_ARRAY);
		//		//glPopMatrix(); // restore matrix
		//	}
		//}
		//glPopMatrix(); // restore matrix
	}

	void c3ds::setScale(GLfloat scale)
	{
		/*for (int j=0; j<3; j++)
		{
			directionMatrix[j*4+j]=scale;
		}*/
	}

	bool c3ds::load(wstring fname)
	{
																					//// идентификатор чанка
	//unsigned short chunkId;
	//// позиция чанка в файле
	//unsigned int chunkPos;
	//// позиция более верхнего чанка
	//unsigned int chunkTmpPos;
	//// длина чанка
	//unsigned int chunkLen;
	//// длина блока с моделями
	//unsigned int chunkModels;
	//// длина чанка модели
	//unsigned int chunkModelPos;
	//// позиция чанка модели
	//unsigned int chunkModelLen;
	//// флаг нормальной загрузки хотя бы одной модели
	//bool hasAnyLoaded=false;
	//// флаг нормальной загрузки последней модели
	//bool lastFailed;
	//// одна единица измерения
	//GLfloat oneUnit=1.0f;

		ifstream ifs;
		ifs.open(fname.c_str(),ios_base::in|ios_base::binary|ios_base::beg);
		if(!ifs.is_open())
		{
			return false; // error while opening file
		}
		else
		{
			// чтение идентификатора чанка
			return n3ds::c3dsLoader::load(ifs, *this);
		}
			//ifs.read((char*)&chunkId,2);
			//// чтение длины чанка
			//ifs.read((char*)&chunkLen,4);
			//if(chunkId!=n3ds::chunks::MAIN) return false; // not 3ds
			//// перемещение к началу чанка
			//ifs.seekg((int)ifs.tellg()-6);
			//// поиск следующего необходимого чанка
			//chunkPos=findChunk(ifs,n3ds::chunks::EDITOR);
			//if(chunkPos==0) return false; // not found
			//// чтение идентификатора чанка
			//ifs.read((char*)&chunkId,2);
			//// чтение длины чанка
			//ifs.read((char*)&chunkModels,4);
			//// перемещение к началу чанка
			////ifs.seekg((int)ifs.tellg()-6);
			//// сохранение длины чанка моделей
			//chunkModels+=chunkPos;

			//// сохранение положения чанка
			//chunkTmpPos=ifs.tellg();
			//chunkPos=findChunk(ifs,n3ds::chunks::ONEUNIT,false);
			//if(chunkPos!=0) // found
			//{
			//	ifs.ignore(6);
			//	ifs.read((char*)&oneUnit,sizeof(GLfloat));
			//	// возврат к сохраненному положению
			//	ifs.seekg(chunkTmpPos);
			//}

			//// матрица поворота объекта
			//GLfloat lMatrixTemplate[]={
			//	1,0,0,0,
			//	0,1,0,0,
			//	0,0,1,0,
			//	0,0,0,1
			//};

			//// количество полигонов
			//unsigned short iCount;
			//// список координат вершин и их нормалей по индексам
			//vertexNormalTex *iVertex;
			//// количество вершин
			//unsigned short vCount;
			//// количество текстурных координат вершин
			//unsigned short texCount;
			//// список вершин
			//GLfloat *vList;
			//// список текстурных координат
			//GLfloat *texCoord;
			//// матрица поворота объекта
			//GLfloat *lMatrix;

			//do
			//{
			//	// имя объекта
			//	string objectName="";

			//	lastFailed=false;
			//	// поиск следующего необходимого чанка
			//	chunkPos=findChunk(ifs,n3ds::chunks::OBJECT,false);
			//	if(chunkPos==0) { lastFailed=(chunkPos==0); break; }//return false; // not found
			//	chunkModelPos=chunkPos;
			//	ifs.read((char*)&chunkId,2);
			//	// чтение длины чанка
			//	ifs.read((char*)&chunkModelLen,4);
			//	unsigned char ch;
			//	do
			//	{
			//		ifs.read((char*)&ch,1); // reading one char
			//		objectName+=ch; // put char to name string
			//	} while(ch!='\0' && !ifs.eof()); // reading until string ends

			//	// сохранение положения чанка
			//	chunkTmpPos=ifs.tellg();

			//	// поиск следующего необходимого чанка
			//	//chunkPos=findChunk(ifs,obj3ds::HIDDEN,false);
			//	//if(chunkPos!=0) { lastFailed=false; break; }//return false; // not found

			//	// возврат к сохраненному положению
			//	ifs.seekg(chunkTmpPos);

			//	// поиск следующего необходимого чанка
			//	chunkPos=findChunk(ifs,n3ds::chunks::TRIMESH,false);
			//	if(chunkPos==0) { lastFailed=(chunkPos==0); break; } // not found
			//	
			//	chunkTmpPos=chunkPos;

			//	// поиск следующего необходимого чанка
			//	chunkPos=findChunk(ifs,n3ds::chunks::VERTEXLIST);
			//	if(chunkPos==0) return false; // not found
			//	// пропуск 6 байтов (chunkId, chunkLen)
			//	ifs.ignore(6);
			//	// чтение количества точек
			//	ifs.read((char*)&vCount,2);
			//	// выделение памяти для хранения точек
			//	vList=new float[3*vCount];
			//	for(int i=0;i<vCount;i++) // read vertexes
			//	{
			//		ifs.read((char*)&(vList[3*i]),4); vList[3*i]*=oneUnit;
			//		ifs.read((char*)&(vList[3*i+2]),4); vList[3*i+2]*=oneUnit;
			//		ifs.read((char*)&(vList[3*i+1]),4); vList[3*i+1]*=oneUnit;
			//	}

			//	// возврат к сохраненному положению
			//	ifs.seekg(chunkTmpPos);

			//	// поиск следующего необходимого чанка
			//	chunkPos=findChunk(ifs,n3ds::chunks::MAPPINGCOORDS);
			//	if(chunkPos==0) return false; // not found
			//	// пропуск 6 байтов (chunkId, chunkLen)
			//	ifs.ignore(6);
			//	// чтение количества текстурных координат
			//	ifs.read((char*)&texCount,2);
			//	// выделение памяти для хранения текстурных координат
			//	texCoord=new float[2*texCount];
			//	for(int i=0;i<texCount;i++) // read texes coords
			//	{
			//		ifs.read((char*)&(texCoord[2*i]),4);
			//		ifs.read((char*)&(texCoord[2*i+1]),4);
			//	}

			//	// возврат к сохраненному положению
			//	ifs.seekg(chunkTmpPos);

			//	// поиск следующего необходимого чанка
			//	chunkPos=findChunk(ifs,n3ds::chunks::FACESLIST);
			//	if(chunkPos==0) { lastFailed=(chunkPos==0); break; } // not found
			//	// пропуск 6 байтов (chunkId, chunkLen)
			//	ifs.ignore(6);
			//	// чтение количества полигонов
			//	ifs.read((char*)&iCount,2);
			//	// выделение памяти под хранение вершин и нормалей по индексам
			//	iVertex=new vertexNormalTex [3*iCount];
			//	for(int i=0;i<iCount;i++) // read indexes
			//	{
			//		unsigned short tmp;
			//		// чтение индексов и сохранение вершин
			//		ifs.read((char*)&(tmp),2); iVertex[3*i].copyCoord(&vList[3*tmp]); iVertex[3*i].copyTex(&texCoord[2*tmp]);
			//		ifs.read((char*)&(tmp),2); iVertex[3*i+2].copyCoord(&vList[3*tmp]); iVertex[3*i+2].copyTex(&texCoord[2*tmp]);
			//		ifs.read((char*)&(tmp),2); iVertex[3*i+1].copyCoord(&vList[3*tmp]);  iVertex[3*i+1].copyTex(&texCoord[2*tmp]);
			//		// вычисление нормали
			//		vec3 a0=iVertex[3*i].vec3Coordinate();
			//		vec3 a1=iVertex[3*i+1].vec3Coordinate();
			//		vec3 a2=iVertex[3*i+2].vec3Coordinate();
			//		vec3 a1a0=a0-a1,a1a2=a2-a1;
			//		vec3 normal=normalize(cross(a1a2,a1a0));
			//		// сохранение нормали для 3 точек полигона
			//		iVertex[3*i].copyNormal(&normal.x);
			//		iVertex[3*i+1].copyNormal(&normal.x);
			//		iVertex[3*i+2].copyNormal(&normal.x);
			//		ifs.ignore(2); // skip flags
			//	}

			//	// возврат к сохраненному положению
			//	ifs.seekg(chunkTmpPos);
			//	// поиск следующего необходимого чанка
			//	chunkPos=findChunk(ifs,n3ds::chunks::LOCALMATRIX);
			//	if(chunkPos!=0) // found
			//	{
			//		lMatrix=new GLfloat[16];
			//		memcpy(lMatrix,lMatrixTemplate,sizeof(GLfloat)*16);

			//		float tmp[3];
			//		ifs.ignore(6);
			//		//for (int i=0; i<4; i++)
			//		for (int i=0; i<3; i++)
			//		{
			//			ifs.read((char*)(tmp),3*sizeof(float));
			//			lMatrix[4*i]=tmp[0];
			//			lMatrix[4*i+2]=tmp[1];
			//			lMatrix[4*i+1]=tmp[2];
			//		}
			//		/*ifs.read((char*)(tmp),3*sizeof(float));
			//		lMatrix[4*3]=-tmp[0];
			//		lMatrix[4*3+2]=-tmp[1];
			//		lMatrix[4*3+1]=-tmp[2];*/
			//	}

			//	// ставим флаг успешной подгрузки
			//	hasAnyLoaded=true;
			//	// сохраняем считанные данные об объекте
			//	name.push_back(objectName);
			//	indexCount.push_back(iCount);
			//	indexVertexNormal.push_back(iVertex);
			//	localMatrix.push_back(lMatrix);
			//	delete []vList;
			//	delete []texCoord;
			//	// переходим к следующему объекту
			//	ifs.seekg(chunkModelPos+chunkModelLen);
			//} while (!lastFailed);
			//return hasAnyLoaded; // reading finished
		//}
	}
}