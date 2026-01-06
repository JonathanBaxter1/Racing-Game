PLATFORM ?= linux
COMPILE_FLAGS := -Wall -O3

ifeq ($(PLATFORM), linux)
	LINK_FLAGS := -lGL -lGLU -lGLEW -lglfw -lXxf86vm -lXrandr -lXi -ldl -lXinerama -lXcursor -lassimp $(shell pkg-config --libs freetype2)
	COMPILE_FLAGS += $(shell pkg-config --cflags freetype2) -Istb_image
	CXX := g++
	EXE := bin/race
endif
ifeq ($(PLATFORM), windows)
	LINK_FLAGS := -lopengl32 -lgdi32 -Lassimp/build-windows/lib -Lglew-2.2.0/lib -Lglfw-3.4/build-windows/src glfw-3.4/build-windows/src/glfw3.dll glew-2.2.0/lib/glew32.dll assimp/build-windows/bin/libassimp-6.dll -L/opt/freetype-windows/lib -lfreetype -static-libgcc -static-libstdc++
	COMPILE_FLAGS += -Iassimp/build-windows/include -Iglew-2.2.0/include -Iglfw-3.4/include -I/opt/freetype-windows/include/freetype2 -Istb_image
	CXX := x86_64-w64-mingw32-g++
	EXE := bin/race.exe
endif

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:%=build/%.o)
OBJS := $(OBJS:.cpp.o=.o)
OBJS := $(subst /src/,/,$(OBJS))

.PHONY: all
all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LINK_FLAGS)

build/%.o: src/%.cpp
	$(CXX) -c $^ -o $@ $(COMPILE_FLAGS)

.PHONY: clean
clean:
	rm -r build/*
	rm bin/race
