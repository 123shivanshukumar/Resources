#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

// Base class
class Card {
public:
    Card* parent = nullptr;
    virtual int area() const = 0;
    virtual string toString() const = 0;
    virtual ~Card() {}
    bool operator<(const Card& other) const {
        return this->area() < other.area();
    }
    friend ostream& operator<<(ostream& os, const Card& c) {
        os << c.toString();
        return os;
    }
    friend std::istream& operator>>(std::istream &is, Card &c) {
        c.read(is);
        return is;
    }
    virtual void read(std::istream &is) = 0;
};

// Square
class Square : public Card {
    double edge;
public:
    Square(int e) : edge(e) {}
    int area() const override { return edge * edge; }
    string toString() const override {
        ostringstream oss;
        oss << "SQUARE " << edge;
        return oss.str();
    }
    void read(std::istream &is) {
        is >> edge;
    }
};

// Rectangle
class Rectangle : public Card {
    int length, breadth;
public:
    Rectangle(int l, int b) : length(l), breadth(b) {}
    int area() const override { return length * breadth; }
    string toString() const override {
        ostringstream oss;
        oss << "RECTANGLE " << length << " " << breadth;
        return oss.str();
    }
    void read(std::istream &is) {
        is >> length >> breadth;
    }
};

// Triangle
class Triangle : public Card {
    double base, height;
public:
    Triangle(int b, int h) : base(b), height(h) {}
    int area() const override { return (base * height)/2; }
    string toString() const override {
        ostringstream oss;
        oss << "TRIANGLE " << base << " " << height;
        return oss.str();
    }
    void read(std::istream &is) {
        is >> base >> height;
    }
};

// Manager
class CardOrganizer {
    vector<Card*> topCards;
    vector<vector<Card*>> piles;

public:

    void addCard(Card* card) {
        auto it = std::lower_bound(topCards.begin(), topCards.end(), card,
        [](Card* a, Card* b) {
            return *a < *b;
        });
        int index = it - topCards.begin();
        if (it == topCards.end()) {
            piles.push_back({card});
            topCards.push_back(card);
        } else {
            topCards[index] = card;
            piles[index].push_back(card);
            
        }
        if(index > 0) {
            card->parent = piles[index - 1].back();
        }
    }

    int getPileCount() const {
        return piles.size();
    }

    void printLIS() const {
        vector<Card*> lis;
        cout << piles.size() << '\n';
        Card* current = nullptr;
        if(!piles.empty()) {
            current = piles.back().back();
        }
        while(current) {
            lis.push_back(current);
            current = current->parent;
        }
        reverse(lis.begin(), lis.end());
        for (const auto& card : lis) {
            cout << *card << '\n';
        }

    }

    void printPiles() const {
        for (int i=0; i< piles.size(); i++) {
            for (const auto& card : piles[i]) {
                cout << i << ": ";
                cout << *card << '\n';
            }
        }
    }
};

int main() {
    int q;
    cin >> q;
    CardOrganizer manager;

    while (q--) {
        int type;
        string shape;
        cin >> type;
        if (type == 1) {
            cin >> shape;
            Card *c = nullptr;
            if (shape == "SQUARE") {
                c = new Square();
            } else if (shape == "RECTANGLE") {
                c = new Rectangle();
            } else if (shape == "TRIANGLE") {
                c = new Triangle();
            } else {
            }

            std::cin >> *c;
            manager.addCard(c);
        } else if (type == 2) {
            cout << manager.getPileCount() << '\n';
        } else if (type == 3) {
            manager.printLIS();
        }
        // manager.printPiles();
        // cout << "------------------------" << endl;
    }

    return 0;
}
