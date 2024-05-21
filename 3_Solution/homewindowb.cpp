#include "homewindowb.h"
#include "ui_homewindowb.h"
#include "mainwindow.h"
#include "clientconnection.h"
#include "Logger.h"
#include "homeworks.h"
#include <QMessageBox>
#include "codeeditor.h"
#include "homeworksolution.h"
#include <QCheckBox>
#include <QRegularExpression>


HomeWindowB::HomeWindowB(QWidget *parent, IUser* currentUser )
    : QMainWindow(parent)
    , ui(new Ui::HomeWindowB)
{

    QFont font;
    font.setPointSize(15);


    ui->setupUi(this);
    showMaximized();
    ui->label_Username->setText(currentUser->getUserName());

    ui->widget->hide();
    setComboBox();
    CodeEditor *codeEditor = new CodeEditor(ui->widget_7);
    codeEditor->setStyleSheet("background-color: black; color: white;");
    QVBoxLayout *layout = new QVBoxLayout(ui->widget_7);
    layout->addWidget(codeEditor);


    CodeEditor *codeEditor2 = new CodeEditor(ui->widget_10);
    codeEditor2->setStyleSheet("background-color: black; color: white;");
    codeEditor2->setFont(font);

    QVBoxLayout *layout2 = new QVBoxLayout(ui->widget_10);
    layout2->addWidget(codeEditor2);


    codeEditor->setFont(font);

    ui->textEdit_5->setFont(font);


    //getStatistic();


    showCompilatePage();

    connect(ui->pushButton_Compilate, &QPushButton::clicked, this, &HomeWindowB::showCompilatePage);
    connect(ui->pushButton_Homeworks, &QPushButton::clicked, this, std::bind(&HomeWindowB::showHomeworksPage, this, currentUser, codeEditor2));
    connect(ui->pushButton_Statistics, &QPushButton::clicked, this, &HomeWindowB::showStatisticsPage);

    connect(ui->pushButton_SignOut, &QPushButton::clicked, this, &HomeWindowB::showSignOutPage);
    connect(ui->pushButton_CompilateCode, &QPushButton::clicked, this, std::bind(&HomeWindowB::on_pushButton_Compilate_clicked, this, codeEditor));

    connect(ui->pushButton_Compilate2, &QPushButton::clicked, this, &HomeWindowB::showCompilatePage);
    connect(ui->pushButton_Homeworks2, &QPushButton::clicked, this, std::bind(&HomeWindowB::showHomeworksPage, this, currentUser, codeEditor2));
    connect(ui->pushButton_Statistics2, &QPushButton::clicked, this, &HomeWindowB::showStatisticsPage);
    connect(ui->pushButton_SignOut2, &QPushButton::clicked, this, &HomeWindowB::showSignOutPage);

    connect(ui->pushButton_UploadHomeworkProf, &QPushButton::clicked, this, std::bind(&HomeWindowB::uploadHomeworkAndTemplateProfesor, this, currentUser, codeEditor2));
}

void HomeWindowB::helpHomeworks(IUser* currentUser, CodeEditor* codeEditor)
{
    std::vector<Homeworks> homeworks = currentUser->getHomeworks();

    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollArea_Homeworks->widget());
    mainLayout->setSpacing(20);
    mainLayout->setAlignment(Qt::AlignCenter);

    QScrollArea* scrollArea = new QScrollArea(ui->scrollArea_Homeworks);
    scrollArea->setWidgetResizable(true);

    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(mainLayout);
    scrollArea->setWidget(scrollWidget);

    if(currentUser->getType()=="Professor"){
        QPushButton *uploadHomeworkButton = new QPushButton("Click to add new homeworks!");
        uploadHomeworkButton->setFlat(true);
        uploadHomeworkButton->setCursor(Qt::PointingHandCursor);

        QFont font;
        font.setPointSize(12);
        uploadHomeworkButton->setFont(font);

        uploadHomeworkButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        uploadHomeworkButton->setStyleSheet("QPushButton { color: white; background-color: black; border: 2px solid white; border-radius: 10px; }");
        uploadHomeworkButton->setFixedSize(500, 75);
        mainLayout->addWidget(uploadHomeworkButton);

        ui->scrollArea_Homeworks->setWidget(scrollArea);

        connect(uploadHomeworkButton, &QPushButton::clicked, this, &HomeWindowB::showUploadHomeworkProfesor);
    }

    if(homeworks[0].getTitle()=="NULL"){
        if(currentUser->getType()=="Student"){
            mainLayout->addWidget(showNoHomeworksPageStudent());
            ui->scrollArea_Homeworks->setWidget(scrollArea);
        }
    }
    else{
        for (Homeworks& home : homeworks) {
            QFrame *frame = new QFrame;
            frame->setStyleSheet("QFrame { border: 2px solid white; border-radius: 10px; background-color: black; }");
            frame->setFixedSize(500, 250);

            QLabel *titleLabel = new QLabel(QString::fromStdString(home.getTitle()));
            QPushButton *enunciationButton = new QPushButton("Click to see the enunciation!");
            QLabel *nameProfessorLabel = new QLabel(QString::fromStdString(home.getNameProfessor()));

            enunciationButton->setFlat(true);
            enunciationButton->setCursor(Qt::PointingHandCursor);

            QFont font;
            font.setPointSize(12);
            titleLabel->setFont(font);
            enunciationButton->setFont(font);
            nameProfessorLabel->setFont(font);

            titleLabel->setStyleSheet("color: white;");
            nameProfessorLabel->setStyleSheet("color: white;");

            titleLabel->setAlignment(Qt::AlignCenter);
            enunciationButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed); // Fixează dimensiunea butonului
            enunciationButton->setFlat(true); // Elimină aspectul de buton
            enunciationButton->setStyleSheet("QPushButton { color: white;  background-color: black; border: 2px solid white; border-radius: 10px;}"); // Elimină stilurile butonului
            enunciationButton->setFixedSize(480, 75);
            nameProfessorLabel->setAlignment(Qt::AlignCenter);

            QHBoxLayout *titleLayout = new QHBoxLayout;
            titleLayout->addWidget(titleLabel);

            QVBoxLayout* layout = new QVBoxLayout(frame);
            layout->addLayout(titleLayout);
            layout->addWidget(nameProfessorLabel);
            layout->addWidget(enunciationButton);

            mainLayout->addWidget(frame);

            connect(enunciationButton, &QPushButton::clicked, this, std::bind(&HomeWindowB::showHomeworksMore, this, home, currentUser, codeEditor));

        }

        ui->scrollArea_Homeworks->setWidget(scrollArea);
    }
}

void HomeWindowB::showCompilatePage() {

    QFont font;
    font.setPointSize(15);
    ui->label_5->setFont(font);
    ui->label_6->setFont(font);

    ui->stackedWidget->setCurrentIndex(0);
}

QFrame* HomeWindowB::showNoHomeworksPageStudent()
{
    QFrame *frame = new QFrame;
    frame->setStyleSheet("QFrame { border: 2px solid black; border-radius: 10px; background-color: black; }"); // Setarea stilului pentru QFrame
    frame->setFixedSize(500, 250);

    QLabel *homeworksLabel = new QLabel("<div style='color: white; font-size: 20px; font-weight: bold;'>There is no homework!<br/>A teacher will upload new homework soon!</div>");
    homeworksLabel->setStyleSheet("color: black;");

    homeworksLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *homeworkLayout = new QHBoxLayout;
    homeworkLayout->addWidget(homeworksLabel);

    QVBoxLayout *frameLayout = new QVBoxLayout(frame);

    frameLayout->addWidget(homeworksLabel);

    return frame;
}

void HomeWindowB::showHomeworksPage(IUser* currentUser, CodeEditor* codeEditor) {

    if(currentUser->getHomeworks2().empty()){
        helpHomeworks(currentUser, codeEditor);
    }

    ui->stackedWidget->setCurrentIndex(3);
}

void HomeWindowB::showUploadHomeworkProfesor()
{
    ui->lineEdit->setText("Title:");
    ui->textEdit_output->setText("Output:\n");
    ui->textEdit_template->setText("Template:\n");
    ui->textEdit->setText("Requirement:\n");

    QFont font;
    font.setPointSize(16);

    ui->lineEdit->setFont(font);
    ui->textEdit_output->setFont(font);
    ui->textEdit_template->setFont(font);
    ui->textEdit->setFont(font);
    ui->pushButton_UploadHomeworkProf->setFont(font);

    ui->stackedWidget->setCurrentIndex(12);
}

void HomeWindowB::showStatisticsPage() {
    getStatistic();
    ui->stackedWidget->setCurrentIndex(4);

}

void HomeWindowB::showHomeworksMore(Homeworks& home, IUser * currentUser, CodeEditor* codeEditor)
{
    if(currentUser->getType()!="Professor"){

        ui->stackedWidget->setCurrentIndex(2);
        ui->pushButton_Template2->setStyleSheet("QPushButton { color: white;  background-color: black; border: 2px solid white; border-radius: 10px;}");
        ui->pushButton_Homework2->setStyleSheet("QPushButton { color: white;  background-color: black; border: 2px solid white; border-radius: 10px;}"); // Elimină stilurile butonului
        ui->pushButton_Compare->setStyleSheet("QPushButton { color: white;  background-color: black; border: 2px solid white; border-radius: 10px;}"); // Elimină stilurile butonului

        QString title = QString::fromStdString(home.getTitle());
        QString enunciation = QString::fromStdString(home.getEnunciation());

        QString formattedText = "<b>" + title + "<br>" + enunciation + "</b>";
        ui->textEdit_3->setText(formattedText);

        QFont titleFont = ui->textEdit_3->font();
        titleFont.setPointSize(15);
        ui->textEdit_3->setFont(titleFont);

        ui->textEdit_3->setAlignment(Qt::AlignTop);
        ui->pushButton_Template2->setStyleSheet("QPushButton { color: white;  background-color: black; border: 2px solid ; border-radius: 10px;}");
        ui->pushButton_Homework2->setStyleSheet("QPushButton { color: white;  background-color: black; border: 2px solid ; border-radius: 10px;}"); // Elimină stilurile butonului
        ui->pushButton_Compare->setStyleSheet("QPushButton { color: white;  background-color: black; border: 2px solid ; border-radius: 10px;}"); // Elimină stilurile butonului

        if(home.getUpload()==1){
            QFont font;
            font.setPointSize(15);

            ui->textEdit_5->setText("The homework has been alredy loaded!");
            ui->textEdit_5->setFont(font);
            ui->textEdit_5->setReadOnly(true);
            ui->pushButton_Homework2->hide();
        }
        else{

            ui->textEdit_5->clear();
            ui->textEdit_5->setReadOnly(false);
            ui->pushButton_Homework2->show();
        }

        connect(ui->pushButton_Homework2, &QPushButton::clicked, this, std::bind(&HomeWindowB::on_pushButton_3_clicked, this, home, currentUser));
        connect(ui->pushButton_Template2, &QPushButton::clicked, this, std::bind(&HomeWindowB::on_pushButton_Template2_clicked, this, home, currentUser));
        connect(ui->pushButton_Compare, &QPushButton::clicked, this, std::bind(&HomeWindowB::on_pushButton_Compare, this, home, currentUser, codeEditor));

    }
    else
    {
        ui->stackedWidget->setCurrentIndex(7);

        QString title = QString::fromStdString(home.getTitle());
        QString enunciation = QString::fromStdString(home.getEnunciation());
        QString formattedText = title + "\n" + enunciation;
        ui->textEdit_2->setText(formattedText);

        QFont titleFont = ui->textEdit_2->font();
        titleFont.setPointSize(15);
        ui->textEdit_2->setFont(titleFont);
        ui->pushButton_Template2->hide();
        ui->textEdit_5->hide();
        ui->pushButton_Homework2->hide();
        ui->pushButton_Compare->hide();
        ui->textEdit_2->setAlignment(Qt::AlignTop);
        ui->textEdit_2->setReadOnly(true);


        if (strcmp(currentUser->getUserName().toStdString().c_str(), home.getNameProfessor().c_str()) != 0)
        {
            ui->pushButton_SeeStudentsSolution->hide();
        }
        else{
            ui->pushButton_SeeStudentsSolution->show();
            connect(ui->pushButton_SeeStudentsSolution, &QPushButton::clicked, this, std::bind(&HomeWindowB::on_pushButton_SeeStudentsSolution_clicked, this, home, currentUser));
        }
    }
}


void HomeWindowB::showSignOutPage() {
    MainWindow* mainWindow = new MainWindow(this);
    this->hide();
    mainWindow->show();
}

void HomeWindowB::setComboBox()
{
    this->ui->comboBox->addItem("C");
    this->ui->comboBox->addItem("C++");
    this->ui->comboBox->addItem("Python");

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
}

HomeWindowB::~HomeWindowB()
{
    delete ui;
}

void HomeWindowB::on_pushButton_Compilate_clicked(CodeEditor* codeEditor)
{
    QFont font;
    font.setPointSize(15);

    codeEditor->setFont(font);

    char * buff;
    QString Code = codeEditor->toPlainText();
    int TypeCode=ui->comboBox->currentIndex();
    TypeCode+=3;

    if(!Code.isEmpty())
    {
        QByteArray byteCode = Code.toUtf8();

        const char* dataCode = byteCode.constData();

        buff= (char*)malloc(strlen(dataCode) +2);

        strcpy(buff, QString::number(TypeCode).toUtf8().constData());
        strcat(buff,"$");
        strcat(buff, byteCode.constData());

        if (ClientConnection::getInstance().sendData(buff)) {
            QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
            qDebug() << message;
            Logger::getInstance()->log(message.toStdString());
        }

        std::string receivedData = ClientConnection::getInstance().receiveDataCompilate();
        QString s = QString::fromLatin1(receivedData);


        if (!s.isEmpty()) {
            // Crearea widget-ului text
            QTextEdit* textEdit = new QTextEdit(this);
            textEdit->setText(s); // Setează textul dorit
            textEdit->setStyleSheet("color: white;");
            textEdit->setReadOnly(true);

            QFont font;
            font.setPointSize(15);

            textEdit->setFont(font);

            ui->scrollArea->setWidget(textEdit);

            ui->scrollArea->setWidgetResizable(true);
        }
        free(buff);
    }
}


 QString HomeWindowB::getSameProcent(QString code, QString s, Homeworks& home, IUser* currentUser)
{

    QString homeTitle = QString::fromStdString(home.getTitle());
    QString homeEnun = QString::fromStdString(home.getEnunciation());
    QString homeProff = QString::fromStdString(home.getNameProfessor());
    QString studentName = currentUser->getUserName();

    char* buff;
    QByteArray code2 = code.toUtf8();
    QByteArray compilateCode=s.toUtf8();
    QByteArray homeTitle2=homeTitle.toUtf8();
    QByteArray homeEnun2=homeEnun.toUtf8();
    QByteArray homeProff2=homeProff.toUtf8();
    QByteArray studentName2=studentName.toUtf8();


    const char* code3 = code2.constData();
    const char* compilateCode2 = compilateCode.constData();
    const char* homeTitle3 = homeTitle2.constData();
    const char* homeEnun3 = homeEnun2.constData();
    const char* homeProff3 = homeProff2.constData();
    const char* studentName3 = studentName2.constData();

    buff= (char*)malloc(strlen(code3) +strlen(compilateCode2)+strlen(homeTitle3)+strlen(homeEnun3)+strlen(homeProff3)+strlen(studentName3)+8);

    strcpy(buff, "C$");
    strcat(buff, code3);
    strcat(buff, "$");
    strcat(buff, compilateCode2);
    strcat(buff, "$");
    strcat(buff, homeTitle3);
    strcat(buff, "$");
    strcat(buff, homeEnun3);
    strcat(buff, "$");
    strcat(buff, homeProff3);
    strcat(buff, "$");
    strcat(buff, studentName3);

    if (ClientConnection::getInstance().sendData(buff)) {
        QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
        qDebug() << message;
        Logger::getInstance()->log(message.toStdString());
    }

    std::string receivedData = ClientConnection::getInstance().receiveDataCompilate();
    QString grade = QString::fromLatin1(receivedData);

    return grade;
 }


void HomeWindowB::on_pushButton_Compilate_clicked_2(CodeEditor * codreEditor, IUser* currentUser, Homeworks& home)
{

   // ui->label_12->clear();
    char * buff;
    QString Code = codreEditor->toPlainText();
    int TypeCode=ui->comboBox->currentIndex();
    TypeCode+=3;

    if(!Code.isEmpty())
    {
        QByteArray byteCode = Code.toUtf8();

        const char* dataCode = byteCode.constData();

        buff= (char*)malloc(strlen(dataCode) +2);

        strcpy(buff, QString::number(TypeCode).toUtf8().constData());
        strcat(buff,"$");
        strcat(buff, byteCode.constData());

        if (ClientConnection::getInstance().sendData(buff)) {
            QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
            qDebug() << message;
            Logger::getInstance()->log(message.toStdString());
        }

        std::string receivedData = ClientConnection::getInstance().receiveDataCompilate();
        QString s = QString::fromLatin1(receivedData);


        if (!s.isEmpty()) {

            QTextEdit* textEdit = new QTextEdit(this);

            textEdit->setText(s); // Setează textul dorit
            textEdit->setStyleSheet("color: white;");
            textEdit->setReadOnly(true);

            QFont font;
            font.setPointSize(15);

            textEdit->setFont(font);

            ui->scrollArea_4->setWidget(textEdit);

            ui->scrollArea_4->setWidgetResizable(true);

            QString currentText;


            //TREBUIE SA TRIMIT BUFFER UL CU OUTPUP CARE E

            QString output=ui->label_12->text();



            QString grade=getSameProcent(s, output, home, currentUser);


            currentText.append(home.getTitle());
            currentText.append("Output:");
            currentText.append("\n");
            currentText.append(ui->label_12->text());
            currentText.append("\n");
            currentText.append(grade);

            ui->label_12->setText(currentText);

        }
        free(buff);
    }

    //ui->label_12->clear();
    //delete codeEditor;
}

void HomeWindowB::clickUploadHomework(Homeworks& home, IUser* currentUser) {

    if(home.getUpload()==1)
        ui->textEdit_5->setText("The homework has been alredy loaded!");
    else
        ui->textEdit_5->clear();


    QString code = ui->textEdit_5->toPlainText();

    if(!code.isEmpty()) {
        QByteArray byteCode = code.toUtf8();
        const char* dataCode = byteCode.constData();

        char* buff = (char*)malloc(strlen(dataCode) + home.getTitle().length() + home.getNameProfessor().length() + currentUser->getUserName().length() + 6);

        if (buff != nullptr) {
            strcpy(buff, "S$");
            strcat(buff, currentUser->getUserName().toUtf8().constData());
            strcat(buff, "$");
            strcat(buff, home.getTitle().c_str());
            strcat(buff, "$");
            strcat(buff, home.getNameProfessor().c_str());
            strcat(buff, "$");
            strcat(buff, dataCode);

            // Trimiterea datelor către server
            if (ClientConnection::getInstance().sendData(buff)) {
                QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
                qDebug() << message;
                Logger::getInstance()->log(message.toStdString());
            } else {
                qDebug() << "Failed to send data.";
            }

            free(buff);

            if(ClientConnection::getInstance().receiveDataCompilate()=="OK")
            {
                QMessageBox::critical(this, "Homework", "The homework has been successfully loaded!");
                ui->textEdit_5->setReadOnly(true);
                ui->textEdit_5->setText("The homework has been alredy loaded!");

                QFont font;
                font.setPointSize(15);

                ui->textEdit_5->setFont(font);

                ui->pushButton_Homework2->hide();
            }
            else
            {
                QMessageBox::critical(this, "Error", "The homework could not be loaded! Try again!");
            }
        }
    }
}

void HomeWindowB::clickUploadTemplate(Homeworks& home, IUser* currentUser)
{

    char* buff = (char*)malloc(home.getTitle().length() + home.getNameProfessor().length()+4);

    if (buff != nullptr) {

        strcpy(buff, "T$");
        strcat(buff, home.getTitle().c_str());
        strcat(buff, "$");
        strcat(buff, home.getNameProfessor().c_str());

        if (ClientConnection::getInstance().sendData(buff)) {
            QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
            qDebug() << message;
            Logger::getInstance()->log(message.toStdString());
        } else {
            qDebug() << "Failed to send data.";
        }

        free(buff);

        std::string temp=ClientConnection::getInstance().receiveDataCompilate();
        if(!temp.empty())
        {
            QWidget* containerWidget = new QWidget(this);

            QVBoxLayout* layout = new QVBoxLayout(containerWidget);
            layout->setAlignment(Qt::AlignCenter);

            QTextEdit* textEdit = new QTextEdit(this);
            textEdit->setText(QString::fromStdString(temp));
            textEdit->setReadOnly(true);
            textEdit->setAlignment(Qt::AlignCenter);

            textEdit->setStyleSheet("QTextEdit { font-size: 18pt; }");

            layout->addWidget(textEdit);

            ui->scrollArea_2->setWidget(containerWidget);

            ui->scrollArea_2->setWidgetResizable(true);

            ui->stackedWidget->setCurrentIndex(5);
        }
    }
}


void HomeWindowB::uploadHomeworkAndTemplateProfesor(IUser* currentUser, CodeEditor* codeEditor)
{
    QString title = ui->lineEdit->text();
    if (title.startsWith("Title:")) {
        title.remove(0, QString("Title:").length());
    }


    QString homework = ui->textEdit->toPlainText();
    if (homework.startsWith("Requirement:\n")) {
        homework.remove(0, QString("Requirement:\n").length());
    }


    QString output = ui->textEdit_output->toPlainText();
    if (output.startsWith("Output:\n")) {
        output.remove(0, QString("Output:\n").length());
    }

    QString temp=ui->textEdit_template->toPlainText();
    if (temp.startsWith("Template:\n")) {
        temp.remove(0, QString("Template:\n").length());
    }


    if (!title.isEmpty() && !homework.isEmpty() && !temp.isEmpty() && !output.isEmpty()){

        QByteArray titleCode = title.toUtf8();
        QByteArray homeworkCode = homework.toUtf8();
        QByteArray tempCode = temp.toUtf8();
        QByteArray outputCode = output.toUtf8();

        const char* titleCode2 = titleCode.constData();
        const char* homeworkCode2 = homeworkCode.constData();
        const char* tempCode2 = tempCode.constData();
        const char* outputCode2 = outputCode.constData();


        char* buff = (char*)malloc(strlen(titleCode2) + strlen(homeworkCode2)+currentUser->getUserName().length() + 6);

        strcpy(buff, "A$");
        strcat(buff, titleCode2);
        strcat(buff, "$");
        strcat(buff, homeworkCode2);
        strcat(buff, "$");
        strcat(buff, currentUser->getUserName().toUtf8().constData());
        strcat(buff, "$");

        if (ClientConnection::getInstance().sendData(buff)) {
            QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
            qDebug() << message;
            Logger::getInstance()->log(message.toStdString());
        } else {
            qDebug() << "Failed to send data.";
        }

        if(ClientConnection::getInstance().receiveDataCompilate()=="OK"){
            char* buff2 = (char*)malloc(strlen(tempCode2)+strlen(titleCode2)+currentUser->getUserName().length()+ strlen(outputCode2)+6);

            strcpy(buff2, "TT$");
            strcat(buff2, titleCode2);
            strcat(buff2, "$");
            strcat(buff2, currentUser->getUserName().toUtf8().constData());
            strcat(buff2, "$");
            strcat(buff2, tempCode2);
            strcat(buff2, "$");
            strcat(buff2, outputCode2);


            if (ClientConnection::getInstance().sendData(buff2)) {
                QString message = "Message sent: " + QString::fromUtf8(buff2) + "\n";
                qDebug() << message;
                Logger::getInstance()->log(message.toStdString());
            } else {
                qDebug() << "Failed to send data.";
            }
            free(buff2);
        }

        if(ClientConnection::getInstance().receiveDataCompilate()=="OK")
            {
            QMessageBox::information(this, "Homework", "The homework and the template has been successfully loaded!");
            currentUser->setHomeworksEmpty();
            helpHomeworks(currentUser, codeEditor);
            ui->stackedWidget->setCurrentIndex(3);
        }
        else
        {
            QMessageBox::critical(this, "Error", "The homework and the template could not be loaded! Try again!");
        }

        free(buff);
    }
}

void HomeWindowB::on_pushButton_Template2_clicked(Homeworks& home, IUser* currentUser)
{

    char* buff = (char*)malloc(home.getTitle().length() + home.getNameProfessor().length()+4);

    if (buff != nullptr) {

        strcpy(buff, "T$");
        strcat(buff, home.getTitle().c_str());
        strcat(buff, "$");
        strcat(buff, home.getNameProfessor().c_str());

        if (ClientConnection::getInstance().sendData(buff)) {
            QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
            qDebug() << message;
            Logger::getInstance()->log(message.toStdString());
        } else {
            qDebug() << "Failed to send data.";
        }

        free(buff);

        std::string temp=ClientConnection::getInstance().receiveDataCompilate();
        if(!temp.empty())
        {
            QWidget* containerWidget = new QWidget(this);

            QVBoxLayout* layout = new QVBoxLayout(containerWidget);
            layout->setAlignment(Qt::AlignCenter);

            QTextEdit* textEdit = new QTextEdit(this);
            textEdit->setText(QString::fromStdString(temp));
            textEdit->setReadOnly(true);
            textEdit->setAlignment(Qt::AlignCenter);

            textEdit->setStyleSheet("QTextEdit { font-size: 18pt; }");
            layout->addWidget(textEdit);

            ui->scrollArea_2->setWidget(containerWidget);

            ui->scrollArea_2->setWidgetResizable(true);

            ui->stackedWidget->setCurrentIndex(5);
        }
    }

}

void HomeWindowB::on_pushButton_3_clicked(Homeworks& home, IUser* currentUser)
{

    QString code = ui->textEdit_5->toPlainText();

    if(!code.isEmpty()) {
        QByteArray byteCode = code.toUtf8();
        const char* dataCode = byteCode.constData();

        char* buff = (char*)malloc(strlen(dataCode) + home.getTitle().length() + home.getNameProfessor().length() + currentUser->getUserName().length() + 6);

        if (buff != nullptr) {
            strcpy(buff, "S$");
            strcat(buff, currentUser->getUserName().toUtf8().constData());
            strcat(buff, "$");
            strcat(buff, home.getTitle().c_str());
            strcat(buff, "$");
            strcat(buff, home.getNameProfessor().c_str());
            strcat(buff, "$");
            strcat(buff, dataCode);

            // Trimiterea datelor către server
            if (ClientConnection::getInstance().sendData(buff)) {
                QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
                qDebug() << message;
                Logger::getInstance()->log(message.toStdString());
            } else {
                qDebug() << "Failed to send data.";
            }

            free(buff);

            if(ClientConnection::getInstance().receiveDataCompilate()=="OK")
            {

                home.setUpload(1);
                QMessageBox::information(this, "Homework", "The homework has been successfully loaded!");
                ui->textEdit_5->setReadOnly(true);

                QFont font;
                font.setPointSize(15);

                ui->textEdit_5->setText("The homework has been alredy loaded!");
                ui->textEdit_5->setFont(font);

                ui->pushButton_Homework2->hide();
            }
            else
            {
                QMessageBox::critical(this, "Error", "The homework could not be loaded! Try again!");
            }
        }
    }
}


void HomeWindowB::on_pushButton_Compare(Homeworks& home, IUser* currentUser, CodeEditor* codeEditor)
{

    QTextEdit* textEdit = new QTextEdit(this);
    textEdit->setText("");
    ui->scrollArea_4->setWidget(textEdit);

    ui->scrollArea_4->setWidgetResizable(true);
    ui->scrollArea_4->setWidget(textEdit);

    ui->scrollArea_4->setWidgetResizable(true);

    codeEditor->clear();
    QFont font;
    font.setPointSize(15);
    ui->label_8->setFont(font);
    ui->label_11->setFont(font);

    this->ui->comboBox_2->addItem("C");
    this->ui->comboBox_2->addItem("C++");
    this->ui->comboBox_2->addItem("Python");

    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));

    // CodeEditor *codeEditor = new CodeEditor(ui->widget_10);
    // codeEditor->setStyleSheet("background-color: black; color: white;");
    // codeEditor->setFont(font);

    // QVBoxLayout *layout = new QVBoxLayout(ui->widget_10);
    // layout->addWidget(codeEditor);

    ui->stackedWidget->setCurrentIndex(11);

    ui->label_12->setText("Output professor:\n");

    std::string title=home.getTitle();
    std::string nameProfessor=home.getNameProfessor();

    char* buff = (char*)malloc(title.length()+nameProfessor.length()+4);

    if (buff != nullptr){
        strcpy(buff, "V$");
        strcat(buff, title.c_str());
        strcat(buff, "$");
        strcat(buff, nameProfessor.c_str());

        if (ClientConnection::getInstance().sendData(buff)) {

                QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
            qDebug() << message;
            Logger::getInstance()->log(message.toStdString());
            }
        else {
            qDebug() << "Failed to send data.";
        }

        free(buff);

        std::string mess=ClientConnection::getInstance().receiveDataCompilate();

        QString currentText = QString::fromStdString(home.getTitle());

        currentText.append("Output:<br>");
        currentText.append(QString::fromStdString(mess).replace("\n", "<br>"));

        ui->label_12->setText(currentText);

        ui->label_12->setAlignment(Qt::AlignTop);

        ui->label_12->setFont(font);
    }

   // delete codeEditor;

    connect(ui->pushButton_CompilateCode_2, &QPushButton::clicked, this, std::bind(&HomeWindowB::on_pushButton_Compilate_clicked_2, this, codeEditor, currentUser, home));
}


void HomeWindowB::showSolution(HomeworkSolution& home)
{
    ui->label_3->setText(QString::fromStdString(home.getSolution()));

    QFont font = ui->label_3->font();
    font.setPointSize(15);
    ui->label_3->setFont(font);

    ui->label_3->setStyleSheet("QLabel { background-color: black; color: white; }");

    ui->label_3->setTextInteractionFlags(Qt::NoTextInteraction);

    ui->stackedWidget->setCurrentIndex(9);
}

void HomeWindowB::on_pushButton_SeeStudentsSolution_clicked(Homeworks& home, IUser* currentUser)
{

    std::vector<HomeworkSolution> homeworksS = home.getSolution(currentUser->getUserName());

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignCenter);


    QLabel* messageLabel = new QLabel("Please select two themes to view their solutions compared");
    QFont messageFont;
    messageFont.setPointSize(10); // Font size redus
    messageLabel->setFont(messageFont);
    messageLabel->setStyleSheet(
        "QLabel { "
        "background-color: black; "
        "color: white; "
        "border: 2px solid white; "
        "border-radius: 15px; "
        "padding: 5px; "
        "}"
        );
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setFixedSize(500, 50);
    mainLayout->addWidget(messageLabel);

    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(mainLayout);
    ui->scrollArea_3->setWidget(scrollWidget);
    ui->scrollArea_3->setWidgetResizable(true);

    // Adaugă teme în layout
    std::vector<QCheckBox*> checkBoxes;
    for (HomeworkSolution& homework : homeworksS) {
        QFrame* frame = new QFrame;
        frame->setStyleSheet("QFrame { border: 2px solid white; border-radius: 10px; background-color: black; }");
        frame->setFixedSize(500, 250);

        QLabel* nameLabel = new QLabel(QString::fromStdString(homework.getName()));

        QPushButton *solutionButton = new QPushButton("Click to see the solution!");

        solutionButton->setFlat(true);
        solutionButton->setCursor(Qt::PointingHandCursor);

        QCheckBox* selectCheckBox = new QCheckBox("Select this homework");
        checkBoxes.push_back(selectCheckBox);

        QFont font;
        font.setPointSize(12);
        nameLabel->setFont(font);
        solutionButton->setFont(font);
        selectCheckBox->setFont(font);

        nameLabel->setStyleSheet("color: white;");
        nameLabel->setAlignment(Qt::AlignCenter);

        solutionButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        solutionButton->setFlat(true);
        solutionButton->setStyleSheet("QPushButton { color: white;  background-color: black; border: 2px solid white; border-radius: 10px;}");
        solutionButton->setFixedSize(500, 75);

        selectCheckBox->setStyleSheet("QCheckBox { color: white; }");

        QVBoxLayout* layout = new QVBoxLayout(frame);
        layout->addWidget(nameLabel);
        layout->addWidget(solutionButton);
        layout->addWidget(selectCheckBox);

        mainLayout->addWidget(frame);
    }

    // Adaugă un buton pentru confirmarea selecției
    QPushButton* confirmButton = new QPushButton("Confirm Selection");
    confirmButton->setStyleSheet("QPushButton { color: white; background-color: black; border: 2px solid white; border-radius: 10px; }");
    confirmButton->setFixedSize(500, 50);
    mainLayout->addWidget(confirmButton);

    connect(confirmButton, &QPushButton::clicked, this, [this, checkBoxes, homeworksS]() {
        std::vector<HomeworkSolution> selectedHomeworks;
        for (size_t i = 0; i < checkBoxes.size(); ++i) {
            if (checkBoxes[i]->isChecked()) {
                selectedHomeworks.push_back(homeworksS[i]);
            }
        }

        if (selectedHomeworks.size() == 2) {
            showSelectedHomeworksPage(selectedHomeworks);
        } else {
            QMessageBox::warning(this, "Selection Error", "Please select exactly two homeworks.");
        }
    });

    ui->scrollArea_3->setWidget(scrollWidget);
    ui->stackedWidget->setCurrentIndex(8);
}


QString HomeWindowB::highlightMatchingWords(QString& buffer1, const QString& buffer2)
{
    // Splitarea conținutului din buffere în cuvinte folosind spațiul ca separator și ignorarea părților goale

    // QString s1 = "ana are mere";
    // QString s2 = "ana";

    for (int i = 0; i < buffer1.size() - 1; i++)
    {
        if (buffer1[i + 1] == '\n')
        {
            buffer1.insert(i + 1, " ");
            i++;
        }
    }

    QStringList words1 = buffer1.split(" ", Qt::SkipEmptyParts);
    QStringList words2 = buffer2.split(" ", Qt::SkipEmptyParts);

    // Crearea unui text HTML pentru afișare
    QString htmlText = "<html><body>";


    for (const QString& word : words1)
    {
        bool ok=false;
        for (const QString& word2 : words2)
        {
            if(word == word2)
            {
                htmlText += "<span style='color:red;'>" + word + "</span> ";
                ok = true;
                break;
            }
        }
        if(ok==false)
        {
            htmlText += word + " ";
        }
    }

    htmlText.replace("\n", "<br>");

    htmlText += "</body></html>";

    return htmlText;
}


void sendSolutionToServer(const QString& solution1, const QString& solution2) {

    QByteArray solution1Code = solution1.toUtf8();
    QByteArray solution2Code = solution2.toUtf8();

    const char* solution1Code1 = solution1Code.constData();
    const char* solution2Code2 = solution2Code.constData();

    char* buff = (char*)malloc(strlen(solution1Code1) + strlen(solution2Code2)+ 4);

    strcpy(buff, "CS$");
    strcat(buff, solution1Code1);
    strcat(buff, "$");
    strcat(buff, solution2Code2);
    strcat(buff, "$");

    if (ClientConnection::getInstance().sendData(buff)) {
        QString message = "Message sent: " + QString::fromUtf8(buff) + "\n";
        qDebug() << message;
        Logger::getInstance()->log(message.toStdString());
    } else {
        qDebug() << "Failed to send data.";
    }
}

std::string receiveSolutionFromServer() {
    std::string receivedData = ClientConnection::getInstance().receiveDataCompilate();
    return receivedData;
}

void HomeWindowB::showSelectedHomeworksPage(std::vector<HomeworkSolution>& selectedHomeworks) {
    if (selectedHomeworks.size() < 2) {
        qWarning() << "Insufficient number of selected homework solutions.";
        return;
    }

    QString solution1 = QString::fromStdString(selectedHomeworks[0].getSolution());
    QString solution2 = QString::fromStdString(selectedHomeworks[1].getSolution());

    sendSolutionToServer(solution1, solution2);

    std::string sameSolution = receiveSolutionFromServer();
    QString sameSolutionQ = QString::fromStdString(sameSolution);

    ui->label_9->setText(highlightMatchingWords(solution1, sameSolutionQ));
    ui->label_10->setText(highlightMatchingWords(solution2, sameSolutionQ));

    ui->label_9->setWordWrap(true);
    ui->label_9->setTextFormat(Qt::RichText);  // Asigură-te că textul este formatat ca HTML
    ui->label_10->setWordWrap(true);
    ui->label_10->setTextFormat(Qt::RichText);  // Asigură-te că textul este formatat ca HTML

    QFont labelFont;
    labelFont.setPointSize(15);

    ui->label_9->setFont(labelFont);
    ui->label_10->setFont(labelFont);

    ui->label_9->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->label_10->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    ui->label_9->setStyleSheet("QLabel { background-color: black; color: white; padding: 5px; }");
    ui->label_10->setStyleSheet("QLabel { background-color: black; color: white; padding: 5px; }");

    ui->stackedWidget->setCurrentIndex(9);
}


Statistics parseStatistic(const std::string& line) {
    std::istringstream iss(line);
    std::string fisrtName, lastName, procent;


    std::getline(iss, fisrtName, '$');
    std::getline(iss, lastName, '$');
    std::getline(iss, procent, '$');


    return Statistics(fisrtName, lastName, procent);
}


std::vector<Statistics> statisticFunction()
{
    char * buffer=(char*)malloc(strlen("stat")+1);

    std::vector<Statistics> vectorStatistics;

    strcpy(buffer, "stat");

    if (ClientConnection::getInstance().sendData(buffer)) {
        QString message = "Message sent: " + QString::fromUtf8(buffer) + "\n";
        qDebug() << message;
        Logger::getInstance()->log(message.toStdString());
    }

    std::string numberStatistics=ClientConnection::getInstance().receiveDataHomeworks();
    int numberStatisticsInt=stoi(numberStatistics);

    if (ClientConnection::getInstance().sendData("MORE"))
    {
        QString message = "Message sent: " + QString::fromUtf8("MORE") + "\n";
        qDebug() << message;
        Logger::getInstance()->log(message.toStdString());
    }

    std::string statisticStringAll;

    if (numberStatisticsInt == 0) {
        Statistics stat=Statistics("NULL", "NULL", "NULL");
        vectorStatistics.push_back(stat);
    }
    else
    {
        for(int i=0; i<numberStatisticsInt; i++){
            statisticStringAll = ClientConnection::getInstance().receiveDataHomeworks();
            Statistics stat1=parseStatistic(statisticStringAll);
            vectorStatistics.push_back(stat1);
            if (ClientConnection::getInstance().sendData("MORE"))
            {
                QString message = "Message sent: " + QString::fromUtf8("MORE") + "\n";
                qDebug() << message;
                Logger::getInstance()->log(message.toStdString());
            }
        }
    }


    return vectorStatistics;
}

void HomeWindowB::setLabelProperties(QLabel* label)
{
    QFont font;
    font.setPointSize(12);
    label->setFont(font);
    label->setStyleSheet("color: white;");
    label->setAlignment(Qt::AlignCenter);
}

void HomeWindowB::getStatistic()
{
    std::vector<Statistics> vectorStatistics = statisticFunction();

    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollArea_Statistic->widget());
    mainLayout->setSpacing(20);
    mainLayout->setAlignment(Qt::AlignCenter);

    for (Statistics& stat : vectorStatistics) {
        QFrame* frame = new QFrame;
        frame->setStyleSheet("QFrame { border: 2px solid white; border-radius: 10px; background-color: black; }");
        frame->setFixedSize(500, 150);

        QVBoxLayout* layout = new QVBoxLayout(frame);


        QLabel* Name = new QLabel(QString::fromStdString(stat.getFirstName()) + " " + QString::fromStdString(stat.getLastName()));


        QString procentText= QString::fromStdString("Procentul este: " + stat.getProcent());

        int nullIndex =procentText.indexOf('\0');
        if (nullIndex != -1) {
            procentText = procentText.left(nullIndex);
        }

        QLabel* procent = new QLabel(procentText);


        setLabelProperties(Name);
        setLabelProperties(procent);

        layout->addWidget(Name);
        layout->addWidget(procent);

        mainLayout->addWidget(frame);
    }

    ui->scrollArea_Statistic->setWidgetResizable(true);
    ui->scrollArea_Statistic->setWidget(mainLayout->parentWidget());

    vectorStatistics.empty();
}


// void HomeWindowB::getStatistic()
// {
//     std::vector<Statistics> vectorStatistics=statisticFunction();

//     QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollArea_Statistic->widget());
//     mainLayout->setSpacing(20);
//     mainLayout->setAlignment(Qt::AlignCenter);

//     QScrollArea* scrollArea = new QScrollArea(ui->scrollArea_Statistic);
//     scrollArea->setWidgetResizable(true);

//     QWidget* scrollWidget = new QWidget();
//     scrollWidget->setLayout(mainLayout);
//     scrollArea->setWidget(scrollWidget);


//     for (Statistics& stat : vectorStatistics) {
//         QFrame *frame = new QFrame;
//         frame->setStyleSheet("QFrame { border: 2px solid white; border-radius: 10px; background-color: black; }");
//         frame->setFixedSize(500, 250);

//         QLabel *firstName = new QLabel(QString::fromStdString(stat.getFirstName()));
//         QLabel *lastName = new QLabel(QString::fromStdString(stat.getLastName()));
//         QLabel *procent = new QLabel(QString::fromStdString(stat.getProcent()));


//         QFont font;
//         font.setPointSize(12);
//         firstName->setFont(font);
//         lastName->setFont(font);
//         procent->setFont(font);

//         firstName->setStyleSheet("color: white;");
//         lastName->setStyleSheet("color: white;");
//         procent->setStyleSheet("color: white;");

//         firstName->setAlignment(Qt::AlignCenter);
//         lastName->setAlignment(Qt::AlignCenter);
//         procent->setAlignment(Qt::AlignCenter);


//         QVBoxLayout* layout = new QVBoxLayout(frame);
//         layout->addWidget(firstName);
//         layout->addWidget(lastName);
//         layout->addWidget(procent);

//         mainLayout->addWidget(frame);
//     }

//     ui->scrollArea_Statistic->setWidget(scrollArea);
// }

