#include "wrapper.h"

CMaz *cpp_Constructor(unsigned width, unsigned height)
{
    CMon inst;
    CMaz *maze = new CMaz(); //inst.Create();
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
    //maze->Destroy();
    if (maze)
        delete maze;
}

bool cpp_CreateMazePerfect(CMaz *maze)
{
    if (!maze) return false;
    maze->CreateMazePerfect();
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

int cpp_SaveBitmap(CMaz *maze, const char *filename)
{
    if (!maze) return SAVE_NO_MAZE_ERROR;
    return wrapper_FFileSave(maze, cmdSaveBitmap, filename, kvWhite, kvBlack, true, false, false);
}

int cpp_SaveText(CMaz *maze, const char *filename)
{
    if (!maze) return SAVE_NO_MAZE_ERROR;
    return wrapper_FFileSave(maze, cmdSaveText, filename, kvWhite, kvBlack, true, false, false);
}
