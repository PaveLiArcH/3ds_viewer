//#define _HAS_ITERATOR_DEBUGGING 0

// catch mem leaks
//#include <vld.h>
//Glee
#include "glee.h"
//VC++
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <clocale>
#include <wchar.h>
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
#include "camera.h"
#include "vkscancodes.h"
#include "vertex.h"
#include "object3ds.h"
#include "shader.h"
#include "texture.h"

#ifndef __main_h__
#define __main_h__

using namespace glm;
using namespace libconfig;

#define root2 0.70710678118654752440084436210485f

const static string cfgPath="config.cfg";

#endif