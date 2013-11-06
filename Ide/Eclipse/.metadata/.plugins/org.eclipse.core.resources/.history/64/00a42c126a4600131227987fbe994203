#pragma once

#include <string>
#include <fstream>
#include <Utils/ICriticalSection.h>

class Log
{
private:
	static std::ofstream fileStream;
	static std::stringstream logSStream;
	static char dstBuf[1024];
	static char timeBuff[80];
	
	static bool logToConsole;
	static bool logThreadName;
	
	static ICriticalSection *lock;

	static void UpdateTime();
	static void UpdateLog();
		
public:
	static const char* DefaultLogFileName;
	
	static void StartLog(bool logToConsole,
						 bool logThreadName,
						 const char *logFileName = DefaultLogFileName);
	
	static void LogT(const char *formatString, ...);
	static void LogW(const char *formatString, ...);
	static void LogE(const char *formatString, ...);
};
