CC = gcc
CFLAGS = -Iinclude -Wall -g
SRCDIR = src
SOURCES = main.c $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = FCodeViolationManagement.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /f /q src\*.o *.o $(TARGET)

run: all
	./$(TARGET)
