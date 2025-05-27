#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace std;


// Book class
class Book {
    private:

        // private variables
        string isbn;
        int copiesAvailable;
        int totalCopies;

    public:

        // public variables
        string title;
        string author;

        // parameterized constructor
        Book (string passesdTitle, string passedAuthor, string passedIsbn, int passedCopiesAvailable, int passedTotalCopies) {
            title = passesdTitle;
            author = passedAuthor;
            isbn = passedIsbn;
            copiesAvailable = passedCopiesAvailable;
            totalCopies = passedTotalCopies;
        }

        // default constructor
        Book () {
            title = "UnknownTitle";
            author = "UnknownAuthor";
            isbn = "ISBN";
            copiesAvailable = 0;
            totalCopies = 5;
        }

        // copy constructor
        Book (const Book& book, string newIsbn) {
            title = book.title;
            author = book.author;
            isbn = newIsbn;
            copiesAvailable = book.copiesAvailable;
            totalCopies = book.totalCopies;
        }

        // getter methods
        string getIsbn () {
            return isbn;
        }

        int getCopiesAvailable () {
            return copiesAvailable;
        }

        int getTotalCopies () {
            return totalCopies;
        }

        // setter method
        void updateCopies (int count) {
            if ((totalCopies + count < 0) || (copiesAvailable + count < 0)) {
                cout << "Invalid request! Count becomes negative" << endl;
                return;
            }
            totalCopies += count;
            copiesAvailable += count;
        }

        // utility methods
        void increaseAvailableCount(int count) {
            copiesAvailable += count;
        }

        bool borrowBook () {
            // check if the number of copies of the book available is positive
            if (copiesAvailable > 0) {
                copiesAvailable--;
                return true;
            }

            cout << "Invalid request! Copy of book not available" << endl;
            return false;
        }

        bool returnBook () {
            // check that the number of copies available is equal to the total number of copies allowed (in that case the return of the book is not allowed)
            if (copiesAvailable == totalCopies) {
                cout << "Invalid request! Copy of book exceeds total copies" << endl;
                return false;
            }

            copiesAvailable++;
            return true;
        }

        void printDetails () {
            cout << title << " " << author << endl;
        }
};


// Member class
class Member {

    // private variables
    private:
        string memberId;
        map <string, int> borrowedBooks;
        int borrowLimit;
    
    public:
        // public variables
        string name;

        // parameterized constructor
        Member (string passedMemberId, string passedName, int passedBorrowLimit) {
            memberId = passedMemberId;
            name = passedName;
            borrowLimit = passedBorrowLimit;
        }

        // default constructor
        Member (string passedMemberId, string passedName) {
            memberId = passedMemberId;
            name = passedName;
            borrowLimit = 3;
        }

        // getter methods
        string getMemberId () {
            return memberId;
        }

        // utility methods
        bool borrowBook (string isbn) {
            // check if the borrow limit is still there or not
            int totalBorrowedBooks = 0;
            for (auto book : borrowedBooks) {
                totalBorrowedBooks += book.second;
                if (totalBorrowedBooks >= borrowLimit) {
                    cout << "Invalid request! Borrow limit exceeded" << endl;
                    return false;
                }
            }

            // allocate the book to the member
            borrowedBooks[isbn]++;
            return true;
        }

        bool returnBook (string isbn) {
            // check if the member has borrowed the book or not
            if (borrowedBooks.find(isbn) == borrowedBooks.end()) {
                cout << "Invalid request! Book not borrowed" << endl;
                return false;
            }
            
            // deallocate the book from the member
            borrowedBooks[isbn]--;
            if (borrowedBooks[isbn] == 0)
                borrowedBooks.erase(isbn);
            return true;
        }

        void printDetails () {
            for (auto book : borrowedBooks) {
                cout << memberId << " " << name << " " << book.first << " " << book.second << endl;
            }
        }
};


// Library class
class Library {
    private:
        vector<Book> books;
        vector<Member> members;
    
    public:
        
        // constructor
        Library () {}

        // getter methods
        vector<Book> getBooks () {
            return books;
        }

        vector<Member> getMembers () {
            return members;
        }

        // setter method
        void setBooksOnCountUpdate(vector<Book> updatedBooks) {
            books = updatedBooks;
        }

        // utility methods
        bool addBook (Book book) {
            // check if a book with the same isbn already exists of not
            for (int index = 0; index < books.size(); index++) {
                if (books[index].getIsbn() == book.getIsbn()) {
                    cout << "Invalid request! Book with same isbn already exists" << endl;
                    return false;
                }
            }

            // add the book if it is already not there
            books.push_back(book);
            return true;
        }

        bool registerMember (Member member) {
            // check if a member with the same member id already exists of not
            for (int index = 0; index < members.size(); index++) {
                if (members[index].getMemberId() == member.getMemberId()) {
                    cout << "Invalid request! Member with same id already exists" << endl;
                    return false;
                }
            }

            // register the member if it is already not there
            members.push_back(member);
            return true;
        }

        bool borrowBook (string memberId, string isbn) {
            // borrow the book through the utility method of the Book class
            bool bookBorrowPossible = false;
            int bookIndex;
            for (int index = 0; index < books.size(); index++) {
                if (books[index].getIsbn() == isbn) {
                    bookBorrowPossible = books[index].borrowBook();
                    if (bookBorrowPossible)
                        bookIndex = index;
                    break;
                }
            }

            // if the book is available then allocate it to the member through the utility method of the Member class
            if (bookBorrowPossible) {
                for (int index = 0; index < members.size(); index++) {
                    if (members[index].getMemberId() == memberId) {
                        bookBorrowPossible = members[index].borrowBook(isbn);
                        if (!bookBorrowPossible)
                            books[bookIndex].increaseAvailableCount(1);
                        break;
                    }
                }
            }

            return bookBorrowPossible;
        }

        bool returnBook (string memberId, string isbn) {
            // return the book through the utility method of the Book class
            bool bookReturnPossible = false;
            int bookIndex;
            for (int index = 0; index < books.size(); index++) {
                if (books[index].getIsbn() == isbn) {
                    bookReturnPossible = books[index].returnBook();
                    if (bookReturnPossible)
                        bookIndex = index;
                    break;
                }
            }

            // return the book through the utility method of the Member class
            if (bookReturnPossible) {
                for (int index = 0; index < members.size(); index++) {
                    if (members[index].getMemberId() == memberId) {
                        bookReturnPossible = members[index].returnBook(isbn);
                        if (!bookReturnPossible)
                            books[bookIndex].increaseAvailableCount(-1);
                        break;
                    }
                }
            }

            return bookReturnPossible;
        }

        void printLibraryDetails () {
            // print the details of the books
            for (int index = 0; index < books.size(); index++)
                cout << books[index].title << " " << books[index].author << " " << books[index].getCopiesAvailable() << endl;
            
            // print the details of the members
            for (int index = 0; index < members.size(); index++)
                cout << members[index].getMemberId() << " " << members[index].name << endl;
        }
};


// main function
int main() {
    Library library;
    
    string input;
    while(cin >> input) {
        if (input == "Book") {
            string title, author, isbn;
            int copiesAvailable, totalCopies;

            string nextInput;
            cin >> nextInput;

            // case 2 where default constructor needs to be called
            if (nextInput == "None") {
                Book book;
                library.addBook(book);
            }

            // case 3 where copy constructor needs to be called
            else if (nextInput == "ExistingBook") {
                string exisitingBookIsbn;
                cin >> exisitingBookIsbn;

                vector<Book> books = library.getBooks();
                for (int index = 0; index < books.size(); index++) {
                    if (books[index].getIsbn() == exisitingBookIsbn) {
                        string newIsbn;
                        cin >> newIsbn;
                        Book book (books[index], newIsbn);
                        library.addBook(book);
                        break;
                    }
                }
            }

            // case 1 where parameterized constructor needs to be called
            else {
                title = nextInput;
                cin >> author >> isbn >> copiesAvailable >> totalCopies;
                Book book(title, author, isbn, copiesAvailable, totalCopies);
                library.addBook(book);
            }
        }
        else if (input == "UpdateCopiesCount") {
            string isbn;
            int count;
            cin >> isbn >> count;
            vector<Book> books = library.getBooks();
            for (int index = 0; index < books.size(); index++) {
                if (books[index].getIsbn() == isbn) {
                    books[index].updateCopies(count);
                    library.setBooksOnCountUpdate(books);
                    break;
                }
            }
        }
        else if (input == "Member") {
            string memberId, name;
            int borrowLimit;
            
            string nextInput;
            cin >> nextInput;

            if (nextInput == "NoBorrowLimit") {
                cin >> memberId >> name;
                Member member(memberId, name);
                library.registerMember(member);
            }
            else {
                memberId = nextInput;
                cin >> name >> borrowLimit;
                Member member(memberId, name, borrowLimit);
                library.registerMember(member);
            }
        }
        else if (input == "Borrow") {
            string memberId, isbn;
            cin >> memberId >> isbn;
            library.borrowBook(memberId, isbn);
        }
        else if (input == "Return") {
            string memberId, isbn;
            cin >> memberId >> isbn;
            library.returnBook(memberId, isbn);
        }
        else if (input == "PrintBook") {
            string isbn;
            cin >> isbn;
            vector<Book> books = library.getBooks();
            for (int index = 0; index < books.size(); index++) {
                if (books[index].getIsbn() == isbn) {
                    books[index].printDetails();
                    break;
                }
            }
        }
        else if (input == "PrintMember") {
            string memberId;
            cin >> memberId;
            vector<Member> members = library.getMembers();
            for (int index = 0; index < members.size(); index++) {
                if (members[index].getMemberId() == memberId) {
                    members[index].printDetails();
                    break;
                }
            }
        }
        else if (input == "PrintLibrary")
            library.printLibraryDetails();
    }

    return 0;
}