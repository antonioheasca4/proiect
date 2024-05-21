#ifndef HOMEWORKS_H
#define HOMEWORKS_H

#include <iostream>
#include <vector>
#include <QString>
#include "homeworksolution.h"

class Homeworks
{
public:
    Homeworks(std::string title, std::string enunciation, std::string nameProfessor, int upload){
        this->title=title;
        this->enunciation=enunciation;
        this->nameProfessor=nameProfessor;
        this->upload=upload;
    }

    std::string getTitle() {return this->title;}
    std::string getEnunciation() {return this->enunciation;}
    std::string getNameProfessor() {return this->nameProfessor;}


    void setSolution(QString nameProfessor);
    std::vector<HomeworkSolution>getSolution(QString userName)
    {
        setSolution(userName);
        return solution;
    }



    int getUpload() {return this->upload; }
    void setUpload(int upload) {this->upload=upload;}

private:
    std::string title;
    std::string enunciation;
    std::string nameProfessor;
    int upload=0;
    std::vector<HomeworkSolution> solution;
};

#endif // HOMEWORKS_H
