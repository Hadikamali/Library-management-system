#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class Book {
public:
    Book(int id, const std::string& name, const std::string& author, int year) :
        bookID(id), bookName(name), author(author), yearPublished(year) {}

    int getBookID() const { return bookID; }
    std::string getBookName() const { return bookName; }
    std::string getAuthor() const { return author; }
    int getYearPublished() const { return yearPublished; }

    void setBookName(const std::string& name) { bookName = name; }
    void setAuthor(const std::string& authorName) { author = authorName; }
    void setYearPublished(int year) { yearPublished = year; }

    void displayBookInfo() const {
        std::cout << "Book ID: " << bookID << std::endl;
        std::cout << "Book Name: " << bookName << std::endl;
        std::cout << "Author: " << author << std::endl;
        std::cout << "Year Published: " << yearPublished << std::endl;
    }

private:
    int bookID;
    std::string bookName;
    std::string author;
    int yearPublished;
};

class Library {
public:
    Library(const std::string& filename) : fileName(filename) {}

    void addBook(const Book& book) {
        books.push_back(book);
    }

    int searchBook(int bookID) {
        for (size_t i = 0; i < books.size(); ++i) {
            if (books[i].getBookID() == bookID) {
                return i;
            }
        }
        return -1;
    }

    std::vector<int> searchBook(const std::string& term) {
        std::vector<int> indices;
        for (size_t i = 0; i < books.size(); ++i) {
            if (books[i].getBookName().find(term) != std::string::npos ||
                books[i].getAuthor().find(term) != std::string::npos) {
                indices.push_back(i);
            }
        }
        return indices;
    }

    void editBook(int bookID) {
        int index = searchBook(bookID);
        if (index == -1) {
            std::cout << "Book not found!" << std::endl;
            return;
        }

        std::string newName, newAuthor;
        int newYear;

        std::cout << "Enter new book name: ";
        std::getline(std::cin, newName);
        std::cout << "Enter new author: ";
        std::getline(std::cin, newAuthor);
        std::cout << "Enter new year published: ";
        std::cin >> newYear;
        std::cin.ignore(); // Clear input buffer after reading integer

        books[index].setBookName(newName);
        books[index].setAuthor(newAuthor);
        books[index].setYearPublished(newYear);

        std::cout << "Book edited successfully!" << std::endl;
    }


    void deleteBook(int bookID) {
        int index = searchBook(bookID);
        if (index == -1) {
            std::cout << "Book not found!" << std::endl;
            return;
        }

        books.erase(books.begin() + index);
        std::cout << "Book deleted successfully!" << std::endl;
    }

    void displayAllBooks() const {
        if (books.empty()) {
            std::cout << "No books in the library." << std::endl;
            return;
        }

        for (const Book& book : books) {
            book.displayBookInfo();
            std::cout << std::endl;
        }
    }

    bool saveBooksToFile() {
        if (fileName.empty()) {
            std::cout << "No filename specified for saving." << std::endl;
            return false;
        }

        std::ofstream outfile(fileName);
        if (!outfile.is_open()) {
            std::cerr << "Error opening file for writing!" << std::endl;
            return false;
        }

        for (const Book& book : books) {
            outfile << book.getBookID() << ","
                << book.getBookName() << ","
                << book.getAuthor() << ","
                << book.getYearPublished() << std::endl;
        }
        outfile.close();
        std::cout << "Books saved to file successfully." << std::endl;
        return true;
    }

    bool loadBooksFromFile() {
        if (fileName.empty()) {
            std::cout << "No filename specified for loading." << std::endl;
            return false;
        }

        std::ifstream infile(fileName);
        if (!infile.is_open()) {
            std::cerr << "Error opening file for reading!" << std::endl;
            return false;
        }

        books.clear(); // Clear existing books before loading new ones

        std::string line;
        while (std::getline(infile, line)) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.size() == 4) {
                int bookID = std::stoi(tokens[0]);
                std::string bookName = tokens[1];
                std::string author = tokens[2];
                int yearPublished = std::stoi(tokens[3]);
                books.push_back(Book(bookID, bookName, author, yearPublished));
            }
        }
        infile.close();
        std::cout << "Books loaded from file successfully." << std::endl;
        return true;
    }

private:
    std::vector<Book> books;
    std::string fileName;
};

int main() {
    Library library("library.txt"); // Create Library object with filename

    // Example usage:
    library.loadBooksFromFile(); // Load books from file
    library.displayAllBooks(); // Display all books in the library
    // Add, edit, delete, search books...
    library.saveBooksToFile(); // Save books to file
    return 0;
}