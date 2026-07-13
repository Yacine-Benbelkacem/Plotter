CC = gcc
TARGET = plotter

SRCS = $(wildcard *.c)
OBJ = $(SRCS:.c=.o)

LD_FLAGS = -lSDL2 -lSDL2_ttf -lm

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LD_FLAGS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)