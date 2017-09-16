CC=gcc
CFLAGS=-I.
DEPS = StaticFlatMap.h
OBJ = main.o 

%.o: %.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)
