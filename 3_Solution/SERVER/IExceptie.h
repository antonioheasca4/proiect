#pragma once
#include "Logger.h"
#include<iostream>

class IExceptie
{
public:
	virtual void handle() = 0;
};

