#include "dbmanager.h"
#include <QFile>
#include <QTextStream>
#include "log.h"
#include <regex>
#include <cctype>
#include <cmath>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

DbManager::DbManager(const QString& path)
    : m_database(QSqlDatabase::addDatabase("QSQLITE"))
{
    m_database.setDatabaseName(path);

    if (!m_database.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        qDebug() << "Database: connection ok";
    }

    initializeBooks();
}

/* **** INVENTORY **** */

void DbManager::initializeBooks()
{
    QSqlQuery query("SELECT * FROM BOOKS");

    while (query.next()) {
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
        m_books.push_back(book);
    }
}

QVector<Book> DbManager::getBooks() const
{
    return m_books;
}

QStandardItemModel* DbManager::getTableModel() const
{
    QStandardItemModel* model = new QStandardItemModel(m_books.size(), 9, nullptr);

    model->setHeaderData(0, Qt::Horizontal, "ISBN");
    model->setHeaderData(1, Qt::Horizontal, "Title");
    model->setHeaderData(2, Qt::Horizontal, "Author");
    model->setHeaderData(3, Qt::Horizontal, "Year");
    model->setHeaderData(4, Qt::Horizontal, "Publisher");
    model->setHeaderData(5, Qt::Horizontal, "Genre");
    model->setHeaderData(6, Qt::Horizontal, "Description");
    model->setHeaderData(7, Qt::Horizontal, "MSRP");
    model->setHeaderData(8, Qt::Horizontal, "Quantity");

    for (int i = 0; i < m_books.size(); i++) {
        Book book = m_books.at(i);

        model->setData(model->index(i, 0), book.ISBN);
        model->setData(model->index(i, 1), QString::fromStdString(book.title));
        model->setData(model->index(i, 2), QString::fromStdString(book.author));
        model->setData(model->index(i, 3), book.year);
        model->setData(model->index(i, 4), QString::fromStdString(book.publisher));
        model->setData(model->index(i, 5), QString::fromStdString(book.genre));
        model->setData(model->index(i, 6), QString::fromStdString(book.description));
        model->setData(model->index(i, 7), book.MSRP);
        model->setData(model->index(i, 8), book.quantity);
    }

    return model;
}

bool DbManager::addBook(int isbn, QString title, QString author, int year, QString publisher, QString genre, QString description, double MSRP, int quantity)
{
    QSqlQuery query;
    query.prepare("INSERT INTO BOOKS (ISBN, TITLE, AUTHOR, YEAR, PUBLISHER, GENRE, DESCRIPTION, MSRP, QUANTITY) "
    "VALUES (:isbn, :title, :author, :year, :publisher, :genre, :description, :msrp, :quantity)");
    query.bindValue(":isbn", isbn);
    query.bindValue(":title", title);
    query.bindValue(":author", author);
    query.bindValue(":year", year);
    query.bindValue(":publisher", publisher);
    query.bindValue(":genre", genre);
    query.bindValue(":description", description);
    query.bindValue(":msrp", MSRP);
    query.bindValue(":quantity", quantity);

    bool success = query.exec();
    return success;
}

bool DbManager::removeBook(QString title)
{
    QSqlQuery query;
    query.prepare("DELETE FROM BOOKS WHERE TITLE = :title");
    query.bindValue(":title", title);

    bool success = query.exec();
        if (success && query.numRowsAffected() > 0) {
            return true;
        }
        else {
            return false;
        }
    return success;
}

bool DbManager::findBook(QString title)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM BOOKS WHERE TITLE = :title");
    query.bindValue(":title", title);
    query.exec();

    if (query.next()) {
        // Title exists
        return true;
    } else {
        // Title does not exust
        return false;
    }
    return false;
}

Book DbManager::parseQueryToBook(QString title)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM BOOKS WHERE TITLE = :title");
    query.bindValue(":title", title);
    query.exec();

    Book book;
    query.next();
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

/* **** VALIDATION FOR ADDING BOOKS **** */

bool DbManager::validISBNYear(std::string userTitle, int len1, int len2) {
    if ((userTitle.length() != len1) && (userTitle.length() != len2)) {
        return false;
    }
    else {
        for (int i = 0; i < userTitle.length(); i++) {
            if (isdigit(userTitle[i]) == false) {
                return false;
            }
        }
        return true;
    }
    return false;
}

// Validate MSRP
bool DbManager::validMSRP(std::string MSRP) {
    // Formats
    std::regex integer_expr1("[0-9][0-9]\\.[0-9][0-9]"); // 10.00
    std::regex integer_expr2("[0-9][0-9][0-9]\\.[0-9][0-9]"); // 100.00
    std::regex integer_expr3("[0-9]\\.[0-9][0-9]"); // 1.00
    // Check if valid using regex_match()
    if (regex_match(MSRP, integer_expr1) || regex_match(MSRP, integer_expr2) || regex_match(MSRP, integer_expr3)) {
        return true;
    }
    else {
        return false;
    }
}

// Validate quantity
bool DbManager::validQuantity(std::string quant) {
    for (int i = 0; i < quant.length(); i++) {
        if (isdigit(quant[i]) == false) {
            return false;
        }
    }
    return true;
}

// Write user list
void DbManager::writeBooksToCSV(QVector<Book>& books)
{
    QFile file(userListFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Couldn't open file.");
        return;
    }

    QTextStream out(&file);

    // Write header row
    out << "ISBN,Title,Author,Year\n";

    // Write book data
    for (const auto& book : books) {
        out << QString::number(book.ISBN) << "," << QString::fromStdString(book.title) << ","
            << QString::fromStdString(book.author) << "," << QString::number(book.year) << "\n";
    }
    file.close();
}

/* **** CUSTOMER **** */

// Setters
void DbManager::setName(QString name)
{
    this->name = name;
}

void DbManager::setEmail(QString email)
{
    this->email = email;
}

void DbManager::setTotal()
{
    double tax = 0.00;
    for (int i = 0; i < purchasedBooks.size(); i++) {
        tax = purchasedBooks[i].MSRP * purchasedBooks[i].quantity * 0.06;
        total += (purchasedBooks[i].MSRP * purchasedBooks[i].quantity) + tax;
    }

    // format total to two decimal places
    total = std::round(total * 100) / 100.0;
}

// Getters
QString DbManager::getName()
{
    return name;
}

QString DbManager::getEmail()
{
    return email;
}

double DbManager::getTotal()
{
    return total;
}

bool DbManager::findCustomer(QString name, QString email)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM SHOPPERS WHERE NAME = :name AND EMAIL = :email");
    query.bindValue(":name", name);
    query.bindValue(":email", email);
    query.exec();

    if (query.next()) {
        // Customer exists
        return true;
    } else {
        // Customer does not exist
        return false;
    }
}

// Sorting
bool DbManager::compareByValue(const Book &a, const Book &b) {
    return a.MSRP < b.MSRP;
}

void DbManager::sortPurchases() {
    std::sort(purchasedBooks.begin(), purchasedBooks.end(), compareByValue);
}

// Adding to DB
bool DbManager::addCustomerToDB()
{
    // Check if customer already exists in db
    if (findCustomer(name, email)) {
        // Modify customer's total only
        QSqlQuery query;
        query.prepare("UPDATE SHOPPERS SET TOTAL = :total WHERE NAME = :name AND EMAIL = :email");
        query.bindValue(":total", total);
        query.bindValue(":name", name);
        query.bindValue(":email", email);
        bool success = query.exec();
        return success;
    }
    else {
        // Add customer to DB
        QSqlQuery query;
        query.prepare("INSERT INTO SHOPPERS (NAME, EMAIL, TOTAL) VALUES (:name, :email, :total)");
        query.bindValue(":name", name);
        query.bindValue(":email", email);
        query.bindValue(":total", total);
        bool success = query.exec();
        return success;
    }
}

// Writing file to DB
void DbManager::writePurchasesToCSV()
{
    QFile file(purchaseListFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Couldn't open file.");
        return;
    }

    QTextStream out(&file);

    // Sort books
    sortPurchases();

    // Write header row
    out << "ISBN,Title,Author,MSRP,Quantity\n";

    // Write book data
    for (const auto& book : purchasedBooks) {
        out << QString::number(book.ISBN) << "," << QString::fromStdString(book.title) << ","
            << QString::fromStdString(book.author) << "," << QString::number(book.MSRP) << ","
            << QString::number(book.quantity) << "\n";
    }
    file.close();
}

// Show purchases
void DbManager::showDialogBox()
{
    // Create dialog box
    QDialog *dialog = new QDialog();

    // Create a QVBoxLayout to hold the widgets
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Add a label for the customer's name and email
    QLabel* purchaseMsg = new QLabel(QString("Thank your for your purchase."));
    QLabel* nameLabel = new QLabel(QString("Customer: %1").arg(name));
    QLabel* emailLabel = new QLabel(QString("Email: %1").arg(email));
    layout->addWidget(purchaseMsg);
    layout->addWidget(nameLabel);
    layout->addWidget(emailLabel);

    // Create a table view to display the purchased books
    QTableView* tableView = new QTableView(dialog);
    tableView->setStyleSheet("QTableView { background-color: #444444; color: white; }");

    // Create a table model to store the data
    QStandardItemModel* model = new QStandardItemModel(purchasedBooks.size(), 5, dialog);
    model->setHeaderData(0, Qt::Horizontal, "ISBN");
    model->setHeaderData(1, Qt::Horizontal, "Title");
    model->setHeaderData(2, Qt::Horizontal, "Author");
    model->setHeaderData(3, Qt::Horizontal, "MSRP");
    model->setHeaderData(4, Qt::Horizontal, "Quantity");

    // Populate the model with the purchased books
    for (int i = 0; i < purchasedBooks.size(); i++) {
        QStandardItem* isbnItem = new QStandardItem(QString::number(purchasedBooks[i].ISBN));
        QStandardItem* titleItem = new QStandardItem(QString::fromStdString(purchasedBooks[i].title));
        QStandardItem* authorItem = new QStandardItem(QString::fromStdString(purchasedBooks[i].author));
        QStandardItem* msrpItem = new QStandardItem(QString::number(purchasedBooks[i].MSRP));
        QStandardItem* quantityItem = new QStandardItem(QString::number(purchasedBooks[i].quantity));
        model->setItem(i, 0, isbnItem);
        model->setItem(i, 1, titleItem);
        model->setItem(i, 2, authorItem);
        model->setItem(i, 3, msrpItem);
        model->setItem(i, 4, quantityItem);
    }

    // Set the table view's model and resize the columns
    tableView->setModel(model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add the table view to the layout
    layout->addWidget(tableView);

    // Add a label for the total
    QLabel* totalLabel = new QLabel(QString("Total: $%1").arg(QString::number(total, 'f', 2)));
    layout->addWidget(totalLabel);

    // Set the layout for the dialog and show it
    dialog->setLayout(layout);
    dialog->exec();
}

