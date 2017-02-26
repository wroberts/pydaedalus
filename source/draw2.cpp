/*
** Daedalus (Version 3.2) File: draw2.cpp
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
** This file contains advanced perspective graphics routines, unrelated to
** Mazes.
**
** Created: 7/17/1990.
** Last code change: 8/31/2016.
*/

#include <stdio.h>
#include <math.h>
#include "util.h"
#include "graphics.h"
#include "color.h"
#include "threed.h"
#include "draw.h"


/*
******************************************************************************
** Perspective Routines
******************************************************************************
*/

// Draw the background for the FRenderPerspective functions. This handles both
// monochrome and color bitmaps.

void RenderInitialize(CMap &b, real phi)
{
  int h, x, y, y1, y2, y3, y4;
  flag fColor = b.FColor();

  // Figure out the height of the horizon line, and the visible sections of
  // ground and sky.
  h = (int)(ds.rHoriz - ds.rScale*RTanD(phi) + rRound) - ds.nHoriz + 1;
  if (h < 0)
    h = 0;
  else if (h >= b.m_y)
    h = b.m_y;
  y1 = 0; y2 = h-1; y3 = h; y4 = b.m_y-1;
  if (phi > 90.0 || phi <= -90.0) {
    SwapN(y1, y3);
    SwapN(y2, y4);
  }

  // Draw the ground and sky sections in the appropriate colors.
  if (!fColor) {
    b.BitmapOn();
    if (ds.fSkyShade && y2 >= 0)
      b.Block(0, y1, b.m_x-1, y2, fOff);
    if (ds.fGroundShade)
      for (y = y3; y <= y4; y++)
        for (x = 0; x < b.m_x; x++)
          if (!FOdd(x ^ y))
            b.Set0(x, y);
  } else {
    for (y = y1; y <= y2; y++)
      b.LineX(0, b.m_x-1, y, KvBlendN(ds.kvSkyHi, ds.kvSkyLo,
        y - y1, y2 - y1));
    for (y = y3; y <= y4; y++)
      b.LineX(0, b.m_x-1, y, KvBlendN(ds.kvGroundHi, ds.kvGroundLo,
        y - y3, y4 - y3));
  }

  // Draw the horizon line itself.
  if (ds.nHoriz > 0)
    b.Block(0, h, b.m_x-1, h + ds.nHoriz - 1, !fColor ? fOff : ds.kvTrim);
}


// Draw a border around the image for the FRenderPerspective functions. This
// handles both monochrome and color bitmaps.

void RenderFinalize(CMap &b)
{
  flag fColor = b.FColor();

  if (ds.nBorder > 0)
    b.BoxAll(ds.nBorder, ds.nBorder, !fColor ? fOff : ds.kvTrim);
}


// Move the point (x1, y1, z1) along the given line so that y1 = 0. Called
// from RenderPatchPerspective().

void IgnoreNegativeR(real *x1, real *y1, real *z1, real x2, real y2, real z2)
{
  if (y2 != *y1) {
    *x1 -= *y1 * (x2 - *x1) / (y2 - *y1);
    *z1 -= *y1 * (z2 - *z1) / (y2 - *y1);
  }
  *y1 = 0.0;
}


// Move the point (x1, y1) along the given line so that y1 = 0. Called from
// ClipOutside().

void IgnoreNegative(int *x1, int *y1, int x2, int y2)
{
  if (y2 != *y1)
    *x1 -= NMultDiv(*y1, x2 - *x1, y2 - *y1);
  *y1 = 0;
}


// Move the point (x1, y1) along the given line so that y1 = n. Called from
// ClipOutside().

void IgnoreGreaterThan(int *x1, int *y1, int x2, int y2, int n)
{
  if (y2 != *y1)
    *x1 += NMultDiv(n - *y1, x2 - *x1, y2 - *y1);
  *y1 = n;
}


// Clip a line segment so that it lies inside a rectangle, specifically from
// (0, 0) to (xmax, ymax). Called from FRenderPerspectiveWire().

void ClipOutside(int xmax, int ymax, int *x1, int *y1, int *x2, int *y2)
{
  if (*x1 < 0)
    IgnoreNegative(y1, x1, *y2, *x2);
  if (*x2 < 0)
    IgnoreNegative(y2, x2, *y1, *x1);
  if (*y1 < 0)
    IgnoreNegative(x1, y1, *x2, *y2);
  if (*y2 < 0)
    IgnoreNegative(x2, y2, *x1, *y1);
  if (*x1 > xmax)
    IgnoreGreaterThan(y1, x1, *y2, *x2, xmax);
  if (*x2 > xmax)
    IgnoreGreaterThan(y2, x2, *y1, *x1, xmax);
  if (*y1 > ymax)
    IgnoreGreaterThan(x1, y1, *x2, *y2, ymax);
  if (*y2 > ymax)
    IgnoreGreaterThan(x2, y2, *x1, *y1, ymax);
}


// Do perspective projection of a 3D point onto the 2D screen. Called from the
// FRenderPerspective functions.

void CalculateCoordinate(int *h, int *v, real x, real y, real z)
{
  real r;

  if (y < 1.0)
    y = 1.0;

  // Calculate horizontal pixel. The farther away y is, the less effect x has
  // on the horizontal distance from the middle of the screen.
  r = ds.rScale*x / y;
  EnsureBetween(r, -32000.0, 32000.0);
  *h = (ds.xmax >> 1) + (int)r;

  // Calculate vertical pixel. The farther away y is, the less effect z has on
  // the vertical distance from the horizon line.
  r = ds.rScale*z / y;
  EnsureBetween(r, -32000.0, 32000.0);
  *v = (int)(ds.rHoriz - r);
}


/*
******************************************************************************
** Render Perspective Wireframe
******************************************************************************
*/

// Draw a perspective scene composed of a set of line segments in 3D space.
// Implements the Render Wireframe Perspective command for both monochrome and
// color bitmaps.

flag FRenderPerspectiveWire(CMap &b, COOR *coor, long ccoor)
{
  COOR *coorT;
  long ccoor2 = 0, ccoor3 = 0, i;
  int xmax, ymax, x1, y1, x2, y2;
  real xpos, ypos, zpos, yminCoor, ymaxCoor, theta, phi, rS, rC;
  flag fColor = b.FColor();

  xmax = b.m_x; ymax = b.m_y; ds.xmax = xmax;
  xpos  = (real)ds.hormin;
  ypos  = (real)ds.vermin;
  zpos  = (real)ds.depmin;
  theta = (real)ds.theta;
  phi   = (real)ds.phi;
  ds.rHoriz = (real)((ymax >> 1) + ds.verv);

  // Get starting set of coordinates.

  coorT = RgAllocate(Max(ccoor, 1), COOR);
  if (coorT == NULL)
    return fFalse;
  for (i = 0; i < ccoor; i++)
    coorT[i] = coor[i];
  if (ds.fReflect && ccoor > 0) {
    yminCoor = ymaxCoor = coorT[0].y1;
    for (i = 0; i < ccoor; i++) {
      yminCoor = Min(yminCoor, coorT[i].y1);
      yminCoor = Min(yminCoor, coorT[i].y2);
      ymaxCoor = Max(ymaxCoor, coorT[i].y1);
      ymaxCoor = Max(ymaxCoor, coorT[i].y2);
    }
    for (i = 0; i < ccoor; i++) {
      coorT[i].y1 = ymaxCoor + yminCoor - coorT[i].y1;
      coorT[i].y2 = ymaxCoor + yminCoor - coorT[i].y2;
    }
  }

  // Rotate and adjust coordinates so viewing location is central.

  RotateR2Init(rS, rC, theta);
  for (i = 0; i < ccoor; i++) {
    coorT[i].x1 = (coorT[i].x1 - xpos)*ds.rxScale;
    coorT[i].y1 = (coorT[i].y1 - ypos)*ds.ryScale;
    coorT[i].z1 = (coorT[i].z1 - zpos)*ds.rzScale;
    RotateR2(&coorT[i].x1, &coorT[i].y1, rS, rC);
    coorT[i].x2 = (coorT[i].x2 - xpos)*ds.rxScale;
    coorT[i].y2 = (coorT[i].y2 - ypos)*ds.ryScale;
    coorT[i].z2 = (coorT[i].z2 - zpos)*ds.rzScale;
    RotateR2(&coorT[i].x2, &coorT[i].y2, rS, rC);
  }
  if (phi != 0.0) {
    RotateR2Init(rS, rC, phi);
    for (i = 0; i < ccoor; i++) {
      RotateR2(&coorT[i].y1, &coorT[i].z1, rS, rC);
      RotateR2(&coorT[i].y2, &coorT[i].z2, rS, rC);
    }
  }

  // Drop or adjust all coordinate pairs that are behind the viewer.

  for (i = 0; i < ccoor; i++) {
    if (coorT[i].y1 >= 0.0 || coorT[i].y2 >= 0.0) {
      if (coorT[i].y1 < 0.0)
        IgnoreNegativeR(&coorT[i].x1, &coorT[i].y1, &coorT[i].z1,
          coorT[i].x2, coorT[i].y2, coorT[i].z2);
      if (coorT[i].y2 < 0.0)
        IgnoreNegativeR(&coorT[i].x2, &coorT[i].y2, &coorT[i].z2,
          coorT[i].x1, coorT[i].y1, coorT[i].z1);
      coorT[ccoor2] = coorT[i];
      coorT[ccoor2].z1 = coorT[i].z1 - (real)ymax + ds.rHoriz;
      coorT[ccoor2].z2 = coorT[i].z2 - (real)ymax + ds.rHoriz;
      ccoor2++;
    }
  }
  PrintSzL("Visible number of coordinate pairs: %ld\n", ccoor2);

  // Draw the lines on the screen.

  RenderInitialize(b, phi);
  for (i = 0; i < ccoor2; i++) {
    CalculateCoordinate(&x1, &y1, coorT[i].x1, coorT[i].y1, coorT[i].z1);
    CalculateCoordinate(&x2, &y2, coorT[i].x2, coorT[i].y2, coorT[i].z2);
    if ((x1 >= 0 || x2 >= 0) && (y1 >= 0 || y2 >= 0) &&
      (x1 < xmax || x2 < xmax) && (y1 < ymax || y2 < ymax)) {
      ClipOutside(xmax-1, ymax-1, &x1, &y1, &x2, &y2);
      if (b.FLegal(x1, y1) && b.FLegal(x2, y2)) {
        b.Line(x1, y1, x2, y2, !fColor ? fOff : ds.kvTrim);
        ccoor3++;
      }
    }
  }
  PrintSzL("Drawn number of coordinate pairs: %ld\n", ccoor3);

  RenderFinalize(b);
  DeallocateP(coorT);
  return fTrue;
}


/*
******************************************************************************
** Render Perspective Patches
******************************************************************************
*/

#define FComparePatch(pat1, pat2) ((pat1)->rDistance > (pat2)->rDistance)

// Given a list of patches forming a heap style of binary tree, where a parent
// is greater than its two children, and the index of a random value patch in
// it, push the patch down through the tree until the heap condition is met
// again. Called from FRenderPerspectivePatch() to implement heap sort.

void PushdownPatch(PATCH *patch, long p, long size)
{
  PATCH patT;

  while (p < size && (FComparePatch(&patch[p >> 1], &patch[p]) ||
    (p < size-1 && FComparePatch(&patch[p >> 1], &patch[p + 1])))) {
    p += (p < size-1 && FComparePatch(&patch[p], &patch[p + 1]));
    patT = patch[p]; patch[p] = patch[p >> 1]; patch[p >> 1] = patT;
    p <<= 1;
  }
}


// Draw a perspective scene composed of a set of triangular and quadrilateral
// patches in 3D space. Implements the Render Wireframe Perspective command
// for both monochrome and color bitmaps.

flag FRenderPerspectivePatch(CMap &b, PATCH *patch, long cpatch)
{
  PATCH *patchT, patT;
  PATR patrT;
  long cpatch2 = 0, cpatch3 = 0, count, i, j;
  int x1, y1, x2, y2, x3, y3, x4, y4, cpt, xmax, ymax, k;
  flag fSquare, fTouch, fTest;
  KV kv;
  CVector vLight, vEye, v;
  real xpos, ypos, zpos, yminCoor, ymaxCoor, theta, phi, rS, rC, rT;
  char sz[cchSzDef];
  flag fColor = b.FColor();

  xmax = b.m_x; ymax = b.m_y;
  ds.xmax = xmax;
  xpos  = (real)ds.hormin;
  ypos  = (real)ds.vermin;
  zpos  = (real)ds.depmin;
  theta = (real)ds.theta;
  phi   = (real)ds.phi;
  ds.rHoriz = (real)((ymax >> 1) + ds.verv);
  vLight = ds.vLight;

  // Get starting set of patches.

  patchT = RgAllocate(Max(cpatch, 1), PATCH);
  if (patchT == NULL)
    return fFalse;
  for (i = 0; i < cpatch; i++)
    patchT[i] = patch[i];

  if (ds.fReflect) {
    yminCoor = ymaxCoor = patchT[0].p[0].y;
    for (i = 0; i < cpatch; i++)
      for (k = patchT[i].cpt-1; k >= 0; k--) {
        yminCoor = Min(yminCoor, patchT[i].p[k].y);
        ymaxCoor = Max(ymaxCoor, patchT[i].p[k].y);
      }
    for (i = 0; i < cpatch; i++) {
      for (k = patchT[i].cpt-1; k >= 0; k--)
        patchT[i].p[k].y = ymaxCoor + yminCoor - patchT[i].p[k].y;
    }
  } else {
    for (i = 0; i < cpatch; i++) {
      cpt = patchT[i].cpt-1;
      patrT = patchT[i].p[1];
      patchT[i].p[1] = patchT[i].p[cpt];
      patchT[i].p[cpt] = patrT;
      fTest = patchT[i].p[0].fLine;
      for (k = 0; k < cpt; k++)
        patchT[i].p[k].fLine = patchT[i].p[k + 1].fLine;
      patchT[i].p[cpt].fLine = fTest;
    }
  }
  fTouch = ds.fEdges && ds.fTouch;

  // Rotate and adjust patches so viewing location is central.

  RotateR2Init(rS, rC, theta);
  for (i = 0; i < cpatch; i++)
    for (k = patchT[i].cpt-1; k >= 0; k--) {
      patchT[i].p[k].x = (patchT[i].p[k].x - xpos)*ds.rxScale;
      patchT[i].p[k].y = (patchT[i].p[k].y - ypos)*ds.ryScale;
      patchT[i].p[k].z = (patchT[i].p[k].z - zpos)*ds.rzScale;
      RotateR2(&patchT[i].p[k].x, &patchT[i].p[k].y, rS, rC);
    }
  RotateR2(&vLight.m_x, &vLight.m_y, rS, rC);
  if (phi != 0.0) {
    RotateR2Init(rS, rC, phi);
    for (i = 0; i < cpatch; i++)
      for (k = patchT[i].cpt-1; k >= 0; k--)
        RotateR2(&patchT[i].p[k].y, &patchT[i].p[k].z, rS, rC);
    RotateR2(&vLight.m_y, &vLight.m_z, rS, rC);
  }

  // Drop all patches that are behind the viewer.

  for (i = 0; i < cpatch; i++) {
    cpt = patchT[i].cpt;
    if (patchT[i].p[0].y < 0.0 || patchT[i].p[1].y < 0.0 ||
      patchT[i].p[2].y < 0.0 || (cpt >= 3 && patchT[i].p[3].y < 0.0))
      continue;

    // Drop patches facing away from the viewer.
    if (!ds.fRight) {
      v.Normal(patchT[i].p[0].x, patchT[i].p[0].y, patchT[i].p[0].z,
        patchT[i].p[1].x, patchT[i].p[1].y, patchT[i].p[1].z,
        patchT[i].p[2].x, patchT[i].p[2].y, patchT[i].p[2].z);
      vEye.Set(patchT[i].p[0].x, patchT[i].p[0].y, patchT[i].p[0].z);
      if (vEye.Dot(v) > 0.0)
        continue;
    }
    patchT[cpatch2] = patchT[i];
    for (k = 0; k < cpt; k++)
      patchT[cpatch2].p[k].z =
        patchT[i].p[k].z - (real)ymax + ds.rHoriz;
    cpatch2++;
    if (cpt <= 3)
      cpatch3++;
  }
  sprintf(S(sz),
    "Visible number of patches: %ld (%ld triangle, %ld square)\n",
    cpatch2, cpatch3, cpatch2 - cpatch3);
  PrintSz(sz);

  // Sort patches in order from farthest away to closest.

  for (i = 0; i < cpatch2; i++) {
    // Calculate the distance of the center of the patch from viewing plane.
    rT = 0.0;
    for (j = patchT[i].cpt-1; j >= 0; j--)
      rT += Sq(patchT[i].p[j].y) + Sq(patchT[i].p[j].z) +
        Sq(patchT[i].p[j].x);
    patchT[i].rDistance = rT / patchT[i].cpt;
  }
  // Heap sort the list of patches.
  for (i = (cpatch2 - 1) >> 1; i >= 0; i--)
    PushdownPatch(patchT, i << 1, cpatch2);
  for (i = cpatch2 - 1; i > 0; i--) {
    patT = patchT[i]; patchT[i] = patchT[0]; patchT[0] = patT;
    PushdownPatch(patchT, 0L, i);
  }
#ifdef DEBUG
  for (i = 1; i < cpatch2; i++)
    if (FComparePatch(&patchT[i], &patchT[i - 1])) {
      PrintSzN_E("Patch %d was sorted incorrectly.\n", i);
      break;
    }
#endif

  // Draw the patches on the screen.

  RenderInitialize(b, phi);
  count = cpatch3 = 0;
  for (i = 0; i < cpatch2; i++) {
    fSquare = patchT[i].cpt >= 4;
    CalculateCoordinate(&x1, &y1,
      patchT[i].p[0].x, patchT[i].p[0].y, patchT[i].p[0].z);
    CalculateCoordinate(&x2, &y2,
      patchT[i].p[1].x, patchT[i].p[1].y, patchT[i].p[1].z);
    CalculateCoordinate(&x3, &y3,
      patchT[i].p[2].x, patchT[i].p[2].y, patchT[i].p[2].z);
    if (fSquare) {
      CalculateCoordinate(&x4, &y4,
        patchT[i].p[3].x, patchT[i].p[3].y, patchT[i].p[3].z);
    } else {
      x4 = x1; y4 = y1;
    }
    if (((x1 >= 0)  || (x2 >= 0)    || (x3 >= 0)    || (x4 >= 0))    &&
      ((x1 <= xmax) || (x2 <= xmax) || (x3 <= xmax) || (x4 <= xmax)) &&
      ((y1 >= 0)    || (y2 >= 0)    || (y3 >= 0)    || (y4 >= 0))    &&
      ((y1 <= ymax) || (y2 <= ymax) || (y3 <= ymax) || (y4 <= ymax))) {

      // Figure out the color of this patch based on the light vector.
      if (ds.fShading) {
        v.Normal(patchT[i].p[0].x, patchT[i].p[0].y, patchT[i].p[0].z,
          patchT[i].p[1].x, patchT[i].p[1].y, patchT[i].p[1].z,
          patchT[i].p[2].x, patchT[i].p[2].y, patchT[i].p[2].z);
        if (!fColor)
          k = -(int)(vLight.Angle(v) / rPi * 16.99);
        else
          kv = KvShade(patchT[i].kv,
            (vLight.Angle(v) - rPiHalf) / rPiHalf);
      } else {
        if (!fColor)
          k = 1;
        else
          kv = patchT[i].kv;
      }
      if (!fColor) {

        // Draw a monochrome patch.
        if (fSquare)
          b.FQuadrilateral(x1, y1, x2, y2, x3, y3, x4, y4, k, 0);
        else
          b.FTriangle(x1, y1, x2, y2, x3, y3, k, 0);
        if (ds.fEdges) {
          if (patchT[i].p[0].fLine)
            b.Line(x1, y1, x2, y2, fOff);
          if (patchT[i].p[1].fLine)
            b.Line(x2, y2, x3, y3, fOff);
          if (fSquare) {
            if (patchT[i].p[2].fLine)
              b.Line(x3, y3, x4, y4, fOff);
            if (patchT[i].p[3].fLine)
              b.Line(x4, y4, x1, y1, fOff);
          } else
            if (patchT[i].p[2].fLine)
              b.Line(x3, y3, x1, y1, fOff);
        }
      } else {

        // Draw a color patch.
        if (ds.nFog > 0) {
          rT = 0.0;
          for (k = patchT[i].cpt-1; k >= 0; k--)
            rT += patchT[i].p[k].y;
          rT = rT / patchT[i].cpt / 10.0 / (real)ds.nFog;
          kv = KvBlendR(kv, ds.kvTrim, Min(rT, 1.0));
        }
        if (fSquare)
          b.FQuadrilateral(x1, y1, x2, y2, x3, y3, x4, y4,
            kv, patchT[i].nTrans);
        else
          b.FTriangle(x1, y1, x2, y2, x3, y3, kv, patchT[i].nTrans);
        if (ds.fEdges) {
          if (patchT[i].p[0].fLine)
            b.Line(x1, y1, x2, y2, ds.kvTrim);
          if (patchT[i].p[1].fLine)
            b.Line(x2, y2, x3, y3, ds.kvTrim);
          if (fSquare) {
            if (patchT[i].p[2].fLine)
              b.Line(x3, y3, x4, y4, ds.kvTrim);
            if (patchT[i].p[3].fLine)
              b.Line(x4, y4, x1, y1, ds.kvTrim);
          } else
            if (patchT[i].p[2].fLine)
              b.Line(x3, y3, x1, y1, ds.kvTrim);
        }
      }
      if (fTouch) {
        patchT[i].p[0].x = (real)x1; patchT[i].p[0].y = (real)y1;
        patchT[i].p[1].x = (real)x2; patchT[i].p[1].y = (real)y2;
        patchT[i].p[2].x = (real)x3; patchT[i].p[2].y = (real)y3;
        patchT[i].p[3].x = (real)x4; patchT[i].p[3].y = (real)y4;
      }
      count++;
      cpatch3 += !fSquare;
    } else if (fTouch)
      for (k = 0; k < 4; k++)
        patchT[i].p[k].fLine = fFalse;
  }
  sprintf(S(sz), "Drawn number of patches: %ld (%ld triangle, %ld square)\n",
    count, cpatch3, count - cpatch3); PrintSz(sz);

  // Touch up edges on the screen. Very slow, but helps avoid blemishes formed
  // by later patches that slightly overlap edges of earlier patches.

  if (fTouch) {
    for (j = 0; j < cpatch2 - 1; j++) {
      cpt = patchT[j].cpt;
      for (k = 0; k < cpt; k++)
        for (i = j + 1; i < cpatch2; i++)
          if (patchT[j].p[k].fLine) {
            fTouch = fTrue;
            for (count = 0; count < 4; count++)
              fTouch = fTouch &&
                patchT[j].p[k].x <= patchT[i].p[count].x &&
                patchT[j].p[(k + 1) % cpt].x <= patchT[i].p[count].x;
            fTest = fTouch;
            fTouch = fTrue;
            for (count = 0; count < 4; count++)
              fTouch = fTouch &&
                patchT[j].p[k].y <= patchT[i].p[count].y &&
                patchT[j].p[(k + 1) % cpt].y <= patchT[i].p[count].y;
            fTest = fTest || fTouch;
            fTouch = fTrue;
            for (count = 0; count < 4; count++)
              fTouch = fTouch &&
                patchT[j].p[k].x >= patchT[i].p[count].x &&
                patchT[j].p[(k + 1) % cpt].x >= patchT[i].p[count].x;
            fTest = fTest || fTouch;
            fTouch = fTrue;
            for (count = 0; count < 4; count++)
              fTouch = fTouch &&
                patchT[j].p[k].y >= patchT[i].p[count].y &&
                patchT[j].p[(k + 1) % cpt].y >= patchT[i].p[count].y;
            patchT[j].p[k].fLine = fTest || fTouch;
          }
    }
    for (i = 0; i < cpatch2; i++) {
      cpt = patchT[i].cpt;
      for (k = 0; k < cpt; k++)
        if (patchT[i].p[k].fLine) {
          x1 = (int)patchT[i].p[k].x;
          y1 = (int)patchT[i].p[k].y;
          x2 = (int)patchT[i].p[(k + 1) % cpt].x;
          y2 = (int)patchT[i].p[(k + 1) % cpt].y;
          b.Line(x1, y1, x2, y2, !fColor ? fOff : ds.kvTrim);
        }
    }
  }

  RenderFinalize(b);
  DeallocateP(patchT);
  return fTrue;
}


/*
******************************************************************************
** Other Render Routines
******************************************************************************
*/

// Draw a scene of the same line segment list used by FRenderPerspectiveWire,
// in a simple non-perspective view looking straight down from the z-axis.
// Implements the Render Wireframe Aerial command.

flag FRenderAerial(CMon &b, CONST COOR *coor, long count)
{
  long i;
  int xmin = 0, ymin = 0, xmax = 0, ymax = 0, x1, y1, x2, y2;

  // Figure out the bounds of all the line segments.
  for (i = 0; i < count; i++) {
    x1 = (int)coor[i].x1; y1 = (int)coor[i].y1;
    x2 = (int)coor[i].x2; y2 = (int)coor[i].y2;
    xmin = Min(x1, xmin); xmin = Min(x2, xmin);
    ymin = Min(y1, ymin); ymin = Min(y2, ymin);
    xmax = Max(x1, xmax); xmax = Max(x2, xmax);
    ymax = Max(y1, ymax); ymax = Max(y2, ymax);
  }

  // Resize the bitmap to encompass these bounds.
  if (!b.FBitmapSizeSet(xmax-xmin+1, ymax-ymin+1))
    return fFalse;

  // Draw all the line segments.
  b.BitmapOn();
  for (i = 0; i < count; i++) {
    x1 = (int)coor[i].x1 - xmin; y1 = (int)coor[i].y1 - ymin;
    x2 = (int)coor[i].x2 - xmin; y2 = (int)coor[i].y2 - ymin;
    b.Line(x1, y1, x2, y2, fOff);
  }
  return fTrue;
}


// Given a line segment, return whether it already exists in the list of line
// segments. Called from ConvertPatchToWire() to avoid including edges between
// patches more than once.

flag FVerifyWire(COOR *coor, long count)
{
  real rT;
  long i;

  // Sort the two endpoints of the line segment.
  if (coor[count].z2 < coor[count].z1 || (coor[count].z2 == coor[count].z1
    && (coor[count].y2 < coor[count].y1 || (coor[count].y2 ==
    coor[count].y1 && coor[count].x2 < coor[count].x1)))) {
    rT = coor[count].x1;
    coor[count].x1 = coor[count].x2;
    coor[count].x2 = rT;
    rT = coor[count].y1;
    coor[count].y1 = coor[count].y2;
    coor[count].y2 = rT;
    rT = coor[count].z1;
    coor[count].z1 = coor[count].z2;
    coor[count].z2 = rT;
  }

  // Check whether the line segment already exists in the list.
  for (i = count - 1; i >= 0; i--)
    if (coor[count].x1 == coor[i].x1 && coor[count].x2 == coor[i].x2 &&
      coor[count].y1 == coor[i].y1 && coor[count].y2 == coor[i].y2 &&
      coor[count].z1 == coor[i].z1 && coor[count].z2 == coor[i].z2) {
      return fFalse;
    }
  return fTrue;
}


// Generate a wireframe list of line segments based on the list of patches in
// memory, with one line segment for each visible edge of a patch. Implements
// the Convert Patches To Wireframe command.

long ConvertPatchToWire(COOR **pcoor, CONST PATCH *patch, long cpatch)
{
  COOR *coor;
  long count = 0, i;

  if (cpatch == 0)
    return 0;
  coor = RgAllocate(cpatch*4, COOR);
  if (coor == NULL)
    return -1;
  if (*pcoor != NULL)
    DeallocateP(*pcoor);
  *pcoor = coor;
  for (i = 0; i < cpatch; i++) {
    if (patch[i].p[0].fLine) {
      coor[count].x1 = patch[i].p[0].x;
      coor[count].y1 = patch[i].p[0].y;
      coor[count].z1 = patch[i].p[0].z;
      coor[count].x2 = patch[i].p[1].x;
      coor[count].y2 = patch[i].p[1].y;
      coor[count].z2 = patch[i].p[1].z;
      count += FVerifyWire(coor, count);
    }
    if (patch[i].p[1].fLine) {
      coor[count].x1 = patch[i].p[1].x;
      coor[count].y1 = patch[i].p[1].y;
      coor[count].z1 = patch[i].p[1].z;
      coor[count].x2 = patch[i].p[2].x;
      coor[count].y2 = patch[i].p[2].y;
      coor[count].z2 = patch[i].p[2].z;
      count += FVerifyWire(coor, count);
    }
    if (patch[i].cpt <= 3) {
      if (patch[i].p[2].fLine) {
        coor[count].x1 = patch[i].p[2].x;
        coor[count].y1 = patch[i].p[2].y;
        coor[count].z1 = patch[i].p[2].z;
        coor[count].x2 = patch[i].p[0].x;
        coor[count].y2 = patch[i].p[0].y;
        coor[count].z2 = patch[i].p[0].z;
        count += FVerifyWire(coor, count);
      }
    } else {
      if (patch[i].p[2].fLine) {
        coor[count].x1 = patch[i].p[2].x;
        coor[count].y1 = patch[i].p[2].y;
        coor[count].z1 = patch[i].p[2].z;
        coor[count].x2 = patch[i].p[3].x;
        coor[count].y2 = patch[i].p[3].y;
        coor[count].z2 = patch[i].p[3].z;
        count += FVerifyWire(coor, count);
      }
      if (patch[i].p[3].fLine) {
        coor[count].x1 = patch[i].p[3].x;
        coor[count].y1 = patch[i].p[3].y;
        coor[count].z1 = patch[i].p[3].z;
        coor[count].x2 = patch[i].p[0].x;
        coor[count].y2 = patch[i].p[0].y;
        coor[count].z2 = patch[i].p[0].z;
        count += FVerifyWire(coor, count);
      }
    }
  }
  return count;
}

/* draw2.cpp */
