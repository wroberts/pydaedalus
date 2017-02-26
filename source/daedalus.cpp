/*
** Daedalus (Version 3.2) File: daedalus.cpp
** By Walter D. Pullen, Astara@msn.com, http://www.astrolog.org/labyrnth.htm
**
** IMPORTANT NOTICE: Daedalus and all Maze generation and general
** graphics routines used in this program are Copyright (C) 1998-2016 by
** Walter D. Pullen. Permission is granted to freely use, modify, and
** distribute these routines provided these credits and notices remain
** unmodified with any altered or distributed versions of the program.
** The user does have all rights to Mazes and other graphic output
** they make in Daedalus, like a novel created in a word processor.
**
** More formally: This program is free software; you can redistribute it
** and/or modify it under the terms of the GNU General Public License as
** published by the Free Software Foundation; either version 2 of the
** License, or (at your option) any later version. This program is
** distributed in the hope that it will be useful and inspiring, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** General Public License for more details, a copy of which is in the
** LICENSE.HTM included with Daedalus, and at http://www.gnu.org
**
** This file contains Daedalus specific routines not related to the
** underlying operating system.
**
** Created: 11/18/1993.
** Last code change: 8/31/2016.
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <math.h>

#include "resource.h"
#include "util.h"
#include "graphics.h"
#include "color.h"
#include "threed.h"
#include "maze.h"
#include "draw.h"
#include "daedalus.h"


// Global variables

WS ws = {
  // Window settings
  fFalse, 0, 0, 0, 0, 0,
  // File dialog settings
  0, 0, fTrue, fFalse, fFalse,
  // Event dialog settings
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  // Redraw dialog settings
  fTrue, fFalse, fTrue, fTrue, fFalse, fFalse, fFalse, 0, 50, fTrue,
  // Menu settings
  fFalse, fFalse, fFalse, fFalse,
  // Macro accessible only settings
  -1, fFalse, fFalse, 1000,
  // Internal settings
  NULL, NULL,
    fFalse, fTrue, fFalse, fFalse, fFalse, fFalse, fFalse, fFalse, fFalse,
    0, 0, cmdCreatePerfect, cmdCreatePerfect, 0, fFalse, fFalse,
    NULL, NULL, NULL, NULL, 0, 0, 0, 0, 12, 0, NULL, 0, 0, 0, 0, fFalse,
    NULL, NULL, 0, NULL, 0, NULL, NULL, 0, NULL, 0, NULL, 0,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

BM bm = {xStart, yStart, 0, 0, 0, 0, 0, 1, 1, 1,
  NULL, 0L, NULL, 0L,
  fFalse, fFalse, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

DR dr = {
  // Dot dialog settings - Location
  0, 0, 0, 2, 0, 0, 0, 2,
  // Dot dialog settings - Motion
  fFalse, fFalse, fTrue, 0, fFalse, fTrue, fTrue, fFalse, fFalse, fFalse, 0L,
  // Dot dialog settings - Editing
  fFalse, fFalse, fTrue, fFalse, fFalse, fFalse, nEdgeVoid,
  // Inside dialog Settings - Mode & What To Draw
  nInsideSmooth,
  fTrue, fFalse, fFalse, fFalse, fFalse, fTrue, 20, 8, fTrue, fFalse, 500, 10,
  // Inside dialog Settings - Walls
  fTrue, 160, 20, 100, 0, -1, fTrue, 0.4, 0, 100, 55.0, nTransDefault,
  // Inside dialog Settings - Smooth & Free Movement
  15, 15, 15, 20, 10, 8, 0, 0, 0, 0, fTrue,
  // Color dialog settings
  kvWhite, kvBlack, kvGray, kvRed, kvDkBlue, kvDkGreen, kvCyan, kvGray,
    kvBlack, kvMaize, kvLtGray, kvBrown, kvWhite, kvBlack, fFalse,
  // Menu settings
  fFalse, fFalse, fFalse,
  // Macro accessible only settings
  fFalse, fFalse, fFalse, kvGreen, kvYellow, Rgb(255, 255, 223),
    -1, -1, -1, -1, -1, -1, fFalse, 94001225, 0, 0, 50, 333, 10990099, fFalse,
    fFalse, fFalse, -1, fFalse, fFalse, fFalse, fFalse, fFalse, fFalse,
    fFalse, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1,
    fFalse, fFalse, 11, 3412, 4, 427, 0, 0,
  // Internal settings
  fFalse, 0.0, NULL, 0, NULL, NULL, NULL, NULL, 50, 40, 99999};

// Constant data

CONST int rgdirDiag[DIRS][DIRS] =
  {{0, 1, 1, 0}, {1, 1, 2, 2}, {3, 2, 2, 3}, {0, 0, 3, 3}};
CONST int rgcmdMouse[11] = {cmdMove7, cmdMove8, cmdMove9,
  cmdMove4, 0, cmdMove6, cmdMove1, cmdMove2, cmdMove3,
  cmdMoveU, cmdMoveD};
CONST PT rgptSize[cmdSizeLast - cmdSize01 + 1] =
  {{32, 16}, {32, 32}, {64, 32}, {64, 48}, {64, 64},
  {128, 64}, {128, 96}, {128, 128}, {128, 192}, {192, 192},
  {256, 192}, {256, 384}, {512, 384}, {512, 768}, {1024, 384},
  {1024, 768}, {1024, 1024}, {1024, 2048}, {2048, 4096}};

// Script names

CONST char *rgszScript[cmdScriptLast - cmdScript01 + 1] = {
  "demos.ds",    "wordmaze.ds", "gigamaze.ds", "maze4d.ds",   "maze5d.ds",
  "dragon.ds",   "pacman.ds",   "sokoban.ds",  "hunger.ds",
  "survmaz0.ds", "survmaz1.ds", "survmaz2.ds", "survmaz3.ds", "survmaz4.ds",
  "survmaz5.ds", "survmaz6.ds", "survmaz7.ds", "survmaz8.ds", "survmaz9.ds",
  "carletn1.ds", "carletn2.ds", "stocker.ds",  "glacier.ds",  "safari.ds",
  "mousemaz.ds", "squared.ds",  "mandy.ds",    "pentris.ds"};
CONST char *rgszShortcut[cmdScriptLast - cmdScript01 + 1] = {
  "Daedalus demos",        "Word Mazes",            "World's largest Maze",
  "4D Mazes",              "5D Mazes",              "Dragonslayer",
  "Pac-Man game",          "Sokoban game",          "The Hunger Games",
  "Survivor Maze game #10",
  "Survivor Maze game #1", "Survivor Maze game #2", "Survivor Maze game #3",
  "Survivor Maze game #4", "Survivor Maze game #5", "Survivor Maze game #6",
  "Survivor Maze game #7", "Survivor Maze game #8", "Survivor Maze game #9",
  "Carleton Farm Maze #1", "Carleton Farm Maze #2", "Stocker Farms Maze",
  "Glacier Maze game",     "Safari Maze",           "Mouse Maze game",
  "Survivor Squares game", "Mandelbrot set",        "Pentris"};

// Command lines to run on startup. When embedding a complex script file, in
// it first delete the leading DS, then replace \ with \\, " with \", and ?
// with \?, to result in valid C++ strings.

CONST char *rgszStartup[] = {
  NULL}; // Must end with NULL


/*
******************************************************************************
** Function Hooks
******************************************************************************
*/

#ifdef ASSERT
// Assert a condition. If not, display an error message.

void AssertCore(flag f)
{
  if (!f && gs.fErrorCheck) {
    PrintSz_E("Assert failed!\n");

    // Turn off error checking to avoid future assert messages.
    gs.fErrorCheck = fFalse;
  }
}
#endif


// Read an 8 bit byte from a file.

byte BRead(FILE *file)
{
  CONST char *sz;
  char ch;

  if (file != NULL)
    return getc(file);

  // If no file, then read from the startup script string array.
  sz = ws.rgszStartup[ws.iszStartup];
  if (sz == NULL)
    return 0;
  ch = sz[ws.ichStartup];
  if (ch == 0) {
    ws.iszStartup++;
    ws.ichStartup = 0;
    return '\n';
  }
  ws.ichStartup++;
  return ch;
}


// Initialize a new wireframe list in memory of the specified size.

flag FInitCoordinates(int cCoor)
{
  if (bm.coor != NULL)
    DeallocateP(bm.coor);
  bm.coor = RgAllocate(cCoor, COOR);
  bm.ccoor = cCoor;
  ds.cCoorPatch = 0;
  return bm.coor != NULL;
}


// Append a line to the wireframe list in memory.

flag FSetCoordinates(int x1, int y1, int z1, int x2, int y2, int z2)
{
  WriteCoordinates(bm.coor, x1, y1, z1, x2, y2, z2);
  return fTrue;
}


// Initialize a new patch list in memory of the specified size.

flag FInitPatch(int cPat)
{
  if (bm.patch != NULL)
    DeallocateP(bm.patch);
  bm.patch = RgAllocate(cPat, PATCH);
  bm.cpatch = cPat;
  ds.cCoorPatch = 0;
  return bm.patch != NULL;
}


// Append a quadrilateral polygon to the patch list in memory.

flag FSetPatch(int x1, int y1, int z1, int x2, int y2, int z2, KV kv)
{
  PATN pat[cPatch];

  pat[0].x = x1; pat[0].y = y1; pat[0].z = z1; pat[0].fLine = fTrue;
  pat[1].x = x1; pat[1].y = y1; pat[1].z = z2; pat[1].fLine = fTrue;
  pat[2].x = x2; pat[2].y = y2; pat[2].z = z2; pat[2].fLine = fTrue;
  pat[3].x = x2; pat[3].y = y2; pat[3].z = z1; pat[3].fLine = fTrue;
  WritePatch(bm.patch, pat, fFalse, kv < 0 ? ds.kvObject : kv);
  return fTrue;
}


/*
******************************************************************************
** Daedalus Routines
******************************************************************************
*/

// Read a monochrome or color Windows bitmap from a file.

flag FReadBitmap(FILE *file, flag fNoHeader)
{
  int x, y, z, k;
  char sz[cchSzMax];

  if (!FReadBitmapHeader(file, fNoHeader, &x, &y, &z, &k))
    return fFalse;
  if (z != 1 && z != 4 && z != 8 && z != 16 && z != 24 && z != 32) {
    sprintf(S(sz), "This Windows bitmap has %d bits per pixel.\n"
      "Bitmaps must have 1, 4, 8, 16, 24, or 32 bits.\n", z);
    PrintSz_W(sz);
    return fFalse;
  }
  if (z == 1) {
    if (bm.b.FReadBitmapCore(file, x, y))
      FShowColmap(fFalse);
  } else {
    if (bm.k.FReadColmapCore(file, x, y, z, k))
      FShowColmap(fTrue);
  }
  return fTrue;
}


// Read a monochrome or color Daedalus bitmap from a file.

flag FReadDaedalusBitmap(FILE *file)
{
  int x, y, z;
  char ch, ch2;

  ch = getbyte(); ch2 = getbyte();
  if (ch != 'D' || ch2 != 'B') {
    PrintSz_W("This file does not look like a Daedalus bitmap.\n");
    return fFalse;
  }
  if (file != NULL)
    fscanf(file, "%d%d%d", &x, &y, &z);
  else {
    ws.iszStartup++;
    sscanf(ws.rgszStartup[ws.iszStartup], "%d%d%d", &x, &y, &z);
  }
  if (z != 1 && z != 24) {
    PrintSz_W(
      "This Daedalus bitmap is neither monochrome nor 24 bit color.\n");
    return fFalse;
  }
  if (file != NULL) {
    skipcrlf();
  } else {
    ws.iszStartup++;
  }
  if (z == 1) {
    if (bm.b.FReadDaedalusBitmapCore(file, x, y))
      FShowColmap(fFalse);
  } else {
    if (bm.k.FReadDaedalusBitmapCore(file, x, y))
      FShowColmap(fTrue);
  }
  return fTrue;
}


// Read a file from an open file handle.

flag FReadFile(int wCmd, FILE *file, flag fCloseAfter)
{
  size_t cursorPrev = NULL;
  char ch1, ch2;
  long l;

  // For generic open, peek at the first two characters of the file's contents
  // to determine what type it is.
  if (wCmd == cmdOpen) {
    ch1 = getbyte(); ch2 = getbyte();
    if (ch1 == 'B' && ch2 == 'M')
      wCmd = cmdOpenBitmap;
    else if (ch1 == '#' && ch2 == 'd')
      wCmd = cmdOpenXbm;
    else if (ch1 == 'D' && ch2 == '3')
      wCmd = cmdOpen3D;
    else if (ch1 == 'D' && ch2 == 'B')
      wCmd = cmdOpenDB;
    else if (ch1 == 'D' && ch2 == 'S')
      wCmd = cmdOpenScript;
    else if (ch1 == 'D' && ch2 == 'W')
      wCmd = cmdOpenWire;
    else if (ch1 == 'D' && ch2 == 'P')
      wCmd = cmdOpenPatch;
    else if (ch1 >= ' ' && ch2 >= ' ')
      wCmd = cmdOpenText;
    else if (ch1 == 10 && ch2 <= 5)
      wCmd = cmdOpenColmapPaint;
    else
      wCmd = cmdOpenColmapTarga;
    fseek(file, 0, SEEK_SET);
  }

  // Go load the contents of the file based on its type.
  HourglassCursor(&cursorPrev, fTrue);
  switch (wCmd) {
  case cmdOpenBitmap: FReadBitmap(file, fFalse); break;
  case cmdOpenText:   bm.b.FReadText(file);      break;
  case cmdOpenXbm:    bm.b.FReadXbm(file);       break;
  case cmdOpen3D:
    if (!dr.f3D) {
      dr.f3D = fTrue;
      SystemHook(hos3DOn);
    }
    bm.b.FReadCube(file, bm.b.m_w3);
    break;
  case cmdOpenDB:     FReadDaedalusBitmap(file); break;
  case cmdOpenScript: FReadScript(file);         break;
  case cmdOpenColmapTarga: bm.k.FReadColmapTarga(file); break;
  case cmdOpenColmapPaint: bm.k.FReadColmapPaint(file); break;
  case cmdOpenWire:
    l = ReadWirelist(&bm.coor, file);
    if (l >= 0)
      bm.ccoor = l;
    break;
  case cmdOpenPatch:
    l = ReadPatchlist(&bm.patch, file);
    if (l >= 0)
      bm.cpatch = l;
    break;
  }
  HourglassCursor(&cursorPrev, fFalse);

  // Close the file if appropriate.
  if (fCloseAfter)
    fclose(file);

  // For some file types, display information about what was just loaded.
  switch (wCmd) {
  case cmdOpenWire:
    ds.fDidPatch = fFalse;
    if (bm.coor != NULL)
      PrintSzL("Total number of lines read: %ld\n", bm.ccoor);
    break;
  case cmdOpenPatch:
    ds.fDidPatch = fTrue;
    if (bm.patch != NULL)
      PrintSzL("Total number of patches read: %ld\n", bm.cpatch);
    break;
  }
  return fTrue;
}


// Save a bitmap or a wireframe or patch list to a file.

flag FWriteFile(CONST CMaz &b, CONST CMazK &c, int wCmd, CONST char *sz,
  CONST char *szTitle)
{
  FILE *file;
  flag fBinary;
  size_t cursorPrev = NULL;

  // Clipboard copy and wallpaper changing save files during their execution.
  if (wCmd == cmdCopyText)
    wCmd = cmdSaveText;
  else if (wCmd == cmdCopyBitmap || wCmd == cmdSaveWallCenter ||
    wCmd == cmdSaveWallTile || wCmd == cmdSaveWallStretch ||
    wCmd == cmdSaveWallFit || wCmd == cmdSaveWallFill)
    wCmd = cmdSaveBitmap;
  else if (wCmd == cmdCopyPicture)
    wCmd = cmdSavePicture;

  // Open the file for writing given its name.
  fBinary = (wCmd == cmdSaveBitmap || wCmd == cmdSaveColmapTarga ||
    wCmd == cmdSavePicture);
  file = FileOpen(sz, fBinary ? "wb" : "w");
  if (file == NULL) {
    ws.szTitle = szTitle;
    PrintSz_E("The file could not be created.");
    return fFalse;
  }

  // Go save the contents of the file based on the specified type.
  HourglassCursor(&cursorPrev, fTrue);
  switch (wCmd) {
  case cmdSaveBitmap:
    if (!bm.fColor)
      b.WriteBitmap(file, dr.kvOff, dr.kvOn);
    else
      c.WriteColmap(file);
    break;
  case cmdSaveText:
    if (!dr.f3D) {
      if (!bm.fColor)
        b.WriteText(file, ws.fTextClip, ws.fLineChar, ws.fTextTab);
      else
        c.WriteTextColmap(file, ws.fTextClip);
    } else
      b.WriteText3D(file, ws.fLineChar, ws.fTextTab);
    break;
  case cmdSaveDOS:
    if (!ws.fLineChar)
      b.WriteTextDOS(file, 0, ws.fTextClip);
    else
      b.WriteText2(file, ws.fTextClip);
    break;
  case cmdSaveDOS2: b.WriteTextDOS(file, 1, ws.fTextClip); break;
  case cmdSaveDOS3: b.WriteTextDOS(file, 2, ws.fTextClip); break;
  case cmdSaveXbmN: b.WriteXbm(file, sz, 'N'); break;
  case cmdSaveXbmC: b.WriteXbm(file, sz, 'C'); break;
  case cmdSaveXbmS: b.WriteXbm(file, sz, 'S'); break;
  case cmdSaveColmapTarga: c.WriteColmapTarga(file); break;
  case cmdSave3DN: b.WriteCube(file, 1, ws.fTextClip); break;
  case cmdSave3DC: b.WriteCube(file, 2, ws.fTextClip); break;
  case cmdSave3DS: b.WriteCube(file, 3, ws.fTextClip); break;
  case cmdSaveDB:
    if (!bm.fColor)
      b.WriteDaedalusBitmap(file, ws.fTextClip);
    else
      c.WriteDaedalusBitmap(file, ws.fTextClip);
    break;
  case cmdSaveWire:
    WriteWireframe(file, bm.coor, bm.ccoor);
    break;
  case cmdSavePatch:
    WritePatches(file, bm.patch, bm.cpatch);
    break;
  case cmdSavePicture:
    WriteWireframeMetafile(file, bm.coor, bm.ccoor);
    break;
  }
  HourglassCursor(&cursorPrev, fFalse);

  fclose(file);
  return fTrue;
}


// Change the active bitmap. Switch to showing either the main bitmap or the
// color bitmap.

flag FShowColmap(flag fColor)
{
  flag fDirty;

  if (fColor && bm.k.FNull()) {
    if (!bm.k.FColmapGetFromBitmap(bm.b, dr.kvOff, dr.kvOn))
      return fFalse;
  }
  fDirty = (fColor != bm.fColor);
  bm.fColor = fColor;
  DoSetVariableW(vosShowColmap, fColor);
  if (fDirty)
    SystemHook(hosDirtyView);
  return fTrue;
}


// Resize the active bitmap. This implements the action of the Size dialog.

void DoSize(int x, int y, flag fShift, flag fClear)
{
  CMap *b = PbFocus();

  if (!fShift) {
    if (x < 0)
      x = NAbs(b->m_x + x);
    if (y < 0)
      y = NAbs(b->m_y + y);
  }
  if (fClear) {
    if (fShift) {
      x += b->m_x; y += b->m_y;
    }
    if (b->FBitmapSizeSet(x, y))
      b->BitmapOff();
  } else {
    if (!fShift)
       b->FBitmapResizeTo(x, y);
    else
       b->FBitmapShiftBy(x, y);
  }
  if (!bm.fColor)
    bm.b.SetXyh();
}


// Set a macro to the given string.

flag DoDefineMacro(int imacro, CONST char *szMacro, int cchMacro,
  CONST char *szMenu)
{
  char sz[cchSzMax];

  if (!FEnsureMacro(imacro + 1))
    return fFalse;

  // Get rid of the old macro contents.
  if (ws.rgszMacro[imacro] != NULL)
    DeallocateP(ws.rgszMacro[imacro]);

  // Set the new macro contents.
  if (szMacro[0] != chNull) {
    ws.rgszMacro[imacro] = RgAllocate(cchMacro + 1, char);
    if (ws.rgszMacro[imacro] != NULL)
      CopyRgchToSz(szMacro, cchMacro, ws.rgszMacro[imacro], cchMacro + 1);
  } else
    ws.rgszMacro[imacro] = NULL;

  // For macros 1 through 48, if given a menu string, change the menu command
  // text for that macro to the string.
  if (FBetween(imacro, 1, cMacro) && szMenu[0] != chNull) {
    sprintf(S(sz), "%s\t%sF%d", szMenu, imacro <= 12 ? "" : (imacro <= 24 ?
      "Shift+" : (imacro <= 36 ? "Ctrl+" : "Alt+")), (imacro - 1) % 12 + 1);
    DoOperationW(oosMenu, sz, 0, imacro, ~0);
  }
  return fTrue;
}


// Ensure there are at least the given number of slots available in the macro
// array, reallocating if needed.

flag FEnsureMacro(int cszNew)
{
  char **ppchT;

  if (cszNew <= ws.cszMacro)
    return fTrue;

  if (cszNew <= cMacro)
    cszNew = cMacro + 1;
  ppchT = (char **)ReallocateArray(ws.rgszMacro, ws.cszMacro,
    sizeof(char *), cszNew);
  if (ppchT == NULL)
    return fFalse;
  if (ws.rgszMacro != NULL)
    DeallocateP(ws.rgszMacro);
  ws.rgszMacro = ppchT;
  ws.cszMacro = cszNew;
  return fTrue;
}


// Ensure there are at least the given number of slots available in the custom
// variable array, reallocating if needed.

flag FEnsureLVar(int clNew)
{
  long *plT;

  if (clNew <= ws.clVar)
    return fTrue;

  if (clNew <= cLetter)
    clNew = cLetter + 1;
  plT = (long *)ReallocateArray(ws.rglVar, ws.clVar, sizeof(long), clNew);
  if (plT == NULL)
    return fFalse;
  if (ws.rglVar != NULL)
    DeallocateP(ws.rglVar);
  ws.rglVar = plT;
  ws.clVar = clNew;
  return fTrue;
}


// Ensure there are at least the given number of slots available in the custom
// string array, reallocating if needed.

flag FEnsureSzVar(int cszNew)
{
  char **ppchT;

  if (cszNew <= ws.cszVar)
    return fTrue;

  ppchT = (char **)ReallocateArray(ws.rgszVar, ws.cszVar, sizeof(char *),
    cszNew);
  if (ppchT == NULL)
    return fFalse;
  if (ws.rgszVar != NULL)
    DeallocateP(ws.rgszVar);
  ws.rgszVar = ppchT;
  ws.cszVar = cszNew;
  return fTrue;
}


// Set the given slot in the custom string array to the specified string.

flag SetSzVar(CONST char *sz, int cch, int isz)
{
  char *pch;

  if (!FEnsureSzVar(isz + 1))
    return fFalse;

  if (cch < 0)
    cch = CchSz(sz);
  if (cch > 0) {
    pch = RgAllocate(cch + 1, char);
    if (pch == NULL)
      return fFalse;
    CopyRgchToSz(sz, cch, pch, cch + 1);
  } else
    pch = NULL;

  if (ws.rgszVar[isz] != NULL)
    DeallocateP(ws.rgszVar[isz]);
  ws.rgszVar[isz] = pch;
  return fTrue;
}


// Return a pointer to the specified custom monochrome bitmap, as used for
// texture masks. This will point to NULL if it hasn't been allocated yet.

CMaz *BitmapGetMask(int ib)
{
  CMaz *pbT;
  int iT;

  // Special values allow referencing the standard bitmaps.
  if (ib < -3)
    return NULL;
  switch (ib) {
  case -1: return &bm.b;   // Bitmap -1 is the main bitmap
  case -2: return &bm.b2;  // Bitmap -2 is the temporary bitmap
  case -3: return &bm.b3;  // Bitmap -3 is the extra bitmap
  }

  // Ensure there are at least the given number of slots available in the
  // monochrome bitmap array, reallocating if needed.
  if (ib >= ws.cbMask) {
    pbT = (CMaz *)ReallocateArray(ws.rgbMask, ws.cbMask,
      sizeof(CMaz), ib+1);
    if (pbT == NULL)
      return NULL;
    for (iT = ws.cbMask; iT <= ib; iT++)
      new (&pbT[iT]) CMaz();
    if (ws.rgbMask != NULL)
      DeallocateP(ws.rgbMask);
    ws.rgbMask = pbT;
    ws.cbMask = ib+1;
  }
  return &ws.rgbMask[ib];
}


// Return a pointer to the specified custom color bitmap, as used for
// textures. This will point to NULL if it hasn't been allocated yet.

CMazK *ColmapGetTexture(int ic)
{
  CMazK *pcT;
  int iT;

  // Special values allow referencing the standard bitmaps.
  if (ic < -5)
    return NULL;
  switch (ic) {
  case -1: return &bm.k;   // Bitmap -1 is the main color bitmap
  case -2: return &bm.k2;  // Bitmap -2 is the temporary color bitmap
  case -3: return &bm.k3;  // Bitmap -3 is the extra color bitmap
  case -4: return &bm.kI;  // Bitmap -4 is the inside color bitmap
  case -5: return &bm.kR;  // Bitmap -5 is the inside rainbow bitmap
  }

  // Ensure there are at least the given number of slots available in the
  // color bitmap array, reallocating if needed.
  if (ic >= ws.ccTexture) {
    pcT = (CMazK *)ReallocateArray(ws.rgcTexture, ws.ccTexture,
      sizeof(CMazK), ic+1);
    if (pcT == NULL)
      return NULL;
    for (iT = ws.ccTexture; iT <= ic; iT++)
      new (&pcT[iT]) CMazK();
    if (ws.rgcTexture != NULL)
      DeallocateP(ws.rgcTexture);
    ws.rgcTexture = pcT;
    ws.ccTexture = ic+1;
  }
  return &ws.rgcTexture[ic];
}


// Free all custom texture bitmaps and associated data. This is called from
// the Delete textures command, the ResetProgram operation, and when the
// program terminates.

void DeallocateTextures()
{
  int i;

  // Free each monochrome bitmap and the list itself.
  if (ws.rgbMask != NULL) {
    for (i = 0; i < ws.cbMask; i++)
      ws.rgbMask[i].Free();
    DeallocateP(ws.rgbMask);
    ws.rgbMask = NULL;
    ws.cbMask = 0;
  }

  // Free each color bitmap and the list itself.
  if (ws.rgcTexture != NULL) {
    for (i = 0; i < ws.ccTexture; i++)
      ws.rgcTexture[i].Free();
    DeallocateP(ws.rgcTexture);
    ws.rgcTexture = NULL;
    ws.ccTexture = 0;
  }
}


// Apply a texture map to the sky and ground background areas of the
// perspective inside view. Implements the Background apply texture command.

flag FTextureBackground()
{
  CCol cT, *pc, *c1 = &bm.k;
  int iBase;

  // Ensure at least one of the color and temporary color bitmaps exist.
  iBase = Max(ws.ccTexture, ws.cbMask); iBase = Max(iBase, 1);
  if (c1->FNull() && bm.k2.FNull()) {
    c1 = &cT;
    if (!c1->FColmapGetFromBitmap(bm.b, dr.kvOff, dr.kvOn))
      return fFalse;
  }
  if (ColmapGetTexture(iBase + 1) == NULL)
    return fFalse;

  // Set the sky/ground of inside view to a copy of the color bitmap, if
  // present. Put in next new texture slot to avoid conflicting with others.
  if (c1 != NULL) {
    pc = ColmapGetTexture(iBase);
    if (!pc->FBitmapCopy(*c1))
      return fFalse;
    dr.nTexture = iBase;
  }

  // Set the sky/ground of inside view for lower levels of 3D Mazes to a copy
  // of the temporary color bitmap, if present.
  if (!bm.k2.FNull()) {
    pc = ColmapGetTexture(iBase + 1);
    if (!pc->FBitmapCopy(bm.k2))
      return fFalse;
    dr.nTexture2 = iBase + 1;
  }

  return fTrue;
}


// Apply texture maps to all walls of perspective inside view. Implements the
// Color Walls, Overlay Walls, and Color Overlay Walls apply texture commands.

flag FTextureWall(int nMode, bit o)
{
  CCol c1, c2, *pc1 = &bm.k, *pc2 = &bm.k2, *pc;
  CMon b1, b2, *pb1 = &bm.b2, *pb2 = &bm.b3, *pb;
  int iBase, n, x, y, n1, n2, n3;
  flag fShade = dr.rLightFactor != 0.0 && nMode != 2;

  // Figure out how many different textures will be used. Narrow Walls on
  // doubles this number, also a non-zero Light Factor triples this number.
  iBase = Max(ws.ccTexture, ws.cbMask);
  n = (1 + dr.fNarrow) * (1 + fShade*2);
  if (ColmapGetTexture(iBase + n) == NULL ||
    BitmapGetMask(iBase + n) == NULL)
    return fFalse;

  // Set all pixels in the texture lookup bitmap to reference the textures
  // that are about to be defined.
  pc = ColmapGetTexture(iBase);
  if (!pc->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
    return fFalse;
  if (!dr.fNarrow) {

    // Narrow Walls off means all pixels have the same texture per side.
    pc->BitmapSet(fShade ? NWSE(iBase + 2, iBase + 1, iBase + 3, iBase + 1) :
      NWSE(iBase + 1, iBase + 1, iBase + 1, iBase + 1));
  } else {

    // Narrow Walls on means narrow and normal walls have different textures.
    n1 = n2 = n3 = iBase + 2;
    if (fShade) {
      n2 = iBase + 4; n3 = iBase + 6;
    }
    for (y = 0; y < pc->m_y; y++)
      for (x = 0; x < pc->m_x; x++)
        pc->Set(x, y,
          NWSE(n2 - FOdd(x), n1 - FOdd(y), n3 - FOdd(x), n1 - FOdd(y)));
  }

  // Define monochrome overlay textures. Applies to Overlay Walls and Color
  // Overlay Walls, but not Color Walls.
  if (nMode >= 2) {

    // If the overlays are to be white instead of black, set all pixels in the
    // overlay lookup bitmap to indicate all overlays are to be white.
    if (o) {
      pb = BitmapGetMask(0);
      if (!pb->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
        return fFalse;
      pb->BitmapOn();
    }

    // Make copies of bitmaps if they're going to be modified.
    if (nMode == 3) {
      pb1 = &b1; pb2 = &b2;
      if (!bm.b2.FNull() && !pb1->FBitmapCopy(bm.b2))
        return fFalse;
      if (!bm.b3.FNull() && !pb2->FBitmapCopy(bm.b3))
        return fFalse;
    }
    if (pb1->FNull() && pb2->FNull() && !(pb1 = &b1)->FBitmapCopy(bm.b))
      return fFalse;
    if (nMode == 3) {
      pb1->BitmapReverse();
      pb2->BitmapReverse();
    }

    // If the temporary or extra bitmap doesn't exist, copy the other to it,
    // truncated or tessellated in proportion to the narrow walls setting.
    if (pb1->FNull()) {
      if (!(pb1 = &b1)->FAllocate(
        NMultDiv(pb2->m_x, dr.zCell, dr.zCellNarrow), pb2->m_y, pb2))
        return fFalse;
      pb1->BitmapTessellate(*pb2);
    } else if (dr.fNarrow && pb2->FNull()) {
      if (!(pb2 = &b2)->FAllocate(
        NMultDiv(pb1->m_x, dr.zCellNarrow, dr.zCell), pb1->m_y, pb1))
        return fFalse;
      pb2->BitmapTessellate(*pb1);
    }

    // Copy bitmaps to overlay textures.
    if (!BitmapGetMask(iBase + 1)->FBitmapCopy(*pb1))
      return fFalse;
    if (dr.fNarrow && !BitmapGetMask(iBase + 2)->FBitmapCopy(*pb2))
      return fFalse;
    if (fShade) {
      if (!BitmapGetMask(iBase + (dr.fNarrow ? 3 : 2))->FBitmapCopy(*pb1))
        return fFalse;
      if (!BitmapGetMask(iBase + (dr.fNarrow ? 5 : 3))->FBitmapCopy(*pb1))
        return fFalse;
      if (dr.fNarrow) {
        if (!BitmapGetMask(iBase + 4)->FBitmapCopy(*pb2))
          return fFalse;
        if (!BitmapGetMask(iBase + 6)->FBitmapCopy(*pb2))
          return fFalse;
      }
    }
  }

  // Define color textures. Applies to Color Walls and Color Overlay Walls,
  // but not Overlay Walls.
  if (nMode != 2) {

    // Make copies of color bitmaps if they're going to be modified.
    if (nMode == 3) {
      pc1 = &c1; pc2 = &c2;
      if (!bm.k.FNull() && !pc1->FBitmapCopy(bm.k))
        return fFalse;
      if (!bm.k2.FNull() && !pc2->FBitmapCopy(bm.k2))
        return fFalse;
    }
    if (pc1->FNull() && pc2->FNull() &&
      !pc1->FColmapGetFromBitmap(bm.b, dr.kvOff, dr.kvOn))
      return fFalse;

    // If color or temporary color bitmap doesn't exist, copy the other to it,
    // truncated or tessellated in proportion to the narrow walls setting.
    if (pc1->FNull()) {
      if (!(pc1 = &c1)->FAllocate(
        NMultDiv(pc2->m_x, dr.zCell, dr.zCellNarrow), pc2->m_y, pc2))
        return fFalse;
      pc1->BitmapTessellate(*pc2);
    } else if (dr.fNarrow && pc2->FNull()) {
      if (!(pc2 = &c2)->FAllocate(
        NMultDiv(pc1->m_x, dr.zCellNarrow, dr.zCell), pc1->m_y, pc1))
        return fFalse;
      pc2->BitmapTessellate(*pc1);
    }
    if (nMode == 3) {
      pc1->ColmapOrAndFromBitmap(*pb1, kvWhite, kvBlack, o);
      if (dr.fNarrow)
        pc2->ColmapOrAndFromBitmap(*pb2, kvWhite, kvBlack, o);
    }

    // Copy color bitmaps to textures, shaded appropriately.
    if (!ColmapGetTexture(iBase + 1)->FBitmapCopy(*pc1))
      return fFalse;
    if (dr.fNarrow && !ColmapGetTexture(iBase + 2)->FBitmapCopy(*pc2))
      return fFalse;
    if (dr.rLightFactor != 0.0) {
      pc1->ColmapBrightness(fFalse, -dr.rLightFactor, 0);
      if (nMode == 3 && o)
        pc1->ColmapOrAndFromBitmap(*pb1, kvWhite, kvBlack, o);
      if (!ColmapGetTexture(iBase + (dr.fNarrow ? 3 : 2))->FBitmapCopy(*pc1))
        return fFalse;
      pc1->FBitmapCopy(*ColmapGetTexture(iBase + 1));
      pc1->ColmapBrightness(fFalse, dr.rLightFactor, 0);
      if (nMode == 3 && !o)
        pc1->ColmapOrAndFromBitmap(*pb1, kvWhite, kvBlack, o);
      if (!ColmapGetTexture(iBase + (dr.fNarrow ? 5 : 3))->FBitmapCopy(*pc1))
        return fFalse;
      pc1->FBitmapCopy(*ColmapGetTexture(iBase + 1));
      if (dr.fNarrow) {
        pc2->ColmapBrightness(fFalse, -dr.rLightFactor, 0);
        if (nMode == 3 && o)
          pc2->ColmapOrAndFromBitmap(*pb2, kvWhite, kvBlack, o);
        if (!ColmapGetTexture(iBase + 4)->FBitmapCopy(*pc2))
          return fFalse;
        pc2->FBitmapCopy(*ColmapGetTexture(iBase + 2));
        pc2->ColmapBrightness(fFalse, dr.rLightFactor, 0);
        if (nMode == 3 && !o)
          pc2->ColmapOrAndFromBitmap(*pb2, kvWhite, kvBlack, o);
        if (!ColmapGetTexture(iBase + 6)->FBitmapCopy(*pc2))
          return fFalse;
        pc2->FBitmapCopy(*ColmapGetTexture(iBase + 2));
      }
    }
  }

  dr.nTextureWall = iBase;
  return fTrue;
}


// Apply texture maps to all ground pixels of the perspective inside view.
// Implements the Color Ground, Overlay Ground, and Color Overlay Ground
// apply texture commands.

flag FTextureGround(flag fBlock, int nMode, bit o)
{
  CCol *pc1 = &bm.k, c1, c2, c3, c4, *pc;
  CMon *pb1 = &bm.b2, b1, b2, b3, b4, *pb;
  int iBase, n, x, y;

  // Figure out how many different textures will be used. Narrow Walls on
  // quadruples this number.
  iBase = Max(ws.ccTexture, ws.cbMask); iBase = Max(iBase, 1);
  n = (1 + dr.fNarrow * 3);
  if (ColmapGetTexture(iBase + n) == NULL ||
    BitmapGetMask(iBase + n) == NULL)
    return fFalse;

  // Set all pixels in the ground texture lookup bitmap to reference the
  // textures that are about to be defined.
  pc = ColmapGetTexture(iBase);
  if (!pc->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
    return fFalse;
  if (!dr.fNarrow) {

    // Narrow Walls off means all pixels have the same texture.
    n = iBase + 1;
    pc->BitmapSet(UD(n, n));
  } else {

    // Narrow Walls on means narrow and normal cells have different textures.
    for (y = 0; y < pc->m_y; y++)
      for (x = 0; x < pc->m_x; x++) {
        n = iBase + 1 + (!FOdd(x) << 1) + !FOdd(y);
        pc->Set(x, y, UD(n, n));
      }
  }

  // Define monochrome overlay textures. Applies to Overlay Ground and Color
  // Overlay Ground, but not Color Ground.
  if (nMode >= 2) {

    // If the overlays are to be white instead of black, set all pixels in the
    // overlay lookup bitmap to indicate all overlays are to be white.
    if (o) {
      pb = BitmapGetMask(iBase);
      if (!pb->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
        return fFalse;
      pb->BitmapOn();
    }

    // Make copies of bitmaps if they're going to be modified.
    if (nMode == 3) {
      if (!bm.b2.FNull() && !b1.FBitmapCopy(bm.b2))
        return fFalse;
      pb1 = &b1;
    }
    if (pb1->FNull() && !pb1->FBitmapCopy(bm.b))
      return fFalse;
    if (nMode == 3)
      pb1->BitmapReverse();

    // If narrow walls is on, truncate or tessellate the first bitmap in
    // proportion to the narrow walls setting to create the others.
    if (dr.fNarrow) {
      if (!b2.FAllocate(pb1->m_x,
        NMultDiv(pb1->m_y, dr.zCellNarrow, dr.zCell), pb1))
        return fFalse;
      b2.BitmapTessellate(*pb1);
      if (!b3.FAllocate(NMultDiv(pb1->m_x, dr.zCellNarrow, dr.zCell),
        pb1->m_y, pb1))
        return fFalse;
      b3.BitmapTessellate(*pb1);
      if (!b4.FAllocate(NMultDiv(pb1->m_x, dr.zCellNarrow, dr.zCell),
        NMultDiv(pb1->m_y, dr.zCellNarrow, dr.zCell), pb1))
        return fFalse;
      b4.BitmapTessellate(*pb1);
    }

    // Copy bitmap(s) to overlay textures.
    if (!BitmapGetMask(iBase + 1)->FBitmapCopy(*pb1))
      return fFalse;
    if (dr.fNarrow && (!BitmapGetMask(iBase + 2)->FBitmapCopy(b2) ||
      !BitmapGetMask(iBase + 3)->FBitmapCopy(b3) ||
      !BitmapGetMask(iBase + 4)->FBitmapCopy(b4)))
      return fFalse;
  }

  // Define color textures. Applies to Color Ground and Color Overlay Ground,
  // but not Overlay Ground.
  if (nMode != 2) {

    // Make copies of color bitmaps if they're going to be modified.
    if (pc1->FNull() && !pc1->FColmapGetFromBitmap(bm.b, dr.kvOff, dr.kvOn))
      return fFalse;

    // If narrow walls is on, truncate or tessellate the first color bitmap in
    // proportion to the narrow walls setting to create the others.
    if (dr.fNarrow) {
      if (!c2.FAllocate(pc1->m_x,
        NMultDiv(pc1->m_y, dr.zCellNarrow, dr.zCell), pc1))
        return fFalse;
      c2.BitmapTessellate(*pc1);
      if (!c3.FAllocate(NMultDiv(pc1->m_x, dr.zCellNarrow, dr.zCell),
        pc1->m_y, pc1))
        return fFalse;
      c3.BitmapTessellate(*pc1);
      if (!c4.FAllocate(NMultDiv(pc1->m_x, dr.zCellNarrow, dr.zCell),
        NMultDiv(pc1->m_y, dr.zCellNarrow, dr.zCell), pc1))
        return fFalse;
      c4.BitmapTessellate(*pc1);
    }
    if (nMode == 3)
      pc1->ColmapOrAndFromBitmap(b1, kvWhite, kvBlack, o);

    // Copy color bitmap(s) to textures.
    if (!ColmapGetTexture(iBase + 1)->FBitmapCopy(*pc1))
      return fFalse;
    if (dr.fNarrow && (!ColmapGetTexture(iBase + 2)->FBitmapCopy(c2) ||
      !ColmapGetTexture(iBase + 3)->FBitmapCopy(c3) ||
      !ColmapGetTexture(iBase + 4)->FBitmapCopy(c4)))
      return fFalse;
  }

  if (fBlock)
    dr.nTextureVar = iBase;
  else {
    if (!bm.b2.FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
      return fFalse;
    bm.b2.BitmapOn();
    dr.nTextureDirt = iBase;
  }
  return fTrue;
}


// Make all cells in the perspective inside view have a ceiling marking.
// Implements the Ceiling apply texture command.

flag FTextureCeiling()
{
  CMon *pbMask;
  CCol *pcTexture, *pcGround;

  if (dr.nMarkSky < 0) {
    dr.nMarkSky = Max(ws.ccTexture, ws.cbMask);
    dr.nMarkSky = Max(dr.nMarkSky, 1);
  }
  pbMask = BitmapGetMask(dr.nMarkSky);
  if (pbMask == NULL || !pbMask->FBitmapCopy(bm.b))
    return fFalse;
  pbMask->BitmapOn();

  // Make ceiling colors be a copy of the ground marking colors, if any.
  pcGround = dr.fMarkColor ? &bm.k2 : &bm.k;
  if (!pcGround->FNull()) {
    pcTexture = ColmapGetTexture(dr.nMarkSky);
    if (pcTexture == NULL || !pcTexture->FBitmapCopy(*pcGround))
      return fFalse;
  }
  return fTrue;
}


// Convert a Maze so it appears to be on hilly terrain in the perspective
// inside view. Implements the Variable Height Walls apply texture command.

flag FWallVariable(int nPattern)
{
  CMon *pb;
  CCol *pc;
  int x, y, z, d, xnew, ynew, zHi, zLo;
  KV kv;
  flag f1, f2;

  if (dr.nWallVar < 0)
    dr.nWallVar = NMax(Max(ws.ccTexture, ws.cbMask), 1);
  pb = BitmapGetMask(dr.nWallVar);
  if (pb == NULL || !pb->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
    return fFalse;
  pb->BitmapOn();

  pc = ColmapGetTexture(dr.nWallVar);
  if (pc == NULL || !pc->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
    return fFalse;

  // Compute the base altitude for each block.
  pc->BitmapSet(kvBlack);
  f1 = cs.fGraphNumber; f2 = cs.fRainbowBlend;
  cs.fGraphNumber = fTrue; cs.fRainbowBlend = fFalse;
  x = cs.kvBlend1; y = cs.kvBlend2;
  cs.kvBlend1 = 0; cs.kvBlend2 = 4095 - 4 - iVar;
  pc->ColmapReplacePattern(kvBlack, nPattern, dr.x, dr.y, dr.x2, dr.y2);
  cs.kvBlend1 = x; cs.kvBlend2 = y;
  cs.fGraphNumber = f1; cs.fRainbowBlend = f2;

  // Compute the color for each block.
  if (!bm.k.FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
    return fFalse;
  bm.k.BitmapSet(kvBlack);
  bm.k.ColmapReplacePattern(kvBlack, nPattern, dr.x, dr.y, dr.x2, dr.y2);

  // Determine the maximum possible bottom height of each block.
  for (y = 0; y < bm.b.m_y; y++)
    for (x = 0; x < bm.b.m_x; x++) {
      zLo = zHi = pc->Get(x, y);
      for (d = 0; d < DIRS2; d++) {
        xnew = x + xoff[d]; ynew = y + yoff[d];
        z = UdU(pc->Get(xnew, ynew));
        zLo = Min(zLo, z);
      }
      pc->Set(x, y, UD(zHi, zLo));
    }

  // Increase the top height of each block if a wall at that spot in the Maze.
  for (y = 0; y < bm.b.m_y; y++)
    for (x = 0; x < bm.b.m_x; x++) {
      kv = pc->Get(x, y);
      zHi = UdU(kv) + 4;
      if (bm.b.Get(x, y))
        zHi += iVar;
      else
        bm.k.Set(x, y, KvBlend(bm.k.Get(x, y), GrayN(zHi * 255 / 3600)));
      pc->Set(x, y, UD(zHi, UdD(kv)));
    }

  bm.b.BitmapOn();
  return fTrue;
}


// Make all ground marks in the perspective inside view have random hill
// elevations. Implements the Ground Elevation apply texture command.

flag FGroundVariable(void)
{
  CCol *pcElev, *pcVar, c1, c2, *c;
  CMon *pbVar;
  int xmax, ymax, x, y, z, xnew, ynew, d, n;
  flag fSav = cs.fGraphNumber;

  if (dr.nMarkElev < 0)
    dr.nMarkElev = NMax(Max(ws.ccTexture, ws.cbMask), 1);
  if (ColmapGetTexture(dr.nMarkElev) == NULL)
    return fFalse;
  if (dr.nWallVar < 0)
    dr.nWallVar = NMax(Max(ws.ccTexture, ws.cbMask) + 1, 1);
  if (ColmapGetTexture(dr.nWallVar) == NULL)
    return fFalse;
  pcElev = ColmapGetTexture(dr.nMarkElev);
  if (!pcElev->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
    return fFalse;
  pcVar = ColmapGetTexture(dr.nWallVar);
  if (!pcVar->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
    return fFalse;
  pbVar = BitmapGetMask(dr.nWallVar);
  if (!pbVar->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
    return fFalse;
  pbVar->BitmapOn();

  // Generate two random height maps. Start with large rough shapes to give
  // general definition, then create smaller adjustments to make the hills
  // seem more random and natural. Two stages makes simple fractal hills! :)

  cs.fGraphNumber = fTrue;
  for (n = 0; n < 2; n++) {
    if (n <= 0) {
      c = &c1; z = dr.nMarkElevY1; d = dr.nMarkElevX1;
    } else {
      c = &c2; z = dr.nMarkElevY2; d = dr.nMarkElevX2;
    }
    xmax = bm.b.m_x / d + 1; ymax = bm.b.m_y / d + 1;
    if (!c->FBitmapSizeSet(xmax, ymax))
      return fFalse;
    for (y = 0; y < ymax; y++)
      for (x = 0; x < xmax; x++)
        c->Set(x, y, Rnd(0, z));
    if (dr.nEdge == nEdgeTorus) {
      c->BlockMove(*c, 0, 0, c->m_x - 1, 0, 0, c->m_y - 1);
      c->BlockMove(*c, 0, 0, 0, c->m_y - 1, c->m_x - 1, 0);
    }
    c->FBitmapZoomTo(bm.b.m_x + bm.b.m_x / (xmax-1),
      bm.b.m_y + bm.b.m_y / (ymax-1), fTrue);
  }
  cs.fGraphNumber = fSav;

  // Compose final elevation bitmap using both height maps.
  if (bm.fColor && !bm.k.FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
    return fFalse;
  for (y = 0; y < bm.b.m_y; y++)
    for (x = 0; x < bm.b.m_x; x++) {
      z = NMin(c1.Get(x, y) + c2.Get(x, y), 4095);
      pcElev->Set(x, y, z);
      pcVar->Set(x, y, UD(NMin(z + iVar, 4095), z));
      // If color bitmap set, color ground marks based on elevation
      if (bm.fColor)
        bm.k.Set(x, y, Hue(z * 3600 >> 12));
    }

  // Set variable height walls on top of the hills.
  for (y = 0; y < bm.b.m_y; y++)
    for (x = 0; x < bm.b.m_x; x++) {
      n = pcElev->Get(x, y);
      z = n + iVar;
      for (d = 0; d < DIRS2; d++) {
        xnew = x + xoff[d]; ynew = y + yoff[d];
        z = NMax(z, pcElev->Get(xnew, ynew) + iVar);
      }
      pcVar->Set(x, y, UD(Min(z, 4095), n));
    }
  return fTrue;
}


// Convert a formatted range of characters to a string ready to display,
// translating character sequences as appropriate. This is used by the
// various Message and SetString operations.

void FormatRgchToSz(CONST char *pch, int cch, char *sz, int cchMax)
{
  CONST char *pchSrc = pch;
  char szT[cchSzDef], *pchDst = sz, ch1, ch2;
  long l;

  while (pchSrc - pch < cch && pchDst - sz < cchMax - 1) {
    ch1 = *pchSrc; ch2 = *(pchSrc + 1);
    if (ch1 == '\\') {
      if (ch2 == '\\')          // "\\" means a single "\"
        pchSrc++;
      else if (ch2 == 'n') {    // "\n" means a new line
        *pchDst++ = '\n';
        pchSrc += 2;
        continue;
      } else if (ch2 == 'r') {  // "\r" means a carriage return
        *pchDst++ = '\r';
        pchSrc += 2;
        continue;
      } else if (ch2 == 't') {  // "\t" means a tab
        *pchDst++ = chTab;
        pchSrc += 2;
        continue;
      } else if (ch2 == 's') {  // "\s" means a space
        *pchDst++ = ' ';
        pchSrc += 2;
        continue;
      }
    }
    if (ch1 == '@') {
      if (ch2 == '@')           // "@@" means a single "@"
        pchSrc++;
      else {

        // Expand "@x" to the number contained in that custom variable.
        // Expand "@n" to custom variable number n.
        ch2 = ChCap(ch2);
        if (FCapCh(ch2) || FDigitCh(ch2)) {
          if (FCapCh(ch2))
            l = ch2 - '@';
          else {
            l = ch2 - '0';
            loop {
              ch2 = *(pchSrc + 2);
              if (!FDigitCh(ch2))
                break;
              l = l * 10 + (ch2 - '0');
              pchSrc++;
            }
          }
          sprintf(S(szT), "%ld", LVar(l));
          sprintf(SS(pchDst, cchMax - (pchDst - sz)), "%s", szT);
          while (*pchDst)
            pchDst++;
          pchSrc += 2;
          continue;
        }
      }
    }
    if (ch1 == '$') {
      if (ch2 == '$')           // "$$" means a single "$"
        pchSrc++;
      else {

        // Expand "$x" to the custom string pointed to by custom variable @x.
        // Expand "$n" to custom string number n.
        ch2 = ChCap(ch2);
        if (FCapCh(ch2) || FDigitCh(ch2)) {
          if (FCapCh(ch2))
            l = LVar(ch2 - '@');
          else {
            l = ch2 - '0';
            loop {
              ch2 = *(pchSrc + 2);
              if (!FDigitCh(ch2))
                break;
              l = l * 10 + (ch2 - '0');
              pchSrc++;
            }
          }
          if (l >= 0 && l < ws.cszVar && ws.rgszVar[l] != NULL) {
            CopyRgchToSz(ws.rgszVar[l], CchSz(ws.rgszVar[l]),
              pchDst, cchMax - PD(pchDst - sz));
            while (*pchDst)
              pchDst++;
          }
          pchSrc += 2;
          continue;
        }
      }
    }
    *pchDst++ = *pchSrc++;
  }
  *pchDst = chNull;
}


// Set pixels in the temporary bitmap, at the passed in coordinates and at the
// coordinates of the dot. This is called from the dot movement commands when
// Do Drag On Temp is set.

void DotTemp(int x, int y, int z)
{
  int x1, y1, x2, y2;

  if (bm.b2.FNull())
    bm.b2.FBitmapCopy(bm.b);
  if (!bm.b2.FNull()) {
    if (dr.f3D) {
      x1 = bm.b.X2(x, z); y1 = bm.b.Y2(y, z);
      x2 = bm.b.X2(dr.x, dr.z); y2 = bm.b.Y2(dr.y, dr.z);
    } else {
      x1 = x; y1 = y;
      x2 = dr.x; y2 = dr.y;
    }
    bm.b2.Set(x1, y1, dr.fSet);
    bm.b2.Set(x2, y2, dr.fSet);
    if (dr.fNarrow && (!dr.f3D || z == dr.z))
      bm.b2.Set((x1 + x2) >> 1, (y1 + y2) >> 1, dr.fSet);
  }
}


// Shoot an invisible ray of the specified type from the position of the dot
// in the direction it's facing, until whatever sought after is hit.
// Implements the commands on the Zap Dot submenu.

void DotZap(int wCmd, int nParam)
{
  CMazK *c;
  int x, y, dir;
  bit o;

  x = bm.b.X2(dr.x, dr.z); y = bm.b.Y2(dr.y, dr.z);
  o = bm.b.Get(x, y);
  if (FBetween(y, 0, bm.b.m_y-1)) {
    if (x < 0 && dr.dir == 3)
      x = 0;
    else if (x >= bm.b.m_x && dr.dir == 1)
      x = bm.b.m_x-1;
  } else if (FBetween(x, 0, bm.b.m_x-1)) {
    if (y < 0 && dr.dir == 2)
      y = 0;
    else if (y >= bm.b.m_y && dr.dir == 0)
      y = bm.b.m_y-1;
  }
  switch (wCmd) {

  // Destroy wall: Turn off first set pixel encountered in the main bitmap.
  case cmdDotZap:
    while (bm.b.FLegal(x, y) && bm.b.Get(x, y) == o) {
      x += xoff[dr.dir]; y += yoff[dr.dir];
    }
    BitmapDotCore(x, y, o);
    break;

  // Make wall semitransparent: Set the first off pixel encountered in the
  // extra bitmap, where the pixel in the main bitmap is also on.
  case cmdDotZapTrans:
    while (bm.b.FLegal(x, y) &&
      !(bm.b.Get(x, y) && (bm.b3.FNull() || !bm.b3.Get(x, y)))) {
      x += xoff[dr.dir]; y += yoff[dr.dir];
    }
    if (bm.b.FLegal(x, y)) {
      if (bm.b3.FNull()) {
        if (bm.b3.FBitmapCopy(bm.b))
          bm.b3.BitmapOff();
      }
      if (!bm.b3.FNull() && bm.b3.FLegal(x, y)) {
        bm.b3.Set1(x, y);
        if (dr.nTrans == nTransNone)
          dr.nTrans = nTransDefault;
      }
    }
    break;

  // Make unsemitransparent: Turn off the first set pixel encountered in the
  // extra bitmap.
  case cmdDotZapUntrans:
    if (!bm.b3.FNull()) {
      while (bm.b3.FLegal(x, y) && !bm.b3.Get(x, y)) {
        x += xoff[dr.dir]; y += yoff[dr.dir];
      }
      if (bm.b3.FLegal(x, y)) {
        bm.b3.Set0(x, y);
        if (dr.nTrans == nTransNone)
          dr.nTrans = nTransDefault;
      }
    }
    break;

  // ZapTexture operation: For the first set pixel encountered in the main
  // bitmap, set the texture on the side facing the dot.
  case cmdZapTexture:
    while (bm.b.FLegal(x, y) && (!bm.b.Get(x, y) ||
      (!bm.b3.FNull() && bm.b3.Get(x, y)))) {
      x += xoff[dr.dir]; y += yoff[dr.dir];
    }
    if (bm.b.FLegal(x, y)) {
      c = ColmapGetTexture(0);
      if (c != NULL) {
        if (!c->FLegal(x, y)) {
          if (!c->FBitmapSizeSet(bm.b.m_x, bm.b.m_y))
            break;
          c->BitmapOff();
        }
        dir = dr.dir ^ 2;
        c->Set(x, y, SetT(*c, x, y, dir, nParam));
      }
    }
    break;

  default: Assert(fFalse);
  }
}


// Adjust a bitmap either before or after a Maze has been created. This
// implements the scenarios when the Polished Mazes setting is active.
//
// Mode 0: Prepare to make a Maze. Zooms the bitmap down if the polish process
//   zooms it up, so successive Mazes in polish mode won't enlarge the bitmap.
// Mode 1: Polish a raw Maze that was just created in the bitmap.
// Mode 2: Leaving polish mode, so unpolish the Maze in the bitmap.

void PolishMaze(int wCmd, int nMode)
{
  CMon bT;

  if (!dr.fPolishMaze)
    return;
  if (dr.f3D && (wCmd == cmdCreateDivision || wCmd == cmdCreateBinary ||
    wCmd == cmdCreateSidewinder))
    wCmd = cmdCreate3D;

  // Polishing most Maze types means creating a bias zoomed version of it.
  switch (wCmd) {
  case cmdCreatePerfect:
  case cmdCreateBraid:
  case cmdCreateUnicursal:
  case cmdCreateRecursive:
  case cmdCreatePrim:
  case cmdCreatePrim2:
  case cmdCreateKruskal:
  case cmdCreateAldous:
  case cmdCreateWilson:
  case cmdCreateEller:
  case cmdCreateTree:
  case cmdCreateDivision:
  case cmdCreateBinary:
  case cmdCreateSidewinder:
  case cmdCreateRandom:
  case cmdCreateSpiral:
  case cmdCreateDiagonal:
  case cmdCreateSegment:
  case cmdCreateFractal:
  case cmdCreateWeave:
  case cmdCreatePlanair:
  case cmdCreateInfinite0:
  case cmdCreateInfinite1:
  case cmdCreateInfinite2:
  case cmdCreateInfinite3:
    if (FBetween(wCmd, cmdCreateInfinite0, cmdCreateInfinite3)) {
      if (nMode == 0)
        nMode = 2;
    }
    switch (nMode) {
    case 0:
      bm.b.FBitmapSizeSet((bm.b.m_x + 3) >> 1, (bm.b.m_y + 3) >> 1);
      break;
    case 1:
      bm.b.FBitmapResizeTo(Odd(bm.b.m_x), Odd(bm.b.m_y));
      bm.b.FBitmapBias(1, 3, 1, 3);
      dr.x <<= 1; dr.y <<= 1;
      break;
    case 2:
      bm.b.FBitmapResizeTo(bm.b.m_x + 3, bm.b.m_y + 3);
      bm.b.FBitmapZoomTo(bm.b.m_x >> 1, bm.b.m_y >> 1, fFalse);
      dr.x >>= 1; dr.y >>= 1;
      break;
    }
    break;

  // Recursive fractal Mazes are like the above, but affect color bitmap too.
  case cmdCreateFractal2:
    switch (nMode) {
    case 1:
      if (!bm.k.FNull() && bm.k.m_x == bm.b.m_x && bm.k.m_y == bm.b.m_y)
        bm.k.FBitmapBias(1, 3, 1, 3);
      bm.b.BitmapReverse();
      bm.b.FBitmapBias(1, 3, 1, 3);
      dr.x <<= 1; dr.y <<= 1;
      break;
    case 2:
      bm.b.FBitmapResizeTo(bm.b.m_x + 3, bm.b.m_y + 3);
      bm.b.FBitmapZoomTo(bm.b.m_x >> 1, bm.b.m_y >> 1, fFalse);
      dr.x >>= 1; dr.y >>= 1;
      bm.b.FBitmapResizeTo(bm.b.m_x - 1, bm.b.m_y - 1);
      bm.b.BitmapReverse();
      break;
    }
    break;

  // For Mazes with non-orthogonal walls, smooth them out.
  case cmdCreateCrack:
  case cmdCreateCavern:
  case cmdCreateArrow:
  case cmdCreateTilt:
    switch (nMode) {
    case 0:
      bm.b.FBitmapSizeSet(bm.b.m_x >> 1, bm.b.m_y >> 1);
      break;
    case 1:
      bm.b.BitmapSmooth(fTrue);
      dr.x <<= 1; dr.y <<= 1;
      break;
    case 2:
      bm.b.BitmapReverse();
      bm.b.FBitmapZoomTo(bm.b.m_x >> 1, bm.b.m_y >> 1, fTrue);
      bm.b.BitmapReverse();
      dr.x >>= 1; dr.y >>= 1;
      break;
    }
    break;

  // Polished Zeta Mazes are like above, but give bias to off pixels.
  case cmdCreateZeta:
    switch (nMode) {
    case 0:
      bm.b.FBitmapSizeSet((bm.b.m_x + 3) >> 1, (bm.b.m_y + 3) >> 1);
      break;
    case 1:
      bm.b.FBitmapResizeTo(Odd(bm.b.m_x), Odd(bm.b.m_y));
      bm.b.BitmapReverse();
      bm.b.BitmapSmooth(fTrue);
      bm.b.BitmapReverse();
      dr.x <<= 1; dr.y <<= 1;
      break;
    case 2:
      bm.b.FBitmapZoomTo(bm.b.m_x >> 1, bm.b.m_y >> 1, fTrue);
      bm.b.FBitmapResizeTo(bm.b.m_x + 1, bm.b.m_y + 1);
      dr.x >>= 1; dr.y >>= 1;
      break;
    }
    break;

  // For 3D Mazes, convert to a clarified version in main and color bitmaps.
  case cmdCreate3D:
    if (nMode == 1) {
      bm.b.FClarify3D(bm.k, dr.kvOff, dr.kvOn);
      bm.k.ColmapReplaceSimple(kvGreen,  Rgb(0,   170, 0));
      bm.k.ColmapReplaceSimple(kvYellow, Rgb(170, 170, 0));
      bm.b.FClarify3D(bm.b, fOff, fOn);
      ws.wCmdSpree = cmdPolishMaze;
    }
    break;

  // For 4D Mazes, convert to a clarified version in main and color bitmaps.
  case cmdCreate4D:
    if (nMode == 1) {
      bm.b.FClarify4D(bm.k, dr.kvOff, dr.kvOn);
      bm.k.ColmapReplaceSimple(kvGreen,  Rgb(0,   170, 0));
      bm.k.ColmapReplaceSimple(kvYellow, Rgb(170, 170, 0));
      bm.b.FClarify4D(bm.b, fOff, fOn);
      ws.wCmdSpree = cmdPolishMaze;
    }
    break;

  // For Hypermazes, draw a 3D overview of it.
  case cmdCreateHyper:
    if (nMode == 1) {
      DrawOverviewCube(bm.b, bm.b);
      bm.b.BitmapReverse();
      bm.b.FBitmapCollapse();
      ws.wCmdSpree = cmdPolishMaze;
    }
    break;

  // For Chartres Replica, fill in the walls.
  case cmdLabyrinthChartresR:
    if (nMode == 1) {
      if (!bT.FBitmapCopy(bm.b))
        break;
      bm.b.FFill(0, bm.b.m_y-1, fOn);
      bm.b.BitmapXor(bT);
      bm.b.BitmapReverse();
      ws.wCmdSpree = cmdPolishMaze;
    }
    break;
  }

  if (nMode == 0) {
    bm.b.SetXyh();
  }
}


// Fire a macro event if defined, passing in the given parameters and
// returning the result of the macro's execution.

flag FCheckEvent(int iEvent, int x, int y, int z)
{
  if (iEvent <= 0 || !FEnsureLVar(cLetter))
    return fFalse;

  // Store parameters in custom variables @x, @y, and @z.
  ws.rglVar[iLetterX] = x; ws.rglVar[iLetterY] = y; ws.rglVar[iLetterZ] = z;
  RunMacro(iEvent);

  // Return the result that the macro should have stored in @z.
  return ws.rglVar[iLetterZ] != 0;
}


// Fire a macro event that checks whether the dot is allowed to move. If not,
// the dot is moved back to its old location.

void CheckEventMove(int iEvent, int xold, int yold, int zold, int eold,
  flag fUndo)
{
  int xnew = dr.x, ynew = dr.y, znew = dr.z, enew = dr.zElev;

  // Note if the position of the dot was changed by the event macro, that
  // position is left alone, regardless of the return value.
  dr.x = xold; dr.y = yold; dr.z = zold; dr.zElev = eold;
  if (FCheckEvent(iEvent, xnew, ynew, znew) != fUndo &&
    dr.x == xold && dr.y == yold && dr.z == zold && dr.zElev == eold) {
    dr.x = xnew; dr.y = ynew; dr.z = znew; dr.zElev = enew;
  }
}


// Fire a macro event that checks whether the dot is allowed to move to its
// current location and offset within that pixel, returning yes or no.

flag FCheckEventMove2(int iEvent, int xold, int yold, int zold,
  int xold2, int yold2, int zold2)
{
  int xnew = dr.x, ynew = dr.y, znew = dr.z,
    xnew2 = dr.nOffsetX, ynew2 = dr.nOffsetY, znew2 = dr.nOffsetZ;
  flag fRet;

  dr.x = xold; dr.y = yold; dr.z = zold;
  dr.nOffsetX = xold2; dr.nOffsetY = yold2; dr.nOffsetZ = zold2;
  fRet = FCheckEvent(iEvent, xnew, ynew, znew);
  dr.x = xnew; dr.y = ynew; dr.z = znew;
  dr.nOffsetX = xnew2; dr.nOffsetY = ynew2; dr.nOffsetZ = znew2;
  return fRet;
}


// Fire a macro event for a mouse button click. The return value in @z is
// whether to skip doing the normal action associated with that click.

flag FCheckEventClick(int iEvent, long lParam)
{
  int x, y;

  if (iEvent <= 0 || !FEnsureLVar(cLetter))
    return fFalse;
  x = WLo(lParam); y = WHi(lParam);
  if (!dr.fInside) {

    // For overhead view, @x and @y contain the bitmap pixel clicked on.
    x -= bm.xpOrigin; y -= bm.ypOrigin;
    ws.rglVar[iLetterX] = x / bm.xpPoint + bm.xaOrigin - (x < 0);
    ws.rglVar[iLetterY] = y / bm.ypPoint + bm.yaOrigin - (y < 0);
  } else {

    // For perspective inside view, parameters @x and @y are scaled
    // coordinates running from (-10000, -10000) at the upper left corner of
    // the window to (10000, 10000) at bottom right.
    x = NMultDiv(x, 20000, ws.xpClient);
    y = NMultDiv(y, 20000, ws.ypClient);
    ws.rglVar[iLetterX] = x - 10000; ws.rglVar[iLetterY] = y - 10000;
  }

  // The parameter in @z is the raw pixel coordinates within the window.
  ws.rglVar[iLetterZ] = lParam;
  RunMacro(iEvent);
  return ws.rglVar[iLetterZ] != 0;
}


// Handle a mouse click on the window in perspective inside view.

void MouseClickInside(int x, int y)
{
  int i;

  // Based on the quadrant of the window the mouse was clicked in, move the
  // dot forward or backward, or rotate in place.
  y = NMultDiv(y, ws.xpClient, ws.ypClient);
  i = x > y ? (ws.xpClient - x > y ? 1 : 5) :
    (x > ws.xpClient - y ? 7 : 3);
  if (dr.f3D) {

    // For 3D bitmaps, clicking in the top 1/4 of the top quadrant will move
    // up, and clicking in bottom 1/4 of the bottom quadrant will move down.
    if (i == 1) {
      if (y < ws.xpClient / 8)
        i = 9;
    } else if (i == 7) {
      if (y > ws.xpClient * 7 / 8)
        i = 10;
    }
  }
  DoCommand(rgcmdMouse[i]);
}


// Compute the minimum distance between two positions on a bitmap, wrapping
// around the edge of the bitmap if allowed and if shorter that way.

int NDistance(int z1, int z2, int zMax)
{
  int z;

  z = NAbs(z1 - z2);
  if (dr.nEdge != nEdgeTorus || z <= (zMax >> 1))
    return z;
  return zMax - z;
}


// Return whether there's a wall at a pixel. If so, return the height in
// inside units of the bottom and top of that wall.

flag FGetWallVar(int x, int y, int z, int *pzEl, int *pzLo, int *pzHi)
{
  int zEl = 0, zLo, zHi;
  CMaz *bWallVar = NULL;
  CMazK *cWallVar = NULL, *cElev = NULL;
  KV kv;
  flag fWall;

  if (dr.nMarkElev >= 0 && ws.ccTexture > dr.nMarkElev) {
    cElev = &ws.rgcTexture[dr.nMarkElev];
    zEl = cElev->GetF(x, y, z, dr.f3D);
  }
  if (dr.nWallVar >= 0 && ws.cbMask > dr.nWallVar && ws.ccTexture >
    dr.nWallVar) {
    bWallVar = &ws.rgbMask[dr.nWallVar];
    cWallVar = &ws.rgcTexture[dr.nWallVar];
  }

  // If no wall at this pixel, don't need to do anything more.
  fWall = bm.b.GetI(x, y, z, dr.f3D);
  if (!fWall && zEl <= 0)
    return fFalse;

  if (fWall && bWallVar != NULL && cWallVar != NULL &&
    bWallVar->GetI(x, y, z, dr.f3D)) {

    // For variable height walls, translate from percentage to inside units.
    kv = cWallVar->GetF(x, y, z, dr.f3D);
    zLo = ZEye(UdD(kv));
    zHi = ZEye(UdU(kv));
    if (zLo > zHi)
      SwapN(zLo, zHi);
  } else if (cElev != NULL) {

    // Floor marking elevations are treated as variable height walls too.
    zLo = 0;
    zHi = ZEye(zEl);
  } else {

    // Non-variable height walls are always the default wall height.
    zLo = 0;
    zHi = dr.zWall;
  }
  *pzEl = ZEye(zEl); *pzLo = zLo; *pzHi = zHi;
  return fTrue;
}


// Adjust the dot to the middle of the pixel it's over. If wall climbing is
// active, set the viewing height so the dot is on top of whatever wall.

void CenterWithinCell()
{
  int zEl, zLo;

  dr.nOffsetX = dr.nOffsetY = dr.nOffsetZ = dr.nOffsetD = 0;
  if (dr.fWall && dr.zStep >= 0) {
    if (!FGetWallVar(dr.x, dr.y, dr.z, &zEl, &zLo, &dr.zElev))
      dr.zElev = 0;
  }
}


// Return whether the dot is blocked from moving to the given coordinates. If
// not, and wall climbing is active, set zElev to what the new viewing height
// would be after moving there.

flag FHitWall(int x, int y, int z, int *pzElev)
{
  int zEl, zLo, zHi;

  // If dot can move through walls, never blocked by anything.
  if (!dr.fWall)
    return fFalse;

  // If no wall climbing, then always and only blocked by walls.
  if (dr.zStep < 0)
    return bm.b.GetIFast(x, y, z, dr.f3D);

  // Check the wall's height, and if dot can step on top of or under it.
  if (FGetWallVar(x, y, z, &zEl, &zLo, &zHi)) {
    if ((dr.zElev + dr.zWall) >> 1 < zLo - zEl && dr.zElev < zHi - dr.zElev)
      zHi = zEl;
  } else
    zHi = 0;
  if (zHi - dr.zElev > dr.zStep)
    return fTrue;
  if (dr.zStep2 >= 0 && dr.zElev - zHi > dr.zStep2)
    return fTrue;
  if (pzElev != NULL)
    *pzElev = zHi;
  return fFalse;
}


// The dot has moved in one of the free movement modes of the perspective
// inside view. Undo to the dot's previous positioning if necessary.

flag FUndoIfHitWall(int xold, int yold, int zold,
  int xinold, int yinold, int zinold, CONST CMap &b)
{
  int xwall, ywall, zwall, zElev, zElevX, zElevY, cDialog;
  flag fCheck, fHitX, fHitY;

  // If dot can move through walls, don't need to check anything.
  if (!dr.fWall)
    goto LCheckEdge;

  // If dot has landed on a wall pixel it can't move on or under, undo.
  if (FHitWall(dr.x, dr.y, dr.z, &zElev)) {
    xwall = dr.x; ywall = dr.y; zwall = dr.z;
    goto LUndoWall;
  }

  // If dot has moved one orthogonal pixel, don't need to check any more.
  if (NDistance(dr.x, xold, b.m_x) + NDistance(dr.y, yold, b.m_y) +
    NAbs(dr.z - zold) <= 1)
    goto LCheckEdge;

  // If dot has moved one diagonal pixel, check if can hop corners.
  if (dr.z == zold && NAbs(dr.y - yold) == 1 && NAbs(dr.x - xold) == 1) {
    if (dr.fNoCorner &&
      FHitWall(dr.x, yold, zold, NULL) && FHitWall(xold, dr.y, zold, NULL)) {
      xwall = dr.x; ywall = yold; zwall = dr.z;
      goto LUndoWall;
    }
    goto LCheckEdge;
  }

  // Dot has moved at least two pixels. Check the middle pixels.
  xwall = (xold + dr.x) >> 1; ywall = (yold + dr.y) >> 1;
  zwall = (zold + dr.z) >> 1;
  if ((xwall != xold || ywall != yold || zwall != zold) &&
    FHitWall(xwall, ywall, zwall, NULL)) {
    if (dr.fNoCorner)
      goto LUndoWall;
  } else if (!dr.fNoCorner)
    goto LCheckEdge;
  xwall = (xold + dr.x + 1) >> 1; ywall = (yold + dr.y + 1) >> 1;
  zwall = (zold + dr.z + 1) >> 1;
  if ((xwall != xold || ywall != yold || zwall != zold) &&
    FHitWall(xwall, ywall, zwall, NULL))
    goto LUndoWall;

LCheckEdge:
  // Didn't hit a wall. Check if stepped off the bitmap.
  if (bm.b.FLegalLevel(xold, yold, dr.f3D) &&
    !bm.b.FLegalLevel(dr.x, dr.y, dr.f3D) &&
    (dr.nEdge == nEdgeStop || FCheckEventMove2(ws.iEventEdge,
    xold, yold, zold, xinold, yinold, zinold)))
    goto LUndo;
  if (ws.iEventMove > 0 &&
    !dr.fInSmooth && (dr.x != xold || dr.y != yold || dr.z != zold) &&
    FCheckEventMove2(ws.iEventMove, xold, yold, zold, xinold, yinold, zinold))
    goto LUndo;
  if (dr.zStep >= 0 && dr.fWall)
    dr.zElev = zElev;
  return fFalse;

LUndoWall:
  // Hit a wall. Before undoing, fire the move into wall event.
  cDialog = ws.cDialog;
  SwapN(dr.x, xwall); SwapN(dr.y, ywall); SwapN(dr.z, zwall);
  fCheck = FCheckEventMove2(ws.iEventWall,
    xold, yold, zold, xinold, yinold, zinold);
  dr.x = xwall; dr.y = ywall; dr.z = zwall;
  if (fCheck)
    return fFalse;

  // Check if able to slide along the wall, instead of just undoing.
  if (dr.fGlance && cDialog == ws.cDialog) {
    fHitX = FHitWall(dr.x, yold, dr.z, &zElevX) || (NAbs(dr.x - xold) > 1 &&
      FHitWall((dr.x + xold) >> 1, yold, dr.z, NULL));
    fHitY = FHitWall(xold, dr.y, dr.z, &zElevY) || (NAbs(dr.y - yold) > 1 &&
      FHitWall(xold, (dr.y + yold) >> 1, dr.z, NULL));
    if (!fHitX || !fHitY) {
      if (fHitX || (!fHitX && !fHitY &&
        NAbs(dr.nOffsetX - xinold) > NAbs(dr.nOffsetY - yinold))) {
        dr.x = xold; dr.nOffsetX = xinold; zElev = zElevY;
      } else {
        dr.y = yold; dr.nOffsetY = yinold; zElev = zElevX;
      }
      goto LCheckEdge;
    }
  }

LUndo:
  dr.x = xold; dr.y = yold; dr.z = zold;
  dr.nOffsetX = xinold; dr.nOffsetY = yinold; dr.nOffsetZ = zinold;
  return fTrue;
}


// Move the dot or current position one unit. Implements the commands on the
// Move Dot and Move Relative submenus.

void DotMove(int wCmd)
{
  CVector v;
  int nInside = dr.nInside, xold, yold, zold, dold, eold, dnew,
    xsav, ysav, zsav, xcur, ycur, zcur, dcur, din, xinold, yinold, zinold,
    xmax, ymax, dmov, nDot, cMove, zEl, zLo, zHi, i;
  long xin, yin;
  flag fMove, fBreak, fT;

  // Free Movement inside display modes: Move or rotate.
  if ((dr.fInside && FBetween(nInside, nInsideFree, nInsideVeryFree)) ||
    dr.fInSmooth) {
    for (cMove = 1 + (!dr.fInSmooth && (wCmd == cmdMove4 || wCmd == cmdMove6 ?
      dr.fBoth : dr.fTwo)); cMove > 0; cMove--) {
      xold = dr.x; yold = dr.y; zold = dr.z;
      xinold = dr.nOffsetX; yinold = dr.nOffsetY; zinold = dr.nOffsetZ;
      switch (wCmd) {
      case cmdMove8: case cmdMoveForward:
      case cmdMove2: case cmdMoveBack:
      case cmdMove7: case cmdMove9: case cmdMove1: case cmdMove3:
        xin = InsideFromA(dr.x) + dr.nOffsetX;
        yin = InsideFromA(dr.y) + dr.nOffsetY;
        din = dr.dir * 90 + dr.nOffsetD;
        i = wCmd >= cmdMove7 && wCmd != cmdMoveBack ? 1 : -1;
        xin -= i * (int)((real)dr.nSpeedXY * RSinD((real)din));
        yin -= i * (int)((real)dr.nSpeedXY * RCosD((real)din));
        dr.x = AFromInside(xin);
        dr.y = AFromInside(yin);
        dr.nOffsetX = (int)(xin - InsideFromA(dr.x));
        dr.nOffsetY = (int)(yin - InsideFromA(dr.y));
        if (wCmd == cmdMove7 || wCmd == cmdMove3)
          goto LFree4;
        else if (wCmd == cmdMove9 || wCmd == cmdMove1)
          goto LFree6;
        break;
      case cmdMove4: LFree4:
        dr.nOffsetD += dr.nSpeedD;
        if (dr.nOffsetD > 45) {
          dr.nOffsetD -= 90;
          DirInc(dr.dir);
        }
        break;
      case cmdMove6: LFree6:
        dr.nOffsetD -= dr.nSpeedD;
        if (dr.nOffsetD <= -45) {
          dr.nOffsetD += 90;
          DirDec(dr.dir);
        }
        break;
      case cmdMoveU:
        if ((dr.f3DNotFree && !dr.fInSmooth) || !dr.f3D) {
          nInside = nInsideSmooth;
          goto LNormal;
        }
        dr.nOffsetZ -= dr.nSpeedZ;
        if (dr.nOffsetZ <= -(dr.zCell >> 1)) {
          dr.nOffsetZ += dr.zCell;
          dr.z -= 1 + dr.fNarrow;
        }
        break;
      case cmdMoveD:
        if ((dr.f3DNotFree && !dr.fInSmooth) || !dr.f3D) {
          nInside = nInsideSmooth;
          goto LNormal;
        }
        dr.nOffsetZ += dr.nSpeedZ;
        if (dr.nOffsetZ > dr.zCell >> 1) {
          dr.nOffsetZ -= dr.zCell;
          dr.z += 1 + dr.fNarrow;
        }
        break;
      case cmdMoveNorth:
      case cmdMoveWest:
      case cmdMoveSouth:
      case cmdMoveEast:
        i = ((wCmd == cmdMoveSouth || wCmd == cmdMoveEast) << 1) |
          (wCmd == cmdMoveWest || wCmd == cmdMoveEast);
        xin = InsideFromA(dr.x) + dr.nOffsetX + xoff[i] * dr.nSpeedXY;
        yin = InsideFromA(dr.y) + dr.nOffsetY + yoff[i] * dr.nSpeedXY;
        dr.x = AFromInside(xin);
        dr.y = AFromInside(yin);
        dr.nOffsetX = (int)(xin - InsideFromA(dr.x));
        dr.nOffsetY = (int)(yin - InsideFromA(dr.y));
        nInside = nInsideFree;
        break;
      case cmdMoveAround:
        dr.dir ^= 2;
        nInside = nInsideFree;
        break;
      }
      if (dr.nEdge == nEdgeTorus) {
        if (!dr.f3D)
          bm.b.Legalize2(&dr.x, &dr.y);
        else {
          while (dr.x < 0)
            dr.x += bm.b.m_x3;
          while (dr.x >= bm.b.m_x3)
            dr.x -= bm.b.m_x3;
          while (dr.y < 0)
            dr.y += bm.b.m_y3;
          while (dr.y >= bm.b.m_y3)
            dr.y -= bm.b.m_y3;
          while (dr.z < 0)
            dr.z += bm.b.m_z3;
          while (dr.z >= bm.b.m_z3)
            dr.z -= bm.b.m_z3;
        }
      }
      if (wCmd == cmdMove8 || wCmd == cmdMove2 || wCmd == cmdMoveU ||
        wCmd == cmdMoveD || wCmd == cmdMoveForward || wCmd == cmdMoveBack ||
        wCmd == cmdMoveNorth || wCmd == cmdMoveWest || wCmd == cmdMoveSouth ||
        wCmd == cmdMoveEast) {
        if (!FUndoIfHitWall(xold, yold, zold, xinold, yinold, zinold, bm.b)) {
          if (dr.fDrag && dr.fOnTemp)
            DotTemp(xold, yold, zold);
        } else
          cMove = 1;
      }
      if (ws.iEventMove2 > 0 && !dr.fInSmooth &&
        (dr.x != xold || dr.y != yold || dr.z != zold))
        FCheckEvent(ws.iEventMove2, dr.x, dr.y, dr.z);
    }
    if (!(dr.fInside && nInside == nInsideVeryFree && !dr.fInSmooth) ||
      (ws.fSpree && !dr.fRedrawAfter))
      SystemHook(hosRedraw);
    return;
  }

  // Draw coordinates: Move or rotate.
  if (ds.fArrow && !dr.fInside) {
    cMove = 1 + (wCmd == cmdMove4 || wCmd == cmdMove6 ? dr.fBoth : dr.fTwo);
    v.Sphere(rDegQuad - (real)ds.theta, (real)ds.phi);
    v.Mul((real)(ds.speedm * cMove));
    switch (wCmd) {
    case cmdMove8:
    case cmdMove7: case cmdMove9:
      ds.hormin += v.NX(); ds.vermin += v.NY(); ds.depmin -= v.NZ();
      if (wCmd == cmdMove7)
        goto LDraw4;
      else if (wCmd == cmdMove9)
        goto LDraw6;
      break;
    case cmdMove2:
    case cmdMove1: case cmdMove3:
      ds.hormin -= v.NX(); ds.vermin -= v.NY(); ds.depmin += v.NZ();
      if (wCmd == cmdMove3)
        goto LDraw4;
      else if (wCmd == cmdMove1)
        goto LDraw6;
      break;
    case cmdMove4: LDraw4:
      ds.theta -= ds.speedr * cMove;
      if (ds.theta < 0)
        ds.theta += nDegMax;
      break;
    case cmdMove6: LDraw6:
      ds.theta += ds.speedr * cMove;
      if (ds.theta >= nDegMax)
        ds.theta -= nDegMax;
      break;
    case cmdMoveU:
      ds.depmin += ds.speedm * cMove;
      break;
    case cmdMoveD:
      ds.depmin -= ds.speedm * cMove;
      break;
    case cmdMoveAround:
      ds.theta += nDegHalf;
      if (ds.theta >= nDegMax)
        ds.theta -= nDegMax;
      break;
    }

    // Rerender line or patch file in memory since draw positioning changed.
    if (ws.fNoDirty) {
      if (!ds.fDidPatch)
        FRenderPerspectiveWire(BFocus(), bm.coor, bm.ccoor);
      else
        FRenderPerspectivePatch(BFocus(), bm.patch, bm.cpatch);
      SystemHook(hosDirtyView);
    }
    return;
  }

  // Standard case: Change the coordinates of the dot.
LNormal:
  xold = dr.x; yold = dr.y; zold = dr.z; dold = dr.dir; eold = dr.zElev;
  nDot = !dr.fInside ? (1 + dr.fTwo) : (1 + dr.fNarrow);
  dnew = dr.dir; dmov = -1;
  fMove = fFalse;
  switch (wCmd) {
  case cmdMove8: wCmd = dr.fInside ? cmdMoveForward : cmdMoveNorth; break;
  case cmdMove2: wCmd = dr.fInside ? cmdMoveBack    : cmdMoveSouth; break;
  }
  if (!dr.fInside) {
    switch (wCmd) {
    case cmdMove4: wCmd = cmdMoveWest; break;
    case cmdMove6: wCmd = cmdMoveEast; break;
    }
  }
  switch (wCmd) {
  case cmdMoveNorth: dnew = dmov = 0; break;
  case cmdMoveWest:  dnew = dmov = 1; break;
  case cmdMoveSouth: dnew = dmov = 2; break;
  case cmdMoveEast:  dnew = dmov = 3; break;
  case cmdMoveForward: dmov = dnew; break;
  case cmdMoveBack:    dmov = dnew ^ 2; break;
  case cmdMove4: dnew = dr.dir + 1 & DIRS1; break;
  case cmdMove6: dnew = dr.dir - 1 & DIRS1; break;
  case cmdMoveU:
    if (dr.f3D)
      dnew = dmov = 4;
    else {
      if (!dr.fWall || dr.zStep < 0 || !FGetWallVar(dr.x, dr.y, dr.z, &zEl,
        &zLo, &zHi) || !(dr.zElev + (dr.zWall >> 1) <= zLo &&
        dr.zElev + (dr.zWall >> 1) + dr.nSpeedZ > zLo))
        dr.zElev += dr.nSpeedZ;
    }
    break;
  case cmdMoveD:
    if (dr.f3D)
      dnew = dmov = 5;
    else {
      if (!dr.fWall || dr.zStep < 0 || !FGetWallVar(dr.x, dr.y, dr.z, &zEl,
        &zLo, &zHi) || !((dr.zElev >= zHi && dr.zElev - dr.nSpeedZ < zHi) ||
        (dr.zElev >= zEl && dr.zElev - dr.nSpeedZ < zEl)))
        dr.zElev -= dr.nSpeedZ;
      if (dr.zElev < dr.zGround)
        dr.zElev = dr.zGround;
    }
    break;
  case cmdMove1:
    if (dr.fInside) {
      dnew = dr.dir - 1 & DIRS1; dmov = dnew ^ 2;
    } else {
      dnew = rgdirDiag[1][dr.dir]; dmov = DIRS4+1;
    }
    break;
  case cmdMove3:
    if (dr.fInside) {
      dnew = dr.dir + 1 & DIRS1; dmov = dnew ^ 2;
    } else {
      dnew = rgdirDiag[2][dr.dir]; dmov = DIRS4+2;
    }
    break;
  case cmdMove7:
    if (dr.fInside) {
      dnew = dr.dir + 1 & DIRS1; dmov = dnew;
    } else {
      dnew = rgdirDiag[0][dr.dir]; dmov = DIRS4+0;
    }
    break;
  case cmdMove9:
    if (dr.fInside) {
      dnew = dr.dir - 1 & DIRS1; dmov = dnew;
    } else {
      dnew = rgdirDiag[3][dr.dir]; dmov = DIRS4+3;
    }
    break;
  case cmdMoveAround:
    dnew = dr.dir ^ 2;
    break;
  case cmdMoveLeft:
    dnew = dmov = bm.b.PeekWall(dr.x, dr.y, dr.z,
      dr.f3D ? dr.dir2 : dr.dir, dr.f3D, -1);
    break;
  case cmdMoveRight:
    dnew = dmov = bm.b.PeekWall(dr.x, dr.y, dr.z,
      dr.f3D ? dr.dir2 : dr.dir, dr.f3D, 1);
    break;
  case cmdMoveRandom:
    dnew = dmov = bm.b.PeekRandom(dr.x, dr.y, dr.z,
      dr.f3D ? dr.dir2 : dr.dir, dr.f3D);
    break;
  case cmdJump4:
  case cmdJump6:
    dmov = dr.dir + (wCmd == cmdJump4 ? 1 : -1) & DIRS1;
    break;
  }
  if (dmov >= 0) {
    dr.x += xoff3[dmov]*nDot;
    dr.y += yoff3[dmov]*nDot;
    dr.z += zoff3[dmov]*nDot;
  }
  dr.dir2 = dnew;
  if (dnew < DIRS)
    dr.dir = dnew;

  // Handle all the various dot settings.
  if (!bm.fColor) {
    if (!dr.f3D) {
      xmax = bm.b.m_x; ymax = bm.b.m_y;
    } else {
      xmax = bm.b.m_x3; ymax = bm.b.m_y3;
    }
  } else {
    xmax = bm.k.m_x; ymax = bm.k.m_y;
  }
  if (!dr.fInside) {

    // Handle overhead view.
    for (cMove = 0; cMove < 1 + dr.fBoth; cMove++) {
      if (dr.nEdge != nEdgeVoid) {
        if (dr.x < 0)
          dr.x = dr.nEdge == nEdgeTorus ? xmax + dr.x : 0;
        else if (dr.x >= xmax)
          dr.x = dr.nEdge == nEdgeTorus ? dr.x - xmax : xmax-1;
        if (dr.y < 0)
          dr.y = dr.nEdge == nEdgeTorus ? ymax + dr.y : 0;
        else if (dr.y >= ymax)
          dr.y = dr.nEdge == nEdgeTorus ? dr.y - ymax : ymax-1;
      }
      if (!dr.f3D) {

        // Handle overhead view, 2D bitmap.
        if (FHitWall(dr.x, dr.y, dr.z, &dr.zElev)) {
          if (dr.fGlance && dr.x != xold && dr.y != yold &&
            (fT = FHitWall(dr.x, yold, dr.z, NULL)) !=
            FHitWall(xold, dr.y, dr.z, NULL)) {
            if (fT)
              dr.x = xold;
            else
              dr.y = yold;
          } else
            CheckEventMove(ws.iEventWall, xold, yold, dr.z, eold, fFalse);
        } else if (dr.fNoCorner && dr.x != xold && dr.y != yold &&
          FHitWall(dr.x, yold, dr.z, NULL) &&
          FHitWall(xold, dr.y, dr.z, NULL) && !dr.fTwo) {
          dr.x = xold; dr.y = yold;
        }
        if (dr.fDrag) {
          BitmapDot(xold, yold);
          BitmapDot(dr.x, dr.y);
        }
      } else {

        // Handle overhead view, 3D bitmap.
        if (dr.nEdge != nEdgeVoid) {
          if (dr.z < 0)
            dr.z = dr.nEdge == nEdgeTorus ? bm.b.m_z3 + dr.z : 0;
          else if (dr.z >= bm.b.m_z3)
            dr.z = dr.nEdge == nEdgeTorus ? dr.z - bm.b.m_z3 : bm.b.m_z3-1;
        }
        if (FHitWall(dr.x, dr.y, dr.z, &dr.zElev))
          CheckEventMove(ws.iEventWall, xold, yold, zold, eold, fFalse);
        if (dr.fDrag) {
          BitmapDot(bm.b.X2(xold, zold), bm.b.Y2(yold, zold));
          BitmapDot(bm.b.X2(dr.x, dr.z), bm.b.Y2(dr.y, dr.z));
        }
      }
      if (dr.fBoth && cMove == 0) {
        fMove = dr.x != xold || dr.y != yold || dr.z != zold;
        xold = dr.x; yold = dr.y; zold = dr.z;
        if (dmov >= 0) {
          dr.x += xoff3[dmov]*nDot;
          dr.y += yoff3[dmov]*nDot;
          dr.z += zoff3[dmov]*nDot;
        }
      }
    }
    dr.nOffsetX = dr.nOffsetY = dr.nOffsetZ = dr.nOffsetD = 0;
  } else {

    // Handle perspective inside view.
    if (!dr.f3D) {

      // Handle perspective inside view, 2D bitmap.
      if (dr.nEdge == nEdgeStop)
        bm.b.Legalize(&dr.x, &dr.y);
      else if (dr.nEdge == nEdgeTorus)
        bm.b.Legalize2(&dr.x, &dr.y);
    } else {

      // Handle perspective inside view, 3D bitmap.
      if (dr.nEdge == nEdgeStop) {
        if (dr.x < 0)
          dr.x = 0;
        else if (dr.x >= Even(bm.b.m_x3)-nDot)
          dr.x = Odd(bm.b.m_x3)-nDot;
        if (dr.y < 0)
          dr.y = 0;
        else if (dr.y >= Even(bm.b.m_y3)-nDot)
          dr.y = Odd(bm.b.m_y3)-nDot;
        if (dr.z < 0)
          dr.z = 0;
        else if (dr.z >= Odd(bm.b.m_z3))
          dr.z = Odd(bm.b.m_z3)-1;
      } else if (dr.nEdge == nEdgeTorus) {
        while (dr.x < 0)
          dr.x += bm.b.m_x3;
        while (dr.x >= bm.b.m_x3)
          dr.x -= bm.b.m_x3;
        while (dr.y < 0)
          dr.y += bm.b.m_y3;
        while (dr.y >= bm.b.m_y3)
          dr.y -= bm.b.m_y3;
        while (dr.z < 0)
          dr.z += bm.b.m_z3;
        while (dr.z >= bm.b.m_z3)
          dr.z -= bm.b.m_z3;
      }
    }
    if (dr.x != xold || dr.y != yold || dr.z != zold)
      if ((dr.fNarrow && FHitWall((xold + dr.x) >> 1, (yold + dr.y) >> 1,
        (zold + dr.z) >> 1, NULL) || FHitWall(dr.x, dr.y, dr.z, &dr.zElev)))
        CheckEventMove(ws.iEventWall, xold, yold, zold, eold, fFalse);
  }

  // Fire events since the dot has moved.
  if (ws.iEventEdge > 0 && bm.b.FLegalLevel(xold, yold, dr.f3D) &&
    !bm.b.FLegalLevel(dr.x, dr.y, dr.f3D))
    CheckEventMove(ws.iEventEdge, xold, yold, zold, eold, fTrue);
  else if (ws.iEventMove > 0 &&
    (dr.x != xold || dr.y != yold || dr.z != zold))
    CheckEventMove(ws.iEventMove, xold, yold, zold, eold, fTrue);

  // If the dot's positioning has changed...
  fMove |= dr.x != xold || dr.y != yold || dr.z != zold;
  if (dr.zElev != eold && !fMove && ws.fNoDirty &&
    !(dr.fInside && dr.nInside == nInsideVeryFree))
    SystemHook(hosRedraw);
  if (fMove || dr.dir != dold) {
    if (fMove)
      dr.cMove++;

    // Update the screen.
    if (dr.fInside && nInside == nInsideSmooth &&
      NDistance(dr.x, xold, xmax) + NDistance(dr.y, yold, ymax) +
      NDistance(dr.z, zold, bm.b.m_z3) <= 2)
      DotSmoothAnimate(wCmd, xold, yold, zold, dold);
    else if (ws.fNoDirty)
      SystemHook(hosRedraw);

    // If Follow Passages set, keep moving forward until the next junction.
    if (dr.fFollow && fMove && (dr.x == xold || dr.y == yold)) {
      if (dr.z == zold)
        dnew = (dr.x != xold) + ((dr.x > xold || dr.y > yold) << 1);
      else
        dnew = DIRS + (dr.z > zold);
      fBreak = fFalse;
      while (!fBreak && (dr.x != xold || dr.y != yold || dr.z != zold) &&
        (dr.x != dr.x2 || dr.y != dr.y2 || dr.z != dr.z2)) {

        // Pause for the Pixel Display Delay time.
        if (!dr.fInside && dr.fDot)
          for (i = 0; i < ws.nDisplayDelay; i++)
            SystemHook(hosYield);

        // Pressing Space will interrupt passage following.
        if (NGetVariableW(vosEventSpace))
          break;

        if (!dr.f3D) {
          if (!(dr.x > 0 && dr.y > 0 &&
            dr.x < bm.b.m_x-1 && dr.y < bm.b.m_y-1))
            break;
        } else {
          if (!bm.b.FLegalMaze3(dr.x, dr.y, dr.z))
            break;
        }
        xcur = xsav = dr.x; ycur = ysav = dr.y; zcur = zsav = dr.z;
        dcur = dr.dir;
        dnew = bm.b.FollowPassage(&xsav, &ysav, &zsav, dnew, dr.f3D);
        if (FCheckEvent(ws.iEventFollow, xsav, ysav, zsav))
          break;
        dr.x = xsav; dr.y = ysav; dr.z = zsav;
        if (dnew < 0)
          break;
        dr.dir2 = dnew;
        if (dnew < DIRS)
          dr.dir = dnew;
        if (nDot > 1) {
          dr.x += xoff3[dr.dir2];
          dr.y += yoff3[dr.dir2];
          dr.z += zoff3[dr.dir2];
          if (dr.f3D ? !bm.b.FLegalCube(dr.x, dr.y, dr.z) :
            !bm.b.FLegal(dr.x, dr.y)) {
            dr.x = xsav; dr.y = ysav; dr.z = zsav;
            fBreak = fTrue;
          }
        }
        if (dr.x != xcur || dr.y != ycur || dr.z != zcur)
          dr.cMove++;
        if (dr.fDrag)
          BitmapDot(dr.x, dr.y);

        // Update the screen for each new coordinate of the dot.
        if (dr.fInside && nInside == nInsideSmooth) {
          fT = dr.fRedrawAfter; dr.fRedrawAfter &= (ws.iEventFollow > 0);
          DotSmoothAnimate(cmdMove8, xcur, ycur, zcur, dcur);
          dr.fRedrawAfter = fT;
        } else
          SystemHook(hosRedraw);
      }
    }
    if (ws.iEventMove2 > 0 && fMove)
      FCheckEvent(ws.iEventMove2, dr.x, dr.y, dr.z);
  }
}


// Jump the dot or current position by several units, or sidestep by one unit.
// Implements the commands on the Jump Dot submenu.

void DotJump(int wCmd)
{
  CVector v;
  int xold, yold, din, xinold, yinold, xmax, ymax, cMove, i;
  long xin, yin;
  flag fSav;

  // Free Movement inside display modes: Handle sidestepping.
  if ((dr.fInside && FBetween(dr.nInside, nInsideFree, nInsideVeryFree)) ||
    dr.fInSmooth) {
    for (cMove = 1 + !dr.fInSmooth * dr.fTwo; cMove > 0; cMove--) {
      xold = dr.x; yold = dr.y;
      xinold = dr.nOffsetX; yinold = dr.nOffsetY;
      switch (wCmd) {
      case cmdJump4:
      case cmdJump6:
        xin = InsideFromA(dr.x) + dr.nOffsetX;
        yin = InsideFromA(dr.y) + dr.nOffsetY;
        din = dr.dir * 90 + dr.nOffsetD;
        i = wCmd == cmdJump4 ? 1 : -1;
        xin -= i * (int)((real)dr.nSpeedXY * RCosD((real)din));
        yin += i * (int)((real)dr.nSpeedXY * RSinD((real)din));
        dr.x = AFromInside(xin);
        dr.y = AFromInside(yin);
        dr.nOffsetX = (int)(xin - InsideFromA(dr.x));
        dr.nOffsetY = (int)(yin - InsideFromA(dr.y));
        if (dr.nEdge == nEdgeTorus)
          bm.b.Legalize2(&dr.x, &dr.y);
        break;
      }
      if ((wCmd == cmdJump4 || wCmd == cmdJump6)) {
        if (!FUndoIfHitWall(xold, yold, dr.z, xinold, yinold, dr.nOffsetZ,
          bm.b) && dr.fDrag && dr.fOnTemp)
          DotTemp(xold, yold, dr.z);
      }
    }
    if (ws.iEventMove2 > 0 && !dr.fInSmooth && (dr.x != xold || dr.y != yold))
      FCheckEvent(ws.iEventMove2, dr.x, dr.y, dr.z);
    if (!(dr.fInside && dr.nInside == nInsideVeryFree && !dr.fInSmooth))
      SystemHook(hosRedraw);
    return;
  }

  // Draw coordinates: Sidestep or pitch forward/backward.
  if (ds.fArrow && !dr.fInside) {
    cMove = 1 + (wCmd == cmdJump8 || wCmd == cmdJump2 ? dr.fBoth : dr.fTwo);
    v.Sphere((real)ds.theta, (real)ds.phi);
    v.Mul((real)(ds.speedm * cMove));
    switch (wCmd) {
    case cmdJump8:
      ds.phi -= ds.speedr * cMove;
      if (ds.phi < -rDegHalf)
        ds.phi += nDegMax;
      break;
    case cmdJump2:
      ds.phi += ds.speedr * cMove;
      if (ds.phi >= rDegHalf)
        ds.phi -= nDegMax;
      break;
    case cmdJump4:
      ds.hormin -= v.NX(); ds.vermin += v.NY();
      break;
    case cmdJump6:
      ds.hormin += v.NX(); ds.vermin -= v.NY();
      break;
    }

    // Rerender line or patch file in memory since draw positioning changed.
    if (ws.fNoDirty) {
      if (!ds.fDidPatch)
        FRenderPerspectiveWire(BFocus(), bm.coor, bm.ccoor);
      else
        FRenderPerspectivePatch(BFocus(), bm.patch, bm.cpatch);
      SystemHook(hosDirtyView);
    }
    return;
  }

  // Standard case: Change the coordinates of the dot.
  xold = dr.x; yold = dr.y;
  if (!dr.fInside) {
    switch (wCmd) {
    case cmdJump1:
      dr.x -= zJump; dr.y += zJump; dr.dir = rgdirDiag[1][dr.dir]; break;
    case cmdJump2:
                     dr.y += zJump; dr.dir = 2;                    break;
    case cmdJump3:
      dr.x += zJump; dr.y += zJump; dr.dir = rgdirDiag[2][dr.dir]; break;
    case cmdJump4:
      dr.x -= zJump;                dr.dir = 1;                    break;
    case cmdJump6:
      dr.x += zJump;                dr.dir = 3;                    break;
    case cmdJump7:
      dr.x -= zJump; dr.y -= zJump; dr.dir = rgdirDiag[0][dr.dir]; break;
    case cmdJump8:
                     dr.y -= zJump; dr.dir = 0;                    break;
    case cmdJump9:
      dr.x += zJump; dr.y -= zJump; dr.dir = rgdirDiag[3][dr.dir]; break;
    }
  } else {

    // In perspective inside mode, the Dot Jump commands move one unit at a
    // time, so are implemented in DotMove.
    switch (wCmd) {
    case cmdJump4:
    case cmdJump6:
      // Sidestepping in perspective mode is explicitly covered in DotMove.
      DotMove(wCmd);
      return;
    case cmdJump8:
    case cmdJump2:
      // Shift+move is like normal move except with Follow Passages toggled.
      fSav = dr.fFollow;
      inv(dr.fFollow);
      DotMove(wCmd == cmdJump8 ? cmdMove8 : cmdMove2);
      dr.fFollow = fSav;
      return;
    }
  }

  // Handle the dot moving off the edge of the bitmap or level.
  if (dr.f3D) {
    xmax = bm.b.m_x3; ymax = bm.b.m_y3;
  } else if (!bm.fColor) {
    xmax = bm.b.m_x; ymax = bm.b.m_y;
  } else {
    xmax = bm.k.m_x; ymax = bm.k.m_y;
  }
  if (dr.nEdge != nEdgeVoid) {
    if (dr.x < 0)
      dr.x = dr.nEdge == nEdgeTorus ? xmax + dr.x : 0;
    else if (dr.x >= xmax)
      dr.x = dr.nEdge == nEdgeTorus ? dr.x - xmax : xmax-1;
    if (dr.y < 0)
      dr.y = dr.nEdge == nEdgeTorus ? ymax + dr.y : 0;
    else if (dr.y >= ymax)
      dr.y = dr.nEdge == nEdgeTorus ? dr.y - ymax : ymax-1;
  }

  // Fire events since the dot has moved.
  if (ws.iEventEdge > 0 && bm.b.FLegalLevel(xold, yold, dr.f3D) &&
    !bm.b.FLegalLevel(dr.x, dr.y, dr.f3D))
    CheckEventMove(ws.iEventEdge, xold, yold, dr.z, dr.zElev, fTrue);
  else if (ws.iEventMove > 0 && (dr.x != xold || dr.y != yold))
    CheckEventMove(ws.iEventMove, xold, yold, dr.z, dr.zElev, fTrue);
  CenterWithinCell();
  if (dr.x != xold || dr.y != yold)
    SystemHook(hosRedraw);
}


// The dot has moved or rotated one unit. Do smooth animation from the old
// positioning to the current positioning. This implements the intermediate
// frames seen in the Smooth Movement inside display mode.

void DotSmoothAnimate(int wCmd, int xold, int yold, int zold, int dold)
{
  int xnew, ynew, znew, dnew, i, iMax, dMax, dsav, zMax, zsav, wCmdT;
  flag fRedrawAfter;

  dr.fInSmooth = fTrue;
  if (dr.fNarrow) {
    xold |= 1; yold |= 1; zold &= ~1; dr.x |= 1; dr.y |= 1; dr.z &= ~1;
  }
  xnew = dr.x; ynew = dr.y; znew = dr.z; dnew = dr.dir;
  dr.x = xold; dr.y = yold; dr.z = zold; dr.dir = dold;

  // Animate a rotation if the direction has changed.
  if (dnew != dold) {
    if (((dold + 1) & DIRS1) == dnew)
      wCmdT = cmdMove4;
    else if (((dold - 1) & DIRS1) == dnew)
      wCmdT = cmdMove6;
    else {

      // Turning around 180 degrees, i.e. at a dead end. If following the
      // right hand wall, rotate to the left. If following left hand wall,
      // rotate right, otherwise pick one at random.
      if (wCmd == cmdMoveRight)
        wCmdT = cmdMove4;
      else if (wCmd == cmdMoveLeft)
        wCmdT = cmdMove6;
      else
        wCmdT = Rnd(0, 1) ? cmdMove4 : cmdMove6;
    }
    iMax = NAbs(dnew - dold);
    if (iMax >= 3)
      iMax = 1;
    dMax = iMax*90;
    iMax *= dr.nFrameD;
    dsav = dr.nSpeedD;
    for (i = 0; i < iMax; i++) {
      dr.nSpeedD = (i+1)*dMax/iMax - i*dMax/iMax;
      DoCommand(wCmdT);
    }
    dr.nSpeedD = dsav;
  }

  // Animate movement if the location has changed.
  if (xnew != xold || ynew != yold) {
    if (wCmd == cmdJump4 || wCmd == cmdJump6 || wCmd == cmdMove2)
      wCmdT = wCmd;
    else
      wCmdT = (wCmd == cmdMove1 || wCmd == cmdMove3 || wCmd == cmdMoveBack) ?
        cmdMove2 : cmdMove8;
    zMax = (dr.zCell + (dr.fNarrow ? dr.zCellNarrow : dr.zCell)) *
      (NDistance(xnew, xold, dr.f3D ? bm.b.m_x3 : bm.b.m_x) +
      NDistance(ynew, yold, dr.f3D ? bm.b.m_y3 : bm.b.m_y)) / 2;
    iMax = dr.nFrameXY;
    zsav = dr.nSpeedXY;
    fRedrawAfter = dr.fRedrawAfter && ws.iEventMove2 > 0;
    for (i = 0; i < iMax - fRedrawAfter; i++) {
      dr.nSpeedXY = (i+1)*zMax/iMax - i*zMax/iMax;
      DoCommand(wCmdT);
    }
    if (fRedrawAfter) {
      dr.x = xnew; dr.y = ynew;
      dr.nOffsetX = dr.nOffsetY = 0;
    }
    dr.nSpeedXY = zsav;
  }

  // Animate a level change if the level height has changed.
  if (znew != zold) {
    wCmdT = zold - znew > 0 ? cmdMoveU : cmdMoveD;
    zMax = dr.zCell;
    iMax = dr.nFrameZ;
    zsav = dr.nSpeedZ;
    for (i = 0; i < iMax; i++) {
      dr.nSpeedZ = (i+1)*zMax/iMax - i*zMax/iMax;
      DoCommand(wCmdT);
    }
    dr.nSpeedZ = zsav;
  }
  dr.fInSmooth = fFalse;

  // An event may have stopped animation part way between cells.
  if (dr.nOffsetX != 0 || dr.nOffsetY != 0)
    SystemHook(hosRedraw);
}


// Given a window pixel (that the mouse is over) move the dot one unit toward
// that pixel, so the dot moves toward the mouse.

void DotMouseChase(int xp, int yp)
{
  int rgxi[4], rgyi[4], xi, yi, x, y, c, i, j;

  // Determine preferred offset to apply to dot
  x = xp - bm.xpDot;
  x = x / bm.xpPoint - (x < 0);
  y = yp - bm.ypDot;
  y = y / bm.ypPoint - (y < 0);
  xi = NSgn(x); yi = NSgn(y);
  if (xi == 0 && yi == 0)
    return;

  // Determine a few alternate offsets in case the first doesn't work
  c = 3 + (xi != 0 && yi != 0);
  for (i = 0; i < c; i++) {
    rgxi[i] = xi; rgyi[i] = yi;
  }
  if (c >= 4) {
    if (NAbs(x) >= NAbs(y)) {
      rgyi[1] = 0; rgxi[2] = 0; neg(rgyi[3]);
    } else {
      rgxi[1] = 0; rgyi[2] = 0; neg(rgxi[3]);
    }
    if (NAbs(x) == NAbs(y)) // Pure diagonal case only has three options
      c = 3;
  } else {
    if (xi == 0) {
      rgxi[1] = -1; rgxi[2] = 1;
    } else {
      rgyi[1] = -1; rgyi[2] = 1;
    }
    if (NAbs(x + y) <= 1) // Don't vibrate back and forth next to on pixel
      c = 1;
  }

  // Test each offset until one free and can move in that direction
  for (i = 0; i < c; i++) {
    xi = rgxi[i]; yi = rgyi[i];
    if (dr.fNoDiagonal && xi != 0 && yi != 0)
      continue;
    if (dr.fGlance && !dr.fInside) {
      if (FHitWall(dr.x + xi, dr.y + yi, dr.z, NULL))
        continue;
      if (dr.fNoCorner && xi != 0 && yi != 0 &&
        FHitWall(dr.x + xi, dr.y, dr.z, NULL) &&
        FHitWall(dr.x, dr.y + yi, dr.z, NULL) && !dr.fTwo)
        continue;
    }
    j = (yi + 1) * 3 + (xi + 1);
    Assert(j != 4);
    DoCommand(rgcmdMouse[j]);
    break;
  }
}


// Given a range of characters containing a list of space separated strings,
// add the strings to a lookup trie. Implements the DefineConst operation.

flag FCreateConstTrie(CONST char *szIn, int cchIn)
{
  char szStrs[cchSzOpr*4], **rgsz = NULL, *pch;
  int csz = 0, csMax = 16000, isz, cs, i, n;
  flag fRet = fFalse, fPlus;

  // Count strings
  if (cchIn >= cchSzOpr*4)
    goto LDone;
  CopyRgchToSz(szIn, cchIn, szStrs, cchSzOpr*4);
  for (pch = szStrs; *pch; pch += (*pch != chNull)) {
    if (!FDigitCh(*pch) && *pch != '+')
      csz++;
    while (*pch && *pch != ' ')
      pch++;
  }

  // Compose list of strings
  rgsz = RgAllocate(csz, char *);
  if (rgsz == NULL)
    goto LDone;
  if (ws.rgnTrieConst != NULL)
    DeallocateP(ws.rgnTrieConst);
  ws.rgnTrieConst = RgAllocate(csz, int);
  if (ws.rgnTrieConst == NULL)
    goto LDone;
  pch = szStrs;
  for (isz = 0, i = 0; isz < csz; isz++, i++) {

    // Special: A number redefines lookup result for the following strings.
    // Special: +number adds delta to current lookup result for next strings.
    while (FDigitCh(*pch) || *pch == '+') {
      fPlus = (*pch == '+');
      pch += fPlus;
      for (n = 0; *pch && *pch != ' '; pch++)
        n = n * 10 + (*pch - '0');
      i = fPlus ? i + n - 1 : n;
      if (*pch)
        pch++;
    }
    rgsz[isz] = pch;
    ws.rgnTrieConst[isz] = i;
    while (*pch && *pch != ' ')
      pch++;
    if (*pch)
      *pch++ = chNull;
  }

  // Compose trie
  if (ws.rgsTrieConst != NULL)
    DeallocateP(ws.rgsTrieConst);
  ws.rgsTrieConst = RgAllocate(csMax, short);
  if (ws.rgsTrieConst == NULL)
    goto LDone;
  cs = CsCreateTrie((CONST uchar **)rgsz, csz, ws.rgsTrieConst, csMax,
    fFalse);

  // Sanity check trie to ensure all string lookups return expected results
  for (isz = 0; isz < csz; isz++) {
    i = ILookupTrieSz(ws.rgsTrieConst, rgsz[isz], fFalse);
    if (i != isz) {
      PrintSzNN_E("Input string %d maps to index %d.", isz, i);
      goto LDone;
    }
  }

  fRet = fTrue;
LDone:
  if (rgsz != NULL)
    DeallocateP(rgsz);
  return fRet;
}


#ifndef WIN
/*
******************************************************************************
** Command Line Version
******************************************************************************
*/

// Starting point for the command line version of the program.

int main(int argc, char *argv[])
{
  char szLine[cchSzOpr], *pch = szLine;
  int iarg = 1;

  // Initialize globals
  ws.szAppName = szDaedalus;
  ws.szFileTemp = szFileTempCore;

  // Process command line
  szLine[0] = chNull;
  for (iarg = 1; iarg < argc; iarg++) {
    //printf("%d: '%s'\n", iarg, argv[iarg]);
    sprintf(SO(pch, szLine), "%s%s", argv[iarg], iarg < argc ? " " : "");
    while (*pch)
      pch++;
  }
  //printf("Command line: '%s'\n", szLine);

  RunCommandLine(szLine, NULL);

LLoop:
  if (ws.iEventQuit > 0) {
    ws.fQuitting = fFalse;
    RunMacro(ws.iEventQuit);
    if (LVar(iLetterZ) != 0) {
      DoCommandW(cmdCommand);
      goto LLoop;
    }
  }
  if (ws.fNoExit) {
    DoCommandW(cmdCommand);
    goto LLoop;
  }
}


// Display a line of text on the screen.

int PrintSzCore(CONST char *sz, int nPriority)
{
  char szTitle[cchSzDef], szT[cchSzDef], *pchTitle, *pchT;

  // Don't show normal priority messages if Ignore Messages is active.
  if (ws.fIgnorePrint && nPriority == nPrintNormal)
    goto LDone;

  // Figure out what to show as a prefix in the line of text.
  pchTitle = NULL;
  if (ws.nTitleMessage >= 0) {
    pchT = SzVar(ws.nTitleMessage);
    if (pchT != NULL) {
      CopySz(pchT, szT, cchSzDef);
      pchTitle = szT;
    }
  }
  if (pchTitle == NULL || *pchTitle == chNull)
    pchTitle = ws.szAppName;
  switch (nPriority) {
  case nPrintWarning:
    sprintf(S(szTitle), "%s Warning", pchTitle);
    break;
  case nPrintError:
    sprintf(S(szTitle), "%s Error", pchTitle);
    break;
  default:
    sprintf(S(szTitle), "%s", pchTitle);
    break;
  }

  // Actually print the text.
  printf("%s: %s\n", ws.szTitle != NULL ? ws.szTitle : szTitle, sz);
  ws.cDialog++;

LDone:
  ws.szTitle = NULL;
  return 0;
}


// Allocate a memory buffer of a given size, in an OS independent manner.

void *PAllocate(long lcb)
{
  char sz[cchSzMax];
  void *pv;

#ifdef DEBUG
  pv = malloc(lcb + sizeof(dword)*3);
#else
  pv = malloc(lcb);
#endif

  // Handle success or failure of the allocation.
  if (pv == NULL) {
    sprintf(S(sz), "Failed to allocate memory (%ld bytes).\n", lcb);
    PrintSz_E(sz);
  } else {
    us.cAlloc++;
    us.cAllocTotal++;
    us.cAllocSize += lcb;
  }

#ifdef DEBUG
  // Put sentinels at ends of allocation to check for buffer overruns.
  *(dword *)pv = dwCanary;
  *(dword *)((byte *)pv + sizeof(dword)) = lcb;
  *(dword *)((byte *)pv + sizeof(dword)*2 + lcb) = dwCanary;
  return (byte *)pv + sizeof(dword)*2;
#else
  return pv;
#endif
}


// Free a memory buffer allocated with PAllocate.

void DeallocateP(void *pv)
{
  Assert(pv != NULL);
#ifdef DEBUG
  // Ensure buffer wasn't overrun during its existence.
  void *pvSys;
  dword lcb, dw;
  pvSys = (byte *)pv - sizeof(dword)*2;
  Assert(pvSys != NULL);
  dw = *(dword *)pvSys;
  Assert(dw == dwCanary);
  lcb = *(dword *)((byte *)pvSys + sizeof(dword));
  dw = *(dword *)((byte *)pv + lcb);
  Assert(dw == dwCanary);
  free(pvSys);
#else
  free(pv);
#endif
  us.cAlloc--;
}


// Redraw the screen, if Allow Partial Screen Updates is on. This does nothing
// for the command line version of the program, since there's no display.

void UpdateDisplay()
{
}


// Draw a monochrome or color pixel on the display. This does nothing for the
// command line version of the program, since there's no display.

void ScreenDot(int x, int y, bit o, KV kv)
{
}


// Change a pixel in the main bitmap.

void BitmapDotCore(int x, int y, bit o)
{
  if (!bm.b.FLegal(x, y))
    return;
  bm.b.Set(x, y, o);
}


// Draw a spot, updating bitmaps appropriately.

void BitmapDot(int x, int y)
{
  if (dr.fOnTemp) {

    // Draw a spot on the temporary bitmap.
    if (bm.b2.FNull())
      bm.b2.FBitmapCopy(bm.b);
    if (!bm.b2.FNull()) {
      if (!dr.fBig)
        bm.b2.Set(x, y, dr.fSet);
      else
        bm.b2.Block(x-1, y-1, x+1, y+1, dr.fSet);
    }
  } else if (!bm.fColor) {

    // Draw a spot on the main bitmap and on the screen.
    if (!dr.fBig)
      BitmapDotCore(x, y, dr.fSet);
    else
      bm.b.Block(x-1, y-1, x+1, y+1, dr.fSet);
  } else {

    // Draw a spot on the color bitmap and on the screen.
    if (!bm.k.FNull() && bm.k.FLegal(x, y)) {
      if (!dr.fBig)
        bm.k.Set(x, y, kvSet);
      else
        bm.k.Block(x-1, y-1, x+1, y+1, kvSet);
    }
  }
}


// Bring up a color picker dialog, and return the color selected. Does nothing
// for the command line version of the program, since there's no interface.

KV KvDialog()
{
  return kvBlack;
}


// Open a Daedalus script, passing it the given file.

flag FBootExternal(CONST char *szFile, flag fInternal)
{
  return FFileOpen(cmdOpenScript, szFile, NULL);
}


// Execute a system command. This gets run when a command is automatically
// invoked via scripting.

flag DoCommandW(int wCmd)
{
  char sz[cchSzMax], *pch;

  // Implement the "Enter Command Line" dialog in the command line version
  // of the program, by prompting for a command line in the console.
  if (wCmd == cmdCommand) {
    printf("Enter Command Line: ");
    if (fgets(sz, cchSzMax, stdin) == NULL)
      return fFalse;
    for (pch = sz; *pch; pch++)
      ;
    while (pch >= sz && *pch < ' ')
      pch--;
    *(pch + 1) = chNull;
    RunCommandLine(sz, NULL);
    return fTrue;
  }
  return fFalse;
}


// Set a system variable to a numeric value.

void DoSetVariableW(int ivos, int n)
{
}


// Get the value of a system variable, which will be stored in a numeric
// return parameter.

int NGetVariableW(int ivos)
{
  switch (ivos) {
  case vosGetTick:
    return (int)clock();
  }
  return 0;
}


// Execute a system operation given parameters.

void DoOperationW(int ioos, CONST char *rgch, int cch, int n1, int n2)
{
}


// Execute a simple system specific action.

void SystemHook(int ihos)
{
}


// Change to the system wait cursor, or restore the previous cursor.

void HourglassCursor(size_t *pcursor, flag fSet)
{
}


// Implements the various commands and operations that open files.

flag FFileOpen(int wCmd, CONST char *szFile, FILE *fileIn)
{
  FILE *file = NULL;

  char sz[cchSzMax];

  // Get the file handle to read from.
  if (fileIn != NULL)

    // For embedded files in scripts, there's already an open file handle.
    file = fileIn;
  else {
    if (szFile == NULL) {
      if (ws.rgszStartup != NULL) {
        Assert(wCmd != cmdOpen);
        goto LLoad;
      }
      return fFalse;
    }

    // Open the file for reading given its name.
    file = FileOpen(szFile, "rb");
    if (file == NULL) {
      sprintf(S(sz), "The file %s could not be opened.", szFile);
      PrintSz_E(sz);
      return fFalse;
    }
  }

LLoad:
  return FReadFile(wCmd, file, file != NULL && fileIn == NULL);
}


// Implements the various commands that save files.

flag FFileSave(int wCmd, CONST char *szFile)
{
  if (szFile == NULL)
    return fFalse;
  if (!FWriteFile(bm.b, bm.k, wCmd, szFile, szFile))
    return fFalse;
  return fTrue;
}
#endif


/*
******************************************************************************
** Game Routines
******************************************************************************
*/

#define zVis 16   // Visibility distance
#define dobj 17   // Ammo to launcher offset
#define zClimb 64 // Vertical step units

enum _bitmapcustom {
  bmpWall  = 0,  // Block side texture bitmap
  bmpVar   = 1,  // Wall height bitmap
  bmpElev  = 2,  // Floor elevation bitmap
  bmpFloor = 3,  // Floor texture bitmap
  bmpBlock = 4,  // Block top texture bitmap
  bmpSide  = 5,  // Floor side texture bitmap
  bmpLit   = 6,  // Fog lit location bitmap
  bmpInv   = 7,  // Inventory bitmap
  bmpAlly  = 8,  // Faction bitmap
  bmpH2O   = 9,  // Water location bitmap
  bmpWasp  = 10, // Wasp nest bitmap
  bmpTrap  = 11, // Net trap bitmap
  bmpFire  = 12, // Firewall side texture
  bmpFire2 = 13, // Firewall top texture
};

enum _variablecustom {
  numTurn  = 16,   // @p: Turn count
  numLeft  = 19,   // @s: Tribues left
  numStart = 37,   // @SK: Start turn
  numEarly = 63,   // @Sk: Early death chance
  numRng   = 70,   // @Sr: Throwable weapon range distance
  numFinal = 78,   // @Sz: Firewall finale start turn
  numAlly  = 83,   // Alliance proposal probability
  numTrap  = 84,   // Stepping into trap probability
  numMuttS = 91,   // Whether mutts can swim
  numMuttC = 92,   // Whether mutts can climb
  numTube  = 118,  // Launch tube color
  numCorn  = 119,  // Cornucopia color
  numMaze  = 120,  // Maze color
  numDists = 152,  // Max Districts possible
  numDist  = 153,  // Districts in play
  numPer   = 154,  // Tributes per District
  numLaun  = 164,  // Launch tube radius
  numFMax  = 165,  // Maximum food allowed
  numHPMax = 166,  // Maximum health allowed
  numInv   = 167,  // Inventory size
  numDmg   = 200,  // Weapon damage
  numUsage = 250,  // Weapon usage
  numFave  = 300,  // Weapon favored district
  numMater = 350,  // Item material
  numCoor  = 900,  // Tribute X & Y coordinates
  numElev  = 1200, // Tribute Z coordinate
  numHP    = 1500, // Tribute hitpoints
  numFood  = 1800, // Tribute food
  numKill  = 2100, // Tribute kills
  numWield = 2400, // Tribute wielded weapon index
  numWear  = 2700, // Tribute worn item index
  numNet   = 3000, // Tribute netting
  numDest  = 3300, // Tribute destination coordinates
  numWasp  = 3600, // Tribute hallucination
  numLead  = 3900, // Tribute leadership
};

enum _stringcustom {
  strX = 3, // X increments
  strY = 4, // Y increments
};

enum _action {
  actWait  = 0,  // Do nothing and wait
  actUse   = 1,  // Use item
  actGet   = 2,  // Pick up whatever standing over
  actMove  = 3,  // Move or melee attack in direction
  actThrow = 4,  // Throw item in direction
  actDrop  = 5,  // Drop item
  actDown  = 6,  // Climb down
  actUp    = 7,  // Climb up
  actGive  = 8,  // Give item in direction
  actDig   = 9,  // Dig
  actFire  = 10, // Fire weapon in direction
  actFree  = 11, // Free tribute from net
  actAlly  = 12, // Propose alliance
  actLead  = 13, // Claim leadership
};

enum _object {
  objFist  = 0,  // Fist weapon (empty slot)
  objRock  = 1,  // Rock ammo
  objDart  = 2,  // Dart ammo
  objArrow = 3,  // Arrow ammo
  objBull  = 4,  // Gun ammo
  objGren  = 5,  // Grenade explosive
  objNet   = 6,  // Net weapon
  objClub  = 7,  // Club weapon
  objTorch = 8,  // Blowtorch weapon
  objBrick = 9,  // Brick weapon
  objPick  = 10, // Pick weapon
  objTeeth = 11, // Mutt teeth
  objKnife = 12, // Knife weapon
  objMace  = 13, // Mace weapon
  objSpear = 14, // Spear weapon
  objAxe   = 15, // Axe weapon
  objSword = 16, // Sword weapon
  objTrid  = 17, // Trident weapon
  objSling = 18, // Slingshot launcher
  objBlow  = 19, // Blowgun launcher
  objBow   = 20, // Bow launcher
  objGun   = 21, // Gun launcher
  objBerry = 22, // Berries
  objNight = 23, // Nightlock berries
  objBread = 24, // Loaf of bread
  objFish  = 25, // Fish
  objHeal  = 26, // First-aid kit
  objGogg  = 27, // Nightvision goggles
  objHelm  = 28, // Helmet
  objArmor = 29, // Body armor
  objPara  = 30, // Parachute
  objMine0 = 31, // Landmine (inactive)
  objMine1 = 32, // Landmine (active)
  objMax   = 33, // Number of objects
};

enum _desire {
  desPunch   = 0,
  desUp      = 1,
  desDown    = 9999,
  desStart   = 10000,
  desChase   = 10000,
  desBetray  = 30000 + 0,
  desLead    = 30000 + 80,
  desAlly    = 30000 + 90,
  desGive    = 30000 + 100,
  desDrop    = 30000 + 110,
  desDig     = 30000 + 120,
  desMine    = 30000 + 130,
  desWear    = 30000 + 140,
  desEat     = 30000 + 150,
  desHeal    = 30000 + 160,
  desWield   = 30000 + 170,
  desGet     = 30000 + 180,
  desFree    = 30000 + 190,
  desMelee   = 30000 + 500,
  desThrow   = 30000 + 500,
  desFlee    = 40000,
  desGiveHi  = 60000 + 940,
  desGetHi   = 60000 + 950,
  desWieldHi = 60000 + 960,
  desFleeHi  = 60000 + 970,
  desHealHi  = 60000 + 980,
  desEatHi   = 60000 + 990,
};

#define Xoff(d) (ws.rgszVar[strX][d] - '1')
#define Yoff(d) (ws.rgszVar[strY][d] - '1')
#define ZElev(n) ws.rglVar[numElev + (n)]
#define NWater(x, y) \
  (ws.rgbMask[bmpH2O].Get(x, y) ? RgbB(bm.k2.Get(x, y)) : 0)
#define FFirewall(x, y) (ws.rgcTexture[bmpBlock].Get(x, y) == bmpFire2)
#define NDist(i) (((i) >= nTribMax ? nDistMax : (i) / nPer) + 1)
#define NTrib(k) (255 - (k))
#define NObj(k) (RgbG(k) & 127)
#define NWield(i) ws.rglVar[numWield + (i)]
#define FObjInInv(o) ((grfObjInv & (1 << (o))) != 0)
#define FObjsInInv(o1, o2) ((grfObjInv & ((1 << (o1)) | (1 << (o2)))) != 0)
#define ObjInvN(i, n) ((i) < 0 ? 0 : ws.rgcTexture[bmpInv].Get(n, i))
#define ObjInv(i) ObjInvN(i, nSelf)
#define WepDmgN(o, n) \
  (ws.rglVar[numDmg + (o)] + (ws.rglVar[numFave + (o)] == NDist(n))*2)
#define WepDmg(o) WepDmgN(o, nSelf)
#define DmgN(n) (ws.rglVar[numNet + (n)] > 0 ? 1 : \
  WepDmgN(ObjInvN(NWield(n), n), n))
#define FactionN(i, n) ws.rgcTexture[bmpAlly].Get(n, i)
#define Faction(i) FactionN(i, nSelf)
#define FAlly(n) ((n) <= 100)
#define FEnemy(n) ((n) > 150)
#define FWep(o)       ((o) != objFist && (ws.rglVar[numUsage + (o)] > 0))
#define FWepMelee(o)  ((o) != objFist && (ws.rglVar[numUsage + (o)] & 9) == 1)
#define FWepThrow(o)  ((o) != objFist && (ws.rglVar[numUsage + (o)] & 2) > 0)
#define FWepAmmo(o)   ((o) != objFist && (ws.rglVar[numUsage + (o)] & 4) > 0)
#define FWepLaunch(o) ((o) != objFist && (ws.rglVar[numUsage + (o)] & 8) > 0)
#define FWepAuto(o)   ((ws.rglVar[numUsage + (o)] & 24) == 24)
#define FConsum(o) \
  ((o) == objBerry || (o) == objBread || (o) == objFish || (o) == objHeal)
#define FFisher(o) ((o) == objNet || (o) == objSpear || (o) == objTrid)
#define PowerN(n) (ws.rglVar[numHP + (n)] * DmgN(n))
#define NCompare(n) (nPower * 100 / PowerN(n))
#define CheckWrap(x, y) if (dr.nEdge == nEdgeTorus) bm.b.Legalize2(x, y)
#define Desire(a, i, d, p) if ((p) > nPriority) \
  { nAction = (a); nItem = (i); nDir = (d); nPriority = (p); }
#define CheckBad(f, n) if (f) { ws.rglVar[iLetterZ] = -(n); return fFalse; }

// Determine the best move for a computer controlled Hunger Games tribute.
// Contains AI specialized to the Hunger Games script, and implements the
// Hunger operation. Loop over all possible actions, assigning a value to
// each, then select whichever potential action has the highest value.

flag FHungerGame(int nInput)
{
  CMaz b;
  int nAction = actWait, nItem = 0, nDir = 0, nPriority = -1,
    rgnMove[DIRS2], rgnDesObj[objMax], rgnConObj[objMax], rgnValObj[objMax],
    rgnDesDir[DIRS2], nSelf, zLit, nDistMax, nDist, nPer, nTribMax, nTribLeft,
    nTurn, nTurnMin, iDist, xSelf, ySelf, zSelf, zElev, xDest, yDest, zRng,
    grfObjInv = 0, iWield, objWield, iWear, objWear, objHere, nHealth,
    nHealthMax, nFood, nFoodMax, nInv, nPropose, nTrap, iItem, nEarly, nMove,
    nDes, nDmg, nDmgMax, nDmgWield, nHealthEnemy = 0, nDamageEnemy = 0,
    nPower, nPowerEnemy, iItemMax, cInvFree = 0, cConsum = 0, cAlly = 0,
    dGive = -1, nGive = 255, nLead = -1, obj, x, y, d, x2, y2, d2, i, j, l;
  flag fMutt, fNet, fWater, fCanClimb = fFalse, fCanFish, fCanSwim, fCanSee,
    fAllDeep = fTrue, fNearFire, fMelee = fFalse, fRanged = fFalse, fSafe;

  // Ensure valid game space
  Assert(objSling - objRock == dobj && objBlow - objDart == dobj &&
    objBow - objArrow == dobj && objGun - objBull == dobj);
  if (bm.k.FNull() || bm.k2.FNull() || bm.k3.FNull() ||
    ws.cbMask <= bmpH2O || ws.ccTexture <= bmpH2O ||
    ws.rgcTexture[bmpInv].FNull() || ws.rgcTexture[bmpAlly].FNull())
    return fFalse;
  if (!FEnsureLVar(numWasp) || !FEnsureSzVar(strY) ||
    ws.rgszVar[strX] == NULL || ws.rgszVar[strY] == NULL ||
    CchSz(ws.rgszVar[strX]) != 8 || CchSz(ws.rgszVar[strY]) != 8)
    return fFalse;
  i = (zVis << 1) + 1;
  if (!b.FAllocate(i, i, NULL))
    return fFalse;

  // Special case for firewall generation
  if (nInput == -1) {
    for (y = 0; y < bm.k.m_x; y++)
      for (x = 0; x < bm.k.m_x; x++) {
        if (!bm.b.Get(x, y))
          continue;
        bm.b3.Set1(x, y);
        bm.k.Set(x, y, Rgb(Rnd(255, 255-32), Rnd(127+32, 127-32), 0));
        ws.rgcTexture[bmpWall].Set(x, y, UD(bmpFire, bmpFire));
        ws.rgcTexture[bmpBlock].Set(x, y, bmpFire2);
        i = j = ws.rgcTexture[bmpElev].Get(x, y);
        if (bm.b2.Get(x, y))
          j = UdU(ws.rgcTexture[bmpVar].Get(x, y));
        ws.rgcTexture[bmpVar].Set(x, y,
          UD(Min(j + Rnd(zClimb, zClimb << 1), 4095), i));
        bm.b.Set0(x, y);
        bm.b2.Set0(x, y);
        ws.rgbMask[bmpLit].Set1(x, y);
      }
    return fTrue;
  }

  // Setup initial variables
  nSelf = UdU(nInput);
  nDistMax = ws.rglVar[numDists];
  nDist = ws.rglVar[numDist];
  nPer = ws.rglVar[numPer];
  nTribMax = nDist * nPer;
  nTribLeft = ws.rglVar[numLeft];
  nTurn = ws.rglVar[numTurn];
  nTurnMin = ws.rglVar[numStart];
  nHealthMax = ws.rglVar[numHPMax];
  nFoodMax = ws.rglVar[numFMax];
  nInv = ws.rglVar[numInv];
  zRng = ws.rglVar[numRng];
  nPropose = ws.rglVar[numAlly];
  nTrap = ws.rglVar[numTrap];
  iDist = NDist(nSelf);
  fMutt = iDist > nDist;
  l = ws.rglVar[numCoor + nSelf];
  xSelf = UdU(l); ySelf = UdD(l);
  zSelf = ZElev(nSelf);
  zElev = ws.rgcTexture[bmpElev].Get(xSelf, ySelf);
  nEarly = ws.rglVar[numLaun];
  nHealth = ws.rglVar[numHP + nSelf];
  nFood = ws.rglVar[numFood + nSelf];
  fNet = ws.rglVar[numNet + nSelf] > 0;
  fWater = NWater(xSelf, ySelf) > 0;
  iWield = NWield(nSelf);
  objWield = ObjInv(iWield);
  nDmgWield = WepDmg(objWield);
  fCanFish = iDist == 4 || FFisher(objWield);
  fCanSwim = iDist == 4 || objWield == objClub ||
    (fMutt && ws.rglVar[numMuttS]);
  fCanSee = fMutt || iDist == 13;
  iWear = ws.rglVar[numWear + nSelf];
  objWear = ObjInv(iWear);
  l = bm.k3.Get(xSelf, ySelf);
  objHere = NObj(l);
  for (iItem = 0; iItem < nInv; iItem++) {
    obj = ObjInv(iItem);
    grfObjInv |= (1 << obj);
    cInvFree += (obj == objFist);
    cConsum += FConsum(obj);
  }
  zLit = ws.rglVar[numWasp + nSelf] ? 1 : (nInput < 4096 || fCanSee ||
    objWear == objGogg ? zVis : NMin(zVis, UdD(nInput)));
  if (FWepLaunch(objWield) &&
    (!(FObjInInv(objWield-dobj) || FWepAuto(objWield)) || fNet))
    nDmgWield = 1;
  nPower = nHealth * nDmgWield;
  l = ws.rglVar[numDest + nSelf];
  xDest = UdU(l); yDest = UdD(l);
  for (i = 0; i < nTribMax; i++)
    cAlly += ws.rglVar[numHP + i] > 0 && FAlly(Faction(i));
  fNearFire = nTurn == ws.rglVar[numFinal] &&
    (!FBetween(xSelf, 2, bm.b.m_x-3) || !FBetween(ySelf, 2, bm.b.m_y-3));

  // Corruption checks
  j = 0;
  for (i = 0; i < nTribMax; i++) {
    if (ws.rglVar[numHP + i] <= 0)
      continue;
    j++;
    l = ws.rglVar[numCoor + i];
    x = UdU(l); y = UdD(l);
    l = bm.k3.Get(x, y);
    l = RgbR(l);
    CheckBad(l == 0, 2000 + i);
    l = NTrib(l);
    CheckBad(l != i, 3000 + i);
  }
  CheckBad(j != nTribLeft, 4000 + j);

  // Setup what's next to us
  for (d = 0; d < DIRS2; d++) {
    x = xSelf + xoff[d];
    y = ySelf + yoff[d];
    CheckWrap(&x, &y);
    if (d < DIRS && !fCanClimb && (!fMutt || ws.rglVar[numMuttC]) &&
      ((bm.b.Get(x, y) && !RgbR(bm.b3.Get(x, y)) &&
      UdU(ws.rgcTexture[bmpVar].Get(x, y)) >= zSelf + zClimb*3/2) ||
      ws.rgcTexture[bmpElev].Get(x, y) >= zSelf + zClimb*3/2))
      fCanClimb = fTrue;
    i = NWater(x, y);
    if (!(i > 0 && i <= 170))
      fAllDeep = fFalse;
  }
  for (d = 0; d < DIRS2; d++) {
    rgnMove[d] = -2;
    x = xSelf + Xoff(d);
    y = ySelf + Yoff(d);
    CheckWrap(&x, &y);
    if (!bm.k.FLegal(x, y))
      continue;
    l = bm.k3.Get(x, y);
    obj = NObj(l);
    l = RgbR(l);

    // Can't step into solid objects (unless can chop, dig, melt, or burn it)
    if (bm.b.Get(x, y) && l == 0) {
      j = bm.k.Get(x, y);
      if (!((iDist == 7 && objWield == objAxe && RgbG(j) > RgbR(j) &&
          (ws.rgcTexture[bmpBlock].Get(x, y) != bmpWasp ||
          zSelf >= ws.rgcTexture[bmpElev].Get(x, y) + zClimb*2)) ||
        (iDist == 2 && objWield == objPick &&
          (RgbG(j) == RgbB(j) || j == ws.rglVar[numMaze])) ||
        (iDist == 13 && objWield == objTorch &&
          (j == ws.rglVar[numCorn] || RgbG(j) > RgbR(j)))))
        continue;
    } else
      fNearFire |= FFirewall(x, y);
    // Shouldn't step onto activated landmine
    if (obj == objMine1 && zSelf <= zElev && !fMutt &&
      (nTrap == 0 || Rnd(0, 99) >= nTrap))
      continue;
    // Shouldn't step onto wasp nest or Net trap
    i = ws.rgcTexture[bmpFloor].Get(x, y);
    if ((i == bmpWasp || (!fMutt && i == bmpTrap)) && zSelf <= zElev &&
      (nTrap == 0 || Rnd(0, 99) >= nTrap))
      continue;
    // Never step into deep water if can't swim
    i = NWater(x, y);
    if (i > 0 && i <= 170 && !fCanSwim && l == 0)
      continue;
    // Never step into space so fall and take damage
    if (iDist != 11 && objWear != objPara &&
      zSelf >= ws.rgcTexture[bmpElev].Get(x, y) + zClimb*2) {
      for (d2 = 0; d2 < DIRS; d2++) {
        x2 = x + xoff[d2];
        y2 = y + yoff[d2];
        CheckWrap(&x2, &y2);
        if ((bm.b.Get(x2, y2) && !RgbR(bm.k3.Get(x2, y2)) &&
          UdU(ws.rgcTexture[bmpVar].Get(x2, y2)) >= zSelf + zClimb/2) ||
          ws.rgcTexture[bmpElev].Get(x2, y2) >= zSelf)
          break;
      }
      if (d2 >= DIRS)
        continue;
    }
    // Never step next to firewall because it may expand over you
    for (d2 = 0; d2 < DIRS; d2++) {
      x2 = x + xoff[d2];
      y2 = y + yoff[d2];
      CheckWrap(&x2, &y2);
      if (FFirewall(x2, y2))
        break;
    }
    if (d2 < DIRS)
      continue;

    if (l == 0) {
      // Shouldn't move next to launch platforms before the opening gong
      if (nTurn < 0) {
        if (fMutt)
          continue;
        for (d2 = 0; d2 < DIRS2; d2++) {
          x2 = x + xoff[d2];
          y2 = y + yoff[d2];
          CheckWrap(&x2, &y2);
          if (bm.k2.Get(x2, y2) == ws.rglVar[numTube])
            break;
        }
        if (d2 < DIRS2) {
          l = bm.k2.Get(x, y);
          if ((l | 0x000f1f) != kvBrown &&
            Rnd(1, ws.rglVar[numEarly] * DIRS2 * nTurnMin) > 1)
            continue;
        }
      }
      rgnMove[d] = -1;
    } else {
      l = NTrib(l);
      i = Faction(l);
      rgnMove[d] = i;
      if (!FAlly(i)) {
        fMelee = fTrue;
        nHealthEnemy += ws.rglVar[numHP + l];
        nDamageEnemy += DmgN(l);
      }
    }
  }
  nPowerEnemy = nHealthEnemy * nDamageEnemy;

  // Setup how interested in objects
  for (obj = 0; obj < objMax; obj++) {
    nDes = 0;
    nDmg = WepDmg(obj);
    if (FWep(obj)) {
      // Don't want non-throwable ammo unless already have launcher
      // Don't want melee only weapon worse than melee weapon wielded
      if (FWepLaunch(obj))
        nDes = FObjInInv(obj-dobj) || FWepAuto(obj) ? nDmg : 1;
      else if (FWepAmmo(obj))
        nDes = FObjInInv(obj+dobj) ? WepDmg(obj+dobj) : nDmg * FWepThrow(obj);
      else if (FWepThrow(obj) || WepDmg(obj) >= nDmgWield)
        nDes = nDmg;
    } else {
      // Never want Nightlock or active Landmine
      switch (obj) {
      case objFist:  nDes = 0; break;
      case objBerry: nDes = nFood < nFoodMax ? 6 : nDmg; break;
      case objNight: nDes = 0; break;
      case objBread: nDes = nFood < nFoodMax ? 7 : 4; break;
      case objFish:  nDes = nFood < nFoodMax ? 6 : nDmg; break;
      case objHeal:  nDes = nHealth < nHealthMax ? 9 : nDmg; break;
      case objGogg:  nDes = dr.fStar ? 8 : nDmg; break;
      case objHelm:  nDes = !dr.fStar ? 8 : nDmg; break;
      case objArmor: nDes = !dr.fStar ? 9 : nDmg; break;
      case objPara:  nDes = nDmg; break;
      case objMine0: nDes = (iDist == 3) ? 5 : nDmg; break;
      case objMine1: nDes = 0; break;
      default: nDes = nDmg; break;
      }
      // Special: Ignore food if very full, and First-Aid if limit low
      if ((nFood >= 2000 && (obj == objBerry || obj == objBread ||
        obj == objFish)) || (nHealthMax < 2 && obj == objHeal))
        nDes = 0;
    }
    if (fMutt)
      nDes = 0;
    rgnValObj[obj] = nDes;
    // No duplicate launcher or duplicate melee only weapon
    if ((FWepLaunch(obj) || (FWepMelee(obj) && !FWepThrow(obj))) &&
      FObjInInv(obj))
      nDes = 0;
    // No melee only weapon equal to melee only weapon already wielded
    if (FWepMelee(obj) && !FWepThrow(obj) && FWepMelee(objWield) &&
      !FWepThrow(objWield) && WepDmg(obj) <= nDmgWield)
      nDes = 0;
    // No duplicate or extraneous nightvision goggles, armor, or parachute
    if ((obj == objGogg && (fCanSee || FObjInInv(objGogg))) ||
      (obj == objHelm && FObjsInInv(objHelm, objArmor)) ||
      (obj == objArmor && FObjInInv(objArmor)) || (obj == objPara &&
      (FObjsInInv(objPara, objGogg) || FObjsInInv(objHelm, objArmor))))
      nDes = 0;
    rgnConObj[obj] = nDes;
    /* Save last inventory slot for consumables, and don't want anything at
      all if inventory full (unless need First-Aid). */
    if ((cInvFree <= 0 && !(obj == objHeal && nHealth < nHealthMax)) ||
      (cInvFree <= 1 && cConsum <= 0 && !FConsum(obj)))
      nDes = 0;
    rgnDesObj[obj] = nDes;
  }

  // Look around and setup how interested in directions
  b.BitmapOff();
  for (y = -zVis; y <= zVis; y++)
    for (x = -zVis; x <= zVis; x++) {
      x2 = xSelf + x; y2 = ySelf + y;
      CheckWrap(&x2, &y2);
      if (bm.b.Get(x2, y2) && !RgbR(bm.k3.Get(x2, y2)))
        b.Set(x+zVis, y+zVis, fOn);
      else {
        i = NWater(x2, y2);
        if (i > 0 && i <= 170 && !fCanSwim)
          b.Set(x+zVis, y+zVis, fOn);
      }
    }
  b.BitmapSeen(zVis, zVis, zVis);
  ClearPb(rgnDesDir, DIRS2 * sizeof(int));
  for (y = -zLit; y <= zLit; y++)
    for (x = -zLit; x <= zLit; x++) {
      if (b.Get(x+zVis, y+zVis))
        continue;
      x2 = xSelf + x; y2 = ySelf + y;
      CheckWrap(&x2, &y2);
      nDes = 0;
      l = bm.k3.Get(x2, y2);
      obj = NObj(l);
      if (obj > 0) {
        // Ignore objects others are standing on top of
        if (RgbR(bm.k3.Get(x2, y2)))
          obj = 0;
        // Ignore objects in deep water if can't swim
        i = NWater(x2, y2);
        if (i > 0 && i <= 170 && !fCanSwim)
          obj = 0;
        // Ignore Fish if can't catch them
        if (obj == objFish && i > 0 && !fCanFish)
          obj = 0;
      }
      if (obj > 0) {
        i = rgnDesObj[obj];
        if (i > 0)
          nDes = i + (zVis - NMax(NAbs(x), NAbs(y))) * 10;
      } else {
        l = RgbR(l);
        if (l != 0) {
          l = NTrib(l);
          if (fMutt)
            nDes = (l < nTribMax)*3; // Mutts seek out all non-Mutts
          else if (FAlly(Faction(l))) {
            x2 = ws.rglVar[numLead + nSelf]; y2 = ws.rglVar[numLead + l];
            nDes = 1 + (y2 > x2 || (y2 == x2 && l < nSelf))*2; // Seek allies
          } else {
            CheckBad(ws.rglVar[numHP + l] <= 0, 1000 + l);
            i = NCompare(l);
            if (i > 100)
              nDes = (i-1) / 100 + 3; // Seek out weaker enemies
          }
        }
      }
      if (nDes > 0)
        for (d = 0; d < DIRS2; d++) {
          x2 = Xoff(d);
          y2 = Yoff(d);
          if (NMax(NAbs(x - x2), NAbs(y - y2)) < NMax(NAbs(x), NAbs(y)))
            rgnDesDir[d] = NMax(rgnDesDir[d], nDes);
        }
    }

  // Seek out waypoint coordinates
  if (nTurn > nEarly) {
    // Don't if follower in alliance, i.e. if higher leadership tribute nearby
    nLead = nSelf;
    x2 = ws.rglVar[numLead + nLead];
    for (i = 0; i < nTribMax; i++) {
      if (i == nSelf)
        continue;
      if (ws.rglVar[numHP + i] > 0 && FAlly(Faction(i))) {
        l = ws.rglVar[numCoor + i];
        x = UdU(l); y = UdD(l);
        if (NMax(NDistance(xSelf, x, bm.b.m_x),
          NDistance(ySelf, y, bm.b.m_y)) <= zRng) {
          y2 = ws.rglVar[numLead + i];
          if (y2 > x2 || (y2 == x2 && i < nLead)) {
            nLead = i;
            x2 = y2;
          }
        }
      }
    }
    // Move directions toward waypoint coordinates are given extra desire.
    if (nLead == nSelf || fMutt) {
      i = 0;
      for (d = 0; d < DIRS2; d++) {
        if (rgnMove[d] <= -2)
          continue;
        x = xSelf + Xoff(d);
        y = ySelf + Yoff(d);
        if (NMax(NDistance(xDest, x, bm.b.m_x), NDistance(yDest, y,
          bm.b.m_y)) < NMax(NDistance(xDest, xSelf, bm.b.m_x),
          NDistance(yDest, ySelf, bm.b.m_y))) {
          rgnDesDir[d] += Rnd(1, 2);
          i++;
        }
      }
      // If can't make progress, indicate want new destination
      if (i < 1)
        ws.rglVar[numDest + nSelf] = UD(xSelf, ySelf);
    }
  }

  // Move or melee attack
  for (d = 0; d < DIRS2; d++) {
    nMove = rgnMove[d];
    if (nMove <= -2)
      continue;
    x = xSelf + Xoff(d);
    y = ySelf + Yoff(d);
    // Consider move to an adjacent space
    if (nMove == -1) {
      nDes = rgnDesDir[d]*10 + Rnd(1, 9);
      // Approach or flee cornucopia at start of game
      if (nTurn <= nEarly &&
        NMax(NAbs(xDest - x), NAbs(yDest - y)) <
        NMax(NAbs(xDest - xSelf), NAbs(yDest - ySelf)))
        nDes += desStart;
      // Flee firewall
      if (fNearFire) {
        for (d2 = 0; d2 < DIRS; d2++) {
          x2 = x + xoff[d2];
          y2 = y + yoff[d2];
          CheckWrap(&x2, &y2);
          if (FFirewall(x2, y2) ||
            !FBetween(x2, 1, bm.b.m_x-2) || !FBetween(y2, 1, bm.b.m_y-2))
            break;
        }
        if (d2 >= DIRS)
          nDes += desFleeHi;
      // Flee stronger non-allies
      } else if (fMelee && !fMutt && nPower * 100 / nPowerEnemy <= 50) {
        nHealthEnemy = nDamageEnemy = 0;
        for (d2 = 0; d2 < DIRS2; d2++) {
          x2 = x + Xoff(d2);
          y2 = y + Yoff(d2);
          CheckWrap(&x2, &y2);
          l = RgbR(bm.k3.Get(x2, y2));
          if (l != 0) {
            l = NTrib(l);
            if (!FAlly(Faction(l))) {
              nHealthEnemy += ws.rglVar[numHP + l];
              nDamageEnemy += DmgN(l);
            }
          }
        }
        if (nHealthEnemy * nDamageEnemy <= 0 ||
          nPower * 100 / (nHealthEnemy * nDamageEnemy) >= 200)
          nDes += desFlee;
      // Chase weaker non-allies if armed
      } else if (!fMelee && nDmgWield > 1 && objWield > 0) {
        i = 0;
        for (d2 = 0; d2 < DIRS2; d2++) {
          x2 = x + Xoff(d2);
          y2 = y + Yoff(d2);
          CheckWrap(&x2, &y2);
          l = RgbR(bm.k3.Get(x2, y2));
          if (l != 0) {
            l = NTrib(l);
            if (!FAlly(Faction(l)) && (fMutt || NCompare(l) >= 200))
              i = desChase;
          }
        }
        nDes += i;
      }
    // Consider attacking an adjacent tribute
    } else {
      CheckWrap(&x, &y);
      l = RgbR(bm.k3.Get(x, y));
      // Consider giving item to allied or neutral tribute
      if (l != 0) {
        j = NTrib(l);
        if (nMove < nGive && NAbs(zSelf - ZElev(j)) <= zClimb) {
          i = 0;
          for (iItem = 0; iItem < nInv; iItem++) {
            obj = ObjInvN(iItem, j);
            i += (obj == objFist);
          }
          if (i >= 2 &&
            (FAlly(nMove) || (nTribLeft - cAlly > 1 && FAlly(nMove - 50)))) {
            nGive = nMove;
            dGive = d;
          }
        }
      }
      // Interact with allies (shouldn't attack them)
      if (FAlly(nMove)) {
        if (l != 0) {
          j = NTrib(l);
          // Free ally trapped in Net
          if (ws.rglVar[numNet + j])
            Desire(actFree, 0, d, desFree);
          // Declare leadership with respect to ally
          if ((nLead == nSelf || nLead == j) && !fMutt && Rnd(0, 99) <= 0)
            Desire(actLead, j, d, desLead);
          // Backstab non-close ally if can one-shot them
          if (nMove > 50 && ws.rglVar[numHP + j] == 1 && nDmgWield > 1 &&
            !FWepLaunch(objWield) && ws.rglVar[numFave + objWield] == iDist)
            Desire(actMove, 0, d, desBetray + nMove);
        }
        continue;
      }
      // Can't attack more than one square above or below
      if (l != 0) {
        j = NTrib(l);
        if (NAbs(zSelf - ZElev(j)) > zClimb) {
          if (NCompare(j) > 100) {
            if (zSelf > ZElev(j)) {
              Desire(actDown, 0, 0, desUp + desChase);
            } else if (fCanClimb)
              Desire(actUp, 0, 0, desDown + desChase);
          }
          continue;
        }
      }
      nDes = desMelee + nMove;
      // Don't attack with fist unless no other option
      if (nDmgWield <= 1 || FWepLaunch(objWield))
        nDes = desPunch;
    }
    Desire(actMove, 0, d, nDes);
  }

  // Throw attack
  iItemMax = nDmgMax = -1;
  for (iItem = nInv-1; iItem >= 0; iItem--) {
    obj = ObjInv(iItem);
    // Look for ammoless launcher to fire (1st priority)
    nDmg = FWepAuto(obj) && objWield == obj ? WepDmg(obj) : -1;
    if (nDmg > nDmgMax) {
      iItemMax = iItem;
      nDmgMax = nDmg;
    }
    // Look for ammo to launch with wielded launcher (2nd priority)
    nDmg = FWepAmmo(obj) && objWield == obj+dobj ? WepDmg(obj+dobj) : -1;
    if (nDmg > nDmgMax) {
      iItemMax = iItem;
      nDmgMax = nDmg;
    }
    // Look for unwielded throwable weapon (3rd priority)
    nDmg = FWepThrow(obj) && (!FWepMelee(obj) || iItem != iWield) ?
      WepDmg(obj) : -1;
    if (nDmg > nDmgMax) {
      iItemMax = iItem;
      nDmgMax = nDmg;
    }
  }
  for (d = 0; d < DIRS2; d++) {
    // If best item thrown in each direction, what would be hit?
    x = xSelf; y = ySelf;
    for (j = 0; j < zRng; j++) {
      x += Xoff(d);
      y += Yoff(d);
      CheckWrap(&x, &y);
      if (!bm.k.FLegal(x, y))
        break;
      l = RgbR(bm.k3.Get(x, y));
      if (bm.b.Get(x, y) && l == 0)
        break;
      if (l == 0)
        continue;
      l = NTrib(l);
      i = Faction(l);
      if (FAlly(i)) // Don't ever attack allies
        break;
      if (!FEnemy(i) && j >= zVis) // Don't attack non-enemies in darkness
        break;
      fRanged = fTrue;
      if (iItemMax >= 0 && !fNet) {
        obj = ObjInv(iItemMax);
        if (!FWepAuto(obj)) {
          Desire(actThrow, iItemMax, d, desThrow + i);
        } else
          Desire(actFire, iItemMax, d, desThrow + i);
      }
      if (!FEnemy(i) && !fMutt && NDist(l) <= nDist && !fMelee && !fNet &&
        nTribLeft - cAlly > 1 && (nPropose > 0 && Rnd(1, 100) <= nPropose)) {
        i = NCompare(l);
        if (i >= 50 && i <= 200)
          Desire(actAlly, l, d, desAlly);
      }
    }
  }
  fSafe = !fMelee && !fRanged;

  // Climb
  if (zSelf > zElev && fSafe &&
    (zSelf - zClimb > zElev || objHere != objMine1))
    Desire(actDown, 0, 0, desDown);
  if (fCanClimb && nTurn >= 0)
    Desire(actUp, 0, 0, desUp);

  // Dig
  if (zSelf <= zElev && !fWater && objHere == 0 && objWield == objPick &&
    fSafe && cInvFree > 1) {
    if (iDist != 12)
      for (d = 0; d < DIRS2; d++) {
        x = xSelf + xoff[d];
        y = ySelf + yoff[d];
        CheckWrap(&x, &y);
        if (bm.k2.Get(x, y) == ws.rglVar[numTube])
          break;
      }
    if (iDist == 12 || d < DIRS2) {
      l = bm.k2.Get(xSelf, ySelf);
      if (l != ws.rglVar[numTube] && (l | 0x000f1f) != kvBrown)
        Desire(actDig, 0, 0, desDig);
    }
  }
  if (zSelf <= zElev && fSafe) {
    // Destroy item
    if (objHere != 0 && cAlly <= 1 && zSelf - zElev <= zClimb/2 && !fNet &&
      (objWield == objPick || objWield == objTorch) &&
      ((objWield == objPick) == (ws.rglVar[numMater + objHere] == 1)))
      Desire(actDig, 0, 0, desDig);
    // Set Net trap
    if (objWield == objNet && objHere == 0 && FObjsInInv(objSword, objTrid))
      Desire(actDig, 0, 0, desDig);
  }

  // Pick up item
  if (cInvFree > 0 && zSelf - zElev <= zClimb/2 && !fNet &&
    rgnDesObj[objHere] > 0 && !(objHere == objFish && fWater && !fCanFish))
    Desire(actGet, 0, 0, (FWep(objHere) && rgnDesObj[objHere] - nDmgWield >=
      3) || (FWepMelee(objHere) && nDmgWield <= 1) ? desGetHi : desGet);

  // Wield weapon
  for (iItem = 0; iItem < nInv; iItem++) {
    obj = ObjInv(iItem);
    if (!FWep(obj) || iItem == iWield)
      continue;
    nDmg = WepDmg(obj);
    if (FWepLaunch(obj) && (!(FObjInInv(obj-dobj) || FWepAuto(obj)) || fNet))
      nDmg = 1;
    // Wield club if needed to move out of deep water
    if (fAllDeep) {
      if (!fCanSwim && obj == objClub)
        Desire(actUse, iItem, 0, desWieldHi + nDmg);
      if (fSafe)
        continue;
    }
    if (FWepLaunch(obj)) {
      /* Wield launcher if have ammo for it and better than what currently
        have, or if equal to wielded melee weapon and not in melee. */
      if ((FObjInInv(obj-dobj) || FWepAuto(obj)) && (nDmg > nDmgWield ||
        (nDmg >= nDmgWield && FWepMelee(objWield) && !fMelee)))
        Desire(actUse, iItem, 0, (fSafe ? desWield : desWieldHi) + nDmg);
      // Wield launcher if not wielding anything and not threatened.
      if (objWield < 1 && fSafe)
        Desire(actUse, iItem, 0, desWield + nDmg);
    }
    if (FWepMelee(obj)) {
      // Wield if strictly better than what currently have.
      if (nDmg > nDmgWield)
        Desire(actUse, iItem, 0, (fSafe ? desWield : desWieldHi) + nDmg);
      /* Wield if equal to what currently have, and can switch from throwable
        to melee only, e.g. Trident to Sword. */
      if (nDmg == nDmgWield &&
        !FWepThrow(obj) && FWepThrow(objWield) && fSafe)
        Desire(actUse, iItem, 0, desWield + nDmg);
    }
    // Wield lesser weapon if it can be used to catch fish here
    if (FFisher(obj) && objHere == objFish && zSelf - zElev <= zClimb/2 &&
      !fCanFish && nFood < nFoodMax && cInvFree > 0 && fSafe)
      Desire(actUse, iItem, 0, desWield + nDmg);
  }

  // Use, drop, or give item
  for (iItem = nInv-1; iItem >= 0; iItem--) {
    obj = ObjInv(iItem);
    if (obj == objFist)
      continue;

    // Heal self
    if (obj == objHeal &&
      (nHealth < nHealthMax || ws.rglVar[numWasp + nSelf] > 0)) {
      if (nFood <= 1 && nHealth <= 1)
        Desire(actUse, iItem, 0, desHealHi);
      if (fSafe)
        Desire(actUse, iItem, 0, desHeal);
    }

    // Eat food
    if (obj == objBerry || obj == objBread || obj == objFish) {
      if (nFood <= 1 && nHealth <= 1)
        Desire(actUse, iItem, 0, desEatHi + (obj == objBread));
      if (nFood < nFoodMax && nTurn > (nEarly * 3 >> 1) + nSelf && fSafe)
        Desire(actUse, iItem, 0, desEat + (obj == objBread));
    }
    if (obj == objNight && nTribLeft == 2)
      for (i = 0; i < nTribMax; i++)
        if (i != nSelf && ws.rglVar[numHP + i] > 0 && Faction(i) <= 0) {
          Desire(actUse, iItem, 0, desEatHi + 2);
          break;
        }

    // Wear item
    if ((objWear != objPara || zSelf - zElev <= zClimb) && fSafe) {
      if (obj == objGogg && (iWear < 0 ||
        (objWear != objGogg && dr.fStar)) && !fCanSee)
        Desire(actUse, iItem, 0, desWear + rgnValObj[obj])
      if (obj == objHelm && (iWear < 0 ||
        (objWear != objHelm && objWear != objArmor && !dr.fStar)))
        Desire(actUse, iItem, 0, desWear + rgnValObj[obj])
      if (obj == objArmor && (iWear < 0 ||
        (objWear != objArmor && !dr.fStar)))
        Desire(actUse, iItem, 0, desWear + rgnValObj[obj])
    }
    if (obj == objPara && iWear < 0 && fSafe)
      Desire(actUse, iItem, 0, desWear + rgnValObj[obj])

    // Place landmine
    if (fNet)
      continue;
    if (obj == objMine0 && zSelf <= zElev && objHere == 0 && fSafe)
      Desire(actUse, iItem, 0, desMine);

    // Drop item
    if (iItem == iWield || iItem == iWear || nTurn <= (nEarly << 1) ||
      (!fSafe && dGive < 0) || (fMutt && obj == objTeeth) ||
      (objHere == objMine1 && zSelf <= zElev))
      continue;
    // Always drop Nightlock or active Landmine as soon as possible
    if (obj == objNight || obj == objMine1)
      Desire(actDrop, iItem, 0, desDrop);
    // Drop duplicate or extraneous tool
    if (((obj == objGogg || obj == objHelm || obj == objArmor) &&
      objWear == obj) || (obj == objHelm && objWear == objArmor) ||
      (obj == objPara && iWear >= 0) || (obj == objGogg && fCanSee))
      Desire(actDrop, iItem, 0, desDrop);
    /* Drop melee only weapon worse than melee weapon being wielded, or
      equal to melee only weapon already wielded. */
    if (FWepMelee(obj) && !FWepThrow(obj) && FWepMelee(objWield) &&
      objWield != objNet && (WepDmg(obj) < nDmgWield ||
      (WepDmg(obj) == nDmgWield && !FWepThrow(objWield)))) {
      if (dGive < 0 || !FAlly(nGive - ws.rglVar[numDmg + obj]*10) ||
        (FAlly(nGive) && Rnd(0, 1))) {
        Desire(actDrop, iItem, 0, desDrop);
      } else
        Desire(actGive, iItem, dGive, FAlly(nGive) ? desGive : desGiveHi);
    }
    // Improve full inventory if on top of better weapon or better food
    if (cInvFree <= 1) {
      // Don't drop launcher to pick up unthrowable ammo for that launcher
      // Don't drop anything for a Fish if can't catch fish
      // Don't drop last consumable item in inventory
      if (obj > 0 && objHere > 0 && rgnValObj[obj] < rgnConObj[objHere] &&
        !(FWepAmmo(objHere) && !FWepThrow(objHere) && obj == objHere+dobj) &&
        !(objHere == objFish && fWater && !fCanFish) &&
        !(cConsum <= 1 && FConsum(obj))) {
        if (dGive < 0 || !FAlly(nGive - ws.rglVar[numDmg + obj]*10) ||
          (FAlly(nGive) && Rnd(0, 1))) {
          Desire(actDrop, iItem, 0, desDrop - rgnValObj[obj]);
        } else
          Desire(actGive, iItem, dGive,
            (FAlly(nGive) ? desGive : desGiveHi) - rgnValObj[obj]);
      }
    }

    // Give item
    if (dGive >= 0 && !FAlly(nGive) &&
      FAlly(nGive - ws.rglVar[numDmg + obj]*10))
      Desire(actGive, iItem, dGive, desGiveHi - rgnValObj[obj]);
  }

  // Pick best move of all possible
  ws.rglVar[iLetterX] = nItem;
  ws.rglVar[iLetterY] = nDir;
  ws.rglVar[iLetterZ] = nAction;
  return fTrue;
}

/* daedalus.cpp */
