# Compiler settings
CC = g++
CXXFLAGS = -std=c++14 -I. -I../comunication/comm -DICE_CPP11_MAPPING

# Makefile settings
APPNAME = frontend
EXT = .cpp
SRCDIR = ./src
UTILDIR = ./utils
CMMDIR = ../comunication/comm
OBJDIR = ./src/obj

# Linking lib
LDFLAGS =  -lwthttp -lwt -lwtdbo -lIce++11 -lpthread -lssl -lcrypto -lboost_filesystem

# Runtime lib
RLIB = --docroot . --http-address 0.0.0.0 --http-port 9093


############## Creating variables #############
SRC = $(wildcard $(SRCDIR)/*$(EXT)) 
COMM = $(wildcard $(CMMDIR)/*$(EXT))
UTIL = $(wildcard $(UTILDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o) $(UTIL:$(UTILDIR)/%$(EXT)=$(OBJDIR)/%.o) $(COMM:$(CMMDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=$(OBJDIR)/%.d)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ) 
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependecy rules
$(OBJDIR)/%.d: $(SRCDIR)/%$(EXT)
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

$(OBJDIR)/%.d: $(UTILDIR)/%$(EXT)
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT) 
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: $(CMMDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: $(UTILDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

################## Run #################
run:
	./$(APPNAME) $(RLIB)

runTrace:
	./$(APPNAME) --Ice.Trace.Network=2 $(RCMD) $(RLIB)

dbg:
	gdb ./$(APPNAME)

.PHONY: gen_obj_dir
gen_obj_dir:
	mkdir -p $(OBJDIR)

################### Cleaning rules ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(APPNAME) $(DEP) $(OBJ)

cleanDependencies:
	$(RM) $(DEP)

################### Tailwind commands ###################
startTailwind: buildTailwindModules
	cd resources/themes/tailwind && npx tailwindcss -i ./src/input.css -o ./dist/tailwind.css --watch


buildTailwindModules:
ifneq ("$(wildcard ./resources/tailwind/node_modules)","")
	@echo "Tailwind modules already installed"
else
	@echo "Installing Tailwind modules"
	cd resources/themes/tailwind && npm install
endif

################### Display variables ###################
echo:
	@echo $(DEP)
