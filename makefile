# Compiler settings
CC = g++
CXXFLAGS = -std=c++14 -I. -I../comunication/generated -I../comunication  -DICE_CPP11_MAPPING

# Makefile settings
APPNAME = frontend
EXT = .cpp
SRCDIR = ./src
CMMDIR = ../comunication/generated
UTILDIR = ../comunication
OBJDIR = ./src/obj

# Linking lib
LDFLAGS =  -lwthttp -lwt -lwtdbo -lIce++11 -lpthread -lssl -lcrypto

# Runtime lib
RLIB = --docroot . --http-address 0.0.0.0 --http-port 9090


############## Creating variables #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
COMM = $(wildcard $(CMMDIR)/*$(EXT))
UTIL = $(wildcard $(UTILDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o) $(COMM:$(CMMDIR)/%$(EXT)=$(OBJDIR)/%.o) $(UTIL:$(UTILDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependecy rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

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
	./$(APPNAME) --Ice.Trace.Network=2 $(RCMD) 

dbg:
	gdb ./$(APPNAME)

################### Cleaning rules ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(APPNAME) $(DEP) $(OBJ)

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
displayVariables:
	@echo $(SRC)
	@echo $(COMM)
	@echo $(OBJ)
	@echo $(DEP)
