//#define _HAS_ITERATOR_DEBUGGING 0
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
//libconfig
#include "libconfig/libconfig.h++"
//self implemented
#include "stdafx.h"
#include "camera.h"
#include "vkscancodes.h"
#include "vertex.h"
#include "object3ds.h"
#include "shader.h"
#include "texture.h"

using namespace glm;
using namespace std;
using namespace libconfig;

int frame=0,time,timebase=0,w,h,delayPerFrames=20,filterMode=0,g_nMaxAnisotropy;
char s[20];
bool isDrawingFps=false, isDrawingBack=false;
vertex surface[]={
	vertex(-1.5*surfaceSpace,0,-1.5*surfaceSpace),
	vertex(-1.5*surfaceSpace,0,-0.5*surfaceSpace),
	vertex(-1.5*surfaceSpace,0,0.5*surfaceSpace),
	vertex(-1.5*surfaceSpace,0,1.5*surfaceSpace),

	vertex(-0.5*surfaceSpace,0,-1.5*surfaceSpace),
	vertex(-0.5*surfaceSpace,0,-0.5*surfaceSpace),
	vertex(-0.5*surfaceSpace,0,0.5*surfaceSpace),
	vertex(-0.5*surfaceSpace,0,1.5*surfaceSpace),

	vertex(0.5*surfaceSpace,0,-1.5*surfaceSpace),
	vertex(0.5*surfaceSpace,0,-0.5*surfaceSpace),
	vertex(0.5*surfaceSpace,0,0.5*surfaceSpace),
	vertex(0.5*surfaceSpace,0,1.5*surfaceSpace),

	vertex(1.5*surfaceSpace,0,-1.5*surfaceSpace),
	vertex(1.5*surfaceSpace,0,-0.5*surfaceSpace),
	vertex(1.5*surfaceSpace,0,0.5*surfaceSpace),
	vertex(1.5*surfaceSpace,0,1.5*surfaceSpace),
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
vertex axes[]={
	// красная ось Ox
	vertex(-surfaceSpace/2,0.1,0.0,1.0,0.0,0.0),
	vertex(surfaceSpace,0.1, 0.0, 1.0,0.0,0.0),
	// зеленая ось Oz
	vertex(0.0,0.1,-surfaceSpace/2,0.0,1.0,0.0),
	vertex(0.0,0.1,surfaceSpace,0.0,1.0,0.0),
	// синяя ось Oy
	vertex(0.0,-surfaceSpace/2,0.0,0.0,0.0,1.0),
	vertex(0.0,surfaceSpace,0.0,0.0,0.0,1.0),
};

vertex block[]={
	vertex(-halfWi, -halfWi, +halfWi),
	vertex(-halfWi, +halfWi, +halfWi),
	vertex(-halfWi, +halfWi, -halfWi),
	vertex(-halfWi, -halfWi, -halfWi),
	
	vertex(+halfWi, -halfWi, +halfWi),
	vertex(+halfWi, +halfWi, +halfWi),
	vertex(+halfWi, +halfWi, -halfWi),
	vertex(+halfWi, -halfWi, -halfWi),
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

vector<object3DS *> objects;
bool newOGL=false,hasVBO=false,hasFragmentShader=false,hasVertexShader=false;
Config *cfg;
camera *cam;

shader modelShader,planeShader;
material planeMaterial=material(ambMat,difMat,speMat,0,64.0f);
lightSource firstLight=lightSource(ambLig1,difLig1,speLig1,posLig1,dirLig1);
lightSource secondLight=lightSource(ambLig2,difLig2,speLig2,posLig2,dirLig2);

void addBlock(vertex center, vector<vertex> &v)
{
	v.push_back(center);
}

// посимвольная отрисовка строки шрифтом font
void renderBitmapString(float x, float y, void *font, char *string)
{
	char *c;
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

// переход к ортографической проекции
void setOrthographicProjection()
{
	// переход в режим проекции
	glMatrixMode(GL_PROJECTION);
	// сохранение предыдущей матрицы
	glPushMatrix();
	// сброс матрицы
	glLoadIdentity();
	// установка 2D ортографической проекции
	gluOrtho2D(0, w, h, 0);	
	// переход в режим ModelView
	glMatrixMode(GL_MODELVIEW);
}

// возврат к перспективной проекции
void resetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// отрисовка fps
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

// функция вызывается при перерисовке окна
// в том числе и принудительно, по командам glutPostRedisplay
void display (void)
{
	// отчищаем буфер цвета и буфер глубины
	int rst=glGetError();

	glClearColor(0.78125,0.78125,0.76171875,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);
	
	// отрисовка осей
	// задание толщины линии
	glLineWidth(0.5);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3,GL_FLOAT,sizeof(vertex),&axes[0].color);
	glVertexPointer(3,GL_FLOAT,sizeof(vertex),&axes[0].coordinate);
	glDrawArrays(GL_LINES,0,3*2);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_LIGHTING);

	// режим вывода полигонов (выводим в виде линии)
	glPolygonMode(GL_FRONT_AND_BACK,/*GL_LINE*/GL_FILL);

	// устанавливаем камеру
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(0,30,20,0,0,0,0,1,0);
	glMultMatrixd(cam->getOrientation());
	// позиция камеры
	glMultMatrixd(cam->getPosition());

	firstLight.use();
	secondLight.use(GL_LIGHT1);

	planeMaterial.use();
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
	glVertexPointer(3,GL_FLOAT,sizeof(vertex),&surface[0].coordinate);
	glDrawElements(GL_TRIANGLES,18*3,GL_UNSIGNED_INT,surfaceIndex);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);

	modelShader.activate();

	for(unsigned int i=0; i<objects.size(); i++)
	{
		objects[i]->render(filterMode); // call to object3ds method for rendering
	}

	shader::disactivate();

	glCullFace(GL_BACK);

	glColor3f(0.2f,0.2f,0.2f);
	glutWireSphere(0.07, 16, 16);
	
	if (isDrawingFps)
	{
		drawFps();
	}

	// смена переднего и заднего буферов
	glutSwapBuffers();
};

// функция, вызываемая при изменении размеров окна
void reshape (int w, int h)
{
	::w=w; ::h=h;
	// установить новую область просмотра, равную всей области окна
	glViewport(0,0,(GLsizei)w, (GLsizei)h);

	// установить матрицу проекции с правильным аспектом
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(25.0,((float)w)/h,0.2,128.0);

	// принудительно перерисовать окно
	glutPostRedisplay();
};

// функция вызывается когда срабатывает таймер и устанавливает таймер вновь
void simulation(int)
{
	// принудительно перерисовать окно
	glutPostRedisplay();
	// установка таймера
	glutTimerFunc(delayPerFrames,simulation,0);
};

// обработчик нажатия обычных клавиш
void processNormalKeys(unsigned char key, int x, int y)
{
	// получение клавиатурной раскладки заданного потока
	HKL kbLayout=GetKeyboardLayout(0); // 0 - активный поток
	// преобразование ASCII-символа в VK-сканкод по заданной раскладке
	int vkKey=VkKeyScanExA(key,kbLayout);
	// отсечение состояний клавиш, выполнение действий в соответствии с нажатием
	switch(vkKey&0xff) // 0xff - для работы только с младшим байтом, т.к. в старшем будет состояние SHIFT, CTRL, ALT, Hankaku и др.
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

// обработчик нажатия функциональных клавиш
void processSpecialKeys(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_F2 : 
				isDrawingFps=!isDrawingFps; break;
		case GLUT_KEY_F5 : 
			cam->loadCamera(cfg); break;
		case GLUT_KEY_F6 : 
			cam->saveCamera(); break;
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

bool loadObjects(vector<object3DS *> &objs)
{
	Setting &objects=cfg->lookup("application.objects");
	int count=objects.getLength();
	glActiveTexture(GL_TEXTURE0);
	texture *tex=NULL;
	object3DS *object=NULL;
	for (int i=0; i<count; i++)
	{
		vertex _tempVertex;
		string _tempStr;
		objects[i]["color"].lookupValue("r",_tempVertex.color[0]);
		objects[i]["color"].lookupValue("g",_tempVertex.color[1]);
		objects[i]["color"].lookupValue("b",_tempVertex.color[2]);
		objects[i]["position"].lookupValue("x",_tempVertex.coordinate[0]);
		objects[i]["position"].lookupValue("y",_tempVertex.coordinate[1]);
		objects[i]["position"].lookupValue("z",_tempVertex.coordinate[2]);
		objects[i].lookupValue("texture",_tempStr);
		tex=new texture();
		object=new object3DS();
		wchar_t *_tempBuf=new wchar_t[_tempStr.size()];
		int _len=mbstowcs(_tempBuf,_tempStr.c_str(),_tempStr.size());
		wstring _tempWStr(_tempBuf,_len);
		delete []_tempBuf;
		tex->load(_tempWStr,true,true,true);
		objects[i].lookupValue("model",_tempStr);
		_tempBuf=new wchar_t[_tempStr.size()];
		_len=mbstowcs(_tempBuf,_tempStr.c_str(),_tempStr.size());
		_tempWStr.clear();
		_tempWStr=wstring(_tempBuf,_len);
		delete []_tempBuf;
		object->load(_tempWStr);
		_tempWStr.clear();
		object->setPosition(_tempVertex);
		object->setScale(1.0f);
		if (tex!=NULL)
		{
			object->copyTex(tex);
		}
		objs.push_back(object);
	}
	return true;
	//vertex center,tempVertex;
	//bool hasReaded=false;
	//wifstream configFile("config.ini", ios::in);
	//if (!configFile)
	//{
	//	cerr<<"Config file unavailable"<<endl;
	//	return false;
	//}
	//wstring tmp,file;
	//wchar_t c;
	//int kb,kc,kf,ks,kt,posBegin,posEnd;
	//GLfloat scale=1.0f,temp;
	//texture *tex=NULL;
	//object3DS *object=new object3DS();
	//while (!configFile.eof())
	//{
	//	tmp.clear();
	//	configFile>>c;
	//	while (!configFile.eof()&&(c!='\n'))
	//	{
	//		tmp+=c;
	//		configFile.get((wchar_t &)c);
	//	}
	//	try
	//	{
	//		kb=swscanf(tmp.c_str(),L"V = (%f, %f, %f)", &tempVertex.coordinate[0], &tempVertex.coordinate[1], &tempVertex.coordinate[2]);
	//		if (kb==3)
	//		{
	//			center.coordinate[0]=tempVertex.coordinate[0]; center.coordinate[1]=tempVertex.coordinate[1]; center.coordinate[2]=tempVertex.coordinate[2];
	//		}
	//		kc=swscanf(tmp.c_str(),L"C = (%f, %f, %f)", &tempVertex.color[0], &tempVertex.color[1], &tempVertex.color[2]);
	//		if (kc==3)
	//		{
	//			center.color[0]=tempVertex.color[0]; center.color[1]=tempVertex.color[1]; center.color[2]=tempVertex.color[2];
	//		}
	//		kt=swscanf(tmp.c_str(),L"T = \"%c\"", &c);
	//		if (kt==1)
	//		{
	//			posBegin=tmp.find(L"T = \"")+5;
	//			posEnd=tmp.rfind(L'"');
	//			file=tmp.substr(posBegin,posEnd-posBegin);
	//			glActiveTexture(GL_TEXTURE0);
	//			
	//			if (tex)
	//			{
	//				delete tex;
	//				tex=NULL;
	//			};
	//			tex=new texture();
	//			tex->load(file,true,true,true);
	//		}
	//		ks=swscanf(tmp.c_str(),L"S = (%f)", &temp);
	//		if (ks==1)
	//		{
	//			scale=temp;
	//		};
	//		kf=swscanf(tmp.c_str(),L"M = \"%c\"", &c);
	//		if (kf==1)
	//		{
	//			posBegin=tmp.find(L"M = \"")+5;
	//			posEnd=tmp.rfind(L'"');
	//			file=tmp.substr(posBegin,posEnd-posBegin);
	//			object->load(file);
	//			object->setPosition(center);
	//			object->setScale(scale);
	//			if (tex!=NULL)
	//			{
	//				object->copyTex(tex);
	//			}
	//			tex=NULL;
	//			objs.push_back(object);
	//			//delete object;
	//			object=new object3DS();
	//			scale=1.0f;
	//		}
	//	}catch (...)
	//	{
	//	}
	//}
	//if (object)
	//{
	//	delete object;
	//}
	//if (tex)
	//{
	//	delete tex;
	//}
	//return true;
}

void onExit()
{
	for (int i=0; i<objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
	delete cam;
	delete []cfg;
}

void main (int argc,char **argv)
{
	setlocale(LC_CTYPE, ""); //making autoconverting for non cp866 symbols
	// инициализация библиотеки GLUT
	glutInit(&argc,argv);
	// инициализация дисплея (формат вывода)
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// создание окна:
	// 1. устанавливаем верхний левый угол окна
	glutInitWindowPosition(200,200);
	// 2. устанавливаем размер окна
	glutInitWindowSize(600,600);
	// 3. создаем окно
	glutCreateWindow("Anytime, anywhere…");
	// устанавливаем обработчик выхода (для освобождения выделенной памяти)
	atexit(onExit);
	// переходим в полноэкранный режим
	//glutFullScreen();

	cfg=new Config();
	try
	{
		cfg->readFile(cfgPath.c_str());
	} catch (...)
	{
		printf("Config doesn't parsed\n");
	}

	// очистка объектов
	objects.clear();
	cam=new camera();
	// загрузка камеры
	cam->loadCamera(cfg);
	
	hasVBO=glutExtensionSupported("GL_ARB_vertex_buffer_object")!=0; // check if VBO supported
	hasFragmentShader=glutExtensionSupported("GL_ARB_fragment_shader")!=0; // check if fragment shader is supported
	hasVertexShader=glutExtensionSupported("GL_ARB_vertex_shader")!=0; // check if fragment shader is supported
	newOGL=atof((const char *)glGetString(GL_VERSION))>=1.5f; // check OGL version

	// проверяем наличие необходимых расширений
	object3DS::checkExtensions();
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

	// загрузка объектов по файлу конфигурации
	loadObjects(objects);

	for (unsigned int i=0; i<objects.size(); i++)
	{
		objects[i]->buffer(); // buffer objects to VBO
	}

	modelShader.loadVertexShader("Phong.vsh");
	modelShader.loadFragmentShader("Phong.fsh");
	modelShader.createProgram();

	planeShader.loadVertexShader("Phong.vsh");
	planeShader.loadFragmentShader("Phong.l3.fsh");
	planeShader.createProgram();

	// 4. устанавливаем функцию, которая будет вызываться для перерисовки окна
	glutDisplayFunc(display);
	// 5. устанавливаем функцию, которая будет вызываться при изменении размеров окна
	glutReshapeFunc(reshape);
	// 6. устанавливаем таймер для перерисовки окна по времени
	glutTimerFunc(delayPerFrames,simulation,0);
	// 7. устанавливаем обработчик обычных клавиш
	glutKeyboardFunc(processNormalKeys);
	// 8. устанавливаем обработчик функциональных клавиш
	glutSpecialFunc(processSpecialKeys);

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	surfaceTex.load(L"textures\\ceram.jpg");

	// вывод строк описывающих OpenGL
	// вывод производителя
	printf("%s\n",glGetString(GL_VENDOR));
	printf("%s\n",glGetString(GL_RENDERER));
	printf("%s\n",glGetString(GL_VERSION));
	// вывод версии шейдерного языка
	printf("%s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));
	// вывод всех доступных расширений
	char *ext=(char*)glGetString(GL_EXTENSIONS);
	int i=0;
	while (ext[i]!=0)
		printf("%c",ext[i++]==' '?'\n':ext[i-1]);
	
	// основной цикл обработки сообщений ОС
	glutMainLoop();
	return;
};