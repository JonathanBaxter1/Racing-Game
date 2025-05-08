LINK_FLAGS := -lGL -lGLU -lglut -lGLEW -lglfw -lXxf86vm -lXrandr -lXi -ldl -lXinerama -lXcursor -lassimp
#BUILD_DIR = ./build
#SRC_DIR = ./src

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:%=build/%.o)
OBJS := $(OBJS:.cpp.o=.o)
OBJS := $(subst /src/,/,$(OBJS))

all: $(OBJS) bin/race

$(OBJS): build/%.o: src/%.cpp
	g++ -c $^ -o $@ -Wall

bin/race: $(OBJS)
	g++ -o bin/race $(OBJS) $(LINK_FLAGS)

.PHONY: clean
clean:
	rm -r build/*
	rm bin/race
