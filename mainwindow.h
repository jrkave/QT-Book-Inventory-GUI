#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "mainwindow2.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void login();
    void createAccount();

private:
    Ui::MainWindow *ui;
    MainWindow2 *window2;
};
#endif // MAINWINDOW_H
