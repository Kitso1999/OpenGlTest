all: main

main:src/main.cpp
	mkdir -p bin
	g++  -o bin/program src/main.cpp  -Idep/include -Bstatic -Ldep/lib -lglfw3 -lGLEW -Bdynamic  -pthread -ldl -lGL

clean:
	rm -rf bin