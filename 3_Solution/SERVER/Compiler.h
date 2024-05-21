#pragma once
#include<string>
#include <iostream>
#include<fstream>
#include <winsock2.h>
#include "CompilerExceptie.h"

namespace Compiler
{
	int compile(const std::string& cod, int limbaj,SOCKET s);
	static std::string File_name = "output.txt";
	static std::string exeName = "output.exe";
};

