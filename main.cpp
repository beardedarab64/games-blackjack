#include <SFML/Graphics.hpp>
#include <iostream>
#include "blackjack.h"

int main(int argc,char *argv[]){
        //900,600,32
	sf::RenderWindow App(sf::VideoMode(800,600,32),"Blackjack");
        App.SetFramerateLimit(60);
	Deck deck(&App);
	deck.setDeck(&App);
	deck.shuffle();
	Player user(&deck);
	Player house(&deck);
         house.hideCards();
        Table table(&App,"fonts/arial.ttf");
	while(App.IsOpened()){	
		sf::Event Event;
                sf::Vector2f mousePosition= App.ConvertCoords(App.GetInput().GetMouseX(), App.GetInput().GetMouseY());
                // Check for events 
		while(App.GetEvent(Event)){
			if(Event.Type==sf::Event::Closed){
				App.Close();
			}
			if((Event.Type==sf::Event::KeyPressed) && (Event.Key.Code ==sf::Key::Escape)){
				App.Close();
			}
			if((Event.Type==sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up)){
                                // Hit both players upon up button event
				user.hit(&deck);
				house.hit(&deck);
			}
                        if((Event.Type==sf::Event::MouseButtonPressed)&&(Event.MouseButton.Button ==sf::Mouse::Left)&&(table.isPointOverSprite(mousePosition,table.hitBtn))){
                            if(!user.isBusted()&&user.state!=STANDBY){
                              user.hit(&deck);
                              user.state=HITTING;
                            }
                        }
                        if((Event.Type==sf::Event::MouseButtonPressed)&&(Event.MouseButton.Button ==sf::Mouse::Left)&&(table.isPointOverSprite(mousePosition,table.standBtn))){
				 user.state=STANDBY;
				 house.state=HITTING;
                        }
		}
		if(house.state==HITTING){
			if(house.dealerMove(&deck)==false){
				house.state=STANDBY;
			} 

		}
		if(user.state==2&&house.state==2){
                        house.showCards();
                        if(!house.isBusted()&&!user.isBusted()){
	        	  if(house.getTotal()<user.getTotal()){
                            cout<<"You Win!";
	      	            table.showMessage(&App,"You win!",sf::Color(0,0,0),60);
			  } else if(house.getTotal()>user.getTotal()){
                           cout<<"You lose!";
			   table.showMessage(&App,"House wins.",sf::Color(255,9,0),60);
			  } else {	
                           cout<<"TIE";
			   table.showMessage(&App,"Tie!",sf::Color(0,0,255),60);
		          }
                        }
		}
                // Set total
                table.displayPoints(&App,user.getTotal());
                // Hide houses cards
                // Set user and house's hand positions
		user.setPosition(&App,.65);
		house.setPosition(&App,.1);
                // Clear screen
		App.Clear(sf::Color(0,144,74));
                // Draw deck, player hands, buttons, and total points
                App.Draw(table.hitBtn);
                App.Draw(table.standBtn);
		App.Draw(deck.sprite);
                App.Draw(table.pointTotal);
                if(user.isBusted()){
                  table.showMessage(&App,"You Busted!",sf::Color(255,9,0),60);
                  house.showCards();
                  App.Draw(table.sfMessage);
                }
                if(house.isBusted()){
                  table.showMessage(&App,"House Busted!",sf::Color(255,9,0),60);
                  house.showCards();
                  App.Draw(table.sfMessage);
                }
                App.Draw(table.sfMessage);
		for(int i=0;i<user.hand.size();i++){
			App.Draw(user.hand[i].sprite);
		}
		for(int i=0;i<house.hand.size();i++){
			App.Draw(house.hand[i].sprite);
		}
		App.Display();
	}
	App.Close();
	return EXIT_SUCCESS;
}
