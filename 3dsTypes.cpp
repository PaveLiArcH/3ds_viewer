#include "3dsTypes.h"

namespace ns_3ds
{
	std::string wstringToString(std::wstring a_string)
	{
		std::string _retStr="";
		std::size_t _sizeA=wcstombs(NULL,a_string.c_str(),0);
		if (_sizeA>=0)
		{
			char *_szName=new char[_sizeA+1];
			wcstombs(_szName,a_string.c_str(),_sizeA+1);
			_szName[_sizeA]='\0';
			_retStr=_szName;
			delete []_szName;
		}
		return _retStr;
	}

	std::wstring stringToWstring(std::string a_string)
	{
		std::wstring _retStr=L"";
		std::size_t _sizeW=mbstowcs(NULL,a_string.c_str(),0);
		if (_sizeW>=0)
		{
			wchar_t *_szName=new wchar_t[_sizeW+1];
			mbstowcs(_szName,a_string.c_str(),_sizeW+1);
			_szName[_sizeW]='\0';
			_retStr=_szName;
			delete []_szName;
		}
		return _retStr;
	}
}