#include "Logger.h"

Logger* Logger::instance = nullptr;

Logger* Logger::getInstance()
{
    if (instance == nullptr)
        instance = new Logger;
    return instance;
}

void Logger::destroyInstance()
{
    if (instance)
    {
        delete instance;
    }
}

void Logger::log(const std::string& s)
{
    QFile file("logClient.txt");
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << QString::fromStdString(s);
        file.close();
    }
}

