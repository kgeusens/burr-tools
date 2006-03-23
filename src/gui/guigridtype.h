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
#ifndef __GUI_GRID_TYPE__
#define __GUI_GRID_TYPE__

#include "../lib/gridtype.h"

class gridEditor_c;
class VoxelDrawer;
class LFl_Group;

class guiGridType_c {

  private:

    gridType_c * gt;

  public:

    guiGridType_c(gridType_c * gt);

    gridEditor_c * getGridEditor(int x, int y, int w, int h, puzzle_c * puzzle) const;
    VoxelDrawer * getVoxelDrawer(int x, int y, int w, int h) const;

    /* returns a group to edit the parameters for this grid type
     * is is used in the new puzzle grid selection dialog
     * and also in the later possible grid parameters dialog
     */
    LFl_Group * getCofigurationDialog(void) const;

    /* return icon and text for the current grid type */
    char * getIcon(void) const;
    const char * getName(void) const;
};

#endif
