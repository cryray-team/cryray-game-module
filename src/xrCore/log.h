#pragma once

#define VPUSH(a) ((a).x), ((a).y), ((a).z)

#define Msg(msg, ...) _Msg(XR_MODULE, msg, __VA_ARGS__)
#define Log(msg, ...) _Log(XR_MODULE, msg, __VA_ARGS__)
#define ClearMsg(msg, ...) _MsgNoFormat(XR_MODULE, msg, __VA_ARGS__)

void XRCORE_API EnableForwardToConsole(bool bEnable);

struct LogFileStruct
{
    std::string line_info;
    std::string line_content;
    char line_mark;
    std::string GetLine() { return line_info + line_content; }
};

extern XRCORE_API xr_vector<LogFileStruct>* LogFile;

namespace CRLogger
{
    typedef void (*LogCallback)(LPCSTR string);
    LogCallback XRCORE_API SetLogCB(LogCallback cb);

    void XRCORE_API InitLog();
    void XRCORE_API CreateLog();
    void XRCORE_API FlushLog();
    void XRCORE_API CloseLog();
    void XRCORE_API SaveCopyLog(LPCSTR file_name);
    void XRCORE_API _LogNoFormat(std::string Module, const char* s);
    void XRCORE_API _Log(std::string Module, LPCSTR msg);
    void XRCORE_API _MsgNoFormat(std::string Module, LPCSTR format, ...);
    void XRCORE_API _Msg(std::string Module, LPCSTR format, ...);
}
