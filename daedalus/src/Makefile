TARGET   = test
OBJS     = test.o daedalus.o maze.o solve.o util.o graphics.o color.o create.o create2.o create3.o labyrnth.o threed.o draw.o inside.o command.o solids.o draw2.o
FLAGS    = -g -O0 -Wall -std=c++11
INCLUDES =
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
