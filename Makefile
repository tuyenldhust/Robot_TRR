CC=gcc
CFLAGS=-w ${DEBUG} -I${INC_DIR}

all: libfdr.a BT4

.PHONY: libfdr.a

libfdr.a:
	$(MAKE) -C ./libfdr
BT4:
	${CC} -w -o $@ BT4.c libfdr/libfdr.a 
clean:
	rm -f BT4
	$(MAKE) clean -C ./libfdr