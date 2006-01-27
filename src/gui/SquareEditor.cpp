/* Burr Solver
 * Copyright (C) 2003-2006  Andreas R�ver
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


#include "SquareEditor.h"
#include "pieceColor.h"

#include <FL/fl_draw.h>

static int floordiv(int a, int b) {
  if (a > 0)
    return a/b;
  else
    return (a-b+1)/b;
}

void SquareEditor::setPuzzle(puzzle_c * p, unsigned int piecenum) {

  puzzle = p;
  piecenumber = piecenum;

  // check if the current z value is in valid regions
  if ((piecenum < puzzle->shapeNumber()) && (puzzle->getShape(piecenum)->getZ() <= currentZ))
    currentZ = puzzle->getShape(piecenum)->getZ() - 1;

  if (currentZ < 0) currentZ = 0;

  redraw();
}

void SquareEditor::setZ(unsigned int z) {

  // clamp the value to valid values
  if (z >= puzzle->getShape(piecenumber)->getZ()) z = puzzle->getShape(piecenumber)->getZ()-1;

  if (z != currentZ) {

    mZ = currentZ = z;
    redraw();
    callbackReason = RS_MOUSEMOVE;
    do_callback();
  }

}

// this function finds out if a given square is inside the selected region
// this check includes the symmetric and comulmn edit modes
static bool inRegion(int x, int y, int x1, int x2, int y1, int y2, int sx, int sy, int mode) {

  if ((x < 0) || (y < 0) || (x >= sx) || (y >= sy)) return false;

  mode &= ~ (SquareEditor::TOOL_STACK_Z + SquareEditor::TOOL_MIRROR_Z);

  if (mode == 0)
    return (x1 <= x) && (x <= x2) && (y1 <= y) && (y <= y2);
  if (mode == SquareEditor::TOOL_STACK_Y)
    return (x1 <= x) && (x <= x2);
  if (mode == SquareEditor::TOOL_STACK_X)
    return (y1 <= y) && (y <= y2);
  if (mode == SquareEditor::TOOL_STACK_X + SquareEditor::TOOL_STACK_Y)
    return (x1 <= x) && (x <= x2) || (y1 <= y) && (y <= y2);

  if (mode & SquareEditor::TOOL_MIRROR_X)
    return inRegion(x, y, x1, x2, y1, y2, sx, sy, mode & ~SquareEditor::TOOL_MIRROR_X) ||
      inRegion(sx-x-1, y, x1, x2, y1, y2, sx, sy, mode & ~SquareEditor::TOOL_MIRROR_X);

  if (mode & SquareEditor::TOOL_MIRROR_Y)
    return inRegion(x, y, x1, x2, y1, y2, sx, sy, mode & ~SquareEditor::TOOL_MIRROR_Y) ||
      inRegion(x, sy-y-1, x1, x2, y1, y2, sx, sy, mode & ~SquareEditor::TOOL_MIRROR_Y);

  return false;
}

void SquareEditor::calcParameters(int *s, int *tx, int *ty) {

  // calculate the size of the squares
  int sx = (w() > 2) ? (w()-1) / puzzle->getShape(piecenumber)->getX() : 0;
  int sy = (h() > 2) ? (h()-1) / puzzle->getShape(piecenumber)->getY() : 0;

  *s = (sx < sy) ? sx : sy;

  if (*s > 20) *s = 20;

  *tx = x() + (w() - puzzle->getShape(piecenumber)->getX()*(*s) - 1) / 2;
  *ty = y() + (h() - puzzle->getShape(piecenumber)->getY()*(*s) - 1) / 2;
}

void SquareEditor::draw() {

  // draw the background
  fl_color(color());
  fl_rectf(x(), y(), w(), h());

  unsigned char bgr, bgg, bgb;
  Fl::get_color(color(), bgr, bgg, bgb);

  if (piecenumber >= puzzle->shapeNumber())
    return;

  voxel_c * space = puzzle->getShape(piecenumber);

  // if there is no voxelspace or the space is of volumn 0 return
  if ((space->getX() == 0) || (space->getY() == 0) || (space->getZ() == 0))
    return;

  int s, tx, ty;
  calcParameters(&s, &tx, &ty);

  // the color for the grid lines
  if (locked)
    fl_color(color());
  else
    fl_color(labelcolor());

  // the color for the squares
  unsigned char r, g, b;

  for (unsigned int x = 0; x < space->getX(); x++)
    for (unsigned int y = 0; y < space->getY(); y++) {
      if ((x+y+currentZ) & 1) {
        r = int(255*darkPieceColor(pieceColorR(piecenumber)));
        g = int(255*darkPieceColor(pieceColorG(piecenumber)));
        b = int(255*darkPieceColor(pieceColorB(piecenumber)));
      } else {
        r = int(255*lightPieceColor(pieceColorR(piecenumber)));
        g = int(255*lightPieceColor(pieceColorG(piecenumber)));
        b = int(255*lightPieceColor(pieceColorB(piecenumber)));
      }

      switch(space->getState(x, space->getY()-y-1, currentZ)) {
      case voxel_c::VX_FILLED:
        fl_rectf(tx+x*s, ty+y*s, s, s, r, g, b);
        break;
      case voxel_c::VX_VARIABLE:
        fl_rectf(tx+x*s+3, ty+y*s+3, s-5, s-5, r, g, b);
        break;
      default:
        if (currentZ > 0)
          if (space->getState(x, space->getY()-y-1, currentZ-1) != voxel_c::VX_EMPTY) {
            fl_rectf(tx+x*s, ty+y*s, s, s, ((int)bgr*5+r)/6, ((int)bgg*5+g)/6, ((int)bgb*5+b)/6);
          }
      }

      if ((space->getState(x, space->getY()-y-1, currentZ) != voxel_c::VX_EMPTY) &&
          space->getColor(x, space->getY()-y-1, currentZ)) {

        puzzle->getColor(space->getColor(x, space->getY()-y-1, currentZ)-1, &r, &g, &b);
        fl_rectf(tx+x*s, ty+y*s, s/2, s/2, r, g, b);
      }

      if (locked)
        fl_color(color());
      else
        fl_color(labelcolor());
      fl_rect(tx+x*s, ty+y*s, s+1, s+1);
    }

  // when we do something we need to draw a frame around the
  // squares that get edited
  if (inside) {

    int x1, x2, y1, y2;

    if (startX < mX) {
      x1 = startX;
      x2 = mX;
    } else {
      x2 = startX;
      x1 = mX;
    }

    if (x1 < 0) x1 = 0;
    if (x2 >= (int)space->getX()) x2 = (int)space->getX()-1;

    if (startY > mY) {
      y1 = startY;
      y2 = mY;
    } else {
      y2 = startY;
      y1 = mY;
    }

    if (y2 < 0) y2 = 0;
    if (y1 >= (int)space->getY()) y1 = (int)space->getY()-1;

    y1 = space->getY() - y1 - 1;
    y2 = space->getY() - y2 - 1;

    if ((x1 <= x2) && (y1 <= y2)) {

      // ok, we have a valid range selected, now we need to check for
      // edit mode (symmetric modes, ...) and draw the right cursor for the current mode

      fl_color(labelcolor());

      for (unsigned int x = 0; x <= space->getX(); x++)
        for (unsigned int y = 0; y <= space->getY(); y++) {
          bool ins = inRegion(x, y, x1, x2, y1, y2, space->getX(), space->getY(), activeTools);

          if (ins ^ inRegion(x, y-1, x1, x2, y1, y2, space->getX(), space->getY(), activeTools)) {
            fl_line(tx+s*x, ty+s*y+1, tx+s*x+s, ty+s*y+1);
            fl_line(tx+s*x, ty+s*y-1, tx+s*x+s, ty+s*y-1);
          }

          if (ins ^ inRegion(x-1, y, x1, x2, y1, y2, space->getX(), space->getY(), activeTools)) {
            fl_line(tx+s*x+1, ty+s*y, tx+s*x+1, ty+s*y+s);
            fl_line(tx+s*x-1, ty+s*y, tx+s*x-1, ty+s*y+s);
          }
        }
    }
  }
}

static bool set(voxel_c * s, int x, int y, int z, SquareEditor::enTask task, unsigned int currentColor) {

  bool changed = false;
  voxel_type v = voxel_c::VX_EMPTY;

  switch (task) {
  case SquareEditor::TSK_SET:
    v = voxel_c::VX_FILLED;
    break;
  case SquareEditor::TSK_VAR:
    v = voxel_c::VX_VARIABLE;
    break;
  default:
    break;
  }

  // on all other tasks but the color changing one, we need to set the state of the voxel
  if ((task != SquareEditor::TSK_COLOR) && (s->getState(x, y, z) != v)) {
    changed = true;
    s->setState(x, y, z, v);
  }

  // this is for the color change task
  if ((s->getState(x, y, z) != voxel_c::VX_EMPTY) && (s->getColor(x, y, z) != currentColor)) {
    changed = true;
    s->setColor(x, y, z, currentColor);
  }

  return changed;
}

static bool setStacks(voxel_c * s, unsigned char tools, int x, int y, int z, SquareEditor::enTask task, unsigned int currentColor) {

  bool changed = set(s, x, y, z, task, currentColor);

  if (tools & SquareEditor::TOOL_STACK_X)
    for (unsigned int xp = 0; xp < s->getX(); xp++)
      changed |= set(s, xp, y, z, task, currentColor);
  if (tools & SquareEditor::TOOL_STACK_Y)
    for (unsigned int yp = 0; yp < s->getY(); yp++)
      changed |= set(s, x, yp, z, task, currentColor);
  if (tools & SquareEditor::TOOL_STACK_Z)
    for (unsigned int zp = 0; zp < s->getZ(); zp++)
      changed |= set(s, x, y, zp, task, currentColor);

  return changed;
}

static bool setMz(voxel_c * s, unsigned char tools, int x, int y, int z, SquareEditor::enTask task, unsigned int currentColor) {
  bool changed = setStacks(s, tools, x, y, z, task, currentColor);
  if (tools & SquareEditor::TOOL_MIRROR_Z)
    changed |= setStacks(s, tools, x, y, s->getZ()-z-1, task, currentColor);

  return changed;
}

static bool setMy(voxel_c * s, unsigned char tools, int x, int y, int z, SquareEditor::enTask task, unsigned int currentColor) {
  bool changed = setMz(s, tools, x, y, z, task, currentColor);

  if (tools & SquareEditor::TOOL_MIRROR_Y)
    changed |= setMz(s, tools, x, s->getY()-y-1, z, task, currentColor);

  return changed;
}


static bool setMx(voxel_c * s, unsigned char tools, int x, int y, int z, SquareEditor::enTask task, unsigned int currentColor) {
  bool changed = setMy(s, tools, x, y, z, task, currentColor);
  if (tools & SquareEditor::TOOL_MIRROR_X)
    changed |= setMy(s, tools, s->getX()-x-1, y, z, task, currentColor);

  return changed;
}

bool SquareEditor::setLayer(unsigned int z) {
  int x1, x2, y1, y2;

  voxel_c * space = puzzle->getShape(piecenumber);

  if (mX < startX) {
    x1 = mX;
    x2 = startX;
  } else {
    x2 = mX;
    x1 = startX;
  }

  if (mY < startY) {
    y1 = mY;
    y2 = startY;
  } else {
    y2 = mY;
    y1 = startY;
  }

  if (x1 < 0) x1 = 0;
  if (x2 > (int)space->getX()-1) x2 = (int)space->getX()-1;
  if (y1 < 0) y1 = 0;
  if (y2 > (int)space->getY()-1) y2 = (int)space->getY()-1;

  bool changed = false;

  for (int x = x1; x <= x2; x++)
    for (int y = y1; y <= y2; y++)
      if ((x >= 0) && (y >= 0) && (x < (int)space->getX()) && (y < (int)space->getY()))
        changed |= setMx(space, activeTools, x, y, z, task, currentColor);

  return changed;
}


int SquareEditor::handle(int event) {

  if (piecenumber >= puzzle->shapeNumber())
    return 0;

  if (locked)
    return 0;

  voxel_c * space = puzzle->getShape(piecenumber);

  // if there is no valid space, we do nothing
  if ((space->getX() == 0) || (space->getY() == 0) || (space->getZ() == 0))
    return 0;

  switch(event) {
  case FL_RELEASE:
    {
      state = 0;

      int s, tx, ty;
      calcParameters(&s, &tx, &ty);

      long x = Fl::event_x() - tx;
      long y = Fl::event_y() - ty;

      x = floordiv(x, s);
      y = floordiv(y, s);

      y = space->getY() - y - 1;

      // check, if the current position is inside the grid, only if so carry out action
      if (0 <= x && x < (long)space->getX() && 0 <= y && y < (long)space->getY() && setLayer(currentZ)) {
        callbackReason = RS_CHANGESQUARE;
        do_callback();
      }

      redraw();
    }

    return 1;
  case FL_PUSH:
    state = 1;
  case FL_ENTER:
    inside = true;
    if (event == FL_ENTER)
      Fl::belowmouse(this);
    // fallthrough
  case FL_DRAG:
  case FL_MOVE:
    {
      /* find out where the mouse cursor is */
      int s, tx, ty;
      calcParameters(&s, &tx, &ty);

      long x = Fl::event_x() - tx;
      long y = Fl::event_y() - ty;

      x = floordiv(x, s);
      y = floordiv(y, s);

      y = space->getY() - y - 1;

      // clip the coordinates to the size of the space
      if (x < 0) x = 0;
      if (y < 0) y = 0;
      if (x >= (long)space->getX()) x = space->getX() - 1;
      if (y >= (long)space->getY()) y = space->getY() - 1;

      if (event == FL_PUSH || event == FL_MOVE || event == FL_ENTER) {
        mX = startX = x;
        mY = startY = y;
        mZ = currentZ;

        redraw();
        callbackReason = RS_MOUSEMOVE;
        do_callback();

      } else {

        // we move the mouse, if the new position is different from the saved one,
        // do a callback
        if ((x != (long)mX) || (y != (long)mY) || ((long)currentZ != (long)mZ)) {

          mX = x;
          mY = y;
          mZ = currentZ;

          if (editType == EDT_SINGLE) {
            startX = mX;
            startY = mY;
          }

          redraw();
          callbackReason = RS_MOUSEMOVE;
          do_callback();
        }
      }

      if ((event == FL_DRAG || event == FL_PUSH) && (editType == EDT_SINGLE))
        if (setLayer(currentZ)) {
          callbackReason = RS_CHANGESQUARE;
          redraw();
          do_callback();
        }

    }
    return 1;
  case FL_LEAVE:
    inside = false;
    redraw();
    callbackReason = RS_MOUSEMOVE;
    do_callback();
    return 1;
    break;
  }

  return 0;
}

