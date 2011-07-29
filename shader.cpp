#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include "shader.h"
#include "vertex.h"

using namespace std;

bool shader::newOGL=false; // OGL version
bool shader::hasFS=false; // fragment shader support
bool shader::hasVS=false; // vertex shader support

// �������� ����������
void shader::checkExtensions()
{
	shader::newOGL=atof((const char *)glGetString(GL_VERSION))>=1.5f; // check OGL version
	shader::hasFS=glutExtensionSupported("GL_ARB_fragment_shader")!=0; // check if fragment shader is supported
	shader::hasVS=glutExtensionSupported("GL_ARB_vertex_shader")!=0; // check if vertex shader is supported
}

// �������� ���������� ������� �� �����
GLenum shader::load(string fname, GLenum type)
{
	ifstream shaderFile(fname.c_str(), ios::in|ios::binary);
	if (!shaderFile)
	{
		cerr<<"Shader file "<<fname<<" unavailable"<<endl; // file can't be opened for reading
		return 0;
	}
	// �������� ������ �����
	shaderFile.seekg(0,ios::end);
	unsigned int size=shaderFile.tellg();
	shaderFile.seekg(0,ios::beg);
	// �������� ����� �� �������
	if (size)
	{
		// ���������� ������ ������������ �������
		char *buf=new char[size+1];
		// ���������� ������ ��������� 0
		memset(buf,0,size+1);
		// ������ ����� � �����
		shaderFile.read(buf,size);
		// ��������� ������ ������ �� ���������
		char *tmp=strtok(buf,"\n");
		// ������ �����
		vector<char *> strings;
		while(tmp)
		{
			// ���������� ������ � ������
			strings.push_back(tmp);
			// ��������� ��������� ���������
			tmp=strtok(NULL,"\n");
		}
		// ��������� ���������� �����
		int numStrings=strings.size();
		// ���������� �������
		char **cstrings=new char *[numStrings];
		// ������� ����� � ������
		for(int i=0; i<numStrings; i++)
		{
			cstrings[i]=strings[i];
		}
		// ������������� �������
		GLenum id;
		// ������ ����������
		int status;
		if (newOGL)
		{
			// �������� ���������� �������
			id=glCreateShader(type);
			// �������� ���� �������
			glShaderSource(id, numStrings, const_cast<const GLchar **>(cstrings), NULL);
			try
			{
				// ���������� ����
				glCompileShader(id);
			} catch (...)
			{
				cerr<<"Error while trying to compile using driver. Please check that you using lastest version of driver"<<endl;
				cerr<<"Deleting comments from shaders can help"<<endl; // ugly Intel OGL
			}
			// ��������� ������� ����������
			glGetShaderiv(id,GL_COMPILE_STATUS,&status);
		} else
		{
			// �������� ���������� �������
			id=glCreateShaderObjectARB(type);
			// �������� ���� �������
			glShaderSourceARB(id, numStrings, const_cast<const GLchar **>(cstrings), NULL);
			try
			{
				// ���������� ����
				glCompileShaderARB(id);
			} catch (...)
			{
				cerr<<"Error while trying to compile using driver. Please check that you using lastest version of driver"<<endl;
				cerr<<"Deleting comments from shaders can help"<<endl; // ugly Intel OGL
			}
			// ��������� ������� ����������
			glGetObjectParameterivARB(id,GL_OBJECT_COMPILE_STATUS_ARB,&status);
		}
		// �������� ��������� ����������
		delete []buf;
		delete []cstrings;
		strings.clear();

		if (newOGL)
		{
			// �������� ������� �� �����������
			if (status==GL_FALSE)
			{
				// ����� ���������, ���������� ��������� ��������
				int len,readed;
				// ��������� ����� ���������
				glGetShaderiv(id,GL_INFO_LOG_LENGTH,&len);
				// ���������� ������ ��� ���������
				char *log=new char[len+1];
				// ���������� ������ ��������� 0
				memset(log,0,len+1);
				// ��������� ������ ���������
				glGetShaderInfoLog(id,len,&readed,log);
				// ������ ������ ���������
				cerr<<"Error while compiling "<<fname<<endl<<log<<endl;
				// �������� �������
				glDeleteShader(id);
				// �������� ������
				delete []log;
				return 0;
			}
		} else
		{
			// �������� ������� �� �����������
			if (!status)
			{
				// ����� ���������, ���������� ��������� ��������
				int len,readed;
				// ��������� ����� ���������
				glGetObjectParameterivARB(id,GL_OBJECT_INFO_LOG_LENGTH_ARB,&len);
				// ���������� ������ ��� ���������
				char *log=new char[len+1];
				// ���������� ������ ��������� 0
				memset(log,0,len+1);
				// ��������� ������ ���������
				glGetInfoLogARB(id,len,&readed,log);
				// ������ ������ ���������
				cerr<<"Error while compiling "<<fname<<endl<<log<<endl;
				// �������� �������
				glDeleteObjectARB(id);
				// �������� ������
				delete []log;
				return 0;
			}
		}
		return id;
	}
	return 0;
};

bool shader::loadFragmentShader(string fname)
{
	// ������� ��������� ����������� ��������
	if (hasFS)
	{
		// �������� �������
		fS=load(fname,GL_FRAGMENT_SHADER);
		// ��������� �������� �������� ��������
		fSLoaded=fS!=0;
	}
	return fSLoaded;
}

bool shader::loadVertexShader(string fname)
{
	// ������� ��������� ���������� ��������
	if (hasVS)
	{
		// �������� �������
		vS=load(fname,GL_VERTEX_SHADER);
		// ��������� �������� �������� ��������
		vSLoaded=vS!=0;
	}
	return vSLoaded;
}

bool shader::createProgram()
{
	// �������� ���������� �������� ��������
	if (fSLoaded && vSLoaded)
	{
		// ������ ��������
		int status;
		if (newOGL)
		{
			// �������� ���������
			idProgram=glCreateProgram();
			// ����������� ��������
			glAttachShader(idProgram, vS);
			glAttachShader(idProgram, fS);
			// �������� ���������
			glLinkProgram(idProgram);
			// ��������� ������� ��������
			glGetProgramiv(idProgram,GL_COMPILE_STATUS,&status);
			// �������� �� �����������
			//if (status==GL_FALSE)
			//{
			//	// ����� ���������, ���������� ��������� ��������
			//	int len,readed;
			//	// ��������� ����� ���������
			//	glGetProgramiv(idProgram,GL_INFO_LOG_LENGTH,&len);
			//	// ���������� ������ ��� ���������
			//	char *log=new char[len+1];
			//	// ���������� ������ ��������� 0
			//	memset(log,0,len+1);
			//	// ��������� ������ ���������
			//	glGetProgramInfoLog(idProgram,len,&readed,log);
			//	// ������ ������ ���������
			//	cerr<<"Error while linking program"<<endl<<log<<endl;
			//	// �������� �������
			//	glDeleteProgram(idProgram);
			//	// �������� ������
			//	delete []log;
			//	return false;
			//}
		} else
		{
			// �������� ���������
			idProgram=glCreateProgramObjectARB();
			// ����������� ��������
			glAttachObjectARB(idProgram, vS);
			glAttachObjectARB(idProgram, fS);
			// �������� ���������
			glLinkProgramARB(idProgram);
			// ��������� ������� ��������
			glGetObjectParameterivARB(idProgram,GL_OBJECT_LINK_STATUS_ARB,&status);
			// �������� �� �����������
			if (!status)
			{
				// ����� ���������, ���������� ��������� ��������
				int len,readed;
				// ��������� ����� ���������
				glGetObjectParameterivARB(idProgram,GL_OBJECT_INFO_LOG_LENGTH_ARB,&len);
				// ���������� ������ ��� ���������
				char *log=new char[len+1];
				// ���������� ������ ��������� 0
				memset(log,0,len+1);
				// ��������� ������ ���������
				glGetInfoLogARB(idProgram,len,&readed,log);
				// ������ ������ ���������
				cerr<<"Error while linking program"<<endl<<log<<endl;
				// �������� �������
				glDeleteObjectARB(idProgram);
				// �������� ������
				delete []log;
				return false;
			}
		}
		programLoaded=true;
	}
	return programLoaded;
}

// ��������� ������������ �������
bool shader::activate()
{
	// �������� �������� �������� ���������
	if (programLoaded)
	{
		if (newOGL)
		{
			// ��������� ������������ �������
			glUseProgram(idProgram);
		} else
		{
			// ��������� ������������ �������
			glUseProgramObjectARB(idProgram);
		}
	}
	return programLoaded;
}

// ���������� ������������ �������
void shader::disactivate()
{
	// �������� ��������� ��������
	if (hasFS||hasVS)
	{
		if (newOGL)
		{
			// ���������� ������������ �������
			glUseProgram(NULL);
		} else
		{
			// ���������� ������������ �������
			glUseProgramObjectARB(NULL);
		}
	}
}