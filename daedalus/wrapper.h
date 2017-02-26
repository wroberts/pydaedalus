#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include <cstdio>
using namespace std;

#include "util.h"
#include "graphics.h"
#include "color.h"
#include "threed.h"
#include "maze.h"
#include "draw.h"
#include "resource.h"
#include "daedalus.h"

enum FFileSaveRVal {
    SAVE_NO_MAZE_ERROR = 1,
    SAVE_NO_FILENAME_GIVEN_ERROR,
    SAVE_FILE_OPEN_ERROR,
    SAVE_UNKNOWN_CMD_ERROR,

    SAVE_SUCCESS
};

CMaz *cpp_Constructor(unsigned width, unsigned height);
void cpp_Destructor(CMaz *maze);
bool cpp_CreateMazePerfect(CMaz *maze);
bool cpp_Resize(CMaz *maze, unsigned width, unsigned height);
int wrapper_FFileSave(CMaz *maze,
                      int wCmd,
                      const char *filename,
                      KV kvOn,
                      KV kvOff,
                      bool fTextClip,
                      bool fLineChar,
                      bool fTextTab);
int cpp_SaveBitmap(CMaz *maze, const char *filename);
int cpp_SaveText(CMaz *maze, const char *filename);

#endif /* _WRAPPER_H_ */
