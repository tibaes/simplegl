CXX = g++
CXXFLAGS = -O3 -std=c++11
LDFLAGS = -lGL -lGLEW -lglfw

all: main

main: initShaders.o
	$(CXX) $(CXXFLAGS) main.cpp initShaders.o -o main $(LDFLAGS)

initShaders.o:
	$(CXX) $(CXXFLAGS) -c initShaders.cpp -o initShaders.o $(LDFLAGS)

clean:
	rm main *.o
