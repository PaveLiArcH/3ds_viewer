#ifndef __stdafx_h__
#define __stdafx_h__

// catch mem leaks
//#include <vld.h>
//VC++
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <clocale>
#include <wchar.h>

using namespace std;

#include "3dsTypes.h"

#pragma region ValuesDefinitions
#define root2 0.70710678118654752440084436210485f
#define surfaceSpace 5
#define halfWi 1
#pragma endregion Определение значений

extern int _total_frustumed;

#endif