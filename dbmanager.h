#include "book.h"
#include <fstream>
#include <vector>
#include <QtSql/QtSql>
#include <QStandardItemModel>

#ifndef DBMANAGER_H
#define DBMANAGER_H

class DbManager {
public:
    DbManager(const QString& path);

    // Inventory
    QVector<Book> getBooks() const;
    QStandardItemModel* getTableModel() const;
    bool addBook(int isbn, QString title, QString author, int year, QString publisher, QString genre, QString description, double MSRP, int quantity);
    bool removeBook(QString title);
    bool findBook(QString title);
    Book parseQueryToBook(QString title);
    bool validISBNYear(std::string userTitle, int len1, int len2);
    bool validMSRP(std::string MSRP);
    bool validQuantity(std::string quant);
    void writeBooksToCSV(QVector<Book>& books);

    // Customers
    // Setters
    void setName(QString name);
    void setEmail(QString email);
    void setTotal();
    // Getters
    QString getName();
    QString getEmail();
    double getTotal();
    // Check if customer is in DB
    bool findCustomer(QString name, QString email);
    // Add to DB
    bool addCustomerToDB();
    // Add to purchasedBooks
    bool addPurchase(Book &book, int amount);
    // Sorting
    static bool compareByValue(const Book &a, const Book &b);
    void sortPurchases();
    void writePurchasesToCSV();
    void showDialogBox();

    // Public variables
    QVector<Book> m_books;
    QVector<Book> addedBooks;
    QVector<Book> purchasedBooks;

private:
    QSqlDatabase m_database;

    // Inventory
    QString userListFile = "./CSVs/userlist.csv";
    void setupDatabase();
    void initializeBooks();

    // Customer
    QString purchaseListFile = "./CSVs/purchaselist.csv";
    QString name;
    QString email;
    double total = 0.0;
};


#endif // DBMANAGER_H
