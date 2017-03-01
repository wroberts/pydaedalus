#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include <cstdio>
#include <ctime>
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

void cpp_Init();
CMaz *cpp_Constructor(unsigned width, unsigned height);
void cpp_Destructor(CMaz *maze);
bool cpp_Get(CMaz *maze, unsigned x, unsigned y);
bool cpp_CreateMazePerfect ( CMaz *maze,
                             bool  fRiver,
                             bool  fRiverEdge,
                             bool  fRiverFlow,
                             bool  fSection,
                             bool  fTreeWall,
                             int   nEntrancePos,
                             int   nRndBias,
                             int   nRndRun );
bool cpp_CreateMazePerfect2 ( CMaz *maze,
                              bool  fRiver,
                              bool  fSection,
                              int   nEntrancePos,
                              int   nRndBias,
                              int   nRndRun );
bool cpp_CreateMazeBraid ( CMaz *maze,
                           bool  fSection,
                           int   nEntrancePos,
                           int   nRndBias,
                           int   nRndRun );
bool cpp_CreateMazeBraidTilt ( CMaz *maze,
                               bool  fSection,
                               bool  fTiltDiamond,
                               int   nEntrancePos,
                               int   nRndBias,
                               int   nRndRun );
bool cpp_CreateMazeSpiral ( CMaz *maze,
                            int   cRandomAdd,
                            int   cSpiral,
                            int   cSpiralWall,
                            bool  fSection,
                            int   nEntrancePos,
                            int   nRndBias,
                            int   nRndRun );
bool cpp_CreateMazeDiagonal ( CMaz *maze,
                              int   cRandomAdd,
                              bool  fSection,
                              int   nEntrancePos,
                              int   nRndBias,
                              int   nRndRun );
bool cpp_CreateMazeRecursive ( CMaz *maze,
                               bool  fSection,
                               int   nEntrancePos,
                               int   nRndBias,
                               int   nRndRun );
bool cpp_CreateMazePrim ( CMaz *maze,
                          bool  fSection,
                          bool  fTreeWall,
                          int   nEntrancePos,
                          int   nRndBias,
                          int   nRndRun );
bool cpp_CreateMazePrim2 ( CMaz *maze,
                           bool  fSection,
                           bool  fTreeRandom,
                           bool  fTreeWall,
                           int   nEntrancePos,
                           int   nRndBias,
                           int   nRndRun );
bool cpp_CreateMazeKruskal ( CMaz *maze,
                             bool  fClear,
                             CCol *c2,
                             CCol *c3,
                             bool  fKruskalPic,
                             bool  fSection,
                             bool  fTreeWall,
                             int   nEntrancePos,
                             int   nRndBias,
                             int   nRndRun );
bool cpp_CreateMazeTree ( CMaz *maze,
                          bool  fSection,
                          bool  fTreeRandom,
                          bool  fTreeWall,
                          int   nEntrancePos,
                          int   nRndBias,
                          int   nRndRun,
                          int   nTreeRiver );
bool cpp_CreateMazeForest ( CMaz *maze,
                            bool  fWall,
                            bool  fRiverFlow,
                            bool  fSection,
                            bool  fTreeRandom,
                            bool  fTreeWall,
                            int   nEntrancePos,
                            int   nForsAdd,
                            int   nForsInit,
                            int   nRndBias,
                            int   nRndRun,
                            int   nTreeRiver );
bool cpp_CreateMazeAldousBroder ( CMaz *maze,
                                  bool  fSection,
                                  bool  fTreeWall,
                                  int   nEntrancePos,
                                  int   nRndBias,
                                  int   nRndRun );
bool cpp_CreateMazeWilson ( CMaz *maze,
                            bool  fSection,
                            bool  fTreeWall,
                            int   nEntrancePos,
                            int   nRndBias,
                            int   nRndRun );
bool cpp_CreateMazeEller ( CMaz *maze,
                           bool  fSection,
                           bool  fTreeWall,
                           int   nEntrancePos,
                           int   nRndBias,
                           int   nRndRun );
bool cpp_CreateMazeBraidEller ( CMaz *maze,
                                bool  fSection,
                                int   nEntrancePos,
                                int   nRndBias,
                                int   nRndRun );
bool cpp_CreateMazeDivision ( CMaz *maze,
                              bool  fSection,
                              int   nEntrancePos,
                              int   nRndBias,
                              int   nRndRun );
bool cpp_CreateMazeBinary ( CMaz *maze,
                            int   cRandomAdd,
                            bool  fSection,
                            bool  fTreeWall,
                            int   nEntrancePos,
                            int   nRndBias,
                            int   nRndRun );
bool cpp_CreateMazeSidewinder ( CMaz *maze,
                                bool  fSection,
                                bool  fTreeWall,
                                int   nEntrancePos,
                                int   nRndBias,
                                int   nRndRun );
bool cpp_CreateMazeUnicursal ( CMaz *maze,
                               int   cRandomAdd,
                               bool  fRiver,
                               bool  fRiverEdge,
                               bool  fRiverFlow,
                               bool  fSection,
                               bool  fTreeWall,
                               int   nEntrancePos,
                               int   nRndBias,
                               int   nRndRun );
bool cpp_Resize(CMaz *maze, unsigned width, unsigned height);
int wrapper_FFileSave(CMaz *maze,
                      int wCmd,
                      const char *filename,
                      KV kvOn,
                      KV kvOff,
                      bool fTextClip,
                      bool fLineChar,
                      bool fTextTab);
int cpp_SaveBitmap(CMaz *maze,
                   const char *filename,
                   KV kvOn,
                   KV kvOff);
int cpp_SaveText(CMaz *maze,
                 const char *filename,
                 bool fTextClip,
                 bool fLineChar,
                 bool fTextTab);

#endif /* _WRAPPER_H_ */
