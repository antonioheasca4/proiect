#ifndef HOMEWORKSOLUTION_H
#define HOMEWORKSOLUTION_H

#include <string>

class HomeworkSolution
{
public:
    HomeworkSolution(std::string nameStudent, std::string solution){
        this->nameStudent=nameStudent;
        this->solution=solution;
    }

    std::string getName() {return nameStudent;}
    std::string getSolution() {return solution; }

private:
    std::string nameStudent;
    std::string solution;
};

#endif // HOMEWORKSOLUTION_H
