#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define SQL_RETURN_CODE_LEN 1000

#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <vector>
#include"Homeworks.h"
#include "DBExceptie.h"

using namespace std;



class DB
{
	DB(){}
	~DB();
	DB(const DB&) = delete;
	DB(DB&&) = delete;
	DB& operator= (const DB&) = delete;
	DB&& operator= (DB&&) = delete;
	SQLHANDLE sqlConnHandle = NULL;
	SQLHANDLE sqlStmtHandle = NULL;
	SQLHANDLE sqlEnvHandle = NULL;
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
	static DB* instance;
public:

	static DB* getInstance();
	static void destroyInstance();

	bool connect();
	void disconnect();

	bool insertUser(wstring nume,wstring prenume,wstring email,wstring parola, wstring phone,int ocupatie);
	bool checkLogin(wstring email,wstring parola);
	int getOcupatie(wstring email, wstring parola);

	bool insertHomework(wstring titlu, wstring cerinta, wstring numeProfesor);
	Homeworks* getHomeworks(wstring email);

	std::string insertRezolvareTema(wstring email, wstring titlu,wstring autor);
	std::vector<std::pair<std::string, std::string>> getRezolvariTema(wstring titlu, wstring autor);

	std::string veziTemplate(wstring titlu, wstring autor);
	std::pair<std::string,std::string> insertTemplate(wstring titlu, wstring autor);

	std::string veziVectorRezultate(wstring titlu, wstring autor);

	bool insertProcentaj(wstring titlu, wstring cerinta, wstring autor, wstring email, float procent);
	std::vector<std::pair<float, std::pair<std::string, std::string>>> getStatistici();

	void deleteUserByAdmin(wstring email);
};
