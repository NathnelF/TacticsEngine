
build: game

game: main.o camera.o input.o grid.o #Entity.o Tile.o Grid.o utils.o
	g++ main.o camera.o input.o grid.o -o game -lraylib -lm -Wall
	# g++ main.o Entity.o Tile.o Grid.o utils.o camera.o -o game -lraylib -lm -Wall

main.o: main.cpp camera.hpp input.hpp grid.hpp #Entity.hpp Tile.hpp Grid.hpp utils.hpp
	g++ -c main.cpp -o main.o -Wall

# Entity.o: Entity.cpp Entity.hpp
	# g++ -c Entity.cpp -o Entity.o -Wall

grid.o: grid.cpp grid.hpp 
	g++ -c grid.cpp -o grid.o -Wall

# Tile.o: Tile.cpp Tile.hpp
	# g++ -c Tile.cpp -o Tile.o -Wall

# utils.o: utils.cpp utils.hpp
	# g++ -c utils.cpp -o utils.o -Wall

camera.o: camera.cpp camera.hpp input.hpp
	g++ -c camera.cpp -o camera.o -Wall

input.o: input.hpp input.hpp
	g++ -c input.cpp -o input.o -Wall


clean:
	rm -f *.o game

vim:
	nvim *.cpp *.hpp
