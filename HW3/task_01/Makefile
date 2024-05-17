TARGET=./a.out
CC=clang++
SRCS=\
	./main.cpp \
	./IGraph.cpp \
	./ListGraph.cpp \
	./MatrixGraph.cpp \
	./ArcGraph.cpp \
	./SetGraph.cpp 
STD=-std=c++20

all: clean $(TARGET)

$(TARGET):
	$(CC) $(STD) -O2 -lm -x c++ $(SRCS) -o $(TARGET)

build: $(TARGET)

clean:
	rm -rf $(TARGET)
