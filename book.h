#include <string>
#include <QtSql/QtSql>

#ifndef BOOK_H
#define BOOK_H

class Book {

public:
    // Variables
    int ISBN;
    std::string title;
    std::string author;
    int year;
    std::string publisher;
    std::string genre;
    std::string description;
    double MSRP;
    int quantity;

    // Functions
    static Book fromQueryResult(const QSqlQuery& query);
};

#endif // BOOK_H
