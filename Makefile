build: game

game: main.o Entity.o 
	g++ main.o Entity.o -o game -lraylib -lm

main.o: main.cpp Entity.hpp 
	g++ -c main.cpp -o main.o

Entity.o: Entity.cpp Entity.hpp
	g++ -c Entity.cpp  -o Entity.o

clean:
	rm -f *.o game
