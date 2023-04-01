#include "book.h"

Book Book::fromQueryResult(const QSqlQuery& query) {
    Book book;
    book.ISBN = query.value("ISBN").toInt();
    book.title = query.value("TITLE").toString().toStdString();
    book.author = query.value("AUTHOR").toString().toStdString();
    book.year = query.value("YEAR").toInt();
    book.publisher = query.value("PUBLISHER").toString().toStdString();
    book.genre = query.value("GENRE").toString().toStdString();
    book.description = query.value("DESCRIPTION").toString().toStdString();
    book.MSRP = query.value("MSRP").toDouble();
    book.quantity = query.value("QUANTITY").toInt();
    return book;
}
