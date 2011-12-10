#ifndef __3ds_h__
#define __3ds_h__

//Glee
#include "glee.h"
//VC++
#include <string>
#include <vector>
#include <map>
#include <hash_map>
//glm
#include "glm/glm.hpp"
//glut
#include "glut.h"
//self implemented
#include "vertex.h"
#include "3dsTypes.h"
#include "3dsLoader.h"
#include "3dsVertex.h"
#include "3dsTextureDevIL.h"

#include "3dsCamera.h"

using namespace std;
using namespace glm;

namespace ns_3ds
{
	class c3dsMaterial;
	class c3dsObject;
	//class c3dsCamera;

	class c3ds
	{
		friend class ns_3ds::c3dsLoader;
		friend class ns_3ds::c3dsObject;
	private:
		// версия 3ds
		t3dsVersion cf_version;
		// версия меша
		t3dsVersion cf_meshVersion;
		// материалы
		hash_map<string, c3dsMaterial *> cf_material;
		// величина единицы
		tFloat cf_unit;
		// масштаб
		tDouble cf_scale;
		// матрица масштабирования
		tFloat cf_scaleMatrix[16];
		// объекты
		vector<c3dsObject *> cf_object;
		static bool newOGL, hasVBO;
		// инициализация
		void init();
		c3dsCamera cf_camera;
	public:
		// конструктор по умолчанию
		c3ds();
		// конструктор по заданной позиции
		c3ds(sVertex &pos);
		// деструктор
		~c3ds();
		// загрузка файла 3ds и запись в буферы VBO
		bool load(wstring fname);
		// вывод объекта на экран
		void render(int filterMode);
		// буферизация в VBO
		void buffer();
		// установка масштаба
		void cm_SetScale(tDouble a_scale);
		// увеличение размера
		void cm_UpScale();
		// уменьшение размера
		void cm_DownScale();
		// проверка расширений
		static void checkExtensions();
		c3dsCamera * cm_GetCamera()
		{
			return &cf_camera;
		}
	};
}

#include "3dsMaterial.h"
#include "3dsObject.h"

#endif