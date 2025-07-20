build: game

game: main.o Entity.o Tile.o Grid.o utils.o camera.o
	g++ main.o Entity.o Tile.o Grid.o utils.o camera.o -o game -lraylib -lm -Wall

main.o: main.cpp Entity.hpp Tile.hpp Grid.hpp utils.hpp camera.hpp
	g++ -c main.cpp -o main.o -Wall

Entity.o: Entity.cpp Entity.hpp
	g++ -c Entity.cpp -o Entity.o -Wall

Grid.o: Grid.cpp Grid.hpp Tile.hpp
	g++ -c Grid.cpp -o Grid.o -Wall

Tile.o: Tile.cpp Tile.hpp
	g++ -c Tile.cpp -o Tile.o -Wall

utils.o: utils.cpp utils.hpp
	g++ -c utils.cpp -o utils.o -Wall

camera.o: camera.cpp camera.hpp
	g++ -c camera.cpp -o camera.o -Wall


clean:
	rm -f *.o game

vim:
	nvim *.cpp *.hpp
