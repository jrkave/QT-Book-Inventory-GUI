#include "md5.h"
#include "userops.h"

    // Parse lines of users into User struct
    User UserOps::parseUser(string line) {
        User user;
        size_t pos = 0;
        string token;
        int i = 0;
        while ((pos = line.find(",")) != string::npos) {
            token = line.substr(0, pos);
            if (i == 0) {
                user.username = token;
            }
            else {
                user.password = token;
            }
            line.erase(0, pos + 1);
            i++;
        }
        if (i == 1) {
            user.password = line;
        }
        return user;
    }

    // Read all users in csvlogin.csv and add them to vector<User> users
    vector<User> UserOps::readUsers() {
        vector<User> users;
        ifstream file(csvFile);
        if (file) {
            string line;
            while (getline(file, line)) {
                User user = parseUser(line);
                users.push_back(user);
            }
            file.close();
        }
        return users;
    }

    // Write all users to file
    void UserOps::writeUsers() {
        ofstream file(csvFile);
        if (file) {
            for (int i = 0; i < users.size(); i++) {
                file << users[i].username << "," << users[i].password << endl;
            }
            file.close();
        }
    }

    // Write new user to file
    bool UserOps::createNewAccount(string username, string password) {
        string encryptedPassword;
        // Open csv file
        file.open(csvFile, ios_base::app);
        // Exception handling
        if(!file.is_open()){
            return false;
        }
        // Checks if account already exists
        if (verifyReturningUser(username, password)) {
            return false;
        }
        encryptedPassword = md5(password);
        // Write username and password delimited by comma
        file << username << "," << encryptedPassword << endl;
        file.close();
        return true;
    }

    // Verify returning user
    bool UserOps::verifyReturningUser(string username, string password) {
        string encryptedPassword;
        string line;
        ifstream file(csvFile);
        // Exception handling
        if (!file.is_open()) {
            return false;
        }
        // Search csv file for username and password
        while (getline(file, line)) {
            int delimiterPos = line.find(',');
            string storedUsername = line.substr(0, delimiterPos);
            string storedPassword = line.substr(delimiterPos + 1);
            // Run input password through encryption
            encryptedPassword = md5(password);
        // Compare username and encrypted password to password in csvlogin.csv
            if (username == storedUsername && encryptedPassword == storedPassword) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    // Change password and update CSV file
    bool UserOps::changeUserPassword(string username, string password, string newPassword) {
        bool found = false;
        for (int i = 0; i < users.size(); i++) {
            if (users[i].username == username) {
                users[i].password = md5(newPassword);
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
        else {
            writeUsers();
            return true;
        }
    }
