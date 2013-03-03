#include "blackjack.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
// CARD DEFINITIONS
void Card::changeUsed(){
	isUsed=!isUsed;
}
void  Card::flip(){
	isFaceDown=!isFaceDown;
}
bool Card::setSprite(){
	if (isFaceDown==false){
		if(!front.LoadFromFile(fileName)){
			return EXIT_FAILURE;
		}
	} else {
		if(!front.LoadFromFile("images/b1fv.jpg")){
			return EXIT_FAILURE;
		}
	}
	sprite.SetImage(front);
	sprite.Resize(71,96);
}
// DECK DEFINITONS
Deck::Deck(sf::RenderWindow *App){
	back.LoadFromFile("images/b1fv.jpg");
	sprite.SetImage(back);
	sprite.Resize(71,96);
	sprite.SetPosition(App->GetWidth()*.035,App->GetHeight()*.035);
}
void Deck::shuffle() {
    srand(time(0));  // initialize seed "randomly"
        //--- Shuffle elements by randomly exchanging each with one other.
        for (int i=0; i<(52-1); i++) {
            int r = i + (rand() % (52-i)); // Random remaining position.
            Card temp = card[i]; card[i] = card[r]; card[r] = temp;
            card[i].setSprite();
            card[r].setSprite();
        }
}
void Deck::setDeck(sf::RenderWindow *App) {	
	// initializes card, card suit(cSuit), and card rank(cRank)
	int cSuit = 1;
	int cRank = 1;
	// sets values for card
	for (int n = 0; n < 52; n++) {
		card[n].rank = cRank;
       		card[n].isUsed = false;
		cRank++;
		card[n].suit = cSuit;
               // card[n].cardCode = card[n].suit + (card[n].rank -1) * 4 + 1;
	        
	        card[n].cardCode = (card[n].rank-1)*4 + card[n].suit;
                stringstream itos;
                itos << "images/"<<card[n].cardCode << ".jpg";
                card[n].fileName=itos.str();
                card[n].isFaceDown=false;
		card[n].sprite.SetPosition(App->GetWidth()*.025,App->GetHeight()*.025);
	//	card[n].setSprite();
		if (cRank == 14) {
			cRank = 1;
			cSuit++;
		}
	}	
}
	
// PLAYER DEFINITIONS
Player::Player(Deck *deck) {
        state = WAITING;
	hit(deck);
	hit(deck);
}
void Player::setPosition(sf::RenderWindow *App, float yPosition){
	// Find and use horizontal midpoint as starting position for card
	int midpoint = App->GetWidth()/2;
        // 41
	int halfX = hand.size() * 9;
	int startX = midpoint - halfX;
	// Acceleration of card
	float velocity= 0.1;
	for(int i=0;i<hand.size();i++){
		// Set cards position 
		if(hand[i].isFaceDown==true){
			hand[i].setSprite();
		}
		hand[i].x=startX;
		hand[i].y=App->GetHeight()*yPosition;
                if(((int)hand[i].sprite.GetPosition().x +1!= (int)hand[i].x)||((int)hand[i].sprite.GetPosition().y+1 != (int)hand[i].y)){
	         	hand[i].sprite.Move((hand[i].x-hand[i].sprite.GetPosition().x ) * velocity  , (hand[i].y - hand[i].sprite.GetPosition().y ) * velocity );
                     //   cout << (int)hand[i].sprite.GetPosition().x << " " <<(int) hand[i].x << endl;
                }
             //   cout<<"Done if!";
             //   82
		startX+=18;
	}
}
void Player::hit(Deck *deck) {
	for(int n=0;n<52;n++)
	{
		if(deck->card[n].isUsed == false) {
		 	hand.push_back(deck->card[n]);
                      //  hand[n].isFaceDown=false;
			deck->card[n].isUsed = true;
			break;
		}
	}
}
bool Player::isBusted(){
	int total=0;
	for(int i=0;i<hand.size();i++){
		if(hand[i].rank<10){
			total+=hand[i].rank;
		} else {
			total+=10;
		}
	}
	if(total<=21){
		return false;
	} else {
		return true;
	}
}
int Player::getTotal(){
	int total=0;
	int ones=0;
	for(int i=0;i<hand.size();i++){
		if(hand[i].rank<10 && hand[i].rank!=1){
			total+=hand[i].rank;	
		} else if(hand[i].rank!=1){
			total+=10;
		} else {
			ones++;
		}
	}
	if (ones>0){
		for(int i=0;i<ones;i++){
			if (total+11+ones<21){
				total+=11;
			} else {
				total+=1;
			}
		}
	}
	return total;
}	
bool Player::dealerMove(Deck *deck){
	int total=0;
	bool didHit=false;
	int li=hand.size()-1;
	for(int i=0;i<hand.size();i++){
		if(hand[i].rank<10){
			total+=hand[i].rank;
		} else {
			total+=10;
		}
	}
	if (total<=17){
//		if(hand[li].x==hand[li].sprite.GetPosition().x && hand[li].y==hand[li].sprite.GetPosition().y){
			hit(deck);
//		}
		didHit=true;
	}
        return didHit;
}
void Player::displayTotal(){
  for(int i=0;i<hand.size();i++){
      cout<<hand[i].rank<<" "<<hand[i].suit<<" , "<<hand[i].fileName <<" " <<endl;
  }
  cout<<endl;
}
void Player::hideCards(){
  for(int i=1;i<hand.size();i++){
    hand[i].isFaceDown=true;
  }
}
void Player::showCards(){
  for(int i=0;i<hand.size();i++){
    if(hand[i].isFaceDown==true){
      hand[i].isFaceDown=false;
      hand[i].setSprite();
    }
  }
}
// TABLE DEFINITONS
Table::Table(sf::RenderWindow *App,string textFont){
  int x=App->GetWidth()/2-75;
  int y=App->GetHeight()*.85;
  if(!font.LoadFromFile(textFont)){
    cout<<"Missing "<<textFont;
    cout<<"\nAbort!\n";
  }
  if(!hitImg.LoadFromFile("images/hit.png")){
    cout<<"Missing images/hit.png\n";
  } else {
    hitBtn.SetImage(hitImg);
    hitBtn.SetPosition(x,y);
  }
  if(!standImg.LoadFromFile("images/stand.png")){
    cout<<"Missing images/stand.png\n";
  } else {
    standBtn.SetImage(standImg);
    standBtn.SetPosition(x+100,y);
  }
}
void Table::displayPoints(sf::RenderWindow *App, int points){
  ss.str(string());
  ss<<"Total: "<<points;
  pointTotal.SetText(ss.str());
  pointTotal.SetFont(font);
  pointTotal.SetSize(25);
  pointTotal.SetColor(sf::Color(255,243,51));
  pointTotal.SetPosition((App->GetWidth()/2.13),App->GetHeight()*.55);
}
bool Table::isPointOverSprite(const sf::Vector2f Position, sf::Sprite &Sprite){
  return  (Position.x < Sprite.GetPosition().x + Sprite.GetSize().x) && (Sprite.GetPosition().x < Position.x) &&  (Position.y < Sprite.GetPosition().y + Sprite.GetSize().y) && (Sprite.GetPosition().y < Position.y);
}
void Table::showMessage(sf::RenderWindow *App,string message, sf::Color color,int size){
  int x=App->GetWidth()*.5 - message.length() * 12;
  int y=App->GetHeight()*.35;
  sfMessage.SetFont(font);
  sfMessage.SetText(message);
  sfMessage.SetColor(color);
  sfMessage.SetSize(size);
  sfMessage.SetPosition(x,y);
}
