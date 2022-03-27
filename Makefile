C=clang
CFLAGS = -Wall -Wextra -Werror -std=c11 -ggdb -O3
LIBS = `pkg-config raylib --libs`
SRCS = main.c
TARGET = raycaster

make all: $(SRCS)
	$(C) $(CFLAGS) $(LIBS) -o $(TARGET) $(SRCS) -lm

clean:
	rm $(TARGET)
