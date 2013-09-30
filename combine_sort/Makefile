CXXFLAGS = -Wall -g

COMPILE_DIR = .
BIN_DIR = ${COMPILE_DIR}/bin
SRC_DIR = ${COMPILE_DIR}/src
INC_DIR = ${COMPILE_DIR}/inc

CPPFLAGS += -I${INC_DIR}

COMM_OBJ =  ${SRC_DIR}/file_utils.o \
            ${SRC_DIR}/merge_sort.o

FI_OBJ = ${SRC_DIR}/main.o

all: merge_sort
.PHONY: all

merge_sort:  ${FI_OBJ} ${COMM_OBJ}
	g++ ${CXXFLAGS} -o ${BIN_DIR}/merge_sort ${CPPFLAGS} $^

clean:
	rm -f ${SRC_DIR}/*.o ${BIN_DIR}/*


