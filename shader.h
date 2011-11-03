#ifndef __shader_h__
#define __shader_h__

#include "glee.h"
#include "glut.h"
#include <string>
#include <vector>
#include "vertex.h"

using namespace std;

class shader
{
	// ������������� ������������ �������
	GLuint idProgram;
	// �������������� ��������� ��������
	GLenum fS,vS;
	// �������� �������� �������� ��������
	bool fSLoaded, vSLoaded, programLoaded;
	// �������� ������� ����������� ����������
	static bool newOGL, hasFS, hasVS;
	// �������� ���������� ������� �� �����
	GLenum load(string filename, GLenum type);
public:
	// ����������� �� ���������
	shader():fSLoaded(false), vSLoaded(false), programLoaded(false) {};
	// �������� �� ����� ������������ �������
	bool loadFragmentShader(string filename);
	// �������� �� ����� ����������� �������
	bool loadVertexShader(string filename);
	// �������� ���������
	bool createProgram();
	// ��������� ������������ �������
	bool activate();
	// ��������� �������������� ������������ �������
	GLuint getProgramId()
	{
		return programLoaded?idProgram:0;
	}
	// �������� ����������
	static void checkExtensions();
	// ���������� ������������ �������
	static void disactivate();
};

#endif