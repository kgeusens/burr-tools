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
#include "groupseditor.h"
#include "piececolor.h"
#include "WindowWidgets.h"

#include "../lib/voxel.h"
#include "../lib/puzzle.h"

#include "Fl_Table.h"

#include <FL/fl_draw.h>
#include <FL/Fl_Int_Input.H>

/* this is the groups editor table
 */
class groupsEditorTab_c : public Fl_Table {

  /* the puzzle and the problem to edit */
  puzzle_c * puzzle;
  unsigned int prob;

  /* the input line where the user inputs the value
   * it is places over the cell the user clicks onto
   */
  Fl_Int_Input* input;

  /* the position inside the table where the imput line is
   */
  int editShape, editGroup;

  /* the current maximum group number
   */
  unsigned int maxGroup;

  /* was something changed? */
  bool changed;

  /* this is the virtual function called by the table widget to
   * draw a single cell
   */
  void draw_cell(TableContext context, int r = 0, int c = 0,
                 int x = 0, int y = 0, int w = 0, int h = 0);

public:

  groupsEditorTab_c(int x, int y, int w, int h, puzzle_c * puzzle, unsigned int problem);

  /* add a group to the puzzle and a column to the table
   */
  void addGroup(void);

  void cb_input(void);
  void cb_tab(void);

  bool getChanged(void) { return changed; }

  /* take over the currently edited value (if there is one) and
   * close the edit line
   */
  void finishEdit(void);

};

/* draw a cell of the table */
void groupsEditorTab_c::draw_cell(TableContext context, int r, int c, int x, int y, int w, int h) {

  switch(context) {

  case CONTEXT_STARTPAGE:
    return;

  case CONTEXT_ROW_HEADER:
  case CONTEXT_COL_HEADER:
    fl_push_clip(x, y, w, h);
    {
      /* draw the header */

      /* clear the background and paint a raised box */
      fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, color());

      /* create the text of the cell */
      static char s[40];

      /* first column is the shape columns
       * second contains the number of that shape that is in the puzzle
       * the other columns are group golumns
       */
      if (c == 0)
        snprintf(s, 40, "Shape");
      else if (c == 1)
        snprintf(s, 40, "n");
      else
        snprintf(s, 40, "Gr %i", c-1);

      /* output the text */
      fl_color(FL_BLACK);
      fl_draw(s, x, y, w, h, FL_ALIGN_CENTER);

    }
    fl_pop_clip();
    return;

  case CONTEXT_CELL:

    // don't redraw the cell where the input line is activated
    if ((c == editGroup+1) && (r == editShape) && (input->visible())) return;

    fl_push_clip(x, y, w, h);
    {
      /* this will contain the text to display */
      static char s[40];

      /* this contains what to draw, 0: empty cell
       * 1: the string s inside a colored cell
       */
      int type = 0;

      if (c == 0) {

        /* the shape column, find out the shape number and display that one */

        type = 1;

        int num = puzzle->probGetShape(prob, r);
        if (puzzle->getShape(num)->getName().length())
          snprintf(s, 40, " S%i - %s", num+1, puzzle->getShape(num)->getName().c_str());
        else
          snprintf(s, 40, " S%i", num+1);

      } else if (c == 1) {

        /* the 2nd column, display the count for this shape */

        type = 1;

        snprintf(s, 40, "%i", puzzle->probGetShapeCount(prob, r));

      } else {

        /* the group columns, only display, when an entry for this
         * group exists
         */

        for (unsigned int j = 0; j < puzzle->probGetShapeGroupNumber(prob, r); j++)
          if (puzzle->probGetShapeGroup(prob, r, j) == (c - 1)) {
            type = 1;
            snprintf(s, 40, "%i", puzzle->probGetShapeGroupCount(prob, r, j));
            break;
          }

      }

      /* draw the cell */
      switch (type) {
        case 0:

          /* an empty cell */
          fl_color(color());
          fl_rectf(x, y, w, h);

          break;

        case 1:

          /* a cell with a background corresponding with the shape color */
          {
            /* get the color for the cell */
            int p = puzzle->probGetShape(prob, r);
            unsigned char r, g, b;
            r = pieceColorRi(p);
            g = pieceColorGi(p);
            b = pieceColorBi(p);

            /* draw background */
            fl_color(r, g, b);
            fl_rectf(x, y, w, h);
            if ((int)3*r + 6*g + 1*b > 1275)
              fl_color(0, 0, 0);
            else
              fl_color(255, 255, 255);

            /* draw the text */
            if (c == 0)
              fl_draw(s, x, y, w, h, FL_ALIGN_LEFT);
            else
              fl_draw(s, x, y, w, h, FL_ALIGN_CENTER);
          }

          break;
      }

      /* draw a black frame around the cell to make it visible */
      fl_color(FL_BLACK);
      fl_rect(x, y, w, h);
    }

    fl_pop_clip();
    return;

  default:

    return;

  }
}

/* called when the input line has been completed */
static void cb_input_stub(Fl_Widget*, void* v) { ((groupsEditorTab_c*)v)->cb_input(); }
void groupsEditorTab_c::cb_input(void) {

  /* we have either changed the count for the shape, or the group count */
  if (editGroup == 0)
    puzzle->probSetShapeCount(prob, editShape, atoi(input->value()));
  else
    puzzle->probSetShapeGroup(prob, editShape, editGroup, atoi(input->value()));

  /* hide the edit line */
  input->hide();
  changed = true;
}

/* handle mouse events for the tables */
static void cb_tab_stub(Fl_Widget*, void *v) { ((groupsEditorTab_c*)v)->cb_tab(); }
void groupsEditorTab_c::cb_tab(void)
{
  int row = callback_row();
  int col = callback_col();
  TableContext context = callback_context();

  switch ( context ) {
  case CONTEXT_CELL:
    {
      /* no editing in column 0 */
      if (col == 0) return;

      /* if there is an active edit line, finish it */
      if (input->visible()) input->do_callback();

      editShape = row;
      editGroup = col-1;

      /* calculate the cell the user has clicked into */
      int x, y, w, h;

      find_cell(CONTEXT_CELL, row, col, x, y, w, h);

      /* place the input line on that cell */
      input->resize(x, y, w, h);

      /* the text that should be initially in the input line */
      char s[30];
      int count = 0;

      if (editGroup == 0)
        count = puzzle->probGetShapeCount(prob, row);
      else
        for (unsigned int j = 0; j < puzzle->probGetShapeGroupNumber(prob, row); j++)
          if (puzzle->probGetShapeGroup(prob, row, j) == (col - 1))
            count = puzzle->probGetShapeGroupCount(prob, row, j);

      snprintf(s, 30, "%d", count);

      input->value(s);
      input->show();
      input->take_focus();

      return;
    }
  default:
    return;
  }
}

groupsEditorTab_c::groupsEditorTab_c(int x, int y, int w, int h, puzzle_c * puzzle, unsigned int problem) : Fl_Table(x, y, w, h), changed(false) {

  this->puzzle = puzzle;
  this->prob = problem;

  rows(puzzle->probShapeNumber(problem));

  /* find out the number of groups */
  maxGroup = 0;

  for (unsigned int i = 0; i < puzzle->probShapeNumber(problem); i++)
    for (unsigned int j = 0; j < puzzle->probGetShapeGroupNumber(prob, i); j++)
      if (puzzle->probGetShapeGroup(prob, i, j) > maxGroup)
        maxGroup = puzzle->probGetShapeGroup(prob, i, j);

  cols(maxGroup + 2);
  col_header(1);
  row_height_all(20);

  /* set the widths of the columns */
  col_width(0, 150);
  col_width(1, 35);
  for (unsigned int i = 0; i < maxGroup; i++)
    col_width(i+2, 35);

  when(FL_WHEN_RELEASE);
  callback(cb_tab_stub, this);

  /* initialize the input line that is going to be used when asking for values */
  input = new Fl_Int_Input(0, 0, 0, 0);
  input->hide();
  input->when(FL_WHEN_ENTER_KEY_ALWAYS);
  input->callback(cb_input_stub, this);

  end();
}

void groupsEditorTab_c::addGroup(void) {

  maxGroup++;

  cols(maxGroup + 2);
  row_height_all(20);

  col_width(maxGroup+1, 35);
}

void groupsEditorTab_c::finishEdit(void) {
  if (input->visible()) {
    cb_input();
  }
}

static void cb_AddGroup_stub(Fl_Widget* o, void* v) { ((groupsEditor_c*)v)->cb_AddGroup(); }
void groupsEditor_c::cb_AddGroup(void) {
  tab->addGroup();
}

static void cb_CloseWindow_stub(Fl_Widget* o, void* v) { ((groupsEditor_c*)v)->cb_CloseWindow(); }
void groupsEditor_c::cb_CloseWindow(void) {
  hide();
}

/* when hiding the window, first close active editing tasks */
void groupsEditor_c::hide(void) {
  tab->finishEdit();
  Fl_Double_Window::hide();
}

#define SZ_WINDOW_X 300                        // initial size of the window
#define SZ_WINDOW_Y 200
#define SZ_GAP 5                               // gap between elements
#define SZ_BUTTON_Y 20

groupsEditor_c::groupsEditor_c(puzzle_c * p, unsigned int pr) : Fl_Double_Window(SZ_WINDOW_X, SZ_WINDOW_Y) {

  tab = new groupsEditorTab_c(SZ_GAP, SZ_GAP, SZ_WINDOW_X-2*SZ_GAP, SZ_WINDOW_Y-3*SZ_GAP-SZ_BUTTON_Y, p, pr);

  new FlatButton(SZ_GAP, SZ_WINDOW_Y-SZ_GAP-SZ_BUTTON_Y, (SZ_WINDOW_X-3*SZ_GAP)/2, SZ_BUTTON_Y, "Add Group", "Add another group", cb_AddGroup_stub, this);
  new FlatButton((SZ_GAP+SZ_WINDOW_X)/2, SZ_WINDOW_Y-SZ_GAP-SZ_BUTTON_Y, (SZ_WINDOW_X-3*SZ_GAP)/2, SZ_BUTTON_Y, "Close", "Close Window", cb_CloseWindow_stub, this);

  label("Group Editor");

  set_modal();
  size_range(SZ_WINDOW_X, SZ_WINDOW_Y, 0, 0);
  resizable(tab);
}


bool groupsEditor_c::changed(void) {
  return tab->getChanged();
}
