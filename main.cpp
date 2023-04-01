#include "mainwindow.h"
#include "mainwindow2.h"
#include <QSplashScreen>
#include <QApplication>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>
#include <QPixmap>
#include <QScreen>
#include <QDesktopServices>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QScreen *screen = QGuiApplication::primaryScreen();
    /*
    int screenCount = QApplication::desktop->screenCount();
    QRect screenRect = QApplication::desktop()->screenGeometry(1);
    */

    // Create the splash screen and size it
    QSize screenGeometry = screen->size();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    height = height * .15;
    width = width * .15;
    QSplashScreen splash(QPixmap("://Images/med_logo.png").scaled(width, height, Qt::KeepAspectRatio));
    splash.show();
    splash.raise();
    // Delay the application start to display the splash screen
    QTimer::singleShot(3000, &splash, &QWidget::close);

    MainWindow w;
    w.centralWidget()->setGeometry(QGuiApplication::primaryScreen()->geometry()); // set the geometry of the central widget
    QTimer::singleShot(3000, &w, &QWidget::show);
    return a.exec();
}
