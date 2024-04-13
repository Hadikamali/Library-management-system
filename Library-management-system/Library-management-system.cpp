#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


class Book {
public:
    Book(int id, const string& name, const ::string& author, int year) :
        bookID(id), bookName(name), author(author), yearPublished(year) {}

    int getBookID() const { return bookID; }
    string getBookName() const { return bookName; }
    string getAuthor() const { return author; }
    int getYearPublished() const { return yearPublished; }

    void setBookName(const string& name) { bookName = name; }
    void setAuthor(const string& authorName) { author = authorName; }
    void setYearPublished(int year) { yearPublished = year; }

    void displayBookInfo() const {
        cout << "Book ID: " << bookID << endl;
        cout << "Book Name: " << bookName << endl;
        cout << "Author: " << author << endl;
        cout << "Year Published: " << yearPublished << endl;
    }

private:
    int bookID;
    string bookName;
    string author;
    int yearPublished;
};

class Library {
public:
    Library(const string& filename) : fileName(filename) {
        loadBooksFromFile();
    }
    ~Library() {
        saveBooksToFile();
    }

    void addBook() {
        int id, year;
        string name, author;

        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Book Name: ";
        getline(cin, name);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Year Published: ";
        cin >> year;

        books.push_back(Book(id, name, author, year));
        cout << "Book added successfully!\n";
    }

    void displayAllBooks() {
        if (books.empty()) {
            cout << "No books in the library.\n";
            return;
        }
        for (const auto& book : books) {
            book.displayBookInfo();
            cout << endl;
        }
    }

    void searchBook() {
        cout << "Enter book name or author to search: ";
        string term;
        cin.ignore();
        getline(cin, term);

        auto indices = searchBooks(term);
        if (indices.empty()) {
            cout << "No books found.\n";
            return;
        }
        for (int index : indices) {
            books[index].displayBookInfo();
            cout << endl;
        }
    }

    void deleteBook() {
        cout << "Enter book ID to delete: ";
        int id;
        cin >> id;
        int index = searchBook(id);
        if (index == -1) {
            cout << "Book not found!\n";
            return;
        }
        books.erase(books.begin() + index);
        cout << "Book deleted successfully!\n";
    }

private:
    vector<Book> books;
    string fileName;

    void saveBooksToFile() {
        ofstream file(fileName);
        if (!file) {
            cerr << "Failed to open file for writing.\n";
            return;
        }
        for (const auto& book : books) {
            file << book.getBookID() << ',' << book.getBookName() << ','
                 << book.getAuthor() << ',' << book.getYearPublished() << '\n';
        }
        file.close();
    }

    void loadBooksFromFile() {
        ifstream file(fileName);
        if (!file) {
            cerr << "Failed to open file for reading.\n";
            return;
        }
        string line, name, author;
        int id, year;
        while (getline(file, line)) {
            istringstream iss(line);
            getline(iss, name, ',');
            getline(iss, author, ',');
            iss >> id >> year;
            books.emplace_back(id, name, author, year);
        }
        file.close();
    }

    int searchBook(int bookID) {
        for (size_t i = 0; i < books.size(); ++i) {
            if (books[i].getBookID() == bookID) {
                return i;
            }
        }
        return -1;
    }

    vector<int> searchBooks(const string& term) {
        vector<int> indices;
        for (size_t i = 0; i < books.size(); ++i) {
            if (books[i].getBookName().find(term) != string::npos ||
                books[i].getAuthor().find(term) != string::npos) {
                indices.push_back(i);
            }
        }
        return indices;
    }
};

int main() {
    Library myLibrary("library.txt");
    int choice;

    do {
        cout << "\n1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search for a Book\n";
        cout << "4. Delete a Book\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                myLibrary.addBook();
                break;
            case 2:
                myLibrary.displayAllBooks();
                break;
            case 3:
                myLibrary.searchBook();
                break;
            case 4:
                myLibrary.deleteBook();
                break;
            case 5:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid option, please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
