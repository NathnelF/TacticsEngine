
build: game

game: main.o camera.o input.o grid.o movement.o turns.o
	g++ main.o camera.o input.o grid.o movement.o turns.o -o game -lraylib -lm -Wall

main.o: main.cpp camera.hpp input.hpp grid.hpp movement.hpp turns.hpp 
	g++ -c main.cpp -o main.o -Wall

movement.o: movement.cpp movement.hpp grid.hpp
	g++ -c movement.cpp -o movement.o -Wall

turns.o: turns.cpp turns.hpp grid.hpp
	g++ -c turns.cpp -o turns.o -Wall

grid.o: grid.cpp grid.hpp 
	g++ -c grid.cpp -o grid.o -Wall

camera.o: camera.cpp camera.hpp input.hpp
	g++ -c camera.cpp -o camera.o -Wall

input.o: input.hpp input.hpp
	g++ -c input.cpp -o input.o -Wall

clean:
	rm -f *.o game

vim:
	nvim *.cpp *.hpp
