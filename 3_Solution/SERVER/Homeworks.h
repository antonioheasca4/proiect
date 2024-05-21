#pragma once
#include <iostream>
#include<vector>
#include <utility>

class Homeworks
{
private:
	std::vector<std::pair<int,std::pair<std::string, std::pair<std::string, std::string>>>> teme;
	static bool flag; // il folosesc ca atunci cand un profesor mai introduce o tema sa stiu
						//ca trebuie sa chestionez din nou baza de date
						// cand e 0 trebuie sa interoghez baza de date
					// si voi sterge vechiul pointer Homeworks
	//voi seta flag ul la 0 in momentul in care profesorul adauga tema

	
	static Homeworks* homeworks;
public:
	Homeworks();
	void addHomework(int n,const std::string& titlu, const std::string& cerinta, const std::string& autor);
	std::vector<std::pair<int, std::pair<std::string, std::pair<std::string, std::string>>>>& getTeme() { return this->teme; }
	static void setFlag(int n) { flag = n; }
	static bool getFlag() { return flag; }
	static void setPointerHomeworks(Homeworks* h) { homeworks = h; }
	static Homeworks* getPointerHomeworks() { return homeworks; }
	static void deletePointerHomeworks() { delete homeworks; }
	~Homeworks();
};

