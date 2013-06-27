JOBNAME="double_cts200_8_ntimer"
CXX=g++
BIN_DIR = bin
BIN_NAME = rgf
TARGET = $(BIN_DIR)/$(BIN_NAME)
CXXFLAGS = -Isrc/com -Isrc/tet -Isrc/allreduce -O2 -fopenmp
SPANNINGTREE = $(BIN_DIR)/spanning_tree
SPEEDTEST = $(BIN_DIR)/speedTest
ME=$(shell whoami)
SPAN_F = $(words $(shell ps aux | grep '[s]panning_tree' | grep $(ME) ))
RGF_F = $(words $(shell ps aux | grep '[r]gf' ))
SPEEDTEST_F = $(words $(shell ps aux | grep '[s]peedTest' ))
#DATA1 = ctslices_01
#DATA2 = ctslices_02
DATA1 = ctslices_01
DATA2 = ctslices_02
DATA3 = ctslices_03
CLUSTER_DATA = /user/hl1283/RGF_Hadoop/test/sample/cts200.train.dat
all:  $(TARGET) $(SPANNINGTREE) $(SPEEDTEST)
MAP_NUM=8

OBJ=obj
OBJDIR:=$(OBJ)/tet $(OBJ)/com $(OBJ)/allreduce

# Add .d to Make's recognized suffixes.
SUFFIXES += .d

#We don't need to clean up when we're making these targets
NODEPS:=clean

MAINS:=src/spanning_tree.cpp src/allreduce_test.cpp src/speedTest.cpp
#Find all the C++ files in the src/ directory
SOURCES:=$(filter-out $(MAINS),$(shell find src -name "*.cpp"))
#Objects we'd like to build
OBJECTS:=$(patsubst src/%.cpp,$(OBJ)/%.o,$(SOURCES))
#These are the dependency files, which make will clean up after it creates them
DEPFILES:=$(patsubst src/%.cpp,$(OBJ)/%.d,$(SOURCES))

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
#Chances are, these files don't exist.  GMake will create them and
#clean up automatically afterwards
  -include $(DEPFILES)
endif

#This is the rule for creating the dependency files
$(OBJ)/%.d: src/%.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst src/%.cpp,$(OBJ)/%.o,$<)' $< -MF $@

#This rule does the compilation
$(OBJ)/%.o: src/%.cpp $(OBJ)/%.d
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJECTS): | $(OBJDIR)
$(DEPFILES): | $(OBJDIR)

# directory
$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $+

$(SPANNINGTREE): src/spanning_tree.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ $+

$(SPEEDTEST): src/speedTest.cpp  $(SPANNINGTREE) $(OBJECTS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ src/speedTest.cpp $(OBJ)/allreduce/*.o

.PHONY: clean kill all

clean:
	rm -rf $(OBJ)/*
	rm -rf $(BIN_DIR)/*

one: clean
	g++ $(SOURCES) $(CXXFLAGS) -o $(TARGET)

artest: src/allreduce_test.cpp $(SPANNINGTREE) $(OBJECTS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$@ src/allreduce_test.cpp $(OBJ)/allreduce/*.o

run: kill all
	mkdir -p test/output
	$(BIN_DIR)/spanning_tree > /dev/null 2>&1 < /dev/null
	perl test/call_exe.pl ./bin/rgf train_test test/sample/$(DATA1) localhost 1233 2 0 >log1.log  2> test/output/log1_vis.log &  
	perl test/call_exe.pl ./bin/rgf train_test test/sample/$(DATA2) localhost 1233 2 1 >log2.log 2> test/output/log2_vis.log
	@killall spanning_tree

run1: kill all
	mkdir -p test/output
	$(BIN_DIR)/spanning_tree > /dev/null 2>&1 < /dev/null
	perl test/call_exe.pl ./bin/rgf train_test test/sample/$(DATA1) localhost 1233 1 0 >log1_1.log   
	@killall spanning_tree

run3: kill all
	mkdir -p test/output
	$(BIN_DIR)/spanning_tree > /dev/null 2>&1 < /dev/null
	perl test/call_exe.pl ./bin/rgf train_test test/sample/$(DATA1) localhost 1233 3 0 >log1.log  2> test/output/log1_vis.log &
	perl test/call_exe.pl ./bin/rgf train_test test/sample/$(DATA2) localhost 1233 3 1 >log2.log  2> test/output/log2_vis.log &
	perl test/call_exe.pl ./bin/rgf train_test test/sample/$(DATA3) localhost 1233 3 2 >log3.log  2> test/output/log3_vis.log
	@killall spanning_tree

SPEEDTEST_NTIMES=100000
SPEEDTEST_LEN=200
speedtest: all kill
	$(BIN_DIR)/spanning_tree > /dev/null 2>&1 < /dev/null
	$(BIN_DIR)/speedTest --length $(SPEEDTEST_LEN) --times $(SPEEDTEST_NTIMES) --master localhost --unique_id 1233 --total 2 --node_id 0 &
	$(BIN_DIR)/speedTest --length $(SPEEDTEST_LEN) --times $(SPEEDTEST_NTIMES) --master localhost --unique_id 1233 --total 2 --node_id 1 2>/dev/null
	killall spanning_tree


kill:
ifneq (0, $(SPAN_F))
	@killall spanning_tree
endif
ifneq (0, $(RGF_F))
	@killall rgf
endif
ifneq (0, $(SPEEDTEST_F))
	@killall speedTest
endif

# Marco for hadoop cluster
hfs=hadoop fs
hjs=hadoop jar /usr/lib/hadoop/contrib/streaming/hadoop-streaming-1.0.3.16.jar

speedtestHD: all kill
	$(BIN_DIR)/spanning_tree
ifneq (0, $(words $(shell $(hfs) -ls | grep rgfout_$(JOBNAME) )))
	$(hfs) -rmr rgfout_$(JOBNAME)
endif
	$(hjs) -D mapred.job.name=$(JOBNAME) -D mapred.job.map.memory.mb=6000 -D mapred.map.tasks=$(MAP_NUM) -D mapred.reduce.tasks=0 \
		-input $(CLUSTER_DATA) -output rgfout_$(JOBNAME) -mapper runspeedTest.sh -reducer cat \
		-file cluster/runspeedTest.sh bin/speedTest test/call_exe.pl cluster/long.inp 
	killall spanning_tree
	mkdir ~/$(JOBNAME)

cluster: kill $(TARGET) $(SPANNINGTREE)
	$(BIN_DIR)/spanning_tree
ifneq (0, $(words $(shell $(hfs) -ls | grep rgfout )))
	$(hfs) -rmr rgfout
endif
	$(hjs) -D mapred.job.map.memory.mb=6000  -Dmapred.map.tasks=4 -D mapred.reduce.tasks=0 \
		-input /user/hl1283/RGF_Hadoop/test/sample/ctslices.test.dat -output rgfout -mapper runrgf.sh \
		-file cluster/runrgf.sh bin/rgf test/call_exe.pl cluster/long.inp
	killall spanning_tree

clusterCT: kill $(TARGET) $(SPANNINGTREE)
	$(BIN_DIR)/spanning_tree
ifneq (0, $(words $(shell $(hfs) -ls | grep rgfout_$(JOBNAME))))
	$(hfs) -rmr rgfout_$(JOBNAME)
endif
	$(hjs) -D mapred.job.name=$(JOBNAME) -D mapred.job.map.memory.mb=6000 -D mapred.map.tasks=$(MAP_NUM) -D mapred.reduce.tasks=0 \
		-input $(CLUSTER_DATA) -output rgfout_$(JOBNAME) -mapper runrgf.sh -reducer cat \
		-file cluster/runrgf.sh bin/rgf test/call_exe.pl cluster/long.inp
	killall spanning_tree
	mkdir ~/$(JOBNAME)
	$(hjs) -copyToLocal rgfout_$(JOBNAME)/* ~/$(JOBNAME)/ 

