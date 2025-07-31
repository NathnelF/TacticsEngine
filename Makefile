
build: game

game: main.o camera.o input.o grid.o movement.o turns.o abilities.o cover.o
	g++ main.o camera.o input.o grid.o movement.o turns.o abilities.o cover.o -o game -lraylib -lm -Wall

main.o: main.cpp camera.hpp input.hpp grid.hpp movement.hpp turns.hpp 
	g++ -c main.cpp -o main.o -Wall

turns.o: turns.cpp turns.hpp grid.hpp abilities.hpp movement.hpp
	g++ -c turns.cpp -o turns.o -Wall

abilities.o: abilities.cpp abilities.hpp grid.hpp movement.hpp
	g++ -c abilities.cpp -o abilities.o -Wall

movement.o: movement.cpp movement.hpp grid.hpp
	g++ -c movement.cpp -o movement.o -Wall

grid.o: grid.cpp grid.hpp cover.hpp
	g++ -c grid.cpp -o grid.o -Wall

cover.o: cover.hpp cover.cpp
	g++ -c cover.cpp -o cover.o -Wall

camera.o: camera.cpp camera.hpp input.hpp
	g++ -c camera.cpp -o camera.o -Wall

input.o: input.hpp input.hpp
	g++ -c input.cpp -o input.o -Wall

clean:
	rm -f *.o game

vim:
	nvim *.cpp *.hpp

hx:
	hx $(shell find . -name "*.cpp" -o -name "*.hpp" | sort)
