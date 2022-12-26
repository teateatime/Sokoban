CC = g++
CFLAGS = -Wall -Werror -pedantic -std=c++11 -lboost_unit_test_framework
OBJECTS = sokoban.o main.o
SFML = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: sokoban

sokoban: $(OBJECTS)
	$(CC) $(CFLAGS) -o sokoban $(OBJECTS) $(SFML)

%.o: %.cpp sokoban.h status.h
	$(CC) $(CFLAGS) -c $< $(SFML)

lint:
	cpplint main.cpp sokoban.cpp sokoban.h status.h

clean:
	rm *.o sokoban