#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H
#include "dbmanager.h"
#include <QMainWindow>

namespace Ui {
class MainWindow2;
}

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget *parent = nullptr);
    ~MainWindow2();
    void showMenuPage();
    bool validInput(QString ISBN, QString year, QString MSRP, QString quantity);

private:
    Ui::MainWindow2 *ui;
    DbManager db;
    QStandardItemModel* model;
    QSortFilterProxyModel* proxyModel;

public slots:
    // Open notes
    void openNotesDialog();
    void exportNotes(const QString &notes);
    // Navigation
    void goToInvPage();
    void goToMenuPage();
    void goToAdminPage();
    void goToPurchasePage();
    void goToSysInfoPage();
    // Table filter
    void filterTable();
    void filterTable2();
    // Admin Ops
    void createNewAccount();
    void changePassword();
    // DB Ops (Inventory)
    void addToDB();
    void removeFromDB();
    void writeBooks();
    // DB Ops (Customer)
    void setCustomer();
    void setPurchases();
    void makePurchases();
    // Coupon
    void checkCoupon();
};

#endif // MAINWINDOW2_H
