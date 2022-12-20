# project name
TARGET := plot

OUT := 0
WEB := 1
# project type
MODE := $(WEB)
ifeq ($(MODE), $(WEB))
PROJECT_TYPE := "WEB"
else
PROJECT_TYPE := "OUT"
endif

ifeq ($(MODE), $(WEB))
# compiler
	CC := emcc

# build in
	BUILD_IN := js

# project flags
# -sMODULARIZE=1
override CFLAGS += -std=c11\
				-Os\
				-sUSE_SDL_TTF=2\
				-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']\
				-sUSE_SDL=2\
				-sWASM=1\
				--preload-file ./res

else
# compiler
CC := clang

# build in
BUILD_IN := out

# project flags
override CFLAGS += -std=c11\
				-O2\
				-lSDL2\
				-lSDL2_ttf
endif

# source code dir
DSRC :=./src

# build dir
DBUILD :=./build

# list source files
SRC :=$(wildcard $(DSRC)/*.c)

# all targets
.PHONY: all run build debug release clean

ifeq ($(MODE), $(WEB))
all: release
	emrun $(DBUILD)/index.html
else

all: build
	$(DBUILD)/$(TARGET).$(BUILD_IN)
endif

build: debug

release:
	$(CC) $(CFLAGS) $(SRC) -o $(DBUILD)/$(TARGET).$(BUILD_IN)

debug:
	$(CC) $(CFLAGS) -g $(SRC) -o $(DBUILD)/$(TARGET).$(BUILD_IN)

clean:
	rm -rf $(DBUILD)/$(TARGET).{js,wasm}
