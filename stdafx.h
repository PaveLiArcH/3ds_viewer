#ifndef __stdafx_h__
#define __stdafx_h__

// catch mem leaks
//#include <vld.h>
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

using namespace std;

#pragma region TypeDefinitions
#define floatType GLfloat
#define intType GLint
#define uintType GLuint
#pragma endregion Определение типов

#pragma region ValuesDefinitions
#define root2 0.70710678118654752440084436210485f
#define surfaceSpace 5
#define halfWi 1
#pragma endregion Определение значений

const static string cfgPath="config.cfg";

#endif