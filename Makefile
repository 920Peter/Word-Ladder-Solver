CC = gcc
CFLAGS = -g -Wall

all: wordladder

wordladder: wordladder.c
	$(CC) $(CFLAGS) wordladder.c -o wordladder

run: wordladder
	./wordladder dictionary.txt ladder_test_words.txt

clean:
	rm -f wordladder *.o *.exe
