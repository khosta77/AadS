TARGET=./a.out
CC=g++
SRCS=\
	./main.cpp
STD=-std=c++17  # Версия cpp

all: clean $(TARGET) ./a.out check

$(TARGET):
	$(CC) $(STD) $(INCLUDE) $(LDFLAGS) $(LDLIBS)  -o $(TARGET) $(SRCS)

build: $(TARGET)

check:
	bash equall.sh

clean:
	rm -rf $(TARGET)
