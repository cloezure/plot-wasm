# project name
TARGET := plot

OUT := 0
WEB := 1
# project type
MODE := $(0)
ifeq ($(MODE), $(WEB))
PROJECT_TYPE := "WEB"
else
PROJECT_TYPE := "OUT"
endif

ifeq ($(MODE), $(WEB))
# compiler
	CC := emcc

# build in
	BUILD_IN := mjs

override CFLAGS += -std=c11\
				-O2\
				-sUSE_SDL_TTF=2\
				-sNO_EXIT_RUNTIME=1\
				-sENVIRONMENT="web"\
				-sEXPORT_ES6=1\
				-sUSE_SDL_IMAGE=2\
				-sSDL2_IMAGE_FORMATS=["png"]\
				-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']\
				-sUSE_SDL=2\
				-sSINGLE_FILE=1\
				-sMODULARIZE=1\
				-sASSERTIONS=1\
				-sALLOW_MEMORY_GROWTH\
				-sEXPORT_NAME="GraphicsPlot"\
				--preload-file ./res

else
# compiler
CC := clang

# build in
BUILD_IN := out

# project flags
override CFLAGS += -std=c17\
				-Wall\
				-Wextra\
				-pedantic\
				-O2\
				-lSDL2\
				-lSDL2_ttf\
				-lSDL2_image
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
	cp build/plot.mjs ../radius_control_frontend/src/components/spectrum
	cp build/plot.data ../radius_control_frontend/public
	npm start --prefix ../radius_control_frontend

move: release
	cp build/plot.mjs ../radius_control_frontend/src/components/spectrum
	cp build/plot.data ../radius_control_frontend/public

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
