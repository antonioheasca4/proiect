#pragma once
#include <fstream>
#include "Homeworks.h"
#include <winsock2.h>
#include "Admin.h"
#include<thread>
#include <sstream>

namespace Utilitati
{
	std::vector<std::wstring> breakBuffer(const std::string& date,char delimitator);
	bool selectAction(const std::wstring& actiune);
	std::string readFile( std::string& filename);
	void coutFile( std::string& filename, const std::string& buffer);
	void sendTeme(Homeworks* teme, SOCKET s);
	void sendRezolvariTema(std::vector<std::pair<std::string, std::string>>& rezolvari, SOCKET s);
	void sendStatistici(std::vector<std::pair<float, std::pair<std::string, std::string>>>& stats, SOCKET sock);
	void ascultaCombinatiiTaste();
	//void iesireMeniu();
	int distantaLevenshtein(const std::string& s1, const std::string& s2);

	std::vector<std::string> splitString(const std::string& s);

	double GetSimilitudine(std::string& str1, std::string& str2);

	bool verifyCommon(const std::string& cuv);

	void insertSpaces(std::string& s);

	void insertSpaceBeforeEOL(std::string& str);

	void eliminaEnter(std::string& str);

	template <class T>
	T minim(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}

	template <class T>
	T maxim(const T& a, const T& b)
	{
		return (b > a) ? b : a;
	}
};

