#ifndef STATISTICS_H
#define STATISTICS_H

#include "iostream"
#include "string"


class Statistics
{
public:
    Statistics(std::string studentFirstName, std::string studentLastName, std::string procent){
        this->studentFirstName=studentFirstName;
        this->studentLastName=studentLastName;
        this->procent=procent;
    }

    std::string getFirstName() { return this->studentFirstName;}
    std::string getLastName() { return this->studentLastName;}
    std::string getProcent() { return this->procent;}

private:

    std::string studentFirstName;
    std::string studentLastName;
    std::string procent;
};

#endif // STATISTICS_H
