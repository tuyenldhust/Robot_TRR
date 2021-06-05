CC=gcc
INC_DIR=./libfdr
LIB_DIR=./libfdr
LIB_NAME=fdr
LIB_FILE=${LIB_DIR}/lib${LIB_NAME}.a

CFLAGS=-w ${DEBUG} -I${INC_DIR}

.PHONY: all clean

BT4: BT4.c ${LIB_FILE}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ BT4.c ${LIB_FILE} 
clean:
	rm -f B4