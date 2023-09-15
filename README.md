# Qt Creator Book Inventory QUI
## Description
The Qt Book Inventory GUI is a robust application developed using Qt and C++ to streamline book inventory management. The GUI allows users to create/change/add personal accounts, add/view/edit/delete books from inventory, and pseudo-"purchase" books or create wish lists for items they're interested in. 

## Features
1. **Create Account/Login**: user can create a new account or login as an existing user
2. **Encryption**: usernames and passwords are encrypted using the md5 library and saved into a .txt file
3. **Change Users**: users may choose to update their accounts (e.g. changing username/password) or add new users
4. **Add Book Entries**: easily add new books to the inventory with detailed information such as title, author, ISBN, MSRP, genre, description, etc. 
5. **View Inventory**: browse the entire inventory with functionalities to sort and filter entries for efficient management
6. **Delete Book Entries**: remove book entries that are no longer needed from the inventory
7. **Purchasing**: pseudo-purchase a book by inputting a list of book titles and desired quantity, which, after purchasing, will generate a receipt in the form of a .txt file and adjust the book inventory accordingly
8. **Wish List**: create a wish list of books to purchase and save it as a .txt file
9. **Display Information**: includes options to display information about users' hardware to the screen
10. **Database Functionality**: uses QSql for management of book inventory
11. **Slots and Signals**: uses concept of 'slots and signals' in Qt to connect user actions (e.g. typing in the search bar) with a response (e.g. beginning to filter inventory results based on input text - to learn more, click [here](https://doc.qt.io/qt-6/signalsandslots.html/)
12. **Logging**: logs operations performed by users in a .txt file

## Skills Practiced
- Qt Design/Creator
- Database Management
- Encryption
- Read/Write Operations
- Object-Oriented Programming (OOP)
