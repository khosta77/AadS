TARGET=./a.out
CC=g++
SRCS=\
	./main.cpp
STD=-std=c++20  # Версия cpp

all: clean $(TARGET)

$(TARGET):
	$(CC) -Wall -Wextra -Werror -ansi $(STD) $(INCLUDE) $(LDFLAGS) $(LDLIBS)  -o $(TARGET) $(SRCS)

build: $(TARGET)

clean:
	rm -rf $(TARGET)
