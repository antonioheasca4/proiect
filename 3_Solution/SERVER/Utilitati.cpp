#define _CRT_SECURE_NO_WARNINGS
#include "Utilitati.h"
#include <windows.h>
#include<string>
using namespace Utilitati;


std::vector<std::wstring> Utilitati::breakBuffer(const std::string& date, char delimitator)
{
    std::vector<std::wstring> cuv; 

    std::wstring aux;
    for (int i = 0; i < date.size(); i++) 
    {
        if (date[i] != delimitator)
        {
            aux += date[i];
        }
        else
        {
            cuv.push_back(aux);
            aux = L"";
        }
    }

    //if (!aux.empty()) 
    {
        cuv.push_back(aux);
    }

    return cuv;
}

bool Utilitati::selectAction(const std::wstring& actiune)
{
    if (actiune == L"0") return true;
    return false;
}

std::string Utilitati::readFile(std::string& filename)
{

    char caracter = ':';

    size_t pozitie = filename.find(caracter);

    if (pozitie != std::string::npos) 
        filename.erase(pozitie, 1);

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Eroare la deschiderea fisierului " << filename << std::endl;
        return "";
    }
   
    std::string buffer = "";
    std::string tmp = "";

    while (!file.eof())
    {
        std::getline(file, tmp);
        buffer += tmp;
      if(!buffer.empty())  buffer += "\n";
    }
    
    buffer[buffer.size()] = '\0';

    file.close();
    return buffer;
}

void Utilitati::coutFile( std::string& filename, const std::string& buffer)
{
    char caracter = ':';

    size_t pozitie = filename.find(caracter);

    if (pozitie != std::string::npos)
        filename.erase(pozitie, 1);

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Eroare la deschiderea fisierului " << filename << std::endl;
    }

    file << buffer;

    file.close();
}

void Utilitati::sendTeme(Homeworks* teme,SOCKET s)
{
    send(s, std::to_string(teme->getTeme().size()).c_str(),5, 0);  //trimit nr de teme

    char buffer[1024];
    int bytesReceived = recv(s, buffer, sizeof(buffer), 0);

    if (bytesReceived == SOCKET_ERROR)
    {
        std::cout << "EROARE -> RECEIVED FAILED\n";
        //Logger::getInstance()->log("EROARE -> RECEIVED FAILED\n");
    }
    else if (bytesReceived == 0)
    {
        std::cout << "CLIENT DECONECTAT...\n";
        //Logger::getInstance()->log("CLIENT DECONECTAT...\n");
    }
    else
    {
        buffer[bytesReceived] = '\0';
        if (strcmp(buffer, "MORE") != 0)
        {
            return;
        }
    }


    for (auto& tema : teme->getTeme())
    {
        char deTrimis[1024] = "";
     
        strcat(deTrimis, tema.second.first.c_str());
        strcat(deTrimis, "$");
        strcat(deTrimis, tema.second.second.first.c_str());
        strcat(deTrimis, "$");
        strcat(deTrimis, tema.second.second.second.c_str()); //   emailProfesor/autor
        strcat(deTrimis, "$");
        strcat(deTrimis, std::to_string(tema.first).c_str());

        send(s, deTrimis, sizeof(deTrimis), 0);

        int bytesReceived = recv(s, buffer, sizeof(buffer), 0);

        if (bytesReceived == SOCKET_ERROR)
        {
            std::cout << "EROARE -> RECEIVED FAILED\n";
            //Logger::getInstance()->log("EROARE -> RECEIVED FAILED\n");
            break;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "CLIENT DECONECTAT...\n";
            //Logger::getInstance()->log("CLIENT DECONECTAT...\n");
            break;
        }
        else
        {
            buffer[bytesReceived] = '\0';
            if (strcmp(buffer,"MORE") == 0)
            {
                continue;
            }
        }
    }
}

void Utilitati::sendRezolvariTema(std::vector<std::pair<std::string, std::string>>& rezolvari, SOCKET s)
{
    send(s, std::to_string(rezolvari.size()).c_str(), 5, 0);  //trimit nr de rezolvari

    char buffer[1024];
    int bytesReceived = recv(s, buffer, sizeof(buffer), 0);

    if (bytesReceived == SOCKET_ERROR)
    {
        std::cout << "EROARE -> RECEIVED FAILED\n";
        //Logger::getInstance()->log("EROARE -> RECEIVED FAILED\n");
    }
    else if (bytesReceived == 0)
    {
        std::cout << "CLIENT DECONECTAT...\n";
        //Logger::getInstance()->log("CLIENT DECONECTAT...\n");
    }
    else
    {
        buffer[bytesReceived] = '\0';
        if (strcmp(buffer, "MORE") != 0)
        {
            return;
        }
    }


    for (auto& rez : rezolvari)
    {

        std::string buff = readFile(rez.second);

        char deTrimis[1024] = "";

        strcat(deTrimis, rez.first.c_str());
        strcat(deTrimis, "$");
        strcat(deTrimis, buff.c_str());

        send(s, deTrimis, sizeof(deTrimis), 0);

        int bytesReceived = recv(s, buffer, sizeof(buffer), 0);

        if (bytesReceived == SOCKET_ERROR)
        {
            std::cout << "EROARE -> RECEIVED FAILED\n";
            //Logger::getInstance()->log("EROARE -> RECEIVED FAILED\n");
            break;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "CLIENT DECONECTAT...\n";
            //Logger::getInstance()->log("CLIENT DECONECTAT...\n");
            break;
        }
        else
        {
            buffer[bytesReceived] = '\0';
            if (strcmp(buffer, "MORE") == 0)
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
}

void Utilitati::sendStatistici(std::vector<std::pair<float, std::pair<std::string, std::string>>>& stats, SOCKET sock)
{
    send(sock, std::to_string(stats.size()).c_str(), 5, 0);  //trimit nr de rezolvari

    char buffer[1024];
    int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);

    if (bytesReceived == SOCKET_ERROR)
    {
        std::cout << "EROARE -> RECEIVED FAILED\n";
        //Logger::getInstance()->log("EROARE -> RECEIVED FAILED\n");
    }
    else if (bytesReceived == 0)
    {
        std::cout << "CLIENT DECONECTAT...\n";
        //Logger::getInstance()->log("CLIENT DECONECTAT...\n");
    }
    else
    {
        buffer[bytesReceived] = '\0';
        if (strcmp(buffer, "MORE") != 0)
        {
            return;
        }
    }


    for (auto& stat : stats)
    {

        char deTrimis[1024] = "";

        strcat(deTrimis, stat.second.first.c_str());
        strcat(deTrimis, "$");
        strcat(deTrimis, stat.second.second.c_str());
        strcat(deTrimis, "$");
        strcat(deTrimis, std::to_string(stat.first).c_str());

        send(sock, deTrimis, sizeof(deTrimis), 0);

        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);

        if (bytesReceived == SOCKET_ERROR)
        {
            std::cout << "EROARE -> RECEIVED FAILED\n";
            //Logger::getInstance()->log("EROARE -> RECEIVED FAILED\n");
            break;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "CLIENT DECONECTAT...\n";
            //Logger::getInstance()->log("CLIENT DECONECTAT...\n");
            break;
        }
        else
        {
            buffer[bytesReceived] = '\0';
            if (strcmp(buffer, "MORE") == 0)
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
}

void Utilitati::ascultaCombinatiiTaste()
{
    while (true)
    {
        //pentru introducere username si parola
        if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && //CTRL
            (GetAsyncKeyState(VK_MENU) & 0x8000) &&  //ALT
            (GetAsyncKeyState(0x41) & 0x8000))
        {  // 0x41 este codul pentru tasta A

            std::cout << "\nCombination Ctrl+Alt+A detected!" << std::endl;

            if (Admin::getInstance()->getCredentiale())
            {
                std::cout << "Alege una dintre cele 2 optiuni(1/2):\n1)InsertUser\n2)DeleteUser\n";
                int optiune;
                std::cin >> optiune;
                switch (optiune)
                {
                case 1: 
                    Admin::getInstance()->insertUser();
                    break;
                case 2:
                    Admin::getInstance()->deleteUser();
                    break;
                default:
                    std::cout << "Optiune invalida!!!\n";
                    break;

                }
            }

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Pauza pentru a reduce utilizarea procesorului
    }
}

//void Utilitati::iesireMeniu()
//{
//    while (true)
//    {
//        //pentru a iesi din "meniu"
//        if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && //CTRL
//            (GetAsyncKeyState(VK_MENU) & 0x8000) &&  //ALT
//            (GetAsyncKeyState(0x45) & 0x8000))  // E
//        {
//            std::cout << "\nCombination Ctrl+Alt+E detected!" << std::endl;
//            TerminateThread(h, 0);
//        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    }
//}

int Utilitati::distantaLevenshtein(const std::string& s1, const std::string& s2)
{
    int m = s1.size();
    int n = s2.size();
    std::vector<std::vector<int>> dist(m + 1, std::vector<int>(n + 1));

    for (int i = 0; i <= m; ++i) dist[i][0] = i;
    for (int j = 0; j <= n; ++j) dist[0][j] = j;

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) 
        {
            
            dist[i][j] = minim(minim(dist[i - 1][j] + 1,dist[i][j - 1] + 1),dist[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1));
        }
    }
    return dist[m][n];
}

std::vector<std::string> Utilitati::splitString(const std::string& s)
{
    std::istringstream iss(s);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word) 
    {
        words.push_back(word);
    }
    return words;
}

double Utilitati::GetSimilitudine(std::string& str1, std::string& str2)
{

    //insertSpaces(str1);
    //insertSpaces(str2);

    std::string comuneTotal = "";
    std::string comunePartial = "";

    std::vector<std::string> cuvinte1 = splitString(str1);
    std::vector<std::string> cuvinte2 = splitString(str2);

    double procentSimilitudine = 0.0;
    int numar = 0;
    for (auto& cuv1 : cuvinte1)
    {
        double procentMaxim = 0.0;
        double procent = 0.0;

        if (!Utilitati::verifyCommon(cuv1))
        {
            goto label;
        }

        for (auto& cuv2 : cuvinte2)
        {
            int distanta = distantaLevenshtein(cuv1,cuv2);
            int lungime = maxim(cuv1.size(), cuv2.size());
            procent =  (1.0 - static_cast<double>(distanta) / lungime) * 100.0;
            procentMaxim = maxim(procent, procentMaxim);
            if (procentMaxim == 100.0) break;
        }
        
        if (procentMaxim == 100.0)
        {
            comuneTotal += cuv1;
            comuneTotal += " ";
            
        }
        else if (procentMaxim >= 80.0)
        {
            comunePartial += cuv1;
            comunePartial += "   ";
        }

        procentSimilitudine += procentMaxim;
        numar++;
        continue;
    label:
        comuneTotal += cuv1;
        comuneTotal += " ";
        continue;
    }


    std::string fileName100 = "Cuvinte100.txt";
    std::string fileName = "CuvinteAproape100.txt";

    Utilitati::coutFile(fileName100,comuneTotal);
    Utilitati::coutFile(fileName,comunePartial);

    return procentSimilitudine/numar;
}

bool Utilitati::verifyCommon(const std::string& cuv)
{
    std::ifstream file("common.txt");
    if (!file.is_open())
    {
        std::cerr << "Eroare la deschiderea fisierului " << "common.txt" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line == cuv) return 0;
    }
    return 1;
}

void Utilitati::insertSpaces(std::string& s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (!isalpha(s[i+1]))
        {
            s.insert(i+1, " ");
            i++;
        }
    }
}

void Utilitati::insertSpaceBeforeEOL(std::string& str)
{
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i + 1] == '\n')
        {
            str.insert(i + 1, " ");
            i++;
        }
    }
}

void Utilitati::eliminaEnter(std::string& str)
{
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
}


