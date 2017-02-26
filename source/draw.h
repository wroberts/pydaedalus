/*
** Daedalus (Version 3.2) File: draw.h
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
** This file contains definitions for advanced non-perspective and perspective
** graphics routines, unrelated to Mazes.
**
** Created: 6/15/1990.
** Last code change: 8/31/2016.
*/

/*
******************************************************************************
** Macros
******************************************************************************
*/

#define GetT3(c, x, y, z, d) \
  ((int)(((c).Get3(x, y, z) >> ((d) * 6)) & (cTexture-1)))
#define SetT3(c, x, y, z, d, t) ((c).Get3(x, y, z) & \
  ~((KV)(cTexture-1) << ((d) * 6)) | ((KV)(t) << ((d) * 6)))


/*
******************************************************************************
** Types
******************************************************************************
*/

typedef struct _drawsettings {
  // Draw settings

  int horsiz;
  int versiz;
  int depsiz;
  int horbias;
  int verbias;
  int hormin;
  int vermin;
  int depmin;
  flag fArrow;
  int horv;
  int verv;
  int theta;
  int phi;
  int speedm;
  int speedr;
  flag fRight;
  flag fMerge;

  // Obscure draw settings

  flag fReflect;
  flag fSkyShade;
  flag fGroundShade;
  flag fEdges;
  flag fShading;
  flag fTouch;
  int nBorder;
  int nHoriz;
  real rScale;
  real rxScale;
  real ryScale;
  real rzScale;
  KV kvTrim;
  KV kvSkyHi;
  KV kvSkyLo;
  KV kvGroundHi;
  KV kvGroundLo;
  KV kvObject;
  CVector vLight;

  // Macro accessible only settings

  int nTrans;
  int nFog;

  // Internal settings

  flag fDidPatch;
  int xmax;
  real rHoriz;
  long cCoorPatch;
} DS;

typedef struct _coordinates {
  real x1, y1, z1;
  real x2, y2, z2;
} COOR;

typedef struct _patn {
  int x, y, z;
  flag fLine;
} PATN;

typedef struct _patr {
  real x, y, z;
  flag fLine;
} PATR;

#define cPatch 4

typedef struct _patch {
  PATR p[cPatch];
  real rDistance;
  KV kv;
  short cpt;
  short nTrans;
} PATCH;

extern DS ds;


/*
******************************************************************************
** Render Overview and Pyramid
******************************************************************************
*/

extern flag DrawOverview(CMap &, CONST CMon &);
extern flag DrawOverviewCube(CMap &, CONST CMon3 &);
extern flag DrawOverview2(CMon &);
extern flag DrawOverviewCube2(CMon3 &);
extern flag DrawOverviewAltitude(CMap &, CONST CCol &, int, int);
extern flag DrawPyramid(CMon &, CCol *);


/*
******************************************************************************
** File Reading and Writing Routines
******************************************************************************
*/

#define ReadCoordinates(file, x1, y1, z1, x2, y2, z2) \
  fscanf(file, "%lf%lf%lf%lf%lf%lf", x1, y1, z1, x2, y2, z2)
extern long ReadWirelist(COOR **, FILE *);
#define ReadPatch1(file, stat) fscanf(file, "%d", stat)
#define ReadPatch2(file, x1, y1, z1, x2, y2, z2, x3, y3, z3) \
  fscanf(file, "%lf%lf%lf%lf%lf%lf%lf%lf%lf", \
  x1, y1, z1, x2, y2, z2, x3, y3, z3)
#define ReadPatch3(file, x1, y1, z1) fscanf(file, "%lf%lf%lf", x1, y1, z1)
extern long ReadPatchlist(PATCH **, FILE *);

extern void WriteWireframe(FILE *, CONST COOR *, long);
extern void WritePatches(FILE *, CONST PATCH *, long);
extern void WriteWireframeMetafile(FILE *, CONST COOR *, long);


/*
******************************************************************************
** Create Wireframe and Patches
******************************************************************************
*/

extern void WriteCoordinates(COOR *, int, int, int, int, int, int);
extern void WritePatch(PATCH *, PATN[cPatch], flag, KV);
extern long CreateWireframe(CONST CMon3 &, COOR **, flag);
extern long CreatePatches(CONST CMon3 &, PATCH **, flag, CONST CCol *);
extern flag CreateSolids(CONST char *);


/*
******************************************************************************
** Render Perspective
******************************************************************************
*/

extern flag FRenderPerspectiveWire(CMap &, COOR *, long);
extern flag FRenderPerspectivePatch(CMap &, PATCH *, long);
extern flag FRenderAerial(CMon &, CONST COOR *, long);
extern long ConvertPatchToWire(COOR **, CONST PATCH *, long);

/* draw.h */
