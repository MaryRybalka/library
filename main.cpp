#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <fstream>

using namespace std;
class Autor;

class Book {
    string name;
    string author;
public:
    Book(const string name, const string author) {
        this->name = name;
        this->author = author;
    }

    const string getName() const {
        return name;
    }

    const string getAuthor() const {
        return author;
    }
};

class Library {
private:
    vector<const Book *> books;
    void reloadBooks(){
        books.clear();
        ifstream f;
        f.open("../Library");
        string newLine;
        string newName;
        string newAut;

        while (!f.eof()){
            newName = "";
            newAut = "";
            getline(f, newLine);
            int i = 0;
            while (newLine[i]!='-'){
                newName +=newLine[i++];
            }
            i++;
            while (newLine[i]!='\0'){
                newAut +=newLine[i++];
            }
            books.push_back(new Book(newName, newAut));
        }
        f.close();
    }
public:
    const Book* findBook(string name, string autor) {
        reloadBooks();
        for(auto & book : books){
            if ((book->getName() == name) &&(book->getAuthor() == autor)) return book;
        }
        return new Book("empty", "empty");
    }

    vector<const Book*> findBookByAutor(string autor) {
        reloadBooks();
        vector<const Book*> autorBook;
        for(auto & book : books){
            if (book->getAuthor() == autor)
                autorBook.push_back(book);
        }
        return autorBook;
    }

    vector<const Book*> findBookByName(string name) {
        reloadBooks();
        vector<const Book*> thisBook;
        for(auto & book : books){
            if (book->getName() == name)
                thisBook.push_back(book);
        }
        return thisBook;
    }
};

class Worker: public Library{
    friend class Customer;

    int askAboutChoise(){
        cout << "Wanna choose one of this books?   1 - Yes/  -1 - No" << endl;
        int choise = 0;
        cin >> choise;
        if (choise == 1) {
            cout << "Write number of this book." << endl;
            cin >> choise;
        }
        return choise;
    }
public:
    const Book* giveBook(string name, string author) {
        const Book* fBook = findBook(name, author);
        if (fBook->getAuthor() == "empty") throw logic_error("There are no needs book in the library");
        return fBook;
    }
    const Book* giveBookByAutor(string author) {
        vector <const Book*> fBook = findBookByAutor(author);

        if (fBook.empty()) throw logic_error("There are no needs book of this author in the library");
        else {
            cout << "There are all books of "<< author <<" in this list below: " << endl;

            for (int i = 0, len = fBook.size(); i < len; i++){
                cout << i << ") "<< fBook[i]->getName()  << endl;
            }
            cout << endl;

            int choise = askAboutChoise();
            if (choise == -1 ) throw runtime_error("Goodbye");
            return fBook[choise%fBook.size()];
        }
    }
    const Book* giveBookByName(string name) {
        vector<const Book *> fBook = findBookByName(name);
        if (fBook.empty()) throw logic_error("There are no needs book with this name in the library");
        else {
        cout << "There are all authors, who has book with this name '"<< name << "' in this list below: " << endl;

            for (int i = 0, len = fBook.size(); i < len; i++){
                cout << i << ") "<< fBook[i]->getAuthor()  << endl;
            }
            cout << endl;

            int choise = askAboutChoise();
            if (choise == -1 ) throw logic_error("");
            return fBook[choise%fBook.size()];
        }
    }
};

class Customer : protected Worker{
public:
    bool askAboutBook(string _name, string _author){
        try{
            const Book *needsBook;
            if (_name == "") needsBook = giveBookByAutor(_author);
            else{
                if (_author == "") needsBook = giveBookByName(_name);
                else needsBook = giveBook(_name, _author);
            }

            cout << "i've got '"<< needsBook->getName() << " " << needsBook->getAuthor() << "' book." << endl;
            return true;
        }
        catch (exception &e) {
            cout << e.what() << endl;
            return false;
        }
    }
};

int main() {
    Customer Bob;
    Bob.askAboutBook("white whale", "");
    Bob.askAboutBook("", "fedor dostoyevsky");
    Bob.askAboutBook("weak heart", "fedor dostoyevsky");
    Bob.askAboutBook("bobo dead", "joseph brodsky");
    Bob.askAboutBook("bobo is dead", "joseph brodsky");
    return 0;
}