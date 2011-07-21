//Glee
#include "glee.h"
//VC++
#include <string>
#include <vector>
//glm
#include "glm-0.9.1/glm.hpp"
//glut
#include "glut.h"
//self implemented
#include "vertex.h"
#include "texture.h"

using namespace std;
using namespace glm;

#ifndef __object3ds_h__
#define __object3ds_h__

class object3DS
{
private:
	// ��� �������
	vector<string> name;
	// ���������� ���������
	vector<unsigned short> indexCount;
	// ������ ��������� ������ � �� �������� �� ��������
	vector<vertexNormalTex *> indexVertexNormal;
	// ������� �������
	vertex position;
	// ������� �������� ��������
	vector<GLfloat *> localMatrix;
	// ������� �������� �������
	GLfloat *directionMatrix;
	// ����� VBO ��� ������
	vector<GLuint> vertexVBO;
	// ��������
	texture tex;
	static bool newOGL, hasVBO;
	// ����� ����� �� ������
	unsigned int findChunk(ifstream& ifs, unsigned short id, bool parent=true);
public:
	// ����������� �� �������� �������
	object3DS(vertex pos=vertex(0,0,0));
	// �������� ����� 3ds � ������ � ������ VBO
	bool load(wstring fname);
	// ����� ������� �� �����
	void render(int filterMode);
	// ����������� � VBO
	void buffer();
	// ��������� ��������
	void setScale(GLfloat scale);
	// ��������� ��������� �������
	void setPosition(vertex pos)
	{
		position=pos;
		directionMatrix[12]=pos.coordinate[0];
		directionMatrix[13]=pos.coordinate[1];
		directionMatrix[14]=pos.coordinate[2];
	}
	void copyTex(texture *t)
	{
		memcpy(&tex,t,sizeof(texture));
	}
	GLfloat * getColor()
	{
		return position.color;
	}
	static void checkExtensions();
};

#endif