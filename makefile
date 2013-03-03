all: 
	g++ -o main blackjack.cpp main.cpp -lsfml-graphics -lsfml-system -lsfml-window
	./main
