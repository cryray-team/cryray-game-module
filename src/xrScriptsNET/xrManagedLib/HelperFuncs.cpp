#include "stdafx.h"
#include "HelperFuncs.h"

// char* ConvertWidecharToAscii(const wchar_t* pData, unsigned int size)
// {
// 	char* Result = new char[size];
// 	int BytesWritten = WideCharToMultiByte(CP_ACP, 0, pData, size, Result, size, nullptr, nullptr);
// 	return Result;
// }
// 
// void ConvertWidecharToAscii(const wchar_t* pData, unsigned int size, string1024& OutStr)
// {
// 	WideCharToMultiByte(CP_ACP, 0, pData, size, OutStr, 1024, nullptr, nullptr);
// }
// 
// void ConvertWidecharToAscii(const wchar_t* pData, unsigned int size, string512& OutStr)
// {
// 	WideCharToMultiByte(CP_ACP, 0, pData, size, OutStr, 512, nullptr, nullptr);
// }
// 
// void ConvertWidecharToAscii(const wchar_t* pData, unsigned int size, string256& OutStr)
// {
// 	WideCharToMultiByte(CP_ACP, 0, pData, size, OutStr, 256, nullptr, nullptr);
// }
