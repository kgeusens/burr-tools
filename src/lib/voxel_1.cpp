/* Burr Solver
 * Copyright (C) 2003-2006  Andreas Röver
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include "voxel_1.h"

#include <string.h>
#include <math.h>

void voxel_1_c::rotatex(int by) {

  by &= 1;

  if (by != 0) {

    // this works, when the vertical size is even

    unsigned int nsy = sy + (sy & 1);
    unsigned int nsx = sx;
    unsigned nsz = sz;

    voxel_type *s = new voxel_type[nsx*nsy*nsz];
    memset(s, outside, nsx*nsy*nsz);

    for (unsigned int x = 0; x < sx; x++)
      for (unsigned int z = 0; z < sz; z++)
        for (unsigned int y = 0; y < sy; y++)
          s[x+nsx*((nsy-y-1)+nsy*(nsz-z-1))] = space[x+sx*(y+sy*z)];

    delete [] space;
    space = s;
    sx = nsx;
    sy = nsy;
    sz = nsz;

    // TODO: recalc bounding box and hotspot position

    voxels = nsx*nsy*nsz;
  }

  symmetries = symmetryInvalid();
}

void voxel_1_c::rotatey(int by) {

  by &= 1;

  if (by != 0) {

    unsigned int nsy = sy;
    unsigned int nsx = sx + (1-(sx & 1));
    unsigned int nsz = sz;

    voxel_type *s = new voxel_type[nsx*nsy*nsz];
    memset(s, outside, nsx*nsy*nsz);

    for (unsigned int x = 0; x < sx; x++)
      for (unsigned int z = 0; z < sz; z++)
        for (unsigned int y = 0; y < sy; y++)
          s[(nsx-x-1)+nsx*(y+nsy*(nsz-z-1))] = space[x+sx*(y+sy*z)];

    delete [] space;
    space = s;
    sx = nsx;
    sy = nsy;
    sz = nsz;

    voxels = nsx*nsy*nsz;

    // TODO: recalc bounding box and hotspot position
  }

  symmetries = symmetryInvalid();
}

void voxel_1_c::rotatez(int by) {

  /* this is the complicated case
   * it works by first calculating directions for the x any y axis in the
   * rotated shape, this can be done with 2 vectors that are alternatingly
   * applied and let you skip from one triangle to the next
   *
   * with this vectors the resulting area of the shape is calculated with
   * this area the final grid size and position is calculated
   */
  by %= 6;

  // calculate rotation center and new grid size
  if (by != 0) {
    // the rotation center
    // translation, the

    int d1x1, d1y1, d1x2, d1y2;  // the 2 vectors for the x-direction
    int d2x1, d2y1, d2x2, d2y2;  // the 2 vectors for the y-direction

    d1x1 = d1y1 = d1x2 = d1y2 = d2x1 = d2y1 = d2x2 = d2y2 = 0;
    int px = 0;
    int py = 0;

    switch (by) {
    case 1:
      d1x1 =  0; d1y1 = 1; d1x2 =  1; d1y2 = 0; d2x1 = -2; d2y1 =  1; d2x2 = -1; d2y2 =  0;
      break;
    case 2:
      d1x1 = -1; d1y1 = 0; d1x2 =  0; d1y2 = 1; d2x1 = -2; d2y1 = -1; d2x2 = -1; d2y2 =  0;
      break;
    case 3:
      d1x1 = -1; d1y1 = 0; d1x2 = -1; d1y2 = 0; d2x1 =  0; d2y1 = -1; d2x2 =  0; d2y2 = -1;
      break;
    case 4:
      d1x1 = 0; d1y1 = -1; d1x2 =  -1; d1y2 = 0; d2x1 = 2; d2y1 =  -1; d2x2 = 1; d2y2 =  0;
      break;
    case 5:
      d1x1 =  1; d1y1 = 0; d1x2 =  0; d1y2 = -1; d2x1 =  2; d2y1 =  1; d2x2 =  1; d2y2 =  0;
      break;
    default:
      bt_assert(0);
    }

    // now find out the area that is occupied by the rotated space

    int ax1 = 10000;
    int ay1 = 10000;
    int ax2 = -10000;
    int ay2 = -10000;

    for (unsigned int y = 0; y < sy; y++) {

      int tx = px;
      int ty = py;

      for (unsigned int x = 0; x < sx; x++) {

        // if it is occupied, save the bounding box of the result shape
        for (unsigned int z = 0; z < sz; z++)
          if (get(x, y, z)) {
            if (tx < ax1) ax1 = tx;
            if (tx > ax2) ax2 = tx;

            if (ty < ay1) ay1 = ty;
            if (ty > ay2) ay2 = ty;
          }

        if ((x+y) & 1) {
          tx += d1x2;
          ty += d1y2;
        } else {
          tx += d1x1;
          ty += d1y1;
        }
      }

      if (y & 1) {
        px += d2x2;
        py += d2y2;
      } else {
        px += d2x1;
        py += d2y1;
      }
    }

    // when empty, don't do anything
    if (ax1 == 10000) return;

    // calculate the size of the new voxel space with the rotated solution
    int nsx = sx;
    int nsy = sy;
    int nsz = sz;

    // the rotated solution must fit into the new space, so make it larger if required
    if (ax2-ax1+1 > nsx) nsx = ax2-ax1+1;
    if (ay2-ay1+1 > nsy) nsy = ay2-ay1+1;

    // now shift, so that the rotated shape is in the center

    px = -ax1;
    py = -ay1;

    px += (nsx - (ax2-ax1+1)) / 2;
    py += (nsy - (ay2-ay1+1)) / 2;

    // now check, that the starting point has the right parity
    if (by & 1) {
      if (!((px+py) & 1)) px++;
    } else {
      if ((px+py) & 1) px++;
    }

    // if the grid now doesn't fit any more, make ist bigger
    if (ax2 + px >= nsx) nsx++;

    // perform the rotations and copy into new buffer
    voxel_type *s = new voxel_type[nsx*nsy*nsz];
    memset(s, outside, nsx*nsy*nsz);

    for (unsigned int y = 0; y < sy; y++) {

      int tx = px;
      int ty = py;

      for (unsigned int x = 0; x < sx; x++) {

        if ((tx >= 0) && (tx < nsx) && (ty >= 0) && (ty < nsy))
          for (unsigned int z = 0; z < nsz; z++)
            s[tx + nsx*(ty + nsy*z)] = space[x + sx*(y + sy*z)];

        if ((x+y) & 1) {
          tx += d1x2;
          ty += d1y2;
        } else {
          tx += d1x1;
          ty += d1y1;
        }
      }

      if (y & 1) {
        px += d2x2;
        py += d2y2;
      } else {
        px += d2x1;
        py += d2y1;
      }
    }

    delete [] space;
    space = s;
    sx = nsx;
    sy = nsy;
    sz = nsz;

    voxels = nsx*nsy*nsz;

    recalcBoundingBox();

    // FIXME: hotpot position

  }
  symmetries = symmetryInvalid();
}

void voxel_1_c::minimizePiece(void) {

  // find a first voxel
  unsigned int fx, fy;
  fx = fy = 0;

  for (unsigned int x = 0; x < sx; x++)
    for (unsigned int y = 0; y < sy; y++)
      for (unsigned int z = 0; z < sz; z++)
        if (!isEmpty(x, y, z)) {
          fx = x;
          fy = y;

          // break out of loops
          z = sz;
          y = sy;
          x = sx;
        }

  voxel_c::minimizePiece();

  // find the first voxel again
  for (unsigned int x = 0; x < sx; x++)
    for (unsigned int y = 0; y < sy; y++)
      for (unsigned int z = 0; z < sz; z++)
        if (!isEmpty(x, y, z)) {

          // if the parity of that voxel changed, add an empty column at the left
          if ( ((fx+fy) & 1) != ((x+y) & 1) ) {
            resize(sx+1, sy, sz, 0);
            translate(1, 0, 0, 0);
          }

          // break out of loops
          z = sz;
          y = sy;
          x = sx;
        }
}

