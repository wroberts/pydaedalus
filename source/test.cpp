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
    bm.b.FAllocate(xStart, yStart, NULL);
    InitRndL(109);
    bm.b.CreateMazePerfect();

    // save the maze to bitmap
    FFileSave(cmdSaveBitmap, "test.bmp");
    FFileSave(cmdSaveText, "test.txt");

    return 0;
}
