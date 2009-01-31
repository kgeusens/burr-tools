/* Burr Solver
 * Copyright (C) 2003-2008  Andreas Röver
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
#include "converter.h"

#include "puzzle.h"
#include "problem.h"
#include "voxel.h"

bool canConvert(const puzzle_c * p, gridType_c::gridType type) {

  return (p->getGridType()->getType() == gridType_c::GT_BRICKS &&
      type == gridType_c::GT_RHOMBIC);

}

/* do the conversion, if it can't be done (you should check first)
 * nothing will happen.
 * the shapes inside the puzzle will be converted and then the
 * gridtype within the puzzle will be changed
 */
void doConvert(puzzle_c * p, gridType_c::gridType type) {

  if (!canConvert(p, type)) return;

  // for now only the conversion from brick to rhombic is available

  // create a gridType for the target grid
  gridType_c * gt = new gridType_c(type);

  // first remove all solutions, as they will no longer be valid
  for (unsigned int pr = 0; pr < p->problemNumber(); pr++)
    p->getProblem(pr)->removeAllSolutions();

  std::vector<voxel_c*> cvoxels;

  // now convert all shapes
  for (unsigned int i = 0; i < p->shapeNumber(); i++) {

    const voxel_c * v = p->getShape(i);

    // this is now conversion specific....
    voxel_c * vn = gt->getVoxel(v->getX()*5, v->getY()*5, v->getZ()*5, voxel_c::VX_EMPTY);

    for (unsigned int x = 0; x < v->getX(); x++)
      for (unsigned int y = 0; y < v->getY(); y++)
        for (unsigned int z = 0; z < v->getZ(); z++) {
          voxel_type st = v->get(x, y, z);

          for (int ax = 0; ax < 5; ax++)
            for (int ay = 0; ay < 5; ay++)
              for (int az = 0; az < 5; az++)
                if (vn->validCoordinate(ax, ay, az))
                  vn->set(5*x+ax, 5*y+ay, 5*z+az, st);
        }

    cvoxels.push_back(vn);
  }

  // now replace the gridType within puzzle

  p->setGridType(gt);

  // remove all shapes and add the new ones

  while (p->shapeNumber() > 0)
    p->removeShape(0);

  for (unsigned int i = 0; i < cvoxels.size(); i++)
    p->addShape(cvoxels[i]);
}

