all: main

main:src/main.cpp
	mkdir -p bin
	g++ -Wall -o bin/program src/main.cpp  -Idep/include -Bstatic -Ldep/lib -lglfw3 -lGLEW -Bdynamic  -pthread -ldl -lGL

debug:src/main.cpp
	mkdir -p bin
	g++ -Wall -DOPENGL_TEST_DEBUG_MODE -g -o bin/program src/main.cpp  -Idep/include -Bstatic -Ldep/lib -lglfw3 -lGLEW -Bdynamic  -pthread -ldl -lGL

clean:
	rm -rf bin