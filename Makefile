build: game

game: main.o Entity.o Tile.o Grid.o
	g++ main.o Entity.o Tile.o Grid.o -o game -lraylib -lm -Wall

main.o: main.cpp Entity.hpp Tile.hpp Grid.hpp
	g++ -c main.cpp -o main.o -Wall

Entity.o: Entity.cpp Entity.hpp
	g++ -c Entity.cpp -o Entity.o -Wall

Grid.o: Grid.cpp Grid.hpp Tile.hpp
	g++ -c Grid.cpp -o Grid.o -Wall

Tile.o: Tile.cpp Tile.hpp
	g++ -c Tile.cpp -o Tile.o -Wall


clean:
	rm -f *.o game
