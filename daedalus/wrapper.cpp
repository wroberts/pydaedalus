#include "wrapper.h"

CMaz *cpp_Constructor(unsigned width, unsigned height)
{
    CMaz *maze = new CMaz();
    if (maze)
    {
        if (!maze->FAllocate(width, height, NULL))
        {
            maze->Destroy();
            maze = 0;
        }
    }
    return maze;
}

void cpp_Destructor(CMaz *maze)
{
    if (maze)
        delete maze;
}

bool cpp_Get(CMaz *maze, unsigned x, unsigned y)
{
    if (!maze) return false;
    return maze->Get(x, y);
}

bool cpp_CreateMazePerfect(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.fTreeWall
    // ms.fRiver
    // ms.fRiverEdge
    // ms.fRiverFlow
    if (!maze) return false;
    return maze->CreateMazePerfect();
}

bool cpp_CreateMazePerfect2(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.fRiver
    if (!maze) return false;
    return maze->CreateMazePerfect2();
}

bool cpp_CreateMazeBraid(CMaz *maze)
{
    // ms.nEntrancePos
    if (!maze) return false;
    return maze->CreateMazeBraid();
}

bool cpp_CreateMazeBraidTilt(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.fTiltDiamond
    if (!maze) return false;
    return maze->CreateMazeBraidTilt();
}

bool cpp_CreateMazeSpiral(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.iSpiralIndex
    // ms.cSpiral
    // ms.cSpiralWall
    // ms.cRandomAdd
    if (!maze) return false;
    return maze->CreateMazeSpiral();
}

bool cpp_CreateMazeDiagonal(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.cRandomAdd
    if (!maze) return false;
    return maze->CreateMazeDiagonal();
}

bool cpp_CreateMazeRecursive(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.nRndRun
    // ms.nRndBias
    if (!maze) return false;
    return maze->CreateMazeRecursive();
}

bool cpp_CreateMazePrim(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.fTreeWall
    if (!maze) return false;
    return maze->CreateMazePrim();
}

bool cpp_CreateMazePrim2(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.fTreeWall
    // ms.fTreeRandom
    if (!maze) return false;
    return maze->CreateMazePrim2();
}

bool cpp_CreateMazeKruskal(CMaz *maze, bool fClear, CCol *c2, CCol *c3)
{
    // ms.nEntrancePos
    // ms.fTreeWall
    // ms.fKruskalPic
    if (!maze) return false;
    return maze->CreateMazeKruskal(fClear, c2, c3);
}

bool cpp_CreateMazeTree(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.fTreeWall
    // ms.fTreeRandom
    // ms.nTreeRiver
    if (!maze) return false;
    return maze->CreateMazeTree();
}

bool cpp_CreateMazeForest(CMaz *maze, bool fWall)
{
    // ms.nEntrancePos
    // ms.fTreeWall
    // ms.nForsInit
    // ms.nForsAdd
    // ms.fRiverFlow
    // ms.fTreeRandom
    // ms.nTreeRiver
    if (!maze) return false;
    return maze->CreateMazeForest(fWall);
}

bool cpp_CreateMazeAldousBroder(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.fTreeWall
    if (!maze) return false;
    return maze->CreateMazeAldousBroder();
}

bool cpp_CreateMazeWilson(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.fTreeWall
    if (!maze) return false;
    return maze->CreateMazeWilson();
}

bool cpp_CreateMazeEller(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.fTreeWall
    if (!maze) return false;
    return maze->CreateMazeEller();
}

bool cpp_CreateMazeBraidEller(CMaz *maze)
{
    // ms.nEntrancePos
    if (!maze) return false;
    return maze->CreateMazeBraidEller();
}

bool cpp_CreateMazeDivision(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.nRndRun
    // ms.nRndBias
    if (!maze) return false;
    return maze->CreateMazeDivision();
}

bool cpp_CreateMazeBinary(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.nRndBias
    // ms.cRandomAdd
    // ms.nRndRun
    // ms.fTreeWall
    if (!maze) return false;
    return maze->CreateMazeBinary();
}

bool cpp_CreateMazeSidewinder(CMaz *maze)
{
    // ms.nEntrancePos
    // ms.nRndBias
    // ms.fTreeWall
    if (!maze) return false;
    return maze->CreateMazeSidewinder();
}

bool cpp_Resize(CMaz *maze, unsigned width, unsigned height)
{
    if (!maze) return false;
    return maze->FBitmapResizeTo(width, height);
}

int wrapper_FFileSave(CMaz *maze,
                      int wCmd,
                      const char *filename,
                      KV kvOn,
                      KV kvOff,
                      bool fTextClip,
                      bool fLineChar,
                      bool fTextTab)
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

int cpp_SaveBitmap(CMaz *maze,
                   const char *filename,
                   KV kvOn,
                   KV kvOff)
{
    if (!maze) return SAVE_NO_MAZE_ERROR;
    return wrapper_FFileSave(maze, cmdSaveBitmap, filename, kvOn, kvOff, true, false, false);
}

int cpp_SaveText(CMaz *maze,
                 const char *filename,
                 bool fTextClip,
                 bool fLineChar,
                 bool fTextTab)
{
    if (!maze) return SAVE_NO_MAZE_ERROR;
    return wrapper_FFileSave(maze, cmdSaveText, filename, kvWhite, kvBlack, fTextClip, fLineChar, fTextTab);
}
