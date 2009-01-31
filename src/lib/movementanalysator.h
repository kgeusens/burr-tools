/* Burr Solver
 * Copyright (C) 2003-2008  Andreas R�ver
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
#ifndef __MOVEMENT_ANALYSATOR_H__
#define __MOVEMENT_ANALYSATOR_H__

#include <vector>

class puzzle_c;
class disassemblerNode_c;
class movementCache_c;
class assembly_c;
class countingNodeHash;

/* this class is can do analysation of movements within a puzzle
 *
 * is is implemented using Bill Cuttlers algorithm, so please read there
 * in case you are interested how it works. The comments are written with
 * the thought that you know his algorithm
 */
class movementAnalysator_c {

  private:

    /* matrix should normally have 6 subarrays, for each of the 6 possible
     * directions (positive x negative x, positive y, ...) one, but because
     * the matrix for the negative direction in the same dimension is the
     * transposition (m[i][j] == m[j][i]) we save the calculation or copying
     * and rather do the transposition inside the checkmovement function
     */
    unsigned int ** matrix;
    unsigned int * movement;
    int * weights;
    unsigned int piecenumber;

    movementCache_c * cache;

    countingNodeHash * nodes;

    /* these variables are used for the routine that looks
     * for the pieces to move find, checkmovement
     */
    int nextpiece, next_pn, nextstate, nextpiece2, state99nextState;
    unsigned int nextdir;
    unsigned int maxstep, nextstep;
    disassemblerNode_c * state99node;
    disassemblerNode_c * searchnode;
    const std::vector<unsigned int> * pieces;

    void prepare(void);
    bool checkmovement(unsigned int maxPieces, unsigned int nextstep);
    disassemblerNode_c * newNode(unsigned int amount);
    disassemblerNode_c * newNodeMerge(const disassemblerNode_c *n0, const disassemblerNode_c *n1);

  public:

    /* construct the disassembler for this concrete problem, is can not be
     * changed, once you done that but you can analyse many assemblies for
     * disassembability
     */
    movementAnalysator_c(const puzzle_c *puz, unsigned int problem);
    ~movementAnalysator_c(void);

    /* when you start with a new assembly you first have to call this function before performing any
     * find operations but it is only necessary once per assembly
     * and then you can do as many find operations as you want
     */
    void prepareAssembly(const assembly_c * a);

    /* you use either the 2 functions below, or complete Find
     * the below functions return one possible movement after another and you can stop as soon
     * as you want, while complete Find will always find all possible movements
     *
     * to use the functions below first call init_find and then repeatedly find until
     * either you don't want no more, or find returns 0
     */
    void init_find(disassemblerNode_c * nd, const std::vector<unsigned int> & pieces);
    disassemblerNode_c * find(void);

    void completeFind(disassemblerNode_c * searchnode, const std::vector<unsigned int> & pieces, std::vector<disassemblerNode_c*> * result);

};

#endif