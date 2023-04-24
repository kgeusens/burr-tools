libSources = ../src/lib/assembler.cpp   \
        ../src/lib/assembler_0.cpp \
        ../src/lib/assembler_1.cpp \
        ../src/lib/assembly.cpp    \
        ../src/lib/bt_assert.cpp   \
        ../src/lib/disasmtomoves.cpp   \
        ../src/lib/disassembler_0.cpp  \
        ../src/lib/disassembler_a.cpp  \
        ../src/lib/disassemblerhashes.cpp  \
        ../src/lib/disassemblernode.cpp    \
        ../src/lib/disassembly.cpp \
        ../src/lib/gridtype.cpp    \
        ../src/lib/grouping.cpp    \
        ../src/lib/millable.cpp    \
        ../src/lib/movementanalysator.cpp  \
        ../src/lib/movementcache.cpp   \
        ../src/lib/movementcache_0.cpp \
        ../src/lib/movementcache_1.cpp \
        ../src/lib/problem.cpp \
        ../src/lib/puzzle.cpp  \
        ../src/lib/solution.cpp    \
        ../src/lib/solvethread.cpp \
        ../src/lib/symmetries_0.cpp    \
        ../src/lib/symmetries_1.cpp    \
        ../src/lib/symmetries_2.cpp    \
        ../src/lib/thread.cpp \
        ../src/lib/voxel.cpp   \
        ../src/lib/voxel_0.cpp \
        ../src/lib/voxel_1.cpp \
        ../src/lib/voxel_2.cpp \
        ../src/lib/voxel_2_mesh.cpp    \
        ../src/lib/voxel_3.cpp \
        ../src/lib/voxel_4.cpp \
        ../src/lib/voxeltable.cpp  \
        ../src/lib/stl.cpp \
        ../src/lib/stl_0.cpp   \
        ../src/lib/stl_2.cpp
edgeSources = ../src/halfedge/face.cpp   \
        ../src/halfedge/halfedge.cpp   \
        ../src/halfedge/modifiers.cpp  \
        ../src/halfedge/polyhedron.cpp \
        ../src/halfedge/vector3.cpp    \
        ../src/halfedge/vertex.cpp \
        ../src/halfedge/volume.cpp
toolSources = ../src/tools/xml.cpp 
srcSources =  ../src/lib_interface.cpp

libObjects = $(patsubst ../src/lib/%.cpp, %.o, $(libSources))
edgeObjects = $(patsubst ../src/halfedge/%.cpp, %.o, $(edgeSources))
toolObjects = $(patsubst ../src/tools/%.cpp, %.o, $(toolSources))
srcObjects = $(patsubst ../src/%.cpp, %.o, $(srcSources))

TARGETS = burr-tools.js
targetSources = $(patsubst %.js, %.cpp, $(TARGETS))
targetObjects = $(patsubst %.cpp, %.o, $(targetSources))

Sources = $(libSources) $(edgeSources) $(toolSources) $(srcSources)
Objects = $(libObjects) $(edgeObjects) $(toolObjects) $(srcObjects) $(targetObjects)

burr-tools.js: $(Objects) post.js
	emcc -O2 $(Objects) -o $@ -lstdc++ -sEXPORTED_FUNCTIONS=_solvePuzzle -sEXPORTED_RUNTIME_METHODS=cwrap --post-js=post.js

$(libObjects) : %.o : ../src/lib/%.cpp
	emcc -DNO_THREADING -I. -Wall -W -Wextra -I/usr/local/include -g -O2 -c $<

$(edgeObjects) : %.o : ../src/halfedge/%.cpp
	emcc -DNO_THREADING -I. -Wall -W -Wextra -I/usr/local/include -g -O2 -c $<

$(toolObjects) : %.o : ../src/tools/%.cpp
	emcc -DNO_THREADING -I. -Wall -W -Wextra -I/usr/local/include -g -O2 -c $<

$(srcObjects) : %.o : ../src/%.cpp
	emcc -DNO_THREADING -I. -Wall -W -Wextra -I/usr/local/include -g -O2 -c $<

$(targetObjects): %.o : %.cpp
	emcc -I. -Wall -W -Wextra -I/usr/local/include -g -O2 -c $<

clean:
	rm $(Objects)
