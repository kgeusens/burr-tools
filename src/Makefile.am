bin_PROGRAMS=burrTxt burrTxt2 
noinst_LIBRARIES=lib/libburr.a tools/libtools.a halfedge/libhalfedge.a

XML_CPP_ADD=@XML_CPP_ADD@
XML_LD_ADD=@XML_LD_ADD@
GUI_CPP_ADD=@GUI_CPP_ADD@
GUI_LD_ADD=@GUI_LD_ADD@
PTHREAD_CPP_ADD=@PTHREAD_CPP_ADD@
PTHREAD_LD_ADD=@PTHREAD_LD_ADD@

AM_CPPFLAGS=-Wall -W -Wextra $(XML_CPP_ADD) $(GUI_CPP_ADD) $(PTHREAD_CPP_ADD) @LARGEFILE@
LDADD= $(GUI_LD_ADD) $(XML_LD_ADD) $(PTHREAD_LD_ADD)

EXTRA_DIST =

tools_libtools_a_CPPFLAGS = $(AM_CPPFLAGS) -Werror
tools_libtools_a_SOURCES =
tools_libtools_a_SOURCES += tools/fileexists.cpp tools/fileexists.h
tools_libtools_a_SOURCES += tools/gzstream.cpp tools/gzstream.h
tools_libtools_a_SOURCES += tools/homedir.cpp tools/homedir.h
tools_libtools_a_SOURCES += tools/intdiv.h
tools_libtools_a_SOURCES += tools/xml.cpp tools/xml.h

halfedge_libhalfedge_a_CPPFLAGS = $(AM_CPPFLAGS) -Werror
halfedge_libhalfedge_a_SOURCES =
halfedge_libhalfedge_a_SOURCES += halfedge/face.cpp halfedge/face.h
halfedge_libhalfedge_a_SOURCES += halfedge/halfedge.cpp halfedge/halfedge.h
halfedge_libhalfedge_a_SOURCES += halfedge/modifiers.cpp halfedge/modifiers.h
halfedge_libhalfedge_a_SOURCES += halfedge/polyhedron.cpp halfedge/polyhedron.h 
halfedge_libhalfedge_a_SOURCES += halfedge/polyhedron_iterators.h 
halfedge_libhalfedge_a_SOURCES += halfedge/vector3.cpp halfedge/vector3.h
halfedge_libhalfedge_a_SOURCES += halfedge/vertex.cpp halfedge/vertex.h
halfedge_libhalfedge_a_SOURCES += halfedge/volume.cpp halfedge/volume.h
EXTRA_DIST += halfedge/README

burrTxt_LDADD = lib/libburr.a tools/libtools.a halfedge/libhalfedge.a $(LDADD)
burrTxt_SOURCES=burrTxt.cpp

burrTxt2_LDADD = lib/libburr.a tools/libtools.a halfedge/libhalfedge.a $(LDADD)
burrTxt2_SOURCES=burrTxt2.cpp

lib_libburr_a_CPPFLAGS = $(AM_CPPFLAGS) -Werror
lib_libburr_a_SOURCES =
lib_libburr_a_SOURCES += lib/assembler.cpp lib/assembler.h
lib_libburr_a_SOURCES += lib/assembler_0.cpp lib/assembler_0.h
lib_libburr_a_SOURCES += lib/assembler_1.cpp lib/assembler_1.h
lib_libburr_a_SOURCES += lib/assembly.cpp lib/assembly.h
lib_libburr_a_SOURCES += lib/bitfield.h
lib_libburr_a_SOURCES += lib/bt_assert.cpp lib/bt_assert.h
lib_libburr_a_SOURCES += lib/burrgrower.cpp lib/burrgrower.h
lib_libburr_a_SOURCES += lib/converter.cpp lib/converter.h
lib_libburr_a_SOURCES += lib/disassembler.h
lib_libburr_a_SOURCES += lib/disassembler_0.cpp lib/disassembler_0.h
lib_libburr_a_SOURCES += lib/disassembler_a.cpp lib/disassembler_a.h
lib_libburr_a_SOURCES += lib/disassemblerhashes.cpp lib/disassemblerhashes.h
lib_libburr_a_SOURCES += lib/disassemblernode.cpp lib/disassemblernode.h
lib_libburr_a_SOURCES += lib/disassembly.cpp lib/disassembly.h
lib_libburr_a_SOURCES += lib/disasmtomoves.cpp lib/disasmtomoves.h
lib_libburr_a_SOURCES += lib/gridtype.cpp lib/gridtype.h
lib_libburr_a_SOURCES += lib/grouping.cpp lib/grouping.h
lib_libburr_a_SOURCES += lib/millable.cpp lib/millable.h
lib_libburr_a_SOURCES += lib/movementanalysator.cpp lib/movementanalysator.h
lib_libburr_a_SOURCES += lib/movementcache.cpp lib/movementcache.h
lib_libburr_a_SOURCES += lib/movementcache_0.cpp lib/movementcache_0.h
lib_libburr_a_SOURCES += lib/movementcache_1.cpp lib/movementcache_1.h
lib_libburr_a_SOURCES += lib/print.cpp lib/print.h
lib_libburr_a_SOURCES += lib/ps3dloader.cpp lib/ps3dloader.h
lib_libburr_a_SOURCES += lib/problem.cpp lib/problem.h
lib_libburr_a_SOURCES += lib/puzzle.cpp lib/puzzle.h
lib_libburr_a_SOURCES += lib/solution.cpp lib/solution.h
lib_libburr_a_SOURCES += lib/solvethread.cpp lib/solvethread.h
lib_libburr_a_SOURCES += lib/stl.cpp lib/stl.h
lib_libburr_a_SOURCES += lib/stl_0.cpp lib/stl_0.h
lib_libburr_a_SOURCES += lib/stl_2.cpp lib/stl_2.h
lib_libburr_a_SOURCES += lib/symmetries.h
lib_libburr_a_SOURCES += lib/symmetries_0.cpp lib/symmetries_0.h
lib_libburr_a_SOURCES += lib/symmetries_1.cpp lib/symmetries_1.h
lib_libburr_a_SOURCES += lib/symmetries_2.cpp lib/symmetries_2.h
lib_libburr_a_SOURCES += lib/thread.cpp lib/thread.h
lib_libburr_a_SOURCES += lib/types.h
lib_libburr_a_SOURCES += lib/voxel.cpp lib/voxel.h
lib_libburr_a_SOURCES += lib/voxel_0.cpp lib/voxel_0.h
lib_libburr_a_SOURCES += lib/voxel_1.cpp lib/voxel_1.h
lib_libburr_a_SOURCES += lib/voxel_2.cpp lib/voxel_2_mesh.cpp lib/voxel_2.h
lib_libburr_a_SOURCES += lib/voxel_3.cpp lib/voxel_3.h
lib_libburr_a_SOURCES += lib/voxel_4.cpp lib/voxel_4.h
lib_libburr_a_SOURCES += lib/voxeltable.cpp lib/voxeltable.h

lib_libburr_a_SOURCES += lib/tabs_0/meshverts.inc
lib_libburr_a_SOURCES += lib/tabs_0/rotmatrix.inc
lib_libburr_a_SOURCES += lib/tabs_0/symcalc.inc
lib_libburr_a_SOURCES += lib/tabs_0/symmetries.inc
lib_libburr_a_SOURCES += lib/tabs_0/tablesizes.inc
lib_libburr_a_SOURCES += lib/tabs_0/transformmini.inc
lib_libburr_a_SOURCES += lib/tabs_0/transmult.inc
lib_libburr_a_SOURCES += lib/tabs_0/unifiedsym.inc
lib_libburr_a_SOURCES += lib/tabs_0/uniquesym.inc
lib_libburr_a_SOURCES += lib/tabs_1/meshverts.inc
lib_libburr_a_SOURCES += lib/tabs_1/rotmatrix.inc
lib_libburr_a_SOURCES += lib/tabs_1/symcalc.inc
lib_libburr_a_SOURCES += lib/tabs_1/symmetries.inc
lib_libburr_a_SOURCES += lib/tabs_1/tablesizes.inc
lib_libburr_a_SOURCES += lib/tabs_1/transformmini.inc
lib_libburr_a_SOURCES += lib/tabs_1/transmult.inc
lib_libburr_a_SOURCES += lib/tabs_1/unifiedsym.inc
lib_libburr_a_SOURCES += lib/tabs_1/uniquesym.inc
lib_libburr_a_SOURCES += lib/tabs_2/rotmatrix.inc
lib_libburr_a_SOURCES += lib/tabs_2/symcalc.inc
lib_libburr_a_SOURCES += lib/tabs_2/symmetries.inc
lib_libburr_a_SOURCES += lib/tabs_2/tablesizes.inc
lib_libburr_a_SOURCES += lib/tabs_2/transformmini.inc
lib_libburr_a_SOURCES += lib/tabs_2/transmult.inc
lib_libburr_a_SOURCES += lib/tabs_2/unifiedsym.inc
lib_libburr_a_SOURCES += lib/tabs_2/uniquesym.inc
lib_libburr_a_SOURCES += lib/tabs_3/meshverts.inc
lib_libburr_a_SOURCES += lib/tabs_4/meshverts.inc
