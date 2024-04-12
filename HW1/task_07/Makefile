TARGET=./a.out
CC=g++
SRCS=\
	./main.cpp
STD=-std=c++17  # Версия cpp

all: clean $(TARGET)

$(TARGET):
	$(CC) $(STD) $(INCLUDE) $(LDFLAGS) $(LDLIBS)  -o $(TARGET) $(SRCS)

build: $(TARGET)

clean:
	rm -rf $(TARGET)
