#include "lib/puzzle.h"
#include "lib/problem.h"
#include "lib/assembler.h"
#include "lib/assembly.h"
#include "lib/disassembler.h"
#include "lib/disassembler_0.h"
#include "lib/disassembly.h"
#include "lib/print.h"
#include "lib/voxel.h"
#include "lib/solution.h"
#include "tools/xml.h"
#include "lib_interface.h"

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

extern "C" {
    using std::string;
    using std::istringstream;
    using std::ostringstream;

// Example C-code for creating puzzles..
    const char* solvePuzzle(char* xmlstring)
    {
// Parse the XML
        string xml(xmlstring);
        istringstream str(xml);
        xmlParser_c pars(str);
        puzzle_c *puzzle = new puzzle_c(pars);

// Solve the puzzle
        solve(puzzle);

// Create an outputstream to string and capture the result
        ostringstream ostr;
        xmlWriter_c xmlResult(ostr);
        puzzle->save(xmlResult);

// Copy the stream result from heap to stack
        char retval[ostr.str().length()];
        strcpy(retval, ostr.str().c_str());
        return retval;
    }
}