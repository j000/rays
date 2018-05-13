SRC := rays.cpp
SRC += $(filter-out $(SRC),$(wildcard *.cpp))

####################

# c++
# LINK.o := $(LINK.cc)

SHELL := /bin/sh
MKDIR ?= mkdir
RMDIR ?= $(RM) -d

# enable a lot of warnings and then some more
WARNINGS := -Wall -Wextra
# be more strict
ifeq ($(shell expr $(shell $(CC) -dumpversion) "<" "4.8"),1)
        # welcome to 2012
        # -Wpedantic is available since gcc 4.8
        WARNINGS += -pedantic
else
        WARNINGS += -Wpedantic
endif

# shadowing variables, are you sure?
WARNINGS += -Wshadow
# sizeof(void)
WARNINGS += -Wpointer-arith
# unsafe pointer cast qualifiers: `const char*` is cast to `char*`
WARNINGS += -Wcast-qual
# most of the time you don't want this
WARNINGS += -Werror=implicit-function-declaration
# why warning for comments inside comments?
WARNINGS += -Wno-comment

# warn when struct can be better
WARNINGS += -Wpadded

# implicit conversions that may alter a value
WARNINGS += -Wconversion
# "float" is implicitly promoted to "double"
WARNINGS += -Wdouble-promotion

# the new-style casts are less vulnerable to unintended effects
WARNINGS += -Wold-style-cast

CXXFLAGS := $(CXXFLAGS) $(WARNINGS) -std=c++17 -O2 -pipe
####################
LDFLAGS := $(LDFLAGS)
LDFLAGS += -static-libgcc -Wl,-R.
LOADLIBES := -Wl,--as-needed
LOADLIBES += -lm
LOADLIBES += -l:libstdc++.a
####################
# no runtime type information
CXXFLAGS += -fno-rtti
####################
# probably best option
CXXFLAGS += -mtune=native
# use SSE2
CXXFLAGS += -mfpmath=sse -msse2 -ffast-math
# preserve debug info
CXXFLAGS += -g -ggdb
####################
# LTO
CXXFLAGS += -flto
####################

# ncurses
# NCURSES := ncurses
# CFLAGS += -DNCURSES_WIDECHAR=1 $(shell pkg-config --cflags-only-I $(NCURSES))
# CXXFLAGS += -DNCURSES_WIDECHAR=1 $(shell pkg-config --cflags-only-I $(NCURSES))
# LDLIBS += $(shell pkg-config --libs-only-l $(NCURSES))
# LDFLAGS += $(shell pkg-config --libs-only-L $(NCURSES))

# COLOR := echo -en '\033[1;34m'
# RESET := echo -en '\033[0m'
COLOR := [1;34m
RESET := [0m

####################

EXE := $(basename $(firstword $(SRC)))
SRCDIR ?= .
TMPDIR ?= .tmp

OBJ := $(patsubst %.cpp,$(TMPDIR)/%.o,$(SRC))

DEPFLAGS := -MMD -MP
CXXFLAGS += -I$(SRCDIR)

# be silent unless VERBOSE
ifndef VERBOSE
.SILENT: ;
endif

# default target
.PHONY: all
all: $(EXE) ## build executable

.PHONY: run
run: $(EXE) ## run program
	@./$(EXE)

.PHONY: debug
debug: CXXFLAGS := $(filter-out -O2,$(CXXFLAGS)) -D_DEBUG -Og
debug: $(EXE) ## build with debug enabled

.PHONY: debugrun
debugrun: debug run ## run debug version

$(EXE): $(OBJ)
	@echo "$(COLOR)Link $^ -> $@$(RESET)"
# default rule
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

$(TMPDIR)/%.o: $(SRCDIR)/%.cpp Makefile # $(TMPDIR)/%.d
	@echo "$(COLOR)Compile $(SRCDIR)/$*.cpp -> $(TMPDIR)/$*.o$(RESET)"
	$(COMPILE.cpp) $(DEPFLAGS) $(OUTPUT_OPTION) $<

# include dependencies
-include $(wildcard $(OBJ:.o=.d))

# depend on directory
$(OBJ): | $(TMPDIR)/.keepme

# create directory
$(TMPDIR)/.keepme:
	-$(MKDIR) $(TMPDIR)
	touch $@

# delete stuff
.PHONY: clean
clean: mostlyclean ## delete everything this Makefile created
	-$(RM) $(EXE)

.PHONY: mostlyclean
mostlyclean: ## delete everything created, leave executable
	@echo "$(COLOR)Cleaning$(RESET)"
	-$(RM) $(OBJ)
	-$(RM) $(OBJ:.o=.d)
	-$(RM) $(TMPDIR)/.keepme
	-$(RMDIR) $(TMPDIR)

.PHONY: forceclean
forceclean: ## force delete all created temporary folders
	@echo "$(COLOR)Force cleaning$(RESET)"
	-$(RM) -r $(TMPDIR)

.PHONY: help
help: ## show this help
	@awk -F':.*##' '/: [^#]*##/{ printf("%12s: %s\n", $$1, $$2)}' $(MAKEFILE_LIST)

