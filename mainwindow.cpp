#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "userops.h"
#include <QMessageBox>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createAccount() {
    UserOps userOps;
    QString qnewUsername = ui->usr1LineEdit->text();
    QString qnewPassword = ui->pwd1LineEdit->text();
    bool addedUser = false;
    addedUser = userOps.createNewAccount(qnewUsername.toStdString(), qnewPassword.toStdString());

    if (addedUser) {
        QMessageBox::information(this, "Create Account", ("Thanks for creating an account with us, " + qnewUsername + "."));
        window2 = new MainWindow2();
        window2->showMenuPage();
        window2->show();
        hide();
    }
    else {
        QMessageBox::warning(this, "Create Account", "User already exists.");
    }
}

void MainWindow::login()
{
    UserOps userOps;
    QString qLoginUsername = ui->usr2LineEdit->text();
    QString qLoginPassword = ui->pwd2LineEdit->text();
    bool loginUser = false;
    loginUser = userOps.verifyReturningUser(qLoginUsername.toStdString(), qLoginPassword.toStdString());

    if (loginUser) {
        QMessageBox::information(this, "Login", ("Welcome back, " + qLoginUsername));
        window2 = new MainWindow2();
        window2->showMenuPage();
        window2->show();
        hide();
    }
    else {
        QMessageBox::warning(this, "Login", "Invalid username or password. Please try again.");
    }
}
