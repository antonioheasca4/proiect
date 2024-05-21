#define _CRT_SECURE_NO_WARNINGS
#include "Compiler.h"

#include "Utilitati.h"

int Compiler::compile(const std::string& cod, int limbaj,SOCKET s)
{
    try
    {
        std::string fileName;
        std::string compilerCommand;
        std::string execCommand;

        if (limbaj == 3)
        {
            fileName = "temp.c";
            compilerCommand = "gcc -o output temp.c 2>&1";
            execCommand = Compiler::exeName;
        }
        else if (limbaj == 4)
        {
            fileName = "temp.cpp";
            compilerCommand = "g++ -o output temp.cpp 2>&1";
            execCommand = Compiler::exeName;
        }
        else if (limbaj == 5)
        {
            fileName = "temp.py";
            execCommand = "python temp.py 2>&1";
        }
        else
        {
            std::cerr << "Limbaj invalid\n";
            // return -1; 
        }


        std::ofstream file(fileName);
        file << cod;
        file.close();

        FILE* outputFile = freopen(Compiler::File_name.c_str(), "w", stdout);
        if (outputFile == nullptr)
        {
           // throw new CompilerExceptie("Eroare la deschidera fisierului ", Compiler::File_name.c_str());
        }

        int compileResult = 0;
        if (limbaj != 5)
        {
            compileResult = system(compilerCommand.c_str());
            if (compileResult != 0)
            {
                //throw new CompilerExceptie("Eroare la compilare: ", compilerCommand.c_str());
            }
            else
            {
                int execResult = system(execCommand.c_str());
                if (execResult != 0)
                {
                    //throw new CompilerExceptie("Eroare la executie: ", execCommand.c_str());
                }
            }
        }
        else
        {
            int execResult = system(execCommand.c_str());
            if (execResult != 0)
            {
                //throw new CompilerExceptie("Eroare la executie: ", execCommand.c_str());
            }
        }

        fclose(outputFile);

        freopen("CON", "w", stdout); // revin la consola

        std::string buf = Utilitati::readFile(Compiler::File_name);
        if (buf.empty()) buf = "$";  // vreau sa trimit ceva totusi
        send(s, buf.c_str(), 1024, NULL);

        return 0;
    }
    catch (IExceptie* e)
    {
        e->handle();
        delete e;
    }
}
