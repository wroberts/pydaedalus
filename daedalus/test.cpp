#include <iostream>
using namespace std;

#include "wrapper.h"
/*
  #include "util.h"
  #include "graphics.h"
  #include "color.h"
  #include "threed.h"
  #include "maze.h"
  #include "draw.h"
  #include "resource.h"
  #include "daedalus.h"
*/

int main(int argc, char *argv[])
{
    cout << "hello world" << endl;

    // create a maze
    CMaz *maze = cpp_Constructor(63, 63);
    cpp_CreateMazePerfect(maze);

    // resize to given shape (width, height)
    // this truncates the maze
    // maze dimensions must be odd
    // entrance is top left and exit is bottom right
    cpp_Resize(maze, 31, 61);
    // rebuild the maze
    cpp_CreateMazePerfect(maze);

    // save the maze to bitmap
    cpp_SaveBitmap(maze, "test.bmp");
    cpp_SaveText(maze, "test.txt");

    // free memory
    cpp_Destructor(maze);

    return 0;
}
