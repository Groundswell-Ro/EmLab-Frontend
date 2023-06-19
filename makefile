# Compiler settings
CC = g++
CXXFLAGS = -std=c++14 -I. -I../comunication -I../comunication/comm -DICE_CPP11_MAPPING

# Makefile settings
APPNAME = frontend
EXT = .cpp
SRCDIR = ./src
CMMDIR = ../comunication/obj
OBJDIR = ./src/obj

# Linking lib
LDFLAGS =  -lwthttp -lwt -lwtdbo -lIce++11 -lpthread -lssl -lcrypto

# Runtime lib
RLIB = --docroot . --http-address 0.0.0.0 --http-port 9090


############## Creating variables #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJCOMM = $(wildcard $(CMMDIR)/*.o)
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ) $(OBJCOMM)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT) | gen_obj_dir
	$(CC) $(CXXFLAGS) -o $@ -c $<

################## Run #################
run:
	./$(APPNAME) $(RLIB)

runTrace:
	./$(APPNAME) --Ice.Trace.Network=2 $(RCMD) 

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
	@echo $(SRC)
	@echo $(COMM)
	@echo $(OBJ)
	@echo $(DEP)
