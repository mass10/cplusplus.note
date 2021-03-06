#pragma once

#include <tchar.h>
#include <sstream>

// ロギングの提供
class logger {
public:
	static void trace(const _TCHAR* s);
	static void trace(const std::wstring& s);
	static void trace(const std::wstringstream& s);
	static void error(const _TCHAR* s);
	static void error(const std::wstring& s);
	static void error(const std::wstringstream& s);
};
