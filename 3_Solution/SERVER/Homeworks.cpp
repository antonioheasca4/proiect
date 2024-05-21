#include "Homeworks.h"

bool Homeworks::flag = 0;
Homeworks* Homeworks::homeworks = nullptr;

Homeworks::Homeworks()
{}


void Homeworks::addHomework(int n, const std::string& titlu, const std::string& cerinta, const std::string& autor)
{
	this->teme.push_back({ n,{titlu,{cerinta,autor}} });
}

Homeworks::~Homeworks()
{}
