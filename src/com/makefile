
BIN_NAME = rgf
BIN_DIR = bin
TARGET = $(BIN_DIR)/$(BIN_NAME)
CFLAGS = -Isrc/com -Isrc/tet_tools -O2

CFLAGS = -I../com -I../tet -O2
           
CPP_FILES= 	\
		../tet/driv_rgf.cpp	\
	../tet/AzFindSplit.cpp	\
	../tet/AzOptOnTree_TreeReg.cpp	\
	../tet/AzOptOnTree.cpp	\
	../tet/AzReg_Tsrbase.cpp	\
	../tet/AzReg_TsrOpt.cpp	\
	../tet/AzReg_TsrSib.cpp	\
	../tet/AzRgf_FindSplit_Dflt.cpp	\
	../tet/AzRgf_FindSplit_TreeReg.cpp	\
	../tet/AzRgf_Optimizer_Dflt.cpp	\
	../tet/AzRgforest.cpp	\
	../tet/AzRgfTree.cpp	\
	../tet/AzSortedFeat.cpp	\
	../tet/AzTETmain.cpp	\
	../tet/AzTETproc.cpp	\
	../tet/AzTree.cpp	\
	../tet/AzTreeEnsemble.cpp	\
	../tet/AzTrTree.cpp	\
	../tet/AzTrTreeFeat.cpp	\


SRCFILES := $(wildcard *.cpp)

OBJFILES := $(patsubst %.cpp, %.o, ${CPP_FILES})



#$(TARGET): $(CPP_FILES)
all: ${OBJFILES}
	/bin/rm -f $(TARGET)
	#g++ -c $(CPP_FILES) $(CFLAGS)

%.o: %.cpp
	g++ ${CFLAGS} $^ -o $@

clean: 
	/bin/rm -f $(TARGET)

run:
	/usr/bin/perl test/call_exe.pl ./bin/rgf train test/sample/train