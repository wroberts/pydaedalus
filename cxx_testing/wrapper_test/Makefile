TARGET   = main
SRC_PATH = ../../daedalus/src
OBJS     = main.o $(SRC_PATH)/../wrapper.o $(SRC_PATH)/daedalus.o $(SRC_PATH)/maze.o $(SRC_PATH)/solve.o $(SRC_PATH)/util.o $(SRC_PATH)/graphics.o $(SRC_PATH)/color.o $(SRC_PATH)/create.o $(SRC_PATH)/create2.o $(SRC_PATH)/create3.o $(SRC_PATH)/labyrnth.o $(SRC_PATH)/threed.o $(SRC_PATH)/draw.o $(SRC_PATH)/inside.o $(SRC_PATH)/command.o $(SRC_PATH)/solids.o $(SRC_PATH)/draw2.o
FLAGS    = -g -O0 -Wall -std=c++11 -Wno-unused-result -fwrapv -Wstrict-prototypes -fstack-protector -Wformat -DDEBUG
INCLUDES = -I$(SRC_PATH) -I$(SRC_PATH)/..
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
