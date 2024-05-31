TARGET=./a.out
CC=clang++
SRCS=\
	./main.cpp \
	./ListGraph.cpp \
	./IGraph.cpp \
	./DSU.cpp \
	./BoxMuller.cpp \
	./Test.cpp \
	./utils.cpp
STD=-std=c++20

all: clean $(TARGET)

$(TARGET):
	$(CC) $(STD) -O2 -lm -x c++ $(SRCS) -o $(TARGET)

build: $(TARGET)

clean:
	rm -rf $(TARGET)
