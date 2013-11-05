#include "Log.h"
#include <sstream>
#include <Utils/CriticalSectionFactory.h>

const char* Log::DefaultLogFileName = "log.txt";
bool Log::logToConsole;
bool Log::logThreadName;

std::ofstream Log::fileStream;
std::stringstream Log::logSStream;
char Log::dstBuf[1024];
char Log::timeBuff[80];
ICriticalSection* Log::lock;

void Log::UpdateTime()
{
	time_t	rawTime;
	tm		*timeInfo;
	
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	
	strftime(timeBuff, 80, "%X", timeInfo);
}

void Log::UpdateLog()
{
	logSStream.str("");
	logSStream << "[";
	
	if (logThreadName)
	{
		if ([[NSThread currentThread] name].length > 0)
			logSStream << [[[NSThread currentThread] name] UTF8String];
		else
			logSStream << [NSThread currentThread];
		
		logSStream << "-";
	}
	
	logSStream << timeBuff << "-T    ] " << dstBuf << std::endl;
}

void Log::StartLog(bool logToConsole,
				   bool logThreadName,
				   const char *logFileName)
{
	if (fileStream.is_open())
		fileStream.close();
	
	fileStream.open(logFileName, std::ios::trunc);
	
	Log::logToConsole = logToConsole;
	Log::logThreadName = logThreadName;
	
	lock = CriticalSectionFactory::Create();
}

void Log::LogT(const char *formatString, ...)
{	
	lock->Lock();
	
	va_list argptr;
	va_start(argptr, formatString);
	vsprintf(dstBuf, formatString, argptr);
	va_end(argptr);
	
	UpdateTime();
	UpdateLog();
	
	if (logToConsole)
		printf("%s", logSStream.str().c_str());

	fileStream << logSStream.str();
	fileStream.flush();
	lock->Unlock();
}

void Log::LogW(const char *formatString, ...)
{	
	lock->Lock();
	va_list argptr;
	va_start(argptr, formatString);
	vsprintf(dstBuf, formatString, argptr);
	va_end(argptr);
	
	UpdateTime();
	UpdateLog();
	
	if (logToConsole)
		printf("%s", logSStream.str().c_str());
	
	fileStream << logSStream.str();
	fileStream.flush();
	lock->Unlock();
}

void Log::LogE(const char *formatString, ...)
{
	lock->Lock();
	va_list argptr;
	va_start(argptr, formatString);
	vsprintf(dstBuf, formatString, argptr);
	va_end(argptr);
	
	UpdateTime();
	UpdateLog();
	
	if (logToConsole)
		printf("%s", logSStream.str().c_str());
	
	fileStream << logSStream.str();
	fileStream.flush();
	lock->Unlock();
}
