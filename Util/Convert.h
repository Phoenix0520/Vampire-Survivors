#pragma once
#include  <iostream>
#include  <Windows.h>
#include  <string>
using namespace  std;

class Convert
{
public:
	static DWORD Ansi_to_UniCode(wstring& unicode, char* ansi, size_t ansi_size);
	static DWORD UniCode_to_Ansi(string& ansi, wchar_t* unicode, size_t unicode_size);
	static DWORD UniCode_to_UTF8(string& utf8, wchar_t* unicode, size_t unicode_size);
	static DWORD UTF8_to_UniCode(wstring & unicode, char* utf8, size_t utf8_size);
public:
	Convert() {};
	~Convert() {};

};

