#include "Logger.h"
Logger::Logger(){};
Logger::~Logger(){};
void Logger::output(const char* msg)
{
	printf("%s\n", msg);
}
