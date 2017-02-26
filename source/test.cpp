#include <iostream>
using namespace std;

#include "util.h"
#include "graphics.h"
#include "color.h"
#include "threed.h"
#include "maze.h"
#include "draw.h"
#include "resource.h"
#include "daedalus.h"

int main(int argc, char *argv[])
{
    cout << "hello world" << endl;

    // create a maze
    CMaz b;
    b.FAllocate(xStart, yStart, NULL);
    InitRndL(109);
    b.CreateMazePerfect();

    // save the maze to bitmap
    FFileSave(cmdSaveBitmap, NULL);

    return 0;
}
