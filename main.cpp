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
#include <iomanip>
//self implemented
#include "stdafx.h"
#include "vkscancodes.h"
#include "3ds.h"
#include "shader.h"

#include "3dsMaterial.h"
#include "3dsVertex.h"
#include "3dsTextureDevIL.h"
#include "3dsLightingSource.h"

using ns_3ds::sVertexColor;

std::ofstream _cerr;

// переменная для чтения пути к модели
std::wstring _3dsFile;

int _total_frustumed=0, _total_occluded=0,frame=0,_time,timebase=0,w,h,delayPerFrames=20,filterMode=0,texturingEnabled=0,_total_drawed=0;
int _oldW=600, _oldH=600, _oldX=200, _oldY=200,_frames_drawed=0;
std::string _str;
double _lastFps=0;
bool isDrawingFps=true, isDrawingBack=true, isFullScreen=false, isDirty=true;
bool _overlaySupport=false;

GLfloat *surfaceIndexPoint,*surfaceNormal;
sVertexColor axes[]={
	// красная ось Ox
	sVertexColor(-surfaceSpace/2,0.1,0.0,1.0,0.0,0.0),
	sVertexColor(surfaceSpace,0.1, 0.0, 1.0,0.0,0.0),
	// зеленая ось Oz
	sVertexColor(0.0,0.1,-surfaceSpace/2,0.0,1.0,0.0),
	sVertexColor(0.0,0.1,surfaceSpace,0.0,1.0,0.0),
	// синяя ось Oy
	sVertexColor(0.0,-surfaceSpace/2,0.0,0.0,0.0,1.0),
	sVertexColor(0.0,surfaceSpace,0.0,0.0,0.0,1.0),
};

ILuint devilError;

ns_3ds::c3ds *object;
bool newOGL=false,hasVBO=false,hasFragmentShader=false,hasVertexShader=false;

shader modelShader;

// посимвольная отрисовка строки шрифтом font
void renderBitmapString(float x, float y, void *font, char *string)
{
	glRasterPos2f(x, y);
	for (char *c=string; *c!='\0'; c++)
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
	_time=glutGet(GLUT_ELAPSED_TIME);
	if (_time - timebase > 1000)
	{
		_lastFps=frame*1000.0/(_time-timebase);
		timebase = _time;
		frame = 0;
		//printf("%s\n",s);
	}
	std::ostringstream _stream;
	_stream.precision(3);
	_stream<<"FPS: "<<_lastFps<<" frustumed: "<<_total_frustumed<<" occluded: "<<_total_occluded<<" drawed: "<<_total_drawed;
	_str=_stream.str();
	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_INVERT);
	renderBitmapString(5,20,GLUT_BITMAP_9_BY_15,const_cast<char *>(_str.c_str()));
	glDisable(GL_COLOR_LOGIC_OP);
	glPopMatrix();
	resetPerspectiveProjection();
}

// функция вызывается при перерисовке окна
// в том числе и принудительно, по командам glutPostRedisplay
void display (void)
{
	_total_occluded=0;
	_total_drawed=0;
	_frames_drawed++;
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
	glColorPointer(3,GL_FLOAT,sizeof(sVertexColor),&axes[0].sf_color);
	glVertexPointer(3,GL_FLOAT,sizeof(sVertexColor),&axes[0].sf_coordinate);
	glDrawArrays(GL_LINES,0,3*2);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_LIGHTING);

	// режим вывода полигонов (выводим в виде линии)
	glPolygonMode(GL_FRONT_AND_BACK,/*GL_LINE*/GL_FILL);

	// устанавливаем камеру
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMultMatrixd(object->cm_GetCamera()->getOrientation());
	// позиция камеры
	glMultMatrixd(object->cm_GetCamera()->getPosition());

	//firstLight.use();
	//secondLight.use(GL_LIGHT1);

	bool shaderOk=modelShader.activate();

	if (shaderOk)
	{
		texturingEnabled=glGetUniformLocation(modelShader.getProgramId(),"texturingEnabled");
	} else
	{
		texturingEnabled=0;
	}

	object->render(filterMode); // call to c3ds method for rendering

	shader::disactivate();

	glCullFace(GL_BACK);

	if (isDrawingFps)
	{
		drawFps();
	}

	// смена переднего и заднего буферов
	glutSwapBuffers();
	isDirty=false;
};

// функция, вызываемая при изменении размеров окна
void reshape (int w, int h)
{
	isDirty=true;
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

// функция вызывается когда начинается фаза простоя
void onIdle()
{
	// принудительно перерисовать окно
	int _layerDamaged=glutLayerGet(GLUT_NORMAL_DAMAGED);
	int _overlayDamaged=glutLayerGet(GLUT_OVERLAY_DAMAGED);
	if (isDirty||(_frames_drawed<8)||(_layerDamaged==GL_TRUE)||(_overlayDamaged==GL_TRUE))
	{
		if (isDirty)
		{
			_frames_drawed=0;
		}
		//isDirty=false;
		glutPostRedisplay();
	} else
	{
		glutSwapBuffers();
		_frames_drawed--;
		_frames_drawed--;
		frame++;
		_time=glutGet(GLUT_ELAPSED_TIME);
		if (_time - timebase > 1000)
		{
			_lastFps=frame*1000.0/(_time-timebase);
			timebase = _time;
			frame = 0;
			//printf("%s\n",s);
		}
	}
	if (isDrawingFps)
	{
		//drawFps();
	}
};

// обработчик нажатия обычных клавиш
void processNormalKeys(unsigned char key, int x, int y)
{
	isDirty=true;
	// получение клавиатурной раскладки заданного потока
	HKL kbLayout=GetKeyboardLayout(0); // 0 - активный поток
	// преобразование ASCII-символа в VK-сканкод по заданной раскладке
	int vkKey=VkKeyScanExA(key,kbLayout);
	int _modifiers=glutGetModifiers();
	// отсечение состояний клавиш, выполнение действий в соответствии с нажатием
	switch(vkKey&0xff) // 0xff - для работы только с младшим байтом, т.к. в старшем будет состояние SHIFT, CTRL, ALT, Hankaku и др.
	{
		case VK_ESCAPE:
		{
			exit(0);
			break;
		}
		case VK_TAB:
		{
			object->cm_GetCamera()->cm_NextCamera();
			break;
		}
		case VK_D:
		{
			object->cm_GetCamera()->moveRight();
			break;
		}
		case VK_A:
		{
			object->cm_GetCamera()->moveLeft();
			break;
		}
		case VK_W:
		{
			object->cm_GetCamera()->moveUp();
			break;
		}
		case VK_S:
		{
			object->cm_GetCamera()->moveDown();
			break;
		}
		case VK_R:
		{
			object->cm_GetCamera()->resetCamera();
			break;
		}
		case VK_T:
		{
			ns_3ds::c3dsTextureDevIL::changeMode();
			break;
		}
		case VK_L:
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
		case VK_RETURN:
		{
			if (_modifiers&GLUT_ACTIVE_ALT)
			{
				isFullScreen=!isFullScreen;
				if(isFullScreen)
				{
					_oldX=glutGet(GLUT_WINDOW_X);
					_oldY=glutGet(GLUT_WINDOW_Y);
					_oldW=glutGet(GLUT_WINDOW_WIDTH);
					_oldH=glutGet(GLUT_WINDOW_HEIGHT);
					glutFullScreen();
				} else
				{
					glutPositionWindow(_oldX,_oldY);
					glutReshapeWindow(_oldW,_oldH);
				}
			}
		}
	}
	switch(key)
	{
		case '+':
		{
			object->cm_GetCamera()->moveForward();
			break;
		}
		case '-':
		{
			object->cm_GetCamera()->moveBack();
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
	isDirty=true;
	int _modifiers=glutGetModifiers();
	switch(key)
	{
		case GLUT_KEY_F2 : 
				isDrawingFps=!isDrawingFps; break;
		case GLUT_KEY_PAGE_DOWN:
		{
			if (_modifiers & GLUT_ACTIVE_CTRL)
			{
				object->cm_DownScale();
				object->cm_GetCamera()->applyCamera();
			} else
			{
				delayPerFrames=delayPerFrames<100?delayPerFrames+5:delayPerFrames;
			}
			break;
		}
		case GLUT_KEY_PAGE_UP:
		{
			if (_modifiers & GLUT_ACTIVE_CTRL)
			{
				object->cm_UpScale();
				object->cm_GetCamera()->applyCamera();
			} else
			{
				delayPerFrames=delayPerFrames>5?delayPerFrames-5:delayPerFrames;
			}
			break;
		}
		case GLUT_KEY_LEFT:
		{
			object->cm_GetCamera()->moveRight();
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			object->cm_GetCamera()->moveLeft();
			break;
		}
		case GLUT_KEY_UP:
		{
			object->cm_GetCamera()->moveUp();
			break;
		}
		case GLUT_KEY_DOWN:
		{
			object->cm_GetCamera()->moveDown();
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
		std::cerr<<"Возникли ошибки (не удалось открыть файл) при попытке чтения файла \""<<ns_3ds::wstringToString(a_fileName)<<'"'<<std::endl;
		std::wcerr<<L"Возникли ошибки (не удалось открыть файл) при попытке чтения файла \""<<a_fileName<<'"'<<std::endl;
		// error while opening file
		_retVal=false;
	} else
	{
		ns_3ds::tChunkID _chunkID;
		_ifs.read((char *)&_chunkID,2);
		if (_chunkID!=ns_3ds::chunks::MAIN)
		{
			_retVal=false;
			std::cerr<<"Возникли ошибки (некорректный стартовый чанк) при попытке чтения файла \""<<ns_3ds::wstringToString(a_fileName)<<'"'<<std::endl;
			std::wcerr<<L"Возникли ошибки (некорректный стартовый чанк) при попытке чтения файла \""<<a_fileName<<'"'<<std::endl;
		}
		_ifs.close();
	}
	return _retVal;
}

void wmain (int argc, wchar_t **argv)
{
	// установка локали для консоли
	_wsetlocale(LC_ALL, L"");
	// создание файла лога работы
	_cerr.open(L"~info.log",std::ios_base::beg|std::ios_base::out);
	if (_cerr.is_open())
	{
		std::cerr.rdbuf(_cerr.rdbuf());
	}

	// проверка наличия входных аргументов
	if (argc<2)
	{
		// нет входных аргументов
		wcerr<<L"Не указан путь к модели 3ds, укажите путь к модели"<<endl;
		_3dsFile=openFileNameW();
		// выходим с ошибкой
		//exit(-1);
	} else
	{
		// считаем что имя файла передано в качестве первого аргумента
		_3dsFile=argv[1];
	}
	while (!check3ds(_3dsFile))
	{
		_3dsFile=openFileNameW();
	}
	// количество аргументов для инициализации GLUT
	int _argc4GLUT=0;
	// инициализация библиотеки GLUT
	glutInit(&_argc4GLUT,NULL);
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
	
	hasVBO=glutExtensionSupported("GL_ARB_vertex_buffer_object")!=0; // check if VBO supported
	hasFragmentShader=glutExtensionSupported("GL_ARB_fragment_shader")!=0; // check if fragment shader is supported
	hasVertexShader=glutExtensionSupported("GL_ARB_vertex_shader")!=0; // check if fragment shader is supported
	newOGL=atof((const char *)glGetString(GL_VERSION))>=1.5f; // check OGL version

	// проверяем наличие необходимых расширений
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

	// загрузка объектов по файлу конфигурации
	object=new ns_3ds::c3ds();
	wcerr<<L"Загрузка файла модели \""<<_3dsFile<<'"'<<endl;
	object->load(_3dsFile);
	
	object->buffer(); // buffer objects to VBO

	std::cerr<<"=================================="<<std::endl;
	std::cerr<<"Загрузка текстов шейдеров"<<std::endl;
	std::wcerr<<L"Загрузка текстов шейдеров"<<std::endl;

	modelShader.loadVertexShader("Phong.vsh");
	modelShader.loadFragmentShader("Phong.fsh");
	modelShader.createProgram();

	_overlaySupport=glutLayerGet(GLUT_OVERLAY_POSSIBLE)==GL_TRUE;

	// 4. устанавливаем функцию, которая будет вызываться для перерисовки окна
	glutDisplayFunc(display);
	// 5. устанавливаем функцию, которая будет вызываться при изменении размеров окна
	glutReshapeFunc(reshape);
	//// 6. устанавливаем таймер для перерисовки окна по времени
	//glutTimerFunc(delayPerFrames,simulation,0);
	// 6. устанавливаем режим работы с максимально возможным fps
	glutIdleFunc(onIdle);
	// 7. устанавливаем обработчик обычных клавиш
	glutKeyboardFunc(processNormalKeys);
	// 8. устанавливаем обработчик функциональных клавиш
	glutSpecialFunc(processSpecialKeys);

	glDisable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	// вывод строк описывающих OpenGL
	// вывод производителя
	std::cerr<<"=================================="<<std::endl;
	std::cerr<<"Информация о производителе"<<std::endl;
	std::cerr<<"Производитель: "<<glGetString(GL_VENDOR)<<std::endl;
	std::cerr<<"Рендер: "<<glGetString(GL_RENDERER)<<std::endl;
	std::cerr<<"Версия OGL: "<<glGetString(GL_VERSION)<<std::endl;
	// вывод версии шейдерного языка
	std::cerr<<"Версия GLSL: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;
	// вывод всех доступных расширений
	char *ext=(char*)glGetString(GL_EXTENSIONS);
	int i=0;
	std::cerr<<"=================================="<<std::endl;
	std::cerr<<"Информация о поддерживаемых расширениях"<<std::endl;
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
	
	// основной цикл обработки сообщений ОС
	glutMainLoop();
	return;
};