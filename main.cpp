// catch mem leaks
//#include <vld.h>
//Glee
#include "glee.h"
//DevIL
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
//glm
#include "glm/glm.hpp"
//glut (freeglut now)
#include "glut.h"
//self implemented
#include "stdafx.h"
#include "camera.h"
#include "vkscancodes.h"
#include "vertex.h"
#include "3ds.h"
#include "shader.h"
#include "texture.h"

#include "3dsMaterial.h"
#include "3dsVertex.h"

using namespace glm;
using namespace std;
using ns_3ds::sVertex;
using ns_3ds::sVertexColor;

std::ofstream _cerr;

// ���������� ��� ������ ���� � ������
std::wstring _3dsFile;

int frame=0,time,timebase=0,w,h,delayPerFrames=20,filterMode=0,g_nMaxAnisotropy;
char s[20];
bool isDrawingFps=false, isDrawingBack=false;
sVertex surface[]={
	sVertex(-1.5*surfaceSpace,0,-1.5*surfaceSpace),
	sVertex(-1.5*surfaceSpace,0,-0.5*surfaceSpace),
	sVertex(-1.5*surfaceSpace,0,0.5*surfaceSpace),
	sVertex(-1.5*surfaceSpace,0,1.5*surfaceSpace),

	sVertex(-0.5*surfaceSpace,0,-1.5*surfaceSpace),
	sVertex(-0.5*surfaceSpace,0,-0.5*surfaceSpace),
	sVertex(-0.5*surfaceSpace,0,0.5*surfaceSpace),
	sVertex(-0.5*surfaceSpace,0,1.5*surfaceSpace),

	sVertex(0.5*surfaceSpace,0,-1.5*surfaceSpace),
	sVertex(0.5*surfaceSpace,0,-0.5*surfaceSpace),
	sVertex(0.5*surfaceSpace,0,0.5*surfaceSpace),
	sVertex(0.5*surfaceSpace,0,1.5*surfaceSpace),

	sVertex(1.5*surfaceSpace,0,-1.5*surfaceSpace),
	sVertex(1.5*surfaceSpace,0,-0.5*surfaceSpace),
	sVertex(1.5*surfaceSpace,0,0.5*surfaceSpace),
	sVertex(1.5*surfaceSpace,0,1.5*surfaceSpace),
};
GLfloat surfaceTexCoord[]={
	0.0,0.0,
	1.0,0.0,
	2.0,0.0,
	3.0,0.0,

	0.0,1.0,
	1.0,1.0,
	2.0,1.0,
	3.0,1.0,

	0.0,2.0,
	1.0,2.0,
	2.0,2.0,
	3.0,2.0,

	0.0,3.0,
	1.0,3.0,
	2.0,3.0,
	3.0,3.0,
};
GLuint surfaceIndex[]={
	0,1,5,
	0,5,4,
	1,2,6,
	1,6,5,
	2,3,7,
	2,7,6,
	4,5,9,
	4,9,8,
	5,6,10,
	5,10,9,
	6,7,11,
	6,11,10,
	8,9,13,
	8,13,12,
	9,10,14,
	9,14,13,
	10,11,15,
	10,15,14,
};
GLfloat *surfaceIndexPoint,*surfaceNormal;
sVertexColor axes[]={
	// ������� ��� Ox
	sVertexColor(-surfaceSpace/2,0.1,0.0,1.0,0.0,0.0),
	sVertexColor(surfaceSpace,0.1, 0.0, 1.0,0.0,0.0),
	// ������� ��� Oz
	sVertexColor(0.0,0.1,-surfaceSpace/2,0.0,1.0,0.0),
	sVertexColor(0.0,0.1,surfaceSpace,0.0,1.0,0.0),
	// ����� ��� Oy
	sVertexColor(0.0,-surfaceSpace/2,0.0,0.0,0.0,1.0),
	sVertexColor(0.0,surfaceSpace,0.0,0.0,0.0,1.0),
};

sVertex block[]={
	sVertex(-halfWi, -halfWi, +halfWi),
	sVertex(-halfWi, +halfWi, +halfWi),
	sVertex(-halfWi, +halfWi, -halfWi),
	sVertex(-halfWi, -halfWi, -halfWi),
	
	sVertex(+halfWi, -halfWi, +halfWi),
	sVertex(+halfWi, +halfWi, +halfWi),
	sVertex(+halfWi, +halfWi, -halfWi),
	sVertex(+halfWi, -halfWi, -halfWi),
};
GLuint blockIndexes[]={
	0, 4, 5, 1,
	3, 0, 1, 2,
	7, 3, 2, 6,
	4, 7, 6, 5,
	4, 0, 3, 7,
	1, 5, 6, 2,
};
GLfloat blockMatrix[16]={
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

GLfloat ambMat[4]={
	0.4f,0.4f,0.4f,1.0f
};
GLfloat ambLig1[4]={
	0.4f,0.4f,0.4f,1.0f
};
GLfloat ambLig2[4]={
	0.0f,0.4f,0.0f,1.0f
};
GLfloat difMat[4]={
	0.8f,0.8f,0.8f,1.0f
};
GLfloat difLig1[4]={
	0.8f,0.8f,0.8f,1.0f
};
GLfloat difLig2[4]={
	0.0f,0.8f,0.0f,1.0f
};
GLfloat speMat[4]={
	0.4f,0.4f,0.4f,1.0f
};
GLfloat speLig1[4]={
	0.7f,0.7f,0.7f,0.9f
};
GLfloat speLig2[4]={
	0.0f,0.7f,0.0f,0.9f
};
GLfloat posLig1[4]={
	-2*surfaceSpace,10.0f,-2*surfaceSpace,1.0f
};
GLfloat posLig2[4]={
	2*surfaceSpace,10.0f,2*surfaceSpace,1.0f
};
GLfloat dirLig1[3]={
	0.0f,0.0f,-1.0f
};
GLfloat dirLig2[3]={
	0.0f,0.0f,-1.0f
};
ILuint devilError;
texture surfaceTex;

ns_3ds::c3ds *object;
bool newOGL=false,hasVBO=false,hasFragmentShader=false,hasVertexShader=false;
camera *cam;

shader modelShader,planeShader;
ns_3ds::c3dsMaterial planeMaterial=ns_3ds::c3dsMaterial(ambMat,difMat,speMat,0,64.0f);
lightSource firstLight=lightSource(ambLig1,difLig1,speLig1,posLig1,dirLig1);
lightSource secondLight=lightSource(ambLig2,difLig2,speLig2,posLig2,dirLig2);

void addBlock(sVertex center, vector<sVertex> &v)
{
	v.push_back(center);
}

// ������������ ��������� ������ ������� font
void renderBitmapString(float x, float y, void *font, char *string)
{
	char *c;
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

// ������� � ��������������� ��������
void setOrthographicProjection()
{
	// ������� � ����� ��������
	glMatrixMode(GL_PROJECTION);
	// ���������� ���������� �������
	glPushMatrix();
	// ����� �������
	glLoadIdentity();
	// ��������� 2D ��������������� ��������
	gluOrtho2D(0, w, h, 0);	
	// ������� � ����� ModelView
	glMatrixMode(GL_MODELVIEW);
}

// ������� � ������������� ��������
void resetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// ��������� fps
void drawFps()
{
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;
		frame = 0;
		//printf("%s\n",s);
	}

	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();
	renderBitmapString(20,35,GLUT_BITMAP_TIMES_ROMAN_24,s);
	glPopMatrix();
	resetPerspectiveProjection();
}

// ������� ���������� ��� ����������� ����
// � ��� ����� � �������������, �� �������� glutPostRedisplay
void display (void)
{
	// �������� ����� ����� � ����� �������
	int rst=glGetError();

	glClearColor(0.78125,0.78125,0.76171875,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);
	
	// ��������� ����
	// ������� ������� �����
	glLineWidth(0.5);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3,GL_FLOAT,sizeof(sVertexColor),&axes[0].sf_color);
	glVertexPointer(3,GL_FLOAT,sizeof(sVertexColor),&axes[0].sf_coordinate);
	glDrawArrays(GL_LINES,0,3*2);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_LIGHTING);

	// ����� ������ ��������� (������� � ���� �����)
	glPolygonMode(GL_FRONT_AND_BACK,/*GL_LINE*/GL_FILL);

	// ������������� ������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(0,30,20,0,0,0,0,1,0);
	glMultMatrixd(cam->getOrientation());
	// ������� ������
	glMultMatrixd(cam->getPosition());

	firstLight.use();
	secondLight.use(GL_LIGHT1);

	planeMaterial.cm_Use();
	planeShader.activate();

	glEnable(GL_TEXTURE_2D);
	glClientActiveTexture(GL_TEXTURE0);

	GLint texLoc = glGetUniformLocation(planeShader.getProgramId(), "MyTex");

	glUniform1i(texLoc,0);

	glTexEnvi ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	glColor3f(0.4f,0.8f,0.4f);
	glNormal3f(0.0,1.0,0.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	surfaceTex.bind();

	glTexCoordPointer(2,GL_FLOAT,0,surfaceTexCoord);
	glVertexPointer(3,GL_FLOAT,sizeof(sVertex),&surface[0].sf_coordinate);
	glDrawElements(GL_TRIANGLES,18*3,GL_UNSIGNED_INT,surfaceIndex);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);

	modelShader.activate();

	object->render(filterMode); // call to c3ds method for rendering

	shader::disactivate();

	glCullFace(GL_BACK);

	glColor3f(0.2f,0.2f,0.2f);
	glutWireSphere(0.07, 16, 16);
	
	if (isDrawingFps)
	{
		drawFps();
	}

	// ����� ��������� � ������� �������
	glutSwapBuffers();
};

// �������, ���������� ��� ��������� �������� ����
void reshape (int w, int h)
{
	::w=w; ::h=h;
	// ���������� ����� ������� ���������, ������ ���� ������� ����
	glViewport(0,0,(GLsizei)w, (GLsizei)h);

	// ���������� ������� �������� � ���������� ��������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(25.0,((float)w)/h,0.2,128.0);

	// ������������� ������������ ����
	glutPostRedisplay();
};

// ������� ���������� ����� ����������� ������ � ������������� ������ �����
void simulation(int)
{
	// ������������� ������������ ����
	glutPostRedisplay();
	// ��������� �������
	glutTimerFunc(delayPerFrames,simulation,0);
};

// ���������� ������� ������� ������
void processNormalKeys(unsigned char key, int x, int y)
{
	// ��������� ������������ ��������� ��������� ������
	HKL kbLayout=GetKeyboardLayout(0); // 0 - �������� �����
	// �������������� ASCII-������� � VK-������� �� �������� ���������
	int vkKey=VkKeyScanExA(key,kbLayout);
	// ��������� ��������� ������, ���������� �������� � ������������ � ��������
	switch(vkKey&0xff) // 0xff - ��� ������ ������ � ������� ������, �.�. � ������� ����� ��������� SHIFT, CTRL, ALT, Hankaku � ��.
	{
		case VK_ESCAPE:
		{
			exit(0);
			break;
		}
		case VK_D:
		{
			cam->moveRight();
			break;
		}
		case VK_A:
		{
			cam->moveLeft();
			break;
		}
		case VK_W:
		{
			cam->moveUp();
			break;
		}
		case VK_S:
		{
			cam->moveDown();
			break;
		}
		case VK_R:
		{
			cam->resetCamera();
			break;
		}
		case VK_T:
		{
			texture::changeMode();
		}
	}
	switch(key)
	{
		case '+':
		{
			cam->moveForward();
			break;
		}
		case '-':
		{
			cam->moveBack();
			break;
		}
		case '1':
		{
			if (isDrawingBack)
			{
				glEnable(GL_CULL_FACE);
			} else
			{
				glDisable(GL_CULL_FACE);
			}
			isDrawingBack=!isDrawingBack;
			break;
		}
	}
}

// ���������� ������� �������������� ������
void processSpecialKeys(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_F2 : 
				isDrawingFps=!isDrawingFps; break;
		case GLUT_KEY_PAGE_DOWN:
		{
			delayPerFrames=delayPerFrames<100?delayPerFrames+5:delayPerFrames;
			break;
		}
		case GLUT_KEY_PAGE_UP:
		{
			delayPerFrames=delayPerFrames>5?delayPerFrames-5:delayPerFrames;
			break;
		}
		case GLUT_KEY_LEFT:
		{
			cam->moveRight();
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			cam->moveLeft();
			break;
		}
		case GLUT_KEY_UP:
		{
			cam->moveUp();
			break;
		}
		case GLUT_KEY_DOWN:
		{
			cam->moveDown();
			break;
		}
	}
}

void onExit()
{
	if (_cerr.is_open())
	{
		_cerr.close();
	}
	delete cam;
	delete object;
	_3dsFile.empty();
}

#include <CommDlg.h>

std::wstring openFileNameW(wchar_t *a_filter = L"All Files (*.*)\0*.*\0", HWND a_owner = NULL)
{
	std::wstring _retStr;

	OPENFILENAMEW _openFileName;
	wchar_t _fileName[MAX_PATH]=L"";
	ZeroMemory(&_openFileName,sizeof(_openFileName));
 
	_openFileName.lStructSize=sizeof(OPENFILENAME);
	_openFileName.hwndOwner=a_owner;
	_openFileName.lpstrFilter=a_filter;
	_openFileName.lpstrFile=_fileName;
	_openFileName.nMaxFile=MAX_PATH;
	_openFileName.Flags=OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
	_openFileName.lpstrDefExt=L"3ds";
 
	if(GetOpenFileNameW(&_openFileName))
	{
		_retStr = _fileName;
	}
 
	return _retStr;
}

#include "3dsChunks.h"

bool check3ds(std::wstring a_fileName)
{
	bool _retVal=true;
	ifstream _ifs;
	_ifs.open(a_fileName.c_str(),ios_base::in|ios_base::binary|ios_base::beg);
	if(!_ifs.is_open())
	{
		std::cerr<<"�������� ������ (�� ������� ������� ����) ��� ������� ������ ����� "<<ns_3ds::wstringToString(a_fileName)<<std::endl;
		std::wcerr<<L"�������� ������ (�� ������� ������� ����) ��� ������� ������ ����� "<<a_fileName<<std::endl;
		// error while opening file
		_retVal=false;
	} else
	{
		ns_3ds::tChunkID _chunkID;
		_ifs.read((char *)&_chunkID,2);
		if (_chunkID!=ns_3ds::chunks::MAIN)
		{
			_retVal=false;
			std::cerr<<"�������� ������ (������������ ��������� ����) ��� ������� ������ ����� "<<ns_3ds::wstringToString(a_fileName)<<std::endl;
			std::wcerr<<L"�������� ������ (������������ ��������� ����) ��� ������� ������ ����� "<<a_fileName<<std::endl;
		}
		_ifs.close();
	}
	return _retVal;
}

void wmain (int argc, wchar_t **argv)
{
	// ��������� ������ ��� �������
	_wsetlocale(LC_ALL, L"");
	// �������� ����� ���� ������
	_cerr.open(L"~info.log",std::ios_base::beg|std::ios_base::out);
	if (_cerr.is_open())
	{
		std::cerr.rdbuf(_cerr.rdbuf());
	}

	// �������� ������� ������� ����������
	if (argc<2)
	{
		// ��� ������� ����������
		wcerr<<L"�� ������ ���� � ������ 3ds, ������� ���� � ������"<<endl;
		_3dsFile=openFileNameW();
		// ������� � �������
		//exit(-1);
	} else
	{
		// ������� ��� ��� ����� �������� � �������� ������� ���������
		_3dsFile=argv[1];
	}
	while (!check3ds(_3dsFile))
	{
		_3dsFile=openFileNameW();
	}
	wcerr<<L"����������� ���� ������ "<<_3dsFile<<endl;
	// ���������� ���������� ��� ������������� GLUT
	int _argc4GLUT=0;
	// ������������� ���������� GLUT
	glutInit(&_argc4GLUT,NULL);
	// ������������� ������� (������ ������)
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// �������� ����:
	// 1. ������������� ������� ����� ���� ����
	glutInitWindowPosition(200,200);
	// 2. ������������� ������ ����
	glutInitWindowSize(600,600);
	// 3. ������� ����
	glutCreateWindow("Anytime, anywhere�");
	// ������������� ���������� ������ (��� ������������ ���������� ������)
	atexit(onExit);
	// ��������� � ������������� �����
	//glutFullScreen();

	cam=new camera();
	
	hasVBO=glutExtensionSupported("GL_ARB_vertex_buffer_object")!=0; // check if VBO supported
	hasFragmentShader=glutExtensionSupported("GL_ARB_fragment_shader")!=0; // check if fragment shader is supported
	hasVertexShader=glutExtensionSupported("GL_ARB_vertex_shader")!=0; // check if fragment shader is supported
	newOGL=atof((const char *)glGetString(GL_VERSION))>=1.5f; // check OGL version

	// ��������� ������� ����������� ����������
	ns_3ds::c3ds::checkExtensions();
	shader::checkExtensions();

	if (!hasVBO) { printf("VBO does not supported\n"); } // no VBO
	if (!hasFragmentShader) { printf("Fragment shaders not supported\n"); } // no fragment shader
	if (!hasVertexShader) { printf("Vertex shaders not supported\n"); } // no vertex shader
	if (!newOGL) { printf("OLG 2.0 functions not supported\n"); } // old OGL

	ilInit();
	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		wprintf(L"Devil Error (ilInit): %s\n", iluErrorString(devilError));
		exit(2);
	}
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	// �������� �������� �� ����� ������������
	object=new ns_3ds::c3ds();
	object->load(_3dsFile);
	
	object->buffer(); // buffer objects to VBO

	modelShader.loadVertexShader("Phong.vsh");
	modelShader.loadFragmentShader("Phong.fsh");
	modelShader.createProgram();

	planeShader.loadVertexShader("Phong.vsh");
	planeShader.loadFragmentShader("Phong.l3.fsh");
	planeShader.createProgram();

	// 4. ������������� �������, ������� ����� ���������� ��� ����������� ����
	glutDisplayFunc(display);
	// 5. ������������� �������, ������� ����� ���������� ��� ��������� �������� ����
	glutReshapeFunc(reshape);
	// 6. ������������� ������ ��� ����������� ���� �� �������
	glutTimerFunc(delayPerFrames,simulation,0);
	// 7. ������������� ���������� ������� ������
	glutKeyboardFunc(processNormalKeys);
	// 8. ������������� ���������� �������������� ������
	glutSpecialFunc(processSpecialKeys);

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	surfaceTex.load(L"textures\\ceram.jpg");

	// ����� ����� ����������� OpenGL
	// ����� �������������
	std::cerr<<"=================================="<<std::endl;
	std::cerr<<"���������� � �������������"<<std::endl;
	std::cerr<<"�������������: "<<glGetString(GL_VENDOR)<<std::endl;
	std::cerr<<"������: "<<glGetString(GL_RENDERER)<<std::endl;
	std::cerr<<"������ OGL: "<<glGetString(GL_VERSION)<<std::endl;
	// ����� ������ ���������� �����
	std::cerr<<"������ GLSL: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;
	// ����� ���� ��������� ����������
	char *ext=(char*)glGetString(GL_EXTENSIONS);
	int i=0;
	std::cerr<<"=================================="<<std::endl;
	std::cerr<<"���������� � �������������� �����������"<<std::endl;
	while (ext[i]!=0)
	{
		if (ext[i++]==' ')
		{
			std::cerr<<std::endl;
		} else
		{
			std::cerr<<ext[i-1];
		}
	}
	
	// �������� ���� ��������� ��������� ��
	glutMainLoop();
	return;
};