CC=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
CFLAGS=-I.
DEPS = StaticFlatMap.h
OBJ = main.o 

%.o: %.cpp $(DEPS)
		$(CC) -Wall -Wextra -Werror -c -o $@ $< $(CFLAGS)

StaticFlatMap: $(OBJ)
		g++ -Wall -Wextra -Werror -o $@ $^ $(CFLAGS)
