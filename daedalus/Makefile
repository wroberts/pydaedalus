TARGET   = main
OBJS     = main.o wrapper.o src/daedalus.o src/maze.o src/solve.o src/util.o src/graphics.o src/color.o src/create.o src/create2.o src/create3.o src/labyrnth.o src/threed.o src/draw.o src/inside.o src/command.o src/solids.o src/draw2.o
FLAGS    = -g -O0 -Wall -std=c++11
INCLUDES = -Isrc
LIBS     =
CC       = gcc
CXX      = g++

all       : $(TARGET)

clean     :
	rm -f $(TARGET) $(OBJS)

$(TARGET) : $(OBJS)
	$(CXX) $(FLAGS) -o $@ $^ $(LIBS)

%.o       : %.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -o $@ -c $<
