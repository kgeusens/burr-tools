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
#ifndef __ASSEMBLER_0_FRONTENT_1_H__
#define __ASSEMBLER_0_FRONTENT_1_H__

#include "assembler_0.h"
#include "gridtype.h"

class puzzle_c;

class assm_0_frontend_1_c : public assembler_0_c {

private:

  /* this function creates the matrix for the search function
   * because we need to know how many nodes we need to allocate the
   * arrays with the right size, we add a parameter. if this is true
   * the function will not access the array but only count the number
   * of nodes used. this number is returned
   */
  int prepare(const puzzle_c * puz, int res_filles, int res_vari, unsigned int resultnum);

  assm_0_frontend_1_c(void) : assembler_0_c() {}
  friend assembler_0_c * gridType_c::getAssembler(void) const;

};

#endif