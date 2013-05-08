CXX=g++
BIN_DIR = bin
BIN_NAME = rgf
TARGET = $(BIN_DIR)/$(BIN_NAME)
CXXFLAGS = -Isrc/com -Isrc/tet -Isrc/allreduce -O2
SPANNINGTREE = $(BIN_DIR)/spanning_tree

all:  $(TARGET) $(SPANNINGTREE)

OBJ=obj
OBJDIR:=$(OBJ)/tet $(OBJ)/com $(OBJ)/allreduce

# Add .d to Make's recognized suffixes.
SUFFIXES += .d

#We don't need to clean up when we're making these targets
NODEPS:=clean

MAINS:=src/spanning_tree.cpp src/allreduce_test.cpp
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

clean:
	rm -rf $(OBJ)/*
	rm -rf $(BIN_DIR)/*

one: clean
	g++ $(SOURCES) $(CXXFLAGS) -o $(TARGET)

artest: src/allreduce_test.cpp $(SPANNINGTREE) $(OBJECTS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$@ src/allreduce_test.cpp $(OBJ)/allreduce/*.o

run:
	mkdir -p test/output
	perl test/call_exe.pl ./bin/rgf train test/sample/train

predict:
	perl test/call_exe.pl ./bin/rgf predict test/sample/predict

train_test:
	perl test/call_exe.pl ./bin/rgf train_test test/sample/regress_train_test
