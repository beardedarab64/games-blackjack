#ifndef BLACKJACK_H
#define BLACKJACK_H
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <sstream>
using namespace std;
enum state_t {HITTING, WAITING, STANDBY};
class Card{
public:
	// METHODS
	bool setSprite();
	void flip();
	void changeUsed();
	// VARIABLES
	int rank;
	int suit;
	bool isUsed;	
	bool isFaceDown;
	sf::Sprite sprite;
        sf::Image front;
	int x;
	int y;
        int cardCode;
        string fileName;
};
class Deck{
public:	
        // METHODS
	Deck(sf::RenderWindow *App);
	void shuffle();
	void setDeck(sf::RenderWindow *App);
        // VARIABLES
	Card card[52];
	sf::Sprite sprite;
	sf::Image back;
};
class Player {
public:
	// METHODS
        void showCards();
        void hideCards();
	void hit(Deck *deck);
	bool isBusted();
	int getTotal();
        void displayTotal();
	Player(Deck *deck);
	void setPosition(sf::RenderWindow *App,float yPosition);
	bool dealerMove(Deck *deck);
        void displayHand();
	// VARIABLES
        state_t state;
	bool isTurn;
	vector<Card> hand;
	string name;
};
class Table {
public:
        // METHODS
        Table(sf::RenderWindow *App,string textFont);
        void displayPoints(sf::RenderWindow *App,int points);
        bool isPointOverSprite(const sf::Vector2f Position, sf::Sprite &Sprite);
        void showMessage(sf::RenderWindow *App,string message, sf::Color color,int size);
        // VARIABLES
        stringstream ss;
        sf::Image hitImg;
        sf::Image standImg;
        sf::Sprite hitBtn;
        sf::Sprite standBtn;
        sf::Font font;
        sf::String sfMessage;
        sf::String pointTotal;
};
#endif
