########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++ 
# CXXFLAGS = -std=c++17 -g -Wall -Werror -I./include
CXXFLAGS = -std=c++17 -m64 -g -I./include 
LDFLAGS = -lSDL2_image -L./external/lib -lSDL2_mixer -lSDL2 -ldl -L./external/box2d/build/bin -lbox2d
LUA_LIB_DIR = ./external/lua_linux/lib
LDFLAGS += -L$(LUA_LIB_DIR) -llua54

# SDL2 settings
# SDL2_INCLUDE_DIR = ./external/SDL
SDL2_INCLUDE_DIR = /usr/include/SDL2

# Makefile settings - Can be customized.
APPNAME = finalProject
EXT = .cpp
SRCDIR = src
EXTERNALDIR = external
OBJDIR = obj
DEPDIR = dep

# Include directories
INCLUDES = -I$(SDL2_INCLUDE_DIR) -I./external/json/single_include/nlohmann -I./external/box2d/include -I./core/ -I./ecs -I./ecs/subsystem -I./ecs/component -I./ecs/module -I./io -I./external/imgui -I./external/lua_linux/include -I./external/imgui -I./external/SDL

# Compiler flags
CXXFLAGS += $(INCLUDES)

############## Do not change anything from here downwards! #############
# SRC = $(wildcard $(SRCDIR)/*$(EXT) $(EXTERNALDIR)/box2d/src/collision/*$(EXT) $(EXTERNALDIR)/box2d/src/common/*$(EXT) $(EXTERNALDIR)/box2d/src/dynamics/*$(EXT) $(EXTERNALDIR)/box2d/src/rope/*$(EXT))
SRCS_MAIN = $(wildcard $(SRCDIR)/*$(EXT))
SRCS_BOX2D = $(wildcard $(EXTERNALDIR)/box2d/src/collision/*$(EXT) $(EXTERNALDIR)/box2d/src/common/*$(EXT) $(EXTERNALDIR)/box2d/src/dynamics/*$(EXT) $(EXTERNALDIR)/box2d/src/dynamics/contacts/*$(EXT) $(EXTERNALDIR)/box2d/src/dynamics/joints/*$(EXT) $(EXTERNALDIR)/box2d/src/rope/*$(EXT))
SRCS_IMGUI = $(wildcard $(EXTERNALDIR)/imgui/*$(EXT))

SRC = $(SRCS_MAIN) $(SRCS_BOX2D) $(SRCS_IMGUI)

OBJS_MAIN = $(patsubst $(SRCDIR)/%$(EXT), $(OBJDIR)/%.o, $(SRCS_MAIN))
OBJS_BOX2D = $(patsubst $(EXTERNALDIR)/box2d/%$(EXT), $(OBJDIR)/box2d/%.o, $(SRCS_BOX2D))
OBJS_IMGUI = $(patsubst $(EXTERNALDIR)/imgui/%$(EXT), $(OBJDIR)/imgui/%.o, $(SRCS_IMGUI))

OBJ = $(OBJS_MAIN) $(OBJS_BOX2D) $(OBJS_IMGUI)
DEPS_MAIN = $(patsubst $(SRCDIR)/%$(EXT),$(DEPDIR)/%.d,$(SRCS_MAIN))
DEPS_BOX2D = $(patsubst $(EXTERNALDIR)/box2d/%$(EXT),$(DEPDIR)/box2d/%.d,$(SRCS_BOX2D))
DEPS_IMGUI = $(patsubst $(EXTERNALDIR)/imgui/%$(EXT),$(DEPDIR)/imgui/%.d,$(SRCS_IMGUI))

DEP = $(DEPS_MAIN) $(DEPS_BOX2D) $(DEPS_IMGUI)

# DEP = $(patsubst $(SRCDIR)/%$(EXT),$(DEPDIR)/%.d,$(SRC))
# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependecy rules
# %.d: $(SRCDIR)/%$(EXT)
# 	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

$(DEPDIR)/%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:$(DEPDIR)/%.d=$(OBJDIR)/%.o) >$@
	
$(DEPDIR)/box2d/%.d: $(EXTERNALDIR)/box2d/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:$(DEPDIR)/box2d/%.d=$(OBJDIR)/box2d/%.o) >$@

$(DEPDIR)/imgui/%.d: $(EXTERNALDIR)/imgui/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:$(DEPDIR)/imgui/%.d=$(OBJDIR)/imgui/%.o) >$@

$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)/box2d/%.o: $(EXTERNALDIR)/box2d/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)/imgui/%.o: $(EXTERNALDIR)/imgui/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<
# Includes all .h files
-include $(DEP)
# Includes all .d files
# -include $(wildcard $(DEPDIR)/*.d)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)/box2d/%.o:
	mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(DEPDIR)/box2d/%.d:
	mkdir -p $(@D)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:$(DEPDIR)/box2d/%.d=$(OBJDIR)/box2d/%.o) >$@

$(OBJDIR)/imgui/%.o: $(EXTERNALDIR)/imgui/%$(EXT)
	mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -o $@ -c $<

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(DEP)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)
