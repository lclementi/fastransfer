# 
# Luca Clementi luca.clementi@gmail.com
#
# recv.c and send.c copied from 
# https://github.com/matildah/fastcat
#
#

CFLAGS += -Wextra -Wall -ggdb 

SRCS = $(wildcard *.c)
INCLUDE =  $(wildcard *.h)
TARGETS = send recv

all: $(TARGETS)

$(TARGETS): $(SRC) $(INCLUDE)
	$(CC) $(CFLAGS) -o $@ $@.c net.c

clean:
	rm -f send recv


