#include "user.h"
#include <QMessageBox>
#include <QDir>
#include <QCoreApplication>
#include <fstream>
#include <vector>

class UserOps {
public:
    // Variables
    vector<User> users;

    // Constructor
    UserOps() {
        users = readUsers();
    }

    // UserOps functions
    User parseUser(string line);
    vector<User> readUsers();
    void writeUsers();
    bool createNewAccount(string username, string password);
    bool verifyReturningUser(string username, string password);
    bool changeUserPassword(string username, string password, string newPassword);

private:
    string csvFile = "./CSVs/users.csv";
    std::ofstream file;
};
