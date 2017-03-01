#include "wrapper.h"

void
cpp_Init()
{
    // global initialisation
    for (int i = 0; i < cColorMain; i++)
    {
        int j = i*16 + 15;
        ws.rgkv[i] = GrayN(j);
    }
    // initialise random number generator
    InitRndL(time(0));
}

CMaz*
cpp_Constructor ( unsigned width,
                  unsigned height )
{
    CMaz *maze = new CMaz();
    if (maze)
    {
        maze->Init();
        if (!maze->FAllocate(width, height, NULL))
        {
            delete maze;
            maze = 0;
        }
    }
    return maze;
}

void
cpp_Destructor ( CMaz *maze )
{
    if (maze)
    {
        delete maze;
        maze = 0;
    }
}

bool
cpp_Get ( CMaz    *maze,
          unsigned x,
          unsigned y )
{
    if (!maze) return false;
    return maze->Get(x, y);
}

bool
cpp_CreateMazePerfect ( CMaz *maze,
                        bool  fRiver,
                        bool  fRiverEdge,
                        bool  fRiverFlow,
                        bool  fSection,
                        bool  fTreeWall,
                        int   nEntrancePos,
                        int   nRndBias,
                        int   nRndRun )
{
    if (!maze) return false;
    ms.fRiver       = fRiver;
    ms.fRiverEdge   = fRiverEdge;
    ms.fRiverFlow   = fRiverFlow;
    ms.fSection     = fSection;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazePerfect();
}

bool
cpp_CreateMazePerfect2 ( CMaz *maze,
                         bool  fRiver,
                         bool  fSection,
                         int   nEntrancePos,
                         int   nRndBias,
                         int   nRndRun )
{
    if (!maze) return false;
    ms.fRiver       = fRiver;
    ms.fSection     = fSection;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazePerfect2();
}

bool
cpp_CreateMazeBraid ( CMaz *maze,
                      bool  fSection,
                      int   nEntrancePos,
                      int   nRndBias,
                      int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeBraid();
}

bool
cpp_CreateMazeBraidTilt ( CMaz *maze,
                          bool  fSection,
                          bool  fTiltDiamond,
                          int   nEntrancePos,
                          int   nRndBias,
                          int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.fTiltDiamond = fTiltDiamond;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeBraidTilt();
}

bool
cpp_CreateMazeSpiral ( CMaz *maze,
                       int   cRandomAdd,
                       int   cSpiral,
                       int   cSpiralWall,
                       bool  fSection,
                       int   nEntrancePos,
                       int   nRndBias,
                       int   nRndRun )
{
    if (!maze) return false;
    ms.cRandomAdd   = cRandomAdd;
    ms.cSpiral      = cSpiral;
    ms.cSpiralWall  = cSpiralWall;
    ms.fSection     = fSection;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeSpiral();
}

bool
cpp_CreateMazeDiagonal ( CMaz *maze,
                         int   cRandomAdd,
                         bool  fSection,
                         int   nEntrancePos,
                         int   nRndBias,
                         int   nRndRun )
{
    if (!maze) return false;
    ms.cRandomAdd   = cRandomAdd;
    ms.fSection     = fSection;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeDiagonal();
}

bool
cpp_CreateMazeRecursive ( CMaz *maze,
                          bool  fSection,
                          int   nEntrancePos,
                          int   nRndBias,
                          int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeRecursive();
}

bool
cpp_CreateMazePrim ( CMaz *maze,
                     bool  fSection,
                     bool  fTreeWall,
                     int   nEntrancePos,
                     int   nRndBias,
                     int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazePrim();
}

bool
cpp_CreateMazePrim2 ( CMaz *maze,
                      bool  fSection,
                      bool  fTreeRandom,
                      bool  fTreeWall,
                      int   nEntrancePos,
                      int   nRndBias,
                      int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.fTreeRandom  = fTreeRandom;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazePrim2();
}

bool
cpp_CreateMazeKruskal ( CMaz *maze,
                        bool  fClear,
                        CCol *c2,
                        CCol *c3,
                        bool  fKruskalPic,
                        bool  fSection,
                        bool  fTreeWall,
                        int   nEntrancePos,
                        int   nRndBias,
                        int   nRndRun )
{
    if (!maze) return false;
    ms.fKruskalPic  = fKruskalPic;
    ms.fSection     = fSection;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeKruskal(fClear, c2, c3);
}

bool
cpp_CreateMazeTree ( CMaz *maze,
                     bool  fSection,
                     bool  fTreeRandom,
                     bool  fTreeWall,
                     int   nEntrancePos,
                     int   nRndBias,
                     int   nRndRun,
                     int   nTreeRiver )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.fTreeRandom  = fTreeRandom;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    ms.nTreeRiver   = nTreeRiver;
    return maze->CreateMazeTree();
}

bool
cpp_CreateMazeForest ( CMaz *maze,
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
                       int   nTreeRiver )
{
    if (!maze) return false;
    ms.fRiverFlow   = fRiverFlow;
    ms.fSection     = fSection;
    ms.fTreeRandom  = fTreeRandom;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    ms.nForsAdd     = nForsAdd;
    ms.nForsInit    = nForsInit;
    ms.nTreeRiver   = nTreeRiver;
    return maze->CreateMazeForest(fWall);
}

bool
cpp_CreateMazeAldousBroder ( CMaz *maze,
                             bool  fSection,
                             bool  fTreeWall,
                             int   nEntrancePos,
                             int   nRndBias,
                             int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeAldousBroder();
}

bool
cpp_CreateMazeWilson ( CMaz *maze,
                       bool  fSection,
                       bool  fTreeWall,
                       int   nEntrancePos,
                       int   nRndBias,
                       int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeWilson();
}

bool
cpp_CreateMazeEller ( CMaz *maze,
                      bool  fSection,
                      bool  fTreeWall,
                      int   nEntrancePos,
                      int   nRndBias,
                      int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeEller();
}

bool
cpp_CreateMazeBraidEller ( CMaz *maze,
                           bool  fSection,
                           int   nEntrancePos,
                           int   nRndBias,
                           int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeBraidEller();
}

bool
cpp_CreateMazeDivision ( CMaz *maze,
                         bool  fSection,
                         int   nEntrancePos,
                         int   nRndBias,
                         int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeDivision();
}

bool
cpp_CreateMazeBinary ( CMaz *maze,
                       int   cRandomAdd,
                       bool  fSection,
                       bool  fTreeWall,
                       int   nEntrancePos,
                       int   nRndBias,
                       int   nRndRun )
{
    if (!maze) return false;
    ms.cRandomAdd   = cRandomAdd;
    ms.fSection     = fSection;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeBinary();
}

bool
cpp_CreateMazeSidewinder ( CMaz *maze,
                           bool  fSection,
                           bool  fTreeWall,
                           int   nEntrancePos,
                           int   nRndBias,
                           int   nRndRun )
{
    if (!maze) return false;
    ms.fSection     = fSection;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeSidewinder();
}

bool
cpp_CreateMazeUnicursal ( CMaz *maze,
                          int   cRandomAdd,
                          bool  fRiver,
                          bool  fRiverEdge,
                          bool  fRiverFlow,
                          bool  fSection,
                          bool  fTreeWall,
                          int   nEntrancePos,
                          int   nRndBias,
                          int   nRndRun )
{
    if (!maze) return false;
    ms.cRandomAdd   = cRandomAdd;
    ms.fRiver       = fRiver;
    ms.fRiverEdge   = fRiverEdge;
    ms.fRiverFlow   = fRiverFlow;
    ms.fSection     = fSection;
    ms.fTreeWall    = fTreeWall;
    ms.nEntrancePos = nEntrancePos;
    ms.nRndBias     = nRndBias;
    ms.nRndRun      = nRndRun;
    return maze->CreateMazeUnicursal();
}

bool
cpp_Resize ( CMaz    *maze,
             unsigned width,
             unsigned height )
{
    if (!maze) return false;
    return maze->FBitmapResizeTo(width, height);
}

int
wrapper_FFileSave ( CMaz       *maze,
                    int         wCmd,
                    const char *filename,
                    KV          kvOn,
                    KV          kvOff,
                    bool        fTextClip,
                    bool        fLineChar,
                    bool        fTextTab )
{
    if (filename == NULL)
        return SAVE_NO_FILENAME_GIVEN_ERROR;

    // FWriteFile(bm.b, bm.k, wCmd, filename, filename)

    FILE *file;
    flag fBinary;
    size_t cursorPrev = NULL;

    // Open the file for writing given its name.
    fBinary = (wCmd == cmdSaveBitmap ||
               wCmd == cmdSaveColmapTarga ||
               wCmd == cmdSavePicture);
    file = FileOpen(filename, fBinary ? "wb" : "w");
    if (file == NULL) {
        return SAVE_FILE_OPEN_ERROR;
    }

    // Go save the contents of the file based on the specified type.
    switch (wCmd) {
    case cmdSaveBitmap:
        maze->WriteBitmap(file, dr.kvOff, dr.kvOn);
        break;
    case cmdSaveText:
        maze->WriteText(file, fTextClip, fLineChar, fTextTab);
        break;
    case cmdSaveDOS:
        if (!fLineChar)
            maze->WriteTextDOS(file, 0, fTextClip);
        else
            maze->WriteText2(file, fTextClip);
        break;
    case cmdSaveDOS2: maze->WriteTextDOS(file, 1, fTextClip); break;
    case cmdSaveDOS3: maze->WriteTextDOS(file, 2, fTextClip); break;
    case cmdSaveXbmN: maze->WriteXbm(file, filename, 'N'); break;
    case cmdSaveXbmC: maze->WriteXbm(file, filename, 'C'); break;
    case cmdSaveXbmS: maze->WriteXbm(file, filename, 'S'); break;
    case cmdSave3DN: maze->WriteCube(file, 1, fTextClip); break;
    case cmdSave3DC: maze->WriteCube(file, 2, fTextClip); break;
    case cmdSave3DS: maze->WriteCube(file, 3, fTextClip); break;
    case cmdSaveDB:
        maze->WriteDaedalusBitmap(file, fTextClip);
        break;
    default:
        fclose(file);
        return SAVE_UNKNOWN_CMD_ERROR;
    }
    fclose(file);
    return SAVE_SUCCESS;
}

int
cpp_SaveBitmap ( CMaz       *maze,
                 const char *filename,
                 KV          kvOn,
                 KV          kvOff )
{
    if (!maze) return SAVE_NO_MAZE_ERROR;
    return wrapper_FFileSave(maze, cmdSaveBitmap, filename, kvOn, kvOff, true, false, false);
}

int
cpp_SaveText ( CMaz       *maze,
               const char *filename,
               bool        fTextClip,
               bool        fLineChar,
               bool        fTextTab )
{
    if (!maze) return SAVE_NO_MAZE_ERROR;
    return wrapper_FFileSave(maze, cmdSaveText, filename, kvWhite, kvBlack, fTextClip, fLineChar, fTextTab);
}
