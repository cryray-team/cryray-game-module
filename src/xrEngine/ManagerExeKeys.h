#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <shellapi.h>

class ENGINE_API CommandLineArguments
{
public:
    CommandLineArguments(char* lpCmdLine, int nCmdShow);
    ~CommandLineArguments();
    bool CheckKey(const std::string& key);

private:
    char** m_argv;
    int m_argc;
};

extern ENGINE_API std::vector<std::string> keys;
extern ENGINE_API CommandLineArguments* pCommandLineArguments;