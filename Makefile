all: audiobuffer.o

CFLAGS = -Wall

VPATH = src

%.o: %.c %.h

clean:
	rm -f audiobuffer.o *~
