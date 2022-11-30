# project name
TARGET :=plot

# build in
BUILD_IN :=js

# compiler
CC :=emcc

# project flags
override CFLAGS += -std=c11\
					-O2\
					-sUSE_SDL_TTF=2\
					-sEXPORTED_RUNTIME_METHODS=["cwrap","ccall"]\
					-sUSE_SDL=2\
					-sWASM=1\
					--preload-file ./res

# source code dir
DSRC :=./src

# build dir
DBUILD :=./build

# list source files
SRC :=$(wildcard $(DSRC)/*.c)

# all targets
.PHONY: all run clean

all:
	$(CC) $(CFLAGS) $(SRC) -o $(DBUILD)/$(TARGET).$(BUILD_IN)

clean:
	rm -rf $(DBUILD)/$(TARGET).{js,wasm}

run: all
	emrun $(DBUILD)/index.html
