#include "DB.h"
#include<string>


DB* DB::instance = nullptr;

DB::~DB()
{}

DB* DB::getInstance()
{
	if (instance == nullptr)
		instance = new DB;
	return instance;
}

void DB::destroyInstance()
{
    if (instance)
    {
        delete instance;
    }
}

bool DB::connect()
{
    try 
    {
        // Allocate environment handle
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        {
            throw new DBExceptie("Error allocating environment handle");
        }


        // Set environment attributes
        if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        {
            throw new DBExceptie("Error setting environment attributes");
        }


        // Allocate connection handle
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        {
            throw new DBExceptie("Error allocating connection handle");
        }


        std::cout << "ASTEAPTA CONECTAREA LA BAZA DE DATE...\n";

        // Connect to SQL Server
        switch (SQLDriverConnect(sqlConnHandle,
            NULL,
            (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost\\ANTONI0,55555;DATABASE=CheckerApp;Trusted_Connection=yes;",
            SQL_NTS,
            retconstring,
            1024,
            NULL,
            SQL_DRIVER_NOPROMPT)) {
        case SQL_SUCCESS:
            wcout << L"CONECTAT CU SUCCES" << endl;
            break;
        case SQL_SUCCESS_WITH_INFO:
            wcout << L"CONECTAT CU SUCCES" << endl;
            break;

        default:
            wcout << L"NU SE POATE CONECTA LA BAZA DE DATE" << endl;
            //wcout << retconstring;
            throw new DBExceptie("");
        }

        std::cout << "NO MORE WAITING!!!\n";

        // Allocate statement handle
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        {
            throw new DBExceptie("Error allocating statement handle");
        }

        return 1;
    }
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

bool DB::insertUser(wstring nume, wstring prenume, wstring email,wstring parola, wstring phone, int ocupatie)
{
    if (DB::getInstance()->connect())
    {
        std::cout << "Conectat la baza de date!\n";
        Logger::getInstance()->log("Conectat la baza de date!\n");
    }
    else
    {
        throw new DBExceptie("Nu se poate realiza conexiunea cu baza de date!\n");
    }

    SQLWCHAR* sqlQuery = (SQLWCHAR*)L"INSERT INTO Utilizatori (Nume,Prenume,Email,Parola,NumarTelefon,Ocupatie) VALUES (?, ?, ?, ?, ?, ?)";
    SQLWCHAR* sqlQueryCheckEmail = (SQLWCHAR*)L"SELECT COUNT(*) FROM Utilizatori WHERE Email = ?";

    SQLLEN cbNume = SQL_NTSL, cbEmail = SQL_NTSL, cbPassword = SQL_NTSL, cbPhone = SQL_NTSL, cbPrenume = SQL_NTSL, cbOcupatie = 0;


    if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryCheckEmail, SQL_NTS)) 
    {
        throw new DBExceptie("Eroare in prepararea query email\n");
    }

    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)email.c_str(), 0, &cbEmail);

    SQLExecute(sqlStmtHandle);

    SQLLEN count = 0;
    SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &count, sizeof(count), NULL);
    SQLFetch(sqlStmtHandle);


    if (count > 0) 
    {
        throw new DBExceptie("Eroare -> Emailul exista deja in baza de date...\n");
    }

    SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);


    if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS))
    {
        throw new DBExceptie("Eroare in prepararea query\n");
    }

   
    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)nume.c_str(), 0, &cbNume);
    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)prenume.c_str(), 0, &cbPrenume);
    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)email.c_str(), 0, &cbEmail);
    SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)parola.c_str(), 0, &cbPassword);
    SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)phone.c_str(), 0, &cbPhone);
    SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ocupatie, 0, &cbOcupatie);


    SQLRETURN retCode = SQLExecute(sqlStmtHandle);
    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) 
    {
        wcout << L"EXECUTIA A AVUT SUCCES" << endl;
    }
    else
    {
        SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
        SQLSMALLINT msgLength;
        SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
        std::wstring weroare = (std::wstring)errorMsg;
        std::string eroare(weroare.begin(), weroare.end());
        throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
    }


    wcout << L"S-au introdus date in baza de date cu succes" << endl;
    return 1;
}

void DB::deleteUserByAdmin(wstring email)
{
    try
    {
        if (DB::getInstance()->connect())
        {
            std::cout << "Conectat la baza de date!\n";
            Logger::getInstance()->log("Conectat la baza de date!\n");
        }
        else
        {
            std::cout << "Nu se poate realiza conexiunea cu baza de date!\n";
            Logger::getInstance()->log("Nu se poate realiza conexiunea cu baza de date!\n");
            return;
        }

        SQLWCHAR* sqlQuery = (SQLWCHAR*)L"DELETE FROM Utilizatori WHERE Email = ?";

        SQLLEN cbEmail = SQL_NTSL, cbPassword = SQL_NTSL;

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)email.c_str(), 0, &cbEmail);

        SQLRETURN retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }
    }
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

bool DB::checkLogin(wstring email, wstring parola)
{
    try
    {
         SQLWCHAR* sqlQuery = (SQLWCHAR*)L"SELECT COUNT(*) FROM Utilizatori WHERE Email = ? AND Parola = ?";

            SQLLEN cbEmail = SQL_NTSL, cbPassword = SQL_NTSL;

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)email.c_str(), 0, &cbEmail);
            SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)parola.c_str(), 0, &cbPassword);

            SQLRETURN retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN count = 0;
            SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &count, sizeof(count), NULL);
            SQLFetch(sqlStmtHandle);


            if (count > 0) {
                wcout << L"UTILIZATORUL EXISTA!!!\n" << endl;
                return true;
            }
            else
            {
                wcout << L"UTILIZATORUL NU EXISTA!!!\n" << endl;
                return false;
            }
    }
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

int DB::getOcupatie(wstring email, wstring parola)
{
    try
    {
        SQLWCHAR* sqlQuery = (SQLWCHAR*)L"SELECT Ocupatie FROM Utilizatori WHERE Email = ? AND Parola = ?";

        SQLLEN cbEmail = SQL_NTSL, cbPassword = SQL_NTSL, cbOccupation = SQL_NTSL;

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)email.c_str(), 0, &cbEmail);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)parola.c_str(), 0, &cbPassword);

        SQLExecute(sqlStmtHandle);


        SQLLEN occupationLength = 0;
        SQLWCHAR occupation[50];
        SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &occupation, sizeof(occupation), &occupationLength);
        SQLFetch(sqlStmtHandle);


        return std::stoi(occupation);
    }
   
    
        catch (IExceptie* e)
        {
            e->handle();
            delete e;
        }
}

bool DB::insertHomework(wstring titlu, wstring cerinta, wstring numeProfesor)
{
    try
    {
        SQLLEN cbCerinta = SQL_NTSL, cbNume = SQL_NTSL, cbTitlu = SQL_NTS;

        SQLWCHAR* sqlQueryCount = (SQLWCHAR*)L"SELECT COUNT(*) FROM Teme WHERE Titlu = ? AND Cerinta = ? AND Autor = ?";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryCount, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, titlu.size(), 0, (SQLPOINTER)titlu.c_str(), 0, &cbTitlu);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, cerinta.size(), 0, (SQLPOINTER)cerinta.c_str(), 0, &cbCerinta);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, numeProfesor.size(), 0, (SQLPOINTER)numeProfesor.c_str(), 0, &cbNume);


        SQLRETURN retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }

        SQLLEN count = 0;
        SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &count, sizeof(count), NULL);
        SQLFetch(sqlStmtHandle);


        // daca e deja e OK
        if (count != 0)
        {
            return 1;
        }

        SQLWCHAR* sqlQuery = (SQLWCHAR*)L"INSERT INTO Teme (Titlu,Cerinta,Autor) VALUES (?, ?, ?)";


        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, titlu.size(), 0, (SQLPOINTER)titlu.c_str(), 0, &cbTitlu);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, cerinta.size(), 0, (SQLPOINTER)cerinta.c_str(), 0, &cbCerinta);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, numeProfesor.size(), 0, (SQLPOINTER)numeProfesor.c_str(), 0, &cbNume);

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);

        retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }


        wcout << L"S-au introdus date in baza de date cu succes" << endl;
        return 1;
    }
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

Homeworks* DB::getHomeworks(wstring email)
{
    try
    {
        Homeworks* homeworks = new Homeworks;

        if (Homeworks::getFlag() == 0)
        {
            Homeworks::deletePointerHomeworks();
            Homeworks::setPointerHomeworks(homeworks);
        }
        else
        {
            delete homeworks;
            return Homeworks::getPointerHomeworks();
        }
    
    
        SQLWCHAR* sqlQuery = (SQLWCHAR*)L"SELECT Titlu,Cerinta,Autor FROM Teme";

        SQLLEN cb = SQL_NTS;

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLRETURN retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }

        SQLLEN lengthCerinta = 0;
        SQLLEN lengthAutor = 0;
        SQLLEN lengthTitle = 0;

        SQLWCHAR title[1024]{};
        SQLWCHAR enunt[1024]{};
        SQLWCHAR autor[50]{};

        SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &title, sizeof(title), &lengthTitle);
        SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, &enunt, sizeof(enunt), &lengthCerinta);
        SQLBindCol(sqlStmtHandle, 3, SQL_C_WCHAR, &autor, sizeof(autor), &lengthAutor);
    


        std::vector<std::pair<std::wstring, std::pair<std::wstring, std::wstring>>> vector;

        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
        {
            std::wstring _titlu(title, title + lengthTitle);
            std::wstring task(enunt, enunt + lengthCerinta);
            std::wstring author(autor, autor + lengthAutor);
            vector.push_back({ _titlu,{task,author} });
        }

        for (auto& fetch : vector)
        {
            // preiau temaID ca sa il folosesc sa verific in RezolvareTeme daca tema a fost rezolvata sau nu
            SQLWCHAR* sqlQueryTema = (SQLWCHAR*)L"SELECT ID FROM Teme WHERE Titlu = ? AND Autor = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryTema, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, fetch.first.size(), 0, (SQLPOINTER)fetch.first.c_str(), 0, &cb);
            SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, fetch.second.second.size(), 0, (SQLPOINTER)fetch.second.second.c_str(), 0, &cb);

            retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN idTemaLength = 0;
            SQLWCHAR idTema[3]{};

            SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idTema, sizeof(idTema), &idTemaLength);
            SQLFetch(sqlStmtHandle);

            int temaId = std::stoi(std::wstring(idTema));

            // preiau userID
            SQLWCHAR* sqlQueryUser = (SQLWCHAR*)L"SELECT ID FROM Utilizatori WHERE Email = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryUser, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, email.size(), 0, (SQLPOINTER)email.c_str(), 0, &cb);

            retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN idLength = 0;
            SQLWCHAR idUser[3]{};

            SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idUser, sizeof(idUser), &idLength);
            SQLFetch(sqlStmtHandle);

            int userId = std::stoi(std::wstring(idUser));

            //verific in RezolvareTeme daca tema a fost rezolvata sau nu
            SQLWCHAR* sqlQueryID = (SQLWCHAR*)L"SELECT COUNT(*) FROM RezolvareTeme WHERE IdUser = ? AND IdTema = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryID, SQL_NTS)) 
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &userId, 0, &cb);
            SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);

             retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN count = 0;
            SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &count, sizeof(count), NULL);
            SQLFetch(sqlStmtHandle);

            int alreadyResolved;
            if (count > 0) {
                //wcout << L"REZOLVAREA TEMEI EXISTA!!!\n" << endl;
                alreadyResolved = 1;
            }
            else
            {
                //wcout << L"REZOLVAREA TEMEI NU EXISTA!!!\n" << endl;
                alreadyResolved = 0;
            }
  
            std::string __titlu(fetch.first.begin(), fetch.first.end());
            std::string _task(fetch.second.first.begin(), fetch.second.first.end());
            std::string _author(fetch.second.second.begin(), fetch.second.second.end());
        
            homeworks->addHomework(alreadyResolved, __titlu, _task, _author);
        }
        Homeworks::setFlag(1);
        return homeworks;
    }
    
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

std::string DB::insertRezolvareTema(wstring email, wstring titlu,wstring autor)
{
    try
    {
      SQLLEN cb = SQL_NTS;

        //preluare userId
        SQLWCHAR* sqlQueryUser = (SQLWCHAR*)L"SELECT ID FROM Utilizatori WHERE Email = ?";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryUser, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, email.size(), 0, (SQLPOINTER)email.c_str(), 0, &cb);

        SQLRETURN retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }
    
        SQLLEN idLength = 0;
        SQLWCHAR idUser[3]{};

        SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idUser, sizeof(idUser), &idLength);
        SQLFetch(sqlStmtHandle);

        int userId = std::stoi(std::wstring(idUser));

        // preluare temaId

        SQLWCHAR* sqlQueryTema = (SQLWCHAR*)L"SELECT ID FROM Teme WHERE Titlu = ? AND Autor = ?";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryTema, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, titlu.size(), 0, (SQLPOINTER)titlu.c_str(), 0, &cb);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, autor.size(), 0, (SQLPOINTER)autor.c_str(), 0, &cb);

        retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }
    
        SQLLEN idTemaLength = 0;
        SQLWCHAR idTema[3]{};

        SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idTema, sizeof(idTema), &idTemaLength);
        SQLFetch(sqlStmtHandle);

        int temaId = std::stoi(std::wstring(idTema));

   
        // creare filename
        std::wstring tmp = L"rez_";
        std::wstring underscore = L"_";
        std::wstring extensie = L".txt";
        std::wstring fileName = L"";
         fileName = tmp + email + underscore + titlu + extensie;


        // insereaza in RezolvareTeme

            //mai intai se verifica daca studentul a mai rezolvat tema

         SQLWCHAR* sqlQueryCount = (SQLWCHAR*)L"SELECT COUNT(*) FROM RezolvareTeme WHERE IdUser = ? AND IdTema = ?";

         SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
         if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryCount, SQL_NTS))
         {
             throw new DBExceptie("Eroare in prepararea query\n");
         }

         SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &userId, 0, &cb);
         SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);
     

         retCode = SQLExecute(sqlStmtHandle);
         if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
         {
             wcout << L"EXECUTIA A AVUT SUCCES" << endl;
         }
         else
         {
             SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
             SQLSMALLINT msgLength;
             SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
             std::wstring weroare = (std::wstring)errorMsg;
             std::string eroare(weroare.begin(), weroare.end());
             throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
         }

         SQLLEN count = 0;
         SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &count, sizeof(count), NULL);
         SQLFetch(sqlStmtHandle);


         // e OK
         if (count !=  0 )  
         {
             return std::string(fileName.begin(), fileName.end());
         }
     

        SQLWCHAR* sqlQuery = (SQLWCHAR*)L"INSERT INTO RezolvareTeme(IdUser,IdTema,FileName) VALUES(?,?,?)";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }
    
        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &userId, 0, &cb);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, fileName.size(), 0, (SQLPOINTER)fileName.c_str(), 0, &cb);
   

        retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }


        wcout << L"S-au introdus date in baza de date cu succes" << endl;
    
        return std::string(fileName.begin(),fileName.end());
    }
  

    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

std::string DB::veziTemplate(wstring titlu, wstring autor)
{
    try
    {
        SQLLEN cb = SQL_NTS;

        // preluare temaId
        SQLWCHAR* sqlQueryTema = (SQLWCHAR*)L"SELECT ID FROM Teme WHERE titlu = ? AND autor = ?";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryTema, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, titlu.size(), 0, (SQLPOINTER)titlu.c_str(), 0, &cb);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, autor.size(), 0, (SQLPOINTER)autor.c_str(), 0, &cb);

        SQLRETURN retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }

        SQLLEN idTemaLength = 0;
        SQLWCHAR idTema[3]{};

        SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idTema, sizeof(idTema), &idTemaLength);
        SQLFetch(sqlStmtHandle);

        int temaId = std::stoi(std::wstring(idTema));

        // preia filename-ul din TemplateProfesor

        SQLWCHAR* sqlQueryCount = (SQLWCHAR*)L"SELECT FileName FROM TemplateProfesori WHERE IdTema = ?";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryCount, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);

        retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }


        SQLLEN nameLength = 0;
        SQLWCHAR fileName[1024];
        SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &fileName, sizeof(fileName), &nameLength);
        SQLFetch(sqlStmtHandle);

        std::wstring w_fileName = (std::wstring)fileName;

        std::string fileName_s(w_fileName.begin(), w_fileName.end());

        return fileName_s;
    }
   
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

std::pair<std::string, std::string> DB::insertTemplate(wstring titlu, wstring autor)
{
    try
    {
        SQLLEN cb = SQL_NTS;

        // preluare temaId
        SQLWCHAR* sqlQueryTema = (SQLWCHAR*)L"SELECT ID FROM Teme WHERE titlu = ? AND autor = ?";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryTema, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, titlu.size(), 0, (SQLPOINTER)titlu.c_str(), 0, &cb);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, autor.size(), 0, (SQLPOINTER)autor.c_str(), 0, &cb);

        SQLRETURN retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }

        SQLLEN idTemaLength = 0;
        SQLWCHAR idTema[3]{};

        SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idTema, sizeof(idTema), &idTemaLength);
        SQLFetch(sqlStmtHandle);

        int temaId = std::stoi(std::wstring(idTema));


        // creare filename1,2
        std::wstring tmp1 = L"temp_";
        std::wstring tmp2 = L"vector_";
        std::wstring underscore = L"_";
        std::wstring extensie = L".txt";
        std::wstring fileName1 = L"";
        std::wstring fileName2 = L"";
        fileName1 = tmp1 + titlu + underscore + autor + extensie;
        fileName2 = tmp2 + titlu + underscore + autor + extensie;

        std::pair<std::string, std::string> pereche = { std::string(fileName1.begin(), fileName1.end()) , std::string(fileName2.begin(), fileName2.end()) };

        //mai intai se verifica daca profesorul a mai incarcat

        SQLWCHAR* sqlQueryCount = (SQLWCHAR*)L"SELECT COUNT(*) FROM TemplateProfesori WHERE IdTema = ?";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryCount, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);


        retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }

        SQLLEN count = 0;
        SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &count, sizeof(count), NULL);
        SQLFetch(sqlStmtHandle);


        // e OK
        if (count != 0)
        {
            return pereche;
        }


        // inserez in TemplateProfesori

        SQLWCHAR* sqlQuery = (SQLWCHAR*)L"INSERT INTO TemplateProfesori(IdTema,FileName,FileNameVector) VALUES(?,?,?)";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, fileName1.size(), 0, (SQLPOINTER)fileName1.c_str(), 0, &cb);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, fileName2.size(), 0, (SQLPOINTER)fileName2.c_str(), 0, &cb);


        retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }


        wcout << L"S-au introdus date in baza de date cu succes" << endl;

        return pereche;
    }
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }

}

std::string DB::veziVectorRezultate(wstring titlu, wstring autor)
{
    try
    {
        SQLLEN cb = SQL_NTS;

            // preluare temaId
            SQLWCHAR* sqlQueryTema = (SQLWCHAR*)L"SELECT ID FROM Teme WHERE titlu = ? AND autor = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryTema, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, titlu.size(), 0, (SQLPOINTER)titlu.c_str(), 0, &cb);
            SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, autor.size(), 0, (SQLPOINTER)autor.c_str(), 0, &cb);

            SQLRETURN retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN idTemaLength = 0;
            SQLWCHAR idTema[3]{};

            SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idTema, sizeof(idTema), &idTemaLength);
            SQLFetch(sqlStmtHandle);

            int temaId = std::stoi(std::wstring(idTema));

            // preia filename-ul vectorului din TemplateProfesor

            SQLWCHAR* sqlQueryCount = (SQLWCHAR*)L"SELECT FileNameVector FROM TemplateProfesori WHERE IdTema = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryCount, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);

            retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }


            SQLLEN nameLength = 0;
            SQLWCHAR fileName[1024];
            SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &fileName, sizeof(fileName), &nameLength);
            SQLFetch(sqlStmtHandle);

            std::wstring w_fileName = (std::wstring)fileName;

            std::string fileName_s(w_fileName.begin(), w_fileName.end());

            return fileName_s;
    }
    
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

std::vector<std::pair<std::string, std::string>> DB::getRezolvariTema(wstring titlu, wstring autor)
{
    try
    {
    SQLLEN cb = SQL_NTS;

        SQLWCHAR* sqlQueryTema = (SQLWCHAR*)L"SELECT ID FROM Teme WHERE Titlu = ? AND Autor = ?";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryTema, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, titlu.size(), 0, (SQLPOINTER)titlu.c_str(), 0, &cb);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, autor.size(), 0, (SQLPOINTER)autor.c_str(), 0, &cb);

        SQLRETURN retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }

        SQLLEN idTemaLength = 0;
        SQLWCHAR idTema[3]{};

        SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idTema, sizeof(idTema), &idTemaLength);
        SQLFetch(sqlStmtHandle);

        int temaId = std::stoi(std::wstring(idTema));

        // preluare userid si filename pt rezolvarile temei
        SQLWCHAR* sqlQuery = (SQLWCHAR*)L"SELECT IDUser,FileName FROM RezolvareTeme WHERE IdTema = ?";

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuery, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);

        retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }

        SQLLEN lengthID = 0;
        SQLLEN lengthFileName = 0;


        SQLWCHAR ID[3]{};
        SQLWCHAR FileName[512]{};


        SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &ID, sizeof(ID), &lengthID);
        SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, &FileName, sizeof(FileName), &lengthFileName);

        std::vector<std::pair<int, std::wstring>> wvector;

        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
        {
            std::wstring id(ID, ID + lengthID);
            int userId = std::stoi(id);

            std::wstring filename(FileName, FileName + lengthFileName);

            wvector.push_back({ userId,filename });
        }

        std::vector<std::pair<std::string, std::string>> vector;

        for (auto& fetch : wvector)
        {
            // preiau email pe baza userID
            SQLWCHAR* sqlQueryTema = (SQLWCHAR*)L"SELECT Email FROM Utilizatori WHERE ID = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryTema, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &fetch.first, 0, &cb);
       

            retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN emailLength = 0;
            SQLWCHAR _email[512]{};

            SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &_email, sizeof(_email), &emailLength);
            SQLFetch(sqlStmtHandle);

            std::string email(_email, _email + emailLength);
            std::string numefisier(fetch.second.begin(), fetch.second.end());

            vector.push_back({ email,numefisier });
        
        }
        return vector;
    }
    
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

bool DB::insertProcentaj(wstring titlu, wstring cerinta, wstring autor, wstring email,float procent)
{
    try
    {
        SQLLEN cb = SQL_NTSL;

            // preiau temaID 
            SQLWCHAR* sqlQueryTema = (SQLWCHAR*)L"SELECT ID FROM Teme WHERE Titlu = ? AND Cerinta = ? AND Autor = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryTema, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, titlu.size(), 0, (SQLPOINTER)titlu.c_str(), 0, &cb);
            SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, cerinta.size(), 0, (SQLPOINTER)cerinta.c_str(), 0, &cb);
            SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, autor.size(), 0, (SQLPOINTER)autor.c_str(), 0, &cb);

            SQLRETURN retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN idTemaLength = 0;
            SQLWCHAR idTema[3]{};

            SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idTema, sizeof(idTema), &idTemaLength);
            SQLFetch(sqlStmtHandle);

            int temaId = std::stoi(std::wstring(idTema));

            // preiau userID
            SQLWCHAR* sqlQueryUser = (SQLWCHAR*)L"SELECT ID FROM Utilizatori WHERE Email = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryUser, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, email.size(), 0, (SQLPOINTER)email.c_str(), 0, &cb);

            retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN idLength = 0;
            SQLWCHAR idUser[3]{};

            SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &idUser, sizeof(idUser), &idLength);
            SQLFetch(sqlStmtHandle);

            int userId = std::stoi(std::wstring(idUser));

            //verific in Statistici daca studentul deja a mai comparat acea tema 
            SQLWCHAR* sqlQueryVerify = (SQLWCHAR*)L"SELECT COUNT(*) FROM Statistici WHERE IdUtilizator = ? AND IdTema = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryVerify, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &userId, 0, &cb);
            SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);

            retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN count = 0;
  
            SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &count, sizeof(count), NULL);
            SQLFetch(sqlStmtHandle);

            //preia procentajul
            SQLWCHAR* sqlQueryProcent = (SQLWCHAR*)L"SELECT Procentaj FROM Statistici WHERE IdUtilizator = ? AND IdTema = ?";

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryProcent, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &userId, 0, &cb);
            SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);

            retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

    
            SQLLEN procentaj = 0.0f;

            SQLBindCol(sqlStmtHandle, 1, SQL_C_FLOAT, &procentaj, sizeof(procentaj), NULL);
            SQLFetch(sqlStmtHandle);

            if (count > 0)
            {
                // fac update la procentaj daca e mai mare

                if (procentaj > procent)
                {
                    SQLWCHAR* sqlQueryUpdate = (SQLWCHAR*)L"UPDATE Statistici SET Procentaj = ? WHERE IdUtilizator = ? AND IdTema = ?";

                    SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
                    if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryVerify, SQL_NTS))
                    {
                        throw new DBExceptie("Eroare in prepararea query\n");
                    }

                    SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &procent, 0, &cb);
                    SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &userId, 0, &cb);
                    SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);

                    retCode = SQLExecute(sqlStmtHandle);
                    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
                    {
                        wcout << L"EXECUTIA A AVUT SUCCES" << endl;
                    }
                    else
                    {
                        SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                        SQLSMALLINT msgLength;
                        SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                        std::wstring weroare = (std::wstring)errorMsg;
                        std::string eroare(weroare.begin(), weroare.end());
                        throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
                    }
                }
        
            }
            else
            {
                // inserez
                SQLWCHAR* sqlQueryInsert = (SQLWCHAR*)L"INSERT INTO Statistici (IdUtilizator,IdTema,Procentaj) VALUES (?, ?, ?)";

                SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
                if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryInsert, SQL_NTS))
                {
                    throw new DBExceptie("Eroare in prepararea query\n");
                }

        
                SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &userId, 0, &cb);
                SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temaId, 0, &cb);
                SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &procent, 0, &cb);

                retCode = SQLExecute(sqlStmtHandle);
                if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
                {
                    wcout << L"EXECUTIA A AVUT SUCCES" << endl;
                }
                else
                {
                    SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                    SQLSMALLINT msgLength;
                    SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                    std::wstring weroare = (std::wstring)errorMsg;
                    std::string eroare(weroare.begin(), weroare.end());
                    throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
                }
            }
    }
    
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

std::vector<std::pair<float, std::pair<std::string, std::string>>> DB::getStatistici()
{
    try
    {
    SQLWCHAR* sqlQuerySelect = (SQLWCHAR*)L"SELECT IdUtilizator,AVG(Procentaj) AS medie FROM Statistici GROUP BY IdUtilizator";

        SQLLEN cb = SQL_NTS;

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQuerySelect, SQL_NTS))
        {
            throw new DBExceptie("Eroare in prepararea query\n");
        }

        SQLRETURN retCode = SQLExecute(sqlStmtHandle);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
        {
            wcout << L"EXECUTIA A AVUT SUCCES" << endl;
        }
        else
        {
            SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
            SQLSMALLINT msgLength;
            SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
            std::wstring weroare = (std::wstring)errorMsg;
            std::string eroare(weroare.begin(), weroare.end());
            throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
        }

        SQLLEN id = 0;
        SQLLEN lengthProcent = 0;
        SQLWCHAR procent[50]{};
   

        SQLBindCol(sqlStmtHandle, 1, SQL_C_LONG, &id, sizeof(id), NULL);
        SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, &procent, sizeof(procent), &lengthProcent);

        std::vector<std::pair<int,float>> vector;

        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
        {
            std::wstring wprocent = (std::wstring)(procent);
            std::string sprocent(wprocent.begin(), wprocent.end());
            float _procent = std::atof(sprocent.c_str());
            int ID = id;
            vector.push_back({ ID,_procent});
        }

        std::vector<std::pair<float, std::pair<std::string, std::string>>> deReturnat;
        for (auto& fetch : vector)
        {

            //preiau numele si prenumele user-ului
            SQLWCHAR* sqlQueryName = (SQLWCHAR*)L"SELECT Nume,Prenume FROM Utilizatori WHERE ID = ?";

            SQLLEN cb = SQL_NTS;

            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
            if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, sqlQueryName, SQL_NTS))
            {
                throw new DBExceptie("Eroare in prepararea query\n");
            }

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &fetch.first, 0, &cb);

            SQLRETURN retCode = SQLExecute(sqlStmtHandle);
            if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
            {
                wcout << L"EXECUTIA A AVUT SUCCES" << endl;
            }
            else
            {
                SQLWCHAR errorMsg[SQL_RETURN_CODE_LEN];
                SQLSMALLINT msgLength;
                SQLGetDiagField(SQL_HANDLE_STMT, sqlStmtHandle, 1, SQL_DIAG_MESSAGE_TEXT, errorMsg, SQL_RETURN_CODE_LEN, &msgLength);
                std::wstring weroare = (std::wstring)errorMsg;
                std::string eroare(weroare.begin(), weroare.end());
                throw new DBExceptie("EXECUTIA NU A AVUT SUCCES -> " + eroare);
            }

            SQLLEN lengthNume = 0;
            SQLLEN lengthPrenume = 0;

            SQLWCHAR nume[50]{};
            SQLWCHAR prenume[50]{};


            SQLBindCol(sqlStmtHandle, 1, SQL_C_WCHAR, &nume, sizeof(nume), &lengthNume);
            SQLBindCol(sqlStmtHandle, 2, SQL_C_WCHAR, &prenume, sizeof(prenume), &lengthPrenume);

            SQLFetch(sqlStmtHandle);
            std::wstring wname = (std::wstring)(nume);
            std::wstring wprename = (std::wstring)(prenume);

            std::string name(wname.begin(), wname.end());
            std::string prename(wprename.begin(), wprename.end());

            deReturnat.push_back({ fetch.second,{name,prename} });
        }
        return deReturnat;
    }
    
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}

void DB::disconnect()
{
    // Free statement handle
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);

    // Disconnect from SQL Server
    SQLDisconnect(sqlConnHandle);

    // Free connection handle
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);

    // Free environment handle
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

}