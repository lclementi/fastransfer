
all: recv send 

recv: recv.c net.c
	gcc -Wall -ggdb -o recv recv.c net.c

send: send.c net.c
	gcc -Wall -ggdb -o send send.c net.c

clean:
	rm -f send recv

