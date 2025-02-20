CFLAGS = -std=c++20 -Wall -Wextra -Wpedantic
LDFLAGS = -I./include/
LIBS = -lglfw -lGL

all: main
	./main

main: src/main.cpp
	g++ src/main.cpp src/glad.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o main

clean:
	rm -f main
