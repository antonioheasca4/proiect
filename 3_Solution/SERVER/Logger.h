#pragma once
#include <iostream>
#include<fstream>

class Logger
{
	static Logger* instance;
	Logger() {};
	~Logger() {}
	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator= (const Logger&) = delete;
	Logger&& operator= (Logger&&) = delete;
public:
	static Logger* getInstance();
	static void destroyInstance();

	void log(const std::string& s);
};

