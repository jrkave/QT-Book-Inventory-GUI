#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include "dbmanager.h"
#include <vector>
#include "userops.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSortFilterProxyModel>
#include <QSysInfo>
#include <QStringList>

MainWindow2::MainWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2),
    db("./Database/qtbooks.db") // Db for books and shoppers
{
    QSysInfo *sysInfo = new QSysInfo();
    ui->setupUi(this);

    /* ***** Table View ***** */
    model = db.getTableModel(); // Set model to data in db
    proxyModel = new QSortFilterProxyModel(); // proxyModel for filtering results
    ui->tableView->setModel(proxyModel);
    ui->tableView2->setModel(proxyModel);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(1);
    ui->tableView->setModel(proxyModel); // set tableView model to proxyModel
    ui->tableView2->setModel(proxyModel);
    ui->tableView->show();
    ui->tableView2->show();
    // Set some properties of the table views
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* ***** Status Bar ***** */
    int numBooks = model->rowCount();
    ui->statusbar->showMessage(QString("Number of books: %1").arg(numBooks));

    /* **** Table Widget (System Info) **** */
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setRowCount(2);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Property"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("Value"));
    QString buildApi = sysInfo->buildAbi();
    QRegularExpression rx("[-]");
    QStringList list = buildApi.split(rx);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Architecture"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem(list[0]));

    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Endianness"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem(list[1]));

    ui->tableWidget->setRowCount(4);
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("Product Version"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem(sysInfo->productVersion()));
    ui->tableWidget->show();
}

MainWindow2::~MainWindow2()
{
    delete ui;
}

/* ***** NAVIGATING BETWEEN PAGES ***** */

// Show menu page
void MainWindow2::showMenuPage() {
    int menuPageIndex = ui->stackedWidget->indexOf(ui->menuPage);
    ui->stackedWidget->setCurrentIndex(menuPageIndex);
}

// Slots for changing pages of stacked widget
void MainWindow2::goToInvPage() {
    int invPageIndex = ui->stackedWidget->indexOf(ui->invPage);
    ui->stackedWidget->setCurrentIndex(invPageIndex);
}
void MainWindow2::goToMenuPage() {
    int menuPageIndex = ui->stackedWidget->indexOf(ui->menuPage);
    ui->stackedWidget->setCurrentIndex(menuPageIndex);
}
void MainWindow2::goToAdminPage() {
    int adminPageIndex = ui->stackedWidget->indexOf(ui->adminPage);
    ui->stackedWidget->setCurrentIndex(adminPageIndex);
}
void MainWindow2::goToPurchasePage() {
    int purchasePageIndex = ui->stackedWidget->indexOf(ui->purchasePage);
    ui->stackedWidget->setCurrentIndex(purchasePageIndex);
}
void MainWindow2::goToSysInfoPage() {
    int sysInfoPageIndex = ui->stackedWidget->indexOf(ui->sysInfoPage);
    ui->stackedWidget->setCurrentIndex(sysInfoPageIndex);
}

/* ***** INVENTORY OPERATIONS ***** */

// Filter functionality for tableView
void MainWindow2::filterTable()
{
    const QString &searchText = ui->searchInvLineEdit->text();
    QString pattern = QRegularExpression::escape(searchText);
    QRegularExpression filterRegExp(pattern, QRegularExpression::CaseInsensitiveOption);
    QSortFilterProxyModel* proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView->model());
    if (proxyModel) {
        proxyModel->setFilterRegularExpression(filterRegExp);
    }
}

// Filter functionality for tableView2
void MainWindow2::filterTable2()
{
    const QString &searchText = ui->searchInvLineEdit_2->text();
    QString pattern = QRegularExpression::escape(searchText);
    QRegularExpression filterRegExp(pattern, QRegularExpression::CaseInsensitiveOption);
    QSortFilterProxyModel* proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->tableView2->model());
    if (proxyModel) {
        proxyModel->setFilterRegularExpression(filterRegExp);
    }
}

// Add Book to DB
void MainWindow2::addToDB() {
    // Get inputs
    QString ISBN = ui->addISBNLineEdit->text();
    QString title = ui->addTitleLineEdit->text();
    QString author = ui->addAuthorLineEdit->text();
    QString year = ui->addYearLineEdit->text();
    QString publisher = ui->addPublisherLineEdit->text();
    QString genre = ui->addGenreLineEdit->text();
    QString description = ui->addDescLineEdit->text();
    QString MSRP = ui->addMSRPLineEdit->text();
    QString quantity = ui->addQuantityLineEdit->text();

    // Input validation
    if (validInput(ISBN, year, MSRP, quantity) == false) {
        QMessageBox::warning(this, "Invalid Input", "One or more entries contained invalid input. Please try again.");
        return;
    }

    // Add book to DB
    if (db.addBook(ISBN.toInt(), title, author, year.toInt(), publisher, genre, description, MSRP.toDouble(), quantity.toInt()) == false) {
        QMessageBox::warning(this, "Database Insertion", "Insertion to database failed.");
        return;
    }
    else {
        QMessageBox::information(this, "Database Insertion", "Data successfully inserted.");

        /* Resetting tableView */
        // Update m_books
        Book book;
        book.ISBN = ISBN.toInt();
        book.title = title.toStdString();
        book.author = author.toStdString();
        book.year = year.toInt();
        book.publisher = publisher.toStdString();
        book.genre = genre.toStdString();
        book.description = description.toStdString();
        book.MSRP = MSRP.toDouble();
        book.quantity = quantity.toInt();
        db.m_books.push_back(book); // add to initial vector of all books
        db.addedBooks.push_back(book); // add to user list

        // Update the model and reset the proxy model
        model = db.getTableModel();
        proxyModel->setSourceModel(model);
        proxyModel->setFilterKeyColumn(1);
        ui->tableView->setModel(proxyModel);
        ui->tableView2->setModel(proxyModel);
        int numBooks = model->rowCount();
        ui->statusbar->showMessage(QString("Number of books: %1").arg(numBooks));
    }
}

// Remove Book from DB
void MainWindow2::removeFromDB() {
    // Get input
    QString deleteTitle = ui->delTitleLineEdit->text();

    if (db.removeBook(deleteTitle) == false) {
        QMessageBox::warning(this, "Database Removal", "Removal from database failed. Please enter a valid title.");
        return;
    }
    else {
        QMessageBox::information(this, "Database Removal", "Data successfully removed.");

        /* Resetting tableView */
        // Update m_books
        for (auto it = db.m_books.begin(); it != db.m_books.end(); ++it) {
            if (it->title == deleteTitle.toStdString()) {
                // Found the book with the target title, erase it
                db.m_books.erase(it);
                break;
            }
        }
        // Update the model and reset the proxy model
        model = db.getTableModel();
        proxyModel->setSourceModel(model);
        proxyModel->setFilterKeyColumn(1);
        ui->tableView->setModel(proxyModel);
        ui->tableView2->setModel(proxyModel);
        int numBooks = model->rowCount();
        ui->statusbar->showMessage(QString("Number of books: %1").arg(numBooks));
    }
}

bool MainWindow2::validInput(QString ISBN, QString year, QString MSRP, QString quantity)
{
    // ISBN
    if (db.validISBNYear(ISBN.toStdString(), 10, 13) == false) {
        return false;
    }
    // Year
    if (db.validISBNYear(year.toStdString(), 4, 4) == false) {
        return false;
    }
    // MSRP
    if (db.validMSRP(MSRP.toStdString()) == false) {
        return false;
    }
    // Quantity
    if (db.validQuantity(quantity.toStdString()) == false) {
        return false;
    }
    return true;
}

void MainWindow2::writeBooks() {
    db.writeBooksToCSV(db.addedBooks);
}

/* ***** ADMIN OPERATIONS ***** */

void MainWindow2::createNewAccount()
{
    UserOps userOps;
    QString qnewUsername = ui->newUsrLineEdit->text();
    QString qnewPassword = ui->newUsrPwdLineEdit->text();
    bool addedUser = false;
    addedUser = userOps.createNewAccount(qnewUsername.toStdString(), qnewPassword.toStdString());

    if (addedUser) {
        QMessageBox::information(this, "Create Account", ("Thanks for creating an account with us, " + qnewUsername + "."));
    }
    else {
        QMessageBox::warning(this, "Create Account", "User already exists.");
    }
}

void MainWindow2::changePassword()
{
    UserOps userOps;
    QString qcurrUsername = ui->currUsernameLineEdit->text();
    QString qcurrPassword = ui->currPasswordLineEdit->text();
    QString qnewPassword = ui->changePwdLineEdit->text();
    bool changedPwd = false;
    changedPwd = userOps.changeUserPassword(qcurrUsername.toStdString(), qcurrPassword.toStdString(), qnewPassword.toStdString());

    if (changedPwd) {
        QMessageBox::information(this, "Password Changed", "Your password has been changed.");
    }
    else {
        QMessageBox::warning(this, "Password Changed", "Unable to change password. Please double-check username/password.");
    }
}

/* **** PURCHASING OPERATIONS **** */

void MainWindow2::setCustomer()
{
    QString customerName = ui->custNameLineEdit->text();
    QString customerEmail = ui->custEmailLineEdit->text();

    db.setName(customerName);
    db.setEmail(customerEmail);
}

void MainWindow2::setPurchases()
{
    QString qTitle = ui->purchaseTitleLineEdit->text();
    int quantity = ui->purchaseQuantityLineEdit->text().toInt();

    if (db.findBook(qTitle) == false) {
        QMessageBox::warning(this, "Book Not Found", "The book was not found.");
    }
    else {
        Book toPurchase = db.parseQueryToBook(qTitle);
        // Check if enough books are available for purchase
        if (toPurchase.quantity < quantity) {
            QMessageBox::warning(this, "Quantity Limit", "There are not enough books to purchase that amount.");
        }
        else {
            toPurchase.quantity = quantity;
            // Update purchasedBooks vector
            db.purchasedBooks.push_back(toPurchase);
            // Set class member variable "total" to updated total
            db.setTotal();
            QMessageBox::information(this, "Book Added", "Book was added to purchases.");
        }
    }
}

void MainWindow2::makePurchases()
{
    db.addCustomerToDB();
    db.writePurchasesToCSV();
    db.showDialogBox();
}
