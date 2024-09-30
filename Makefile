CFLAGS = -Wall -Wextra -I./include/
LIBS = -lglfw -lGL

all: main
	./main

main: src/main.cpp
	g++ src/main.cpp src/glad.c $(CFLAGS) $(LIBS) -o main

clean:
	rm -f main
