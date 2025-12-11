PLATFORM ?= linux

ifeq ($(PLATFORM), linux)
	LINK_FLAGS := -lGL -lGLU -lGLEW -lglfw -lXxf86vm -lXrandr -lXi -ldl -lXinerama -lXcursor -lassimp
	CXX := g++
	EXE := bin/race
endif
ifeq ($(PLATFORM), windows)
	LINK_FLAGS := -lopengl32 -lglew32 -lglfw3 -lassimp
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
	$(CXX) -c $^ -o $@ -Wall

.PHONY: clean
clean:
	rm -r build/*
	rm bin/race
