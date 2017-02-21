CXX = g++
CXXFLAGS = -O3 -std=c++11 `pkg-config --cflags opencv`
LDFLAGS = -lGL -lGLEW -lglfw `pkg-config --libs opencv`

all: main

main: initShaders.o
	$(CXX) $(CXXFLAGS) main.cpp initShaders.o -o main $(LDFLAGS)

initShaders.o:
	$(CXX) $(CXXFLAGS) -c initShaders.cpp -o initShaders.o $(LDFLAGS)

clean:
	rm -f main *.o
