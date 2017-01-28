CXX = g++
CXXFLAGS = -O3 -std=c++11
LDFLAGS = -lGL -lGLEW -lglfw

main:
	$(CXX) $(CXXFLAGS) main.cpp -o main $(LDFLAGS)
