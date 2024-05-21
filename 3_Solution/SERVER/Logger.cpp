#include "Logger.h"

Logger* Logger::instance = nullptr;

Logger* Logger::getInstance()
{
	if (instance == nullptr)
		instance = new Logger;
	return instance;
}

void Logger::destroyInstance()
{
    if (instance)
    {
        delete instance;
    }
}

void Logger::log(const std::string& s)
{
    std::ofstream f("logServer.txt", std::ios::app);
    f << s;
    f.close();
}
