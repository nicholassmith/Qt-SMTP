#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <smtp.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //Grab some details to login to your sending email account
    QString serverName(ui->ui_server->text());
    QString username(ui->ui_username->text());
    QString password(ui->ui_password->text());

    //Grab some details to send to an email
    //To allows you to use multiple emails as long as they're split with ;
    QStringList to = ui->ui_emailTo->text().split(";");
    QString from(ui->ui_emailFrom->text());
    QString subject(ui->ui_subject->text());
    QString body(ui->ui_body->toPlainText());

    //We create an event loop and connect to it to prevent the main thread from being crippled when sending large or slow emails
    QEventLoop loop;
    Smtp *smtp = new Smtp(serverName, username, password, from, to, subject, body);
    QObject::connect(smtp, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

}
