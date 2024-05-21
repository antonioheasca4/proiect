#ifndef HOMEWINDOWB_H
#define HOMEWINDOWB_H


#include "iuser.h"
#include <QMainWindow>
#include <QFrame>
#include "codeeditor.h"
#include <QLabel>
#include "statistics.h"

namespace Ui {
class HomeWindowB;
}

class HomeWindowB : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindowB(QWidget *parent = nullptr, IUser *curentUser=nullptr);
    ~HomeWindowB();

private slots:
    void on_pushButton_Compilate_clicked(CodeEditor * codreEditor);

    void on_pushButton_Compilate_clicked_2(CodeEditor * codreEditor, IUser* currentUser, Homeworks& home);

    void on_pushButton_Template2_clicked(Homeworks& home, IUser* currentUser);

    void on_pushButton_3_clicked(Homeworks& home, IUser* currentUser);

    void on_pushButton_Compare(Homeworks& home, IUser* currentUser, CodeEditor* codeEditor);

    void on_pushButton_SeeStudentsSolution_clicked(Homeworks& home, IUser* currentUser);

private:

    void setLabelProperties(QLabel* label);
    void getStatistic();
    void showCompilatePage();
    void showHomeworksPage(IUser* currentUser, CodeEditor* codeEditor);
    void showStatisticsPage();
    void showSignOutPage();
    void showUploadHomeworkProfesor();
    void showHomeworksMore(Homeworks& home, IUser* currentUser, CodeEditor* codeEditor);
    void clickUploadHomework(Homeworks& home, IUser *currentUser);
    void clickUploadTemplate(Homeworks& home, IUser* currentUser);
    void uploadHomeworkAndTemplateProfesor(IUser* currentUser, CodeEditor* codeEditor);
    QFrame* showNoHomeworksPageStudent();

    void showSolution(HomeworkSolution& home);
    void showSelectedHomeworksPage(std::vector<HomeworkSolution>& selectedHomeworks);

    QString highlightMatchingWords(QString& buffer1, const QString& buffer2);
    //void highlightWords(const QString& buffer1, const QString& buffer2, QLabel* label);

    QString getSameProcent(QString code, QString s, Homeworks& home, IUser* currentUser);
    void helpHomeworks(IUser* currentUser, CodeEditor* codeEditor);
    void setComboBox();

private:
    Ui::HomeWindowB *ui;
};

#endif // HOMEWINDOWB_H
